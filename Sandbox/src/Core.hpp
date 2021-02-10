#include <signal.h>

#ifdef ARTYS_ENABLE_ASSERTS
	#define ARTSY_ASSERT(x, ...)                                               \
		{                                                                      \
			if (!(x)) {                                                        \
				raise(SIGINT);                                                 \
			}                                                                  \
		}
	#define ARTSY_CORE_ASSERT(x, ...)                                          \
		{                                                                      \
			if (!(x)) {                                                        \
				raise(SIGINT);                                                 \
			}                                                                  \
		}
#else
	#define ARTSY_ASSERT(x, ...)
	#define ARTSY_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
