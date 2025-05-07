#ifndef DELTE_MINIFILTER_LOG_H
#define DELTE_MINIFILTER_LOG_H

#if 1
#define LOG_MSG(format, ...) DbgPrint("FIM : " format "\n\r" __VA_OPT__(,) __VA_ARGS__)
#else
#define LOG_MSG(format, ...) 
#endif

#endif //DELTE_MINIFILTER_LOG_H
