#include "optifetch.h"

static void read_file(const char *path, char *buf, size_t size) {
    FILE *f = fopen(path, "r");
    if (!f) { if(size>0) buf[0] = '\0'; return; }
    size_t n = fread(buf, 1, size - 1, f);
    buf[n] = '\0';
    fclose(f);
}

static void extract_value(const char *buf, const char *key, char *out, size_t size) {
    out[0] = '\0';
    const char *p = strstr(buf, key);
    if (p) {
        p += strlen(key);
        while (*p == ' ' || *p == '\t' || *p == ':' || *p == '=' || *p == '"') p++;
        size_t i = 0;
        while (p[i] && p[i] != '\n' && p[i] != '"' && i < size - 1) {
            out[i] = p[i];
            i++;
        }
        out[i] = '\0';
    }
}

static void clean_newline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

void get_info(SysInfo *info) {
    struct passwd *pw = getpwuid(getuid());
    snprintf(info->user, sizeof(info->user), "%s", pw ? pw->pw_name : "unknown");
    gethostname(info->hostname, sizeof(info->hostname));

    char buf[BUF_SIZE];
    
    read_file("/etc/os-release", buf, sizeof(buf));
    extract_value(buf, "PRETTY_NAME", info->os, sizeof(info->os));
    extract_value(buf, "ID", info->os_id, sizeof(info->os_id));
    extract_value(buf, "ANSI_COLOR", info->distro_color, sizeof(info->distro_color)); // NOUVEAU

    struct utsname uts;
    uname(&uts);
    snprintf(info->kernel, sizeof(info->kernel), "%s", uts.release);
    snprintf(info->arch, sizeof(info->arch), "%s", uts.machine);

    struct sysinfo s_info;
    sysinfo(&s_info);
    int days = s_info.uptime / 86400;
    int hours = (s_info.uptime % 86400) / 3600;
    int mins = (s_info.uptime % 3600) / 60;
    snprintf(info->uptime, sizeof(info->uptime), "%dd %dh %dm", days, hours, mins);

    const char *sh = getenv("SHELL");
    snprintf(info->shell, sizeof(info->shell), "%s", sh ? sh : "unknown");
    const char *ds = getenv("XDG_SESSION_TYPE");
    snprintf(info->display_server, sizeof(info->display_server), "%s", ds ? ds : "Unknown");
    const char *loc = getenv("LANG");
    snprintf(info->locale, sizeof(info->locale), "%s", loc ? loc : "Unknown");

    read_file("/proc/cpuinfo", buf, sizeof(buf));
    extract_value(buf, "model name", info->cpu, sizeof(info->cpu));
    info->cores = sysconf(_SC_NPROCESSORS_ONLN);
    read_file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", buf, sizeof(buf));
    info->cpu_max_freq = atol(buf) / 1000;

    read_file("/sys/class/dmi/id/product_name", info->host_model, sizeof(info->host_model));
    clean_newline(info->host_model);
    if (strlen(info->host_model) == 0) snprintf(info->host_model, sizeof(info->host_model), "Unknown");

    info->ram_total_mb = s_info.totalram * s_info.mem_unit / (1024 * 1024);
    info->ram_used_mb = (s_info.totalram - s_info.freeram) * s_info.mem_unit / (1024 * 1024);
    info->swap_total_mb = s_info.totalswap * s_info.mem_unit / (1024 * 1024);
    info->swap_used_mb = (s_info.totalswap - s_info.freeswap) * s_info.mem_unit / (1024 * 1024);

    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        info->disk_total_mb = (vfs.f_blocks * vfs.f_frsize) / (1024 * 1024);
        info->disk_used_mb = info->disk_total_mb - ((vfs.f_bfree * vfs.f_frsize) / (1024 * 1024));
    } else {
        info->disk_total_mb = 0; info->disk_used_mb = 0;
    }
    FILE *fp_fs = popen("findmnt -no FSTYPE / 2>/dev/null", "r");
    if (fp_fs) { if (fgets(info->disk_fs, sizeof(info->disk_fs), fp_fs)) clean_newline(info->disk_fs); pclose(fp_fs); }

    const char *de = getenv("XDG_CURRENT_DESKTOP");
    const char *wm = getenv("DESKTOP_SESSION");
    snprintf(info->de, sizeof(info->de), "%s", de ? de : "Unknown");
    snprintf(info->wm, sizeof(info->wm), "%s", wm ? wm : info->de);

    const char *term = getenv("TERM");
    snprintf(info->term, sizeof(info->term), "%s", term ? term : "Unknown");

    info->gpu[0] = '\0';
    FILE *fdrm = popen("cat /sys/class/drm/card*/device/uevent 2>/dev/null | grep DRIVER | head -n 1", "r");
    if (fdrm) {
        char drmbuf[256];
        if (fgets(drmbuf, sizeof(drmbuf), fdrm)) {
            extract_value(drmbuf, "DRIVER", info->gpu, sizeof(info->gpu));
            if(info->gpu[0] >= 'a' && info->gpu[0] <= 'z') info->gpu[0] -= 32;
        }
        pclose(fdrm);
    }

    FILE *fp_th = popen("gsettings get org.gnome.desktop.interface gtk-theme 2>/dev/null | tr -d \"'\"", "r");
    if (fp_th) { if (fgets(info->theme, sizeof(info->theme), fp_th)) clean_newline(info->theme); pclose(fp_th); }
    FILE *fp_fn = popen("gsettings get org.gnome.desktop.interface font-name 2>/dev/null | tr -d \"'\"", "r");
    if (fp_fn) { if (fgets(info->font, sizeof(info->font), fp_fn)) clean_newline(info->font); pclose(fp_fn); }

    int pkg_count = 0;
    DIR *d; struct dirent *dir;
    d = opendir("/var/lib/pacman/local");
    if (d) { while ((dir = readdir(d)) != NULL) if (dir->d_type == DT_DIR && dir->d_name[0] != '.') pkg_count++; closedir(d); }
    else {
        FILE *fp = popen("ls /var/lib/dpkg/info 2>/dev/null | grep -c '.list$' ; rpm -qa 2>/dev/null | wc -l", "r");
        if(fp) { char out[32]; if(fgets(out, sizeof(out), fp)) pkg_count = atoi(out); pclose(fp); }
    }
    snprintf(info->packages, sizeof(info->packages), "%d", pkg_count);

    char bat_path[256] = "";
    if (access("/sys/class/power_supply/BAT0/capacity", F_OK) == 0) snprintf(bat_path, sizeof(bat_path), "/sys/class/power_supply/BAT0");
    else if (access("/sys/class/power_supply/BAT1/capacity", F_OK) == 0) snprintf(bat_path, sizeof(bat_path), "/sys/class/power_supply/BAT1");
    if (strlen(bat_path) > 0) {
        char cap_path[512], stat_path[512];
        snprintf(cap_path, sizeof(cap_path), "%s/capacity", bat_path);
        snprintf(stat_path, sizeof(stat_path), "%s/status", bat_path);
        read_file(cap_path, buf, sizeof(buf)); clean_newline(buf);
        snprintf(info->battery, sizeof(info->battery), "%s%%", buf);
        read_file(stat_path, buf, sizeof(buf)); clean_newline(buf);
        snprintf(info->battery_status, sizeof(info->battery_status), "%s", buf);
    } else {
        snprintf(info->battery, sizeof(info->battery), "N/A");
        snprintf(info->battery_status, sizeof(info->battery_status), "N/A");
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(info->date, sizeof(info->date), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    snprintf(info->time_str, sizeof(info->time_str), "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

    info->ip_local[0] = '\0';
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == 0) {
        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == NULL) continue;
            if (ifa->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &sa->sin_addr, ip, INET_ADDRSTRLEN);
                if (strcmp(ip, "127.0.0.1") != 0) {
                    snprintf(info->ip_local, sizeof(info->ip_local), "%s (%s)", ip, ifa->ifa_name);
                    break;
                }
            }
        }
        freeifaddrs(ifaddr);
    }

    read_file("/etc/resolv.conf", buf, sizeof(buf));
    extract_value(buf, "nameserver", info->dns, sizeof(info->dns));

    info->wifi[0] = '\0';
    FILE *fp_wf = popen("iwgetid -r 2>/dev/null", "r");
    if (fp_wf) { if (fgets(info->wifi, sizeof(info->wifi), fp_wf)) clean_newline(info->wifi); pclose(fp_wf); }

    info->ip_public[0] = '\0';
    FILE *fp_ip = popen("curl -s --max-time 2 ifconfig.me 2>/dev/null", "r");
    if (fp_ip) { if (fgets(info->ip_public, sizeof(info->ip_public), fp_ip)) clean_newline(info->ip_public); pclose(fp_ip); }
}