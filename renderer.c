#include "optifetch.h"

#define MAX_ALIGN_IDS 32

typedef struct {
    char id[32];
    int max_width;
} AlignID;

static AlignID align_ids[MAX_ALIGN_IDS];
static int align_count = 0;

static int current_col = 0;
static int skip_output = 0;
static int current_line_num = 0;
static int current_pass = 1;

// NOUVEAU : Variables pour gérer l'index du logo indépendamment des lignes du fichier
static int logo_line_idx = 0;
static int prev_line_had_logo = 0;

static int get_align_width(const char* id) {
    for(int i=0; i<align_count; i++) {
        if(strcmp(align_ids[i].id, id) == 0) return align_ids[i].max_width;
    }
    return 0;
}

static void set_align_width(const char* id, int width) {
    for(int i=0; i<align_count; i++) {
        if(strcmp(align_ids[i].id, id) == 0) {
            if(width > align_ids[i].max_width) align_ids[i].max_width = width;
            return;
        }
    }
    if(align_count < MAX_ALIGN_IDS) {
        snprintf(align_ids[align_count].id, sizeof(align_ids[align_count].id), "%.31s", id);
        align_ids[align_count].max_width = width;
        align_count++;
    }
}

static void my_putchar(char c) {
    if (skip_output) return;
    if (current_pass == 2) {
        putchar(c);
    }
    if ((c & 0xC0) != 0x80) {
        current_col++;
    }
}

static void my_putstr(const char* s) {
    for (int i = 0; s[i]; i++) my_putchar(s[i]);
}

static void print_color(const char* params, int is_bg) {
    if (skip_output || current_pass == 1) return;
    int r, g, b;
    if (sscanf(params, "%d,%d,%d", &r, &g, &b) == 3) {
        printf("\033[%d;2;%d;%d;%dm", is_bg ? 48 : 38, r, g, b);
        return;
    }
    if (strcmp(params, "red") == 0) r=255, g=0, b=0;
    else if (strcmp(params, "green") == 0) r=0, g=255, b=0;
    else if (strcmp(params, "blue") == 0) r=0, g=0, b=255;
    else if (strcmp(params, "yellow") == 0) r=255, g=255, b=0;
    else if (strcmp(params, "cyan") == 0) r=0, g=255, b=255;
    else if (strcmp(params, "magenta") == 0) r=255, g=0, b=255;
    else if (strcmp(params, "white") == 0) r=255, g=255, b=255;
    else if (strcmp(params, "black") == 0) r=0, g=0, b=0;
    else r=200, g=200, b=200;
    printf("\033[%d;2;%d;%d;%dm", is_bg ? 48 : 38, r, g, b);
}

