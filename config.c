#include "optifetch.h"

void generate_default_config(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
        return;

    fprintf(f, "{reset}{align:L}{distro_color}{bold}{user}{reset}@{distro_color}{bold}{host}{reset}\n");
    fprintf(f, "{align:L}─────────────────────────────\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}OS{reset}: {align:V}{os}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Host{reset}: {align:V}{host_model}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Kernel{reset}: {align:V}{kernel} {arch}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Uptime{reset}: {align:V}{uptime}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Packages{reset}: {align:V}{packages}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Shell{reset}: {align:V}{shell}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}DE/WM{reset}: {align:V}{de} ({wm})\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Display{reset}: {align:V}{display_server}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Terminal{reset}: {align:V}{term}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}CPU{reset}: {align:V}{cpu} ({cores}) @ {cpu_max_freq}MHz\n");
    fprintf(f, "{if:gpu}  {distro_color}{logo}    {align:L}{fg:157,211,154}GPU{reset}: {align:V}{gpu}\n{endif}");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Memory{reset}: {align:V}{ram:gb} ({ram:%%})\n");
    fprintf(f, "{if:swap}  {distro_color}{logo}    {align:L}{fg:157,211,154}Swap{reset}: {align:V}{swap:gb} ({swap:%%})\n{endif}");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Disk{reset}: {align:V}{disk:gb} ({disk:%%}) - {disk_fs}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Theme{reset}: {align:V}{theme}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Font{reset}: {align:V}{font}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Locale{reset}: {align:V}{locale}\n");
    fprintf(f, "{if:laptop}  {distro_color}{logo}    {align:L}{fg:157,211,154}Battery{reset}: {align:V}{battery} ({bat_status})\n{endif}");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Date{reset}: {align:V}{date} {time}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Local IP{reset}: {align:V}{ip_local}\n");
    fprintf(f, "{if:wifi}  {distro_color}{logo}    {align:L}{fg:157,211,154}Wifi{reset}: {align:V}{wifi}\n{endif}");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}Public IP{reset}: {align:V}{ip_public}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}DNS{reset}: {align:V}{dns}\n");
    fprintf(f, "  {distro_color}{logo}\n");
    fprintf(f, "  {distro_color}{logo}    {align:L}{fg:157,211,154}███{fg:23,147,209}███{fg:255,200,87}███{fg:255,90,120}███{fg:180,120,255}███{fg:80,220,180}███{fg:240,240,240}███{fg:120,120,120}███{reset}\n");
    fclose(f);
}