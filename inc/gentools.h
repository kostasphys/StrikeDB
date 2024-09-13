#ifndef    __s_gentools_h__
#define    __s_gentools_h__

#include <stddef.h>

#define container_of(ptr, type, member)({   \
	const typeof( ((type*)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );})

#endif 