static void print_variable(const char* name, SysInfo *info) {
    if (skip_output) return;
    
    char buf[1024];
    buf[0] = '\0';
    
    if (strcmp(name, "logo") == 0 || strcmp(name, "small_logo") == 0) {
        // NOUVEAU : On utilise logo_line_idx au lieu de current_line_num
        get_logo_line(info->os_id, name[0] == 's', logo_line_idx, buf, sizeof(buf));
    }
    else if (strcmp(name, "user") == 0) snprintf(buf, sizeof(buf), "%s", info->user);
    else if (strcmp(name, "host") == 0) snprintf(buf, sizeof(buf), "%s", info->hostname);
    else if (strcmp(name, "os") == 0) snprintf(buf, sizeof(buf), "%s", info->os);
    else if (strcmp(name, "kernel") == 0) snprintf(buf, sizeof(buf), "%s", info->kernel);
    else if (strcmp(name, "uptime") == 0) snprintf(buf, sizeof(buf), "%s", info->uptime);
    else if (strcmp(name, "arch") == 0) snprintf(buf, sizeof(buf), "%s", info->arch);
    else if (strcmp(name, "shell") == 0) snprintf(buf, sizeof(buf), "%s", info->shell);
    else if (strcmp(name, "cpu") == 0) snprintf(buf, sizeof(buf), "%s", info->cpu);
    else if (strcmp(name, "cores") == 0) snprintf(buf, sizeof(buf), "%ld", info->cores);
    else if (strcmp(name, "cpu_max_freq") == 0) snprintf(buf, sizeof(buf), "%ld", info->cpu_max_freq);
    else if (strcmp(name, "gpu") == 0) snprintf(buf, sizeof(buf), "%s", info->gpu);
    else if (strcmp(name, "de") == 0) snprintf(buf, sizeof(buf), "%s", info->de);
    else if (strcmp(name, "wm") == 0) snprintf(buf, sizeof(buf), "%s", info->wm);
    else if (strcmp(name, "term") == 0) snprintf(buf, sizeof(buf), "%s", info->term);
    else if (strcmp(name, "packages") == 0) snprintf(buf, sizeof(buf), "%s", info->packages);
    else if (strcmp(name, "battery") == 0) snprintf(buf, sizeof(buf), "%s", info->battery);
    else if (strcmp(name, "bat_status") == 0) snprintf(buf, sizeof(buf), "%s", info->battery_status);
    else if (strcmp(name, "date") == 0) snprintf(buf, sizeof(buf), "%s", info->date);
    else if (strcmp(name, "time") == 0) snprintf(buf, sizeof(buf), "%s", info->time_str);
    else if (strcmp(name, "ip_local") == 0) snprintf(buf, sizeof(buf), "%s", info->ip_local);
    else if (strcmp(name, "ip_public") == 0) snprintf(buf, sizeof(buf), "%s", info->ip_public);
    else if (strcmp(name, "dns") == 0) snprintf(buf, sizeof(buf), "%s", info->dns);
    else if (strcmp(name, "wifi") == 0) snprintf(buf, sizeof(buf), "%s", info->wifi[0] ? info->wifi : "N/A");
    else if (strcmp(name, "host_model") == 0) snprintf(buf, sizeof(buf), "%s", info->host_model);
    else if (strcmp(name, "display_server") == 0) snprintf(buf, sizeof(buf), "%s", info->display_server);
    else if (strcmp(name, "theme") == 0) snprintf(buf, sizeof(buf), "%s", info->theme);
    else if (strcmp(name, "font") == 0) snprintf(buf, sizeof(buf), "%s", info->font);
    else if (strcmp(name, "locale") == 0) snprintf(buf, sizeof(buf), "%s", info->locale);
    else if (strcmp(name, "disk_fs") == 0) snprintf(buf, sizeof(buf), "%s", info->disk_fs);
    else if (strncmp(name, "ram", 3) == 0) {
        char unit[128] = "mb";
        if (strlen(name) > 4 && name[3] == ':') snprintf(unit, sizeof(unit), "%s", name + 4);
        if (strcmp(unit, "gb") == 0) snprintf(buf, sizeof(buf), "%.1fGB / %.1fGB", (double)info->ram_used_mb / 1024.0, (double)info->ram_total_mb / 1024.0);
        else if (strcmp(unit, "%") == 0) snprintf(buf, sizeof(buf), "%.1f%%", info->ram_total_mb ? (double)info->ram_used_mb / (double)info->ram_total_mb * 100.0 : 0);
        else snprintf(buf, sizeof(buf), "%luMB / %luMB", info->ram_used_mb, info->ram_total_mb);
    }
    else if (strncmp(name, "swap", 4) == 0) {
        char unit[128] = "mb";
        if (strlen(name) > 5 && name[4] == ':') snprintf(unit, sizeof(unit), "%s", name + 5);
        if (strcmp(unit, "gb") == 0) snprintf(buf, sizeof(buf), "%.1fGB / %.1fGB", (double)info->swap_used_mb / 1024.0, (double)info->swap_total_mb / 1024.0);
        else if (strcmp(unit, "%") == 0) snprintf(buf, sizeof(buf), "%.1f%%", info->swap_total_mb ? (double)info->swap_used_mb / (double)info->swap_total_mb * 100.0 : 0);
        else snprintf(buf, sizeof(buf), "%luMB / %luMB", info->swap_used_mb, info->swap_total_mb);
    }
    else if (strncmp(name, "disk", 4) == 0) {
        char unit[128] = "mb";
        if (strlen(name) > 5 && name[4] == ':') snprintf(unit, sizeof(unit), "%s", name + 5);
        if (strcmp(unit, "gb") == 0) snprintf(buf, sizeof(buf), "%.1fGB / %.1fGB", (double)info->disk_used_mb / 1024.0, (double)info->disk_total_mb / 1024.0);
        else if (strcmp(unit, "%") == 0) snprintf(buf, sizeof(buf), "%.1f%%", info->disk_total_mb ? (double)info->disk_used_mb / (double)info->disk_total_mb * 100.0 : 0);
        else snprintf(buf, sizeof(buf), "%luMB / %luMB", info->disk_used_mb, info->disk_total_mb);
    }
    else {
        snprintf(buf, sizeof(buf), "{%s}", name);
    }
    
    my_putstr(buf);
}

