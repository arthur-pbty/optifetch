#include "optifetch.h"

void generate_default_config(const char* path) {
    FILE *f = fopen(path, "w");
    if (!f) return;
    
    fprintf(f, "{reset}{align:L}{bold}{user}{reset}@{bold}{host}{reset}\n");
    fprintf(f, "{align:L}-----------------------------\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}OS{reset}: {align:V}{os}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Host{reset}: {align:V}{host_model}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Kernel{reset}: {align:V}{kernel} {arch}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Uptime{reset}: {align:V}{uptime}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Packages{reset}: {align:V}{packages}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Shell{reset}: {align:V}{shell}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}DE/WM{reset}: {align:V}{de} ({wm})\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Display{reset}: {align:V}{display_server}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Terminal{reset}: {align:V}{term}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}CPU{reset}: {align:V}{cpu} ({cores}) @ {cpu_max_freq}MHz\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{if:gpu}{fg:255,255,0}GPU{reset}: {align:V}{gpu}{endif}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Memory{reset}: {align:V}{ram:gb} ({ram:%%})\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{if:swap}{fg:255,255,0}Swap{reset}: {align:V}{swap:gb} ({swap:%%}){endif}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Disk{reset}: {align:V}{disk:gb} ({disk:%%}) - {disk_fs}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Theme{reset}: {align:V}{theme}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Font{reset}: {align:V}{font}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Locale{reset}: {align:V}{locale}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{if:laptop}{fg:255,255,0}Battery{reset}: {align:V}{battery} ({bat_status}){endif}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Date{reset}: {align:V}{date} {time}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Local IP{reset}: {align:V}{ip_local}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{if:wifi}{fg:255,255,0}Wifi{reset}: {align:V}{wifi}{endif}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}Public IP{reset}: {align:V}{ip_public}\n");
    fprintf(f, "{fg:0,255,255}  {logo}      {align:L}{fg:255,255,0}DNS{reset}: {align:V}{dns}\n");
    fprintf(f, "{fg:0,255,255}  {logo}\n");
    fprintf(f, "{fg:0,255,255}  {logo}{align:L}{fg:0,255,0}███{fg:255,255,0}███{fg:255,0,255}███{fg:0,255,255}███{reset}\n");
    fclose(f);
}