#import <UIKit/UIKit.h>
#include "il2cpp.hpp"
#include "config.hpp"
#include "camera.hpp"
#include "find.hpp"

@interface ESPView : UIView
@property (atomic) BOOL enabled;
@end

@implementation ESPView

- (void)drawRect:(CGRect)rect {
    if (!self.enabled) return;
    if (!il2::image) return;

    void* cam = game::get_camera();
    if (!cam) return;

    float MVP[16];
    if (!game::get_mvp(cam, MVP)) return;

    auto players = game::find_players();
    if (players.empty()) return;

    CGSize scr = self.bounds.size;
    float W = scr.width, H = scr.height;

    void* local = game::get_local_player();
    int32_t local_team = local ? *(int32_t*)((uint8_t*)local + cfg::o_team) : -1;

    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGContextSetLineWidth(ctx, 1.5f);

    for (void* p : players) {
        if (p == local) continue;

        void* tr = *(void**)((uint8_t*)p + cfg::o_transform);
        if (!tr) continue;

        float pos[3];
        if (!game::get_position(tr, pos)) continue;

        float sx, sy, hx, hy;
        if (!game::w2s(MVP, pos[0], pos[1], pos[2], W, H, &sx, &sy)) continue;
        if (!game::w2s(MVP, pos[0], pos[1] + 1.7f, pos[2], W, H, &hx, &hy)) continue;

        float h = sy - hy;
        float w = h * 0.5f;
        if (h < 4 || h > H) continue;

        int32_t team = *(int32_t*)((uint8_t*)p + cfg::o_team);
        
        if (team == local_team) CGContextSetRGBStrokeColor(ctx, 0.2f, 0.8f, 1.0f, 1.0f);
        else                    CGContextSetRGBStrokeColor(ctx, 1.0f, 0.2f, 0.2f, 1.0f);
        
        CGContextStrokeRect(ctx, CGRectMake(hx - w/2, hy, w, h));

        float hp = *(float*)((uint8_t*)p + cfg::o_health);
        if (hp > 0 && hp <= 100) {
            CGContextSetRGBFillColor(ctx, 0.2f, 1.0f, 0.2f, 1.0f);
            CGContextFillRect(ctx, CGRectMake(hx - w/2 - 4,
                hy + (1.0f - hp/100.0f) * h, 3, h * (hp/100.0f)));
        }

        float dx = pos[0], dz = pos[2];
        float d = sqrtf(dx*dx + dz*dz);
        if (d > cfg::max_dist) continue;

        NSString* label = [NSString stringWithFormat:@"%.0fm", d];
        [label drawAtPoint:CGPointMake(hx + w/2 + 4, hy)
            withAttributes:@{NSForegroundColorAttributeName:[UIColor yellowColor],
                             NSFontAttributeName:[UIFont boldSystemFontOfSize:10]}];
    }
}

@end
