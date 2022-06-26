#ifndef NSH_CHECKED_H
#define NSH_CHECKED_H

#define checked_add(result, a, b) !__builtin_add_overflow(a, b, result)
#define checked_sub(result, a, b) !__builtin_sub_overflow(a, b, result)
#define checked_mul(result, a, b) !__builtin_mul_overflow(a, b, result)

#endif /* NSH_CHECKED_H */
