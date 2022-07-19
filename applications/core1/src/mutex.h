#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "xmutex.h"
#include "default_types.h"


#define MTX_OK						SYSTEM_SUCCESS
#define MTX_ERROR					0x80010000
#define MTX_LOOKUP_CONFIG			0x80010001
#define MTX_INIT_CONFIG				0x80010002
#define MTX_ALREADY_LOCKED			0x80010003
#define MTX_NOT_OWNED				0x80010004
#define MTX_TIMEOUT					0x80010005


#define MUTEX_DBG_OUT_ID			0
#define MUTEX_SHARED_MEMORY_ID		1
#define MUTEX_IO_ID					2

///////////////////////////////////////////////////
///  Initialize internal Mutex-Variables
HRESULT MTX_Init(void);

///////////////////////////////////////////////////
///  Checks, if mutex #id is locked
///  Returns TRUE, if mutex #id  is locked, otherwise FALSE
BOOL	MTX_IsLocked(U32 id);

///////////////////////////////////////////////////
///  Lock mutex #id. If mutex #id is locked, wait
///  for release of mutex #id and lock it
void 	MTX_Lock(U32 id);

///////////////////////////////////////////////////
///  Try to lock mutex #id. If mutex #id is locked
///  return MTX_ALREADY_LOCKED otherwise MTX_OK
HRESULT MTX_TryLock(U32 id);

///////////////////////////////////////////////////
///  Try to lock mutex #id. If mutex #id is locked
///  try lock till timeout. If is timeout then
///  return MTX_TIMEOUT. If mutex #id is locked
///  return MTX_OK
HRESULT MTX_TryLockWait(U32 id, U32 timeout);

///////////////////////////////////////////////////
///  Unlock mutex #id. If mutex #id is locked by
///  other core, return MTX_NOT_OWNED otherwise MTX_OK
HRESULT MTX_Unlock(U32 id);

#endif
