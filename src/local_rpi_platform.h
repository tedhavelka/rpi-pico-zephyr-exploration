#ifndef LOCAL_RPI_PLATFORM_H
#define LOCAL_RPI_PLATFORM_H


#ifndef _MSC_VER
#ifndef __noreturn
#define __noreturn __attribute((noreturn))
#endif

#ifndef __unused
#define __unused __attribute__((unused))
#endif

#ifndef __noinline
#define __noinline __attribute__((noinline))
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif

#define PICO_WEAK_FUNCTION_DEF(x) _Pragma(__STRING(weak x))
#define PICO_WEAK_FUNCTION_IMPL_NAME(x) x

#else
#ifndef __noreturn
#define __noreturn __declspec(noreturn)
#endif

#ifndef __unused
#define __unused
#endif

#ifndef __noinline
#define __noinline __declspec(noinline)
#endif

#ifndef __aligned
#define __aligned(x) __declspec(align(x))
#endif

#ifndef __CONCAT
#define __CONCAT(x,y) x ## y
#endif

#define __thread __declspec( thread )

#define PICO_WEAK_FUNCTION_DEF(x) __pragma(comment(linker, __STRING(/alternatename:_##x=_##x##__weak)));
#define PICO_WEAK_FUNCTION_IMPL_NAME(x) x ## __weak

static __noreturn void __builtin_unreachable() {
}

#include <intrin.h>
#define __builtin_clz __lzcnt
#endif


#endif // LOCAL_RPI_PLATFORM_H
