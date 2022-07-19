/*
 * default_types.h
 * Contains some default types and definitions for running data diode application
 *  Created on: 27.06.2022
 *      Author: Ha
 */

#ifndef __DEFAULT_TYPES_H__
#define __DEFAULT_TYPES_H__


typedef signed char S8;
typedef signed short S16;
typedef signed long S32;
typedef signed long long S64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef unsigned long long U64;

typedef volatile signed char VS8;
typedef volatile signed short VS16;
typedef volatile signed long VS32;
typedef volatile signed long long VS64;

typedef volatile unsigned char VU8;
typedef volatile unsigned short VU16;
typedef volatile unsigned long VU32;
typedef volatile unsigned long long VU64;


#ifndef NULL
#define NULL						((void *)0)
#endif
#ifndef FALSE
#define FALSE						0
#define TRUE						(!FALSE)
#endif
#define SYSTEM_SUCCESS 				0x00000000
#define SYSTEM_FAILED 				0x80000000
#define SYSTEM_RESET   				0x80000001
#define CONFIG_FAILED   			0x80000010
#define SYSTEM_TIME_OUT 			0x80000011

#define SUCCEEDED(hr)				((hr) == SYSTEM_SUCCESS)
#define FAILED(hr)					((hr) != SYSTEM_SUCCESS)
#define TIMEOUT_INFINITE			0xFFFFFFFF

typedef int BOOL;
typedef int HRESULT;



#define ARM_CORE0					0
#define ARM_CORE1					1





#endif
