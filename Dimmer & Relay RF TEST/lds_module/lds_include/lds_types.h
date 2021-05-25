#ifndef _LDS_TYPES_H_
#define _LDS_TYPES_H_


#ifdef __cplusplus
extern "C" {
#endif  // End for __cplusplus



enum
{
    LDS_FALSE = 0,
    LDS_TRUE = 1
};

enum
{
	LDS_DISABLE = 0,
	LDS_ENABLE = 1
};

#define LDS_RESERVED	0


typedef char xt_s8;
typedef unsigned char xt_u8;
typedef signed short xt_s16;
typedef unsigned short xt_u16;
typedef signed long xt_s32;
typedef unsigned long xt_u32;
typedef long long xt_s64;
typedef unsigned long long xt_u64;
typedef unsigned char xt_bool;
typedef float xt_float;
typedef double xt_double;

#ifndef NULL
#define NULL ((void *)0)
#endif


#ifdef __cplusplus
}
#endif  // End for __cplusplus




#endif


