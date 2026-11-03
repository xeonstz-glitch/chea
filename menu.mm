#import <UIKit/UIKit.h>

@interface ESPView : UIView
@property (atomic) BOOL enabled;
@end

@interface CheatMenu : UIView
@property (nonatomic, strong) UIButton* toggle;
@property (nonatomic, weak) ESPView* esp;
@end

@implementation CheatMenu
- (instancetype)initWithFrame:(CGRect)frame esp:(ESPView*)esp {
    if ((self = [super initWithFrame:frame])) {
        self.esp = esp;
        self.toggle = [UIButton buttonWithType:UIButtonTypeSystem];
        self.toggle.frame = CGRectMake(10, 40, 60, 44);
        [self.toggle setTitle:@"ESP" forState:UIControlStateNormal];
        [self.toggle setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        [self.toggle setBackgroundColor:[UIColor colorWithWhite:0.1 alpha:0.7]];
        self.toggle.layer.cornerRadius = 8;
        [self.toggle addTarget:self action:@selector(onTap) forControlEvents:UIControlEventTouchUpInside];
        [self addSubview:self.toggle];
    }
    return self;
}
- (void)onTap {
    self.esp.enabled = !self.esp.enabled;
    [self.toggle setBackgroundColor:self.esp.enabled
        ? [UIColor colorWithRed:0.2 green:0.8 blue:0.2 alpha:0.8]
        : [UIColor colorWithWhite:0.1 alpha:0.7]];
}
@end