static void process_line(const char* line, SysInfo *info) {
    int i = 0;
    current_col = 0;
    skip_output = 0;

    // NOUVEAU : Détection de la présence du logo sur la ligne courante
    int has_logo = (strstr(line, "{logo}") != NULL || strstr(line, "{small_logo}") != NULL);
    if (has_logo && !prev_line_had_logo) {
        logo_line_idx = 0; // On réinitialise l'index du logo à 0 si c'est le début du logo
    }

    while (line[i] != '\0' && line[i] != '\n') {
        if (line[i] == '{') {
            int j = i + 1;
            char token[128];
            int k = 0;
            while (line[j] != '}' && line[j] != '\0' && line[j] != '\n' && k < 127) {
                token[k++] = line[j++];
            }
            token[k] = '\0';
            if (line[j] == '}') {
                i = j + 1;
                
                if (strcmp(token, "if:laptop") == 0) skip_output = (strcmp(info->battery, "N/A") == 0);
                else if (strcmp(token, "if:gpu") == 0) skip_output = (strlen(info->gpu) == 0);
                else if (strcmp(token, "if:swap") == 0) skip_output = (info->swap_total_mb == 0);
                else if (strcmp(token, "if:wifi") == 0) skip_output = (strlen(info->wifi) == 0);
                else if (strcmp(token, "endif") == 0) skip_output = 0;
                
                else if (strncmp(token, "align:", 6) == 0) {
                    if (!skip_output) {
                        if (current_pass == 1) {
                            set_align_width(token + 6, current_col);
                        }
                        int target = get_align_width(token + 6);
                        while (current_col < target) my_putchar(' ');
                    }
                }
                else if (strncmp(token, "fg:", 3) == 0) print_color(token + 3, 0);
                else if (strncmp(token, "bg:", 3) == 0) print_color(token + 3, 1);
                else if (strcmp(token, "reset") == 0) { if(!skip_output && current_pass == 2) printf("\033[0m"); }
                else if (strcmp(token, "bold") == 0) { if(!skip_output && current_pass == 2) printf("\033[1m"); }
                else print_variable(token, info);
                
            } else {
                my_putchar(line[i++]);
            }
        } else {
            my_putchar(line[i++]);
        }
    }
    if (current_pass == 2 && !skip_output) putchar('\n');
    current_line_num++;
    
    // NOUVEAU : Mise à jour des variables pour la gestion du logo
    prev_line_had_logo = has_logo;
    if (has_logo) logo_line_idx++;
}

void render_config(const char* path, SysInfo *info) {
    FILE *f = fopen(path, "r");
    if (!f) return;

    static char lines[200][1024];
    int line_count = 0;
    while (fgets(lines[line_count], sizeof(lines[line_count]), f) && line_count < 200) {
        line_count++;
    }
    fclose(f);

    int logo_h = 0;
    char logo_token[20] = "";
    
    for (int i = 0; i < line_count; i++) {
        if (strstr(lines[i], "{logo}")) {
            snprintf(logo_token, sizeof(logo_token), "{logo}");
            logo_h = get_logo_height(info->os_id, 0);
        }
        if (strstr(lines[i], "{small_logo}")) {
            snprintf(logo_token, sizeof(logo_token), "{small_logo}");
            int sh = get_logo_height(info->os_id, 1);
            if (sh > logo_h) logo_h = sh;
        }
    }
    
    if (strlen(logo_token) > 0) {
        while (line_count < logo_h && line_count < 200) {
            snprintf(lines[line_count++], 1024, "%s{align:L} \n", logo_token);
        }
    }

    align_count = 0;

    // Pass 1
    current_pass = 1;
    current_line_num = 0;
    logo_line_idx = 0;
    prev_line_had_logo = 0;
    for (int i = 0; i < line_count; i++) {
        process_line(lines[i], info);
    }

    // Pass 2
    current_pass = 2;
    current_line_num = 0;
    logo_line_idx = 0;
    prev_line_had_logo = 0;
    for (int i = 0; i < line_count; i++) {
        process_line(lines[i], info);
    }
}