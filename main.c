#include "optifetch.h"
#include <sys/stat.h> // Nécessaire pour mkdir()

void print_help()
{
    printf("Optifetch - Lightweight system info tool\n\n");
    printf("Usage: optifetch [options]\n\n");
    printf("Options:\n");
    printf("  help      Show this help message\n");
    printf("  (none)    Run optifetch with default or custom config\n\n");
    printf("Configuration:\n");
    printf("  Config file is searched in:\n");
    printf("    1. ./optifetch.conf\n");
    printf("    2. ~/.config/optifetch.conf\n");
    printf("    3. /etc/optifetch.conf\n");
    printf("  It is generated on first run if not found.\n\n");
    printf("Available Variables:\n");
    printf("  {logo} {small_logo} {distro_color} {user} {host} {os} {kernel} {uptime} {arch}\n");
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

int main(int argc, char *argv[])
{
    if (argc > 1 && (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
    {
        print_help();
        return 0;
    }

    SysInfo info;
    get_info(&info);

    char config_path[512] = "optifetch.conf";
    int config_found = 0;

    // 1. Cherche dans le dossier local
    if (access(config_path, F_OK) == 0)
    {
        config_found = 1;
    }

    // 2. Cherche dans ~/.config/optifetch.conf
    if (!config_found)
    {
        const char *home = getenv("HOME");
        if (home)
        {
            snprintf(config_path, sizeof(config_path), "%s/.config/optifetch.conf", home);
            if (access(config_path, F_OK) == 0)
            {
                config_found = 1;
            }
        }
    }

    // 3. Cherche dans /etc/optifetch.conf
    if (!config_found)
    {
        snprintf(config_path, sizeof(config_path), "/etc/optifetch.conf");
        if (access(config_path, F_OK) == 0)
        {
            config_found = 1;
        }
    }

    // 4. Si toujours pas trouvé, on le génère
    if (!config_found)
    {
        const char *home = getenv("HOME");
        if (home)
        {
            // S'assurer que le dossier ~/.config existe bien avant de créer le fichier
            char config_dir[512];
            snprintf(config_dir, sizeof(config_dir), "%s/.config", home);
            mkdir(config_dir, 0755); // Crée le dossier (s'il existe déjà, ne fait rien)

            snprintf(config_path, sizeof(config_path), "%s/.config/optifetch.conf", home);
            generate_default_config(config_path);
        }
        else
        {
            // Si pas de HOME (ex: environnement très restreint), on crée dans le dossier courant
            snprintf(config_path, sizeof(config_path), "optifetch.conf");
            generate_default_config(config_path);
        }
        printf("Fichier de configuration créé : %s\n\n", config_path);
    }

    render_config(config_path, &info);

    printf("\033[0m");
    return 0;
}