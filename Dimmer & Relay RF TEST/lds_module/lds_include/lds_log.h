#ifndef _LDS_LOG_H_
#define _LDS_LOG_H_


#define LOG_LEVEL_DISABLE	0
#define LOG_LEVEL_ERROR		1
#define LOG_LEVEL_INFO		2
#define LOG_LEVEL_DEBUG		3

#define LDS_LOG_LEVEL			LOG_LEVEL_DISABLE


#if(defined(LOG_LOCAL_OPEN) && (LDS_LOG_LEVEL >= LOG_LEVEL_ERROR))
#define LOG_ERROR              printf
#else
#define LOG_ERROR(...)
#endif

#if(defined(LOG_LOCAL_OPEN) && (LDS_LOG_LEVEL >= LOG_LEVEL_INFO))
#define LOG_INFO               printf
#else
#define LOG_INFO(...)
#endif

#if(defined(LOG_LOCAL_OPEN) && (LDS_LOG_LEVEL >= LOG_LEVEL_DEBUG))
#define LOG_DEBUG               printf
#else
#define LOG_DEBUG(...)
#endif

#define LOG_RF					printf


#endif
