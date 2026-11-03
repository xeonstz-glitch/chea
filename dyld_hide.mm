#import <mach-o/dyld.h>
#import <string.h>
#include "dyld_hide.h"
#include "fishhook.h"

static uint32_t (*orig_image_count)(void) = nullptr;
static const char* (*orig_image_name)(uint32_t) = nullptr;
static const struct mach_header* (*orig_image_header)(uint32_t) = nullptr;
static const char* kHide = "cheat";

static uint32_t my_count(void) { return orig_image_count ? orig_image_count() : 0; }

static const char* my_name(uint32_t idx) {
    const char* n = orig_image_name ? orig_image_name(idx) : nullptr;
    if (n && strstr(n, kHide)) return "/usr/lib/system/libsystem_c.dylib";
    return n;
}

static const struct mach_header* my_header(uint32_t idx) {
    const char* n = orig_image_name ? orig_image_name(idx) : nullptr;
    if (n && strstr(n, kHide)) return orig_image_header ? orig_image_header(0) : nullptr;
    return orig_image_header ? orig_image_header(idx) : nullptr;
}

extern "C" void install_dyld_hooks(void) {
    struct rebinding r[] = {
        {"_dyld_image_count", (void*)my_count, (void**)&orig_image_count},
        {"_dyld_get_image_name", (void*)my_name, (void**)&orig_image_name},
        {"_dyld_get_image_header", (void*)my_header, (void**)&orig_image_header},
    };
    rebind_symbols(r, 3);
}
