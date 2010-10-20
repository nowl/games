#ifndef UTILS_H__
#define UTILS_H__

#include <memory.h>

#ifdef NDEBUG
# define LOG
#else
# define LOG(...) { fprintf(stdout, "%s:%d -> ", __FILE__, __LINE__); fprintf(stdout, __VA_ARGS__); }
#endif

#define WARN(...) fprintf(stdout, __VA_ARGS__);
#define ERROR(...) fprintf(stdout, __VA_ARGS__);

void *memory_grow_to_size(void *mem_p, size_t per_block, size_t *mem_cap_ip, size_t mem_size_i);

#endif  /* UTILS_H__ */
