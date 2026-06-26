#include "optifetch.h"

#define MAX_LOGO_LINES 100
#define MAX_LOGO_WIDTH 512

typedef struct {
    char id[64];
    char lines[MAX_LOGO_LINES][MAX_LOGO_WIDTH];
    int count;
    int width;
} LogoCache;

static LogoCache cache_normal[32];
static LogoCache cache_small[32];
static int cache_normal_count = 0;
static int cache_small_count = 0;

static LogoCache* load_logo(const char* os_id, int small) {
    LogoCache* cache_arr = small ? cache_small : cache_normal;
    int* cache_cnt = small ? &cache_small_count : &cache_normal_count;

    for(int i=0; i<*cache_cnt; i++) {
        if(strcmp(cache_arr[i].id, os_id) == 0) return &cache_arr[i];
    }

    if(*cache_cnt >= 32) return NULL;

    LogoCache* new_cache = &cache_arr[(*cache_cnt)++];
    snprintf(new_cache->id, sizeof(new_cache->id), "%s", os_id);
    new_cache->count = 0;
    new_cache->width = 0;

    char path[512];
    const char* search_paths[] = {
        "logos",
        "/usr/share/optifetch/logos",
        "/usr/local/share/optifetch/logos",
        NULL
    };

    FILE *f = NULL;
    for (int i = 0; search_paths[i] != NULL; i++) {
        snprintf(path, sizeof(path), "%s/%s%s.txt", search_paths[i], os_id, small ? "_small" : "");
        f = fopen(path, "r");
        if (f) break;
    }

    if (!f) return new_cache;

    char line[MAX_LOGO_WIDTH];
    while(fgets(line, sizeof(line), f) && new_cache->count < MAX_LOGO_LINES) {
        line[strcspn(line, "\n")] = 0;
        snprintf(new_cache->lines[new_cache->count], MAX_LOGO_WIDTH, "%s", line);
        
        int w = 0;
        // CORRECTION : Utilisation de size_t au lieu de int pour éviter le warning
        for(size_t j=0; j<strlen(line); j++) {
            if((line[j] & 0xC0) != 0x80) w++;
        }
        if(w > new_cache->width) new_cache->width = w;
        
        new_cache->count++;
    }
    fclose(f);
    return new_cache;
}

void get_logo_line(const char* os_id, int small, int idx, char* out, size_t size) {
    LogoCache* c = load_logo(os_id, small);
    
    if(!c) {
        out[0] = '\0';
        return;
    }
    
    if(idx < c->count) {
        snprintf(out, size, "%s", c->lines[idx]);
    } else {
        out[0] = '\0';
    }
}

int get_logo_height(const char* os_id, int small) {
    LogoCache* c = load_logo(os_id, small);
    return c ? c->count : 0;
}