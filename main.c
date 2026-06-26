#include "optifetch.h"

void print_help() {
    printf("Optifetch - Lightweight system info tool\n\n");
    printf("Usage: optifetch [options]\n\n");
    printf("Options:\n");
    printf("  help      Show this help message\n");
    printf("  (none)    Run optifetch with default or custom config\n\n");
    printf("Configuration:\n");
    printf("  A file named 'optifetch.conf' is generated on first run.\n");
    printf("  You can edit it to customize the output.\n\n");
    printf("Available Variables:\n");
    printf("  {logo} {small_logo} {user} {host} {os} {kernel} {uptime} {arch}\n");
    printf("  {shell} {cpu} {cores} {cpu_max_freq} {gpu} {de} {wm} {term}\n");
    printf("  {packages} {battery} {bat_status} {date} {time}\n");
    printf("  {ip_local} {ip_public} {dns} {wifi} {host_model}\n");
    printf("  {display_server} {theme} {font} {locale} {disk_fs}\n\n");
    printf("Variables with Units:\n");
    printf("  {ram:gb} {ram:mb} {ram:%%}\n");
    printf("  {swap:gb} {swap:mb} {swap:%%}\n");
    printf("  {disk:gb} {disk:mb} {disk:%%}\n\n");
    printf("Formatting:\n");
    printf("  {align:id}     Align text to the max width of all 'id' tags\n");
    printf("  {fg:R,G,B}     Set foreground color (e.g. {fg:255,0,0})\n");
    printf("  {bg:R,G,B}     Set background color\n");
    printf("  {fg:red}       Named colors (red, green, blue, yellow, cyan, magenta, white, black)\n");
    printf("  {bold}         Bold text\n");
    printf("  {reset}        Reset formatting\n\n");
    printf("Conditionals:\n");
    printf("  {if:laptop} ... {endif}   Show only if battery exists\n");
    printf("  {if:gpu} ... {endif}      Show only if GPU exists\n");
    printf("  {if:swap} ... {endif}     Show only if swap is active\n");
    printf("  {if:wifi} ... {endif}     Show only if connected to wifi\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1 && (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_help();
        return 0;
    }

    SysInfo info;
    get_info(&info);

    const char *config_path = "optifetch.conf";
    
    if (access(config_path, F_OK) != 0) {
        generate_default_config(config_path);
        printf("Fichier de configuration '%s' créé. Modifiez-le pour personnaliser l'affichage.\n\n", config_path);
    }

    render_config(config_path, &info);
    
    printf("\033[0m");
    return 0;
}