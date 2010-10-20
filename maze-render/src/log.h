#ifndef LOG_H__
#define LOG_H__

#if defined(ENABLE_LOGGING)
# define LOG(format, ...)    printf(format, ##__VA_ARGS__)
#else  /* ENABLE_LOGGING */
# define LOG
#endif	/* ENABLE_LOGGING */

#endif	/* LOG_H__ */
