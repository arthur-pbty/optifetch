#ifndef OPTIFETCH_H
#define OPTIFETCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <pwd.h>
#include <time.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 4096

typedef struct
{
    char user[128];
    char hostname[256];
    char os[256];
    char os_id[64];
    char distro_color[64]; // NOUVEAU
    char kernel[128];
    char uptime[128];
    char arch[128];
    char shell[128];
    char cpu[256];
    char gpu[256];
    char de[128];
    char wm[128];
    char term[128];
    char packages[128];
    long cores;
    long cpu_max_freq;

    char host_model[128];
    char display_server[32];
    char theme[128];
    char font[128];
    char locale[64];
    char dns[64];
    char wifi[128];
    char ip_public[64];
    char disk_fs[32];

    unsigned long ram_total_mb;
    unsigned long ram_used_mb;
    unsigned long swap_total_mb;
    unsigned long swap_used_mb;
    unsigned long disk_total_mb;
    unsigned long disk_used_mb;
    char battery[64];
    char battery_status[64];
    char date[64];
    char time_str[64];
    char ip_local[128];
} SysInfo;

void get_info(SysInfo *info);
void get_logo_line(const char *os_id, int small, int idx, char *out, size_t size);
int get_logo_height(const char *os_id, int small);
void generate_default_config(const char *path);
void render_config(const char *path, SysInfo *info);
void print_help();

#endif