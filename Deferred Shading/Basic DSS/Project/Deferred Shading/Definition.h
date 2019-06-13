
#define ENABLE_BIT(Mask, Flag) Mask |= Flag
#define DISABLE_BIT(Mask, Flag) Mask &= ~Flag
#define IS_BIT_ENABLED(Mask, Flag) Mask & Flag