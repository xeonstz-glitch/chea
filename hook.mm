#import <UIKit/UIKit.h>
#import <objc/runtime.h>
#include "il2cpp.hpp"
#include "dyld_hide.h"

@interface ESPView : UIView
@property (atomic) BOOL enabled;
@end

@interface CheatMenu : UIView
- (instancetype)initWithFrame:(CGRect)frame esp:(ESPView*)esp;
@end

static ESPView* g_esp = nil;
static CheatMenu* g_menu = nil;

static void setup_overlay(UIWindow* window) {
    if (g_esp) return;
    g_esp = [[ESPView alloc] initWithFrame:window.bounds];
    g_esp.backgroundColor = [UIColor clearColor];
    g_esp.userInteractionEnabled = NO;
    g_esp.enabled = NO;
    [window addSubview:g_esp];

    g_menu = [[CheatMenu alloc] initWithFrame:window.bounds esp:g_esp];
    g_menu.backgroundColor = [UIColor clearColor];
    g_menu.userInteractionEnabled = YES;
    [window addSubview:g_menu];
}

static IMP orig_present = nullptr;

static void hook_present(id self, SEL _cmd) {
    if (orig_present) ((void(*)(id,SEL))orig_present)(self, _cmd);

    static dispatch_once_t once;
    dispatch_once(&once, ^{
        for (UIWindow* w in [UIApplication sharedApplication].windows) {
            if ([w.rootViewController isKindOfClass:NSClassFromString(@"UnityViewControllerBase")] ||
                [NSStringFromClass([w.rootViewController class]) containsString:@"Unity"]) {
                setup_overlay(w);
                break;
            }
        }
        if (!g_esp) {
            UIWindow* key = [UIApplication sharedApplication].keyWindow;
            if (key) setup_overlay(key);
        }
    });

    if (g_esp && g_esp.enabled) [g_esp setNeedsDisplay];
}

__attribute__((constructor))
static void on_load() {
    install_dyld_hooks();
    il2::init();

    Class unityView = NSClassFromString(@"UnityView");
    if (!unityView) unityView = NSClassFromString(@"UnityDefaultView");
    if (unityView) {
        Method m = class_getInstanceMethod(unityView, NSSelectorFromString(@"present"));
        if (m) {
            orig_present = method_getImplementation(m);
            method_setImplementation(m, (IMP)hook_present);
        }
    }
}
