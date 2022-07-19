#include "mutex.h"
#include "xstatus.h"
#include "xparameters.h"

static XMutex __mutex_pool[1];
XMutex *mutex_ptr;


/****************************************************************************/
/**
*
* This function used to initialize the Mutex key.
*
* @param	None.
*
* @return	Status of the initialization.
*
* @note		None.
*
*****************************************************************************/
HRESULT MTX_Init(void)
{
	XMutex_Config *ConfigPtr;
	XStatus Status;

	mutex_ptr = &__mutex_pool[0];

	/*
	 * Lookup configuration data in the device configuration table.
	 * Use this configuration info down below when initializing this
	 * driver instance.
	 */
#if XPAR_CPU_ID == 0
	ConfigPtr = XMutex_LookupConfig(XPAR_MUTEX_0_IF_0_DEVICE_ID);
#else
	ConfigPtr = XMutex_LookupConfig(XPAR_MUTEX_0_IF_1_DEVICE_ID);
#endif

	if (ConfigPtr == (XMutex_Config *)NULL)
	{
		return MTX_LOOKUP_CONFIG;
	}

	/*
	 * Perform the rest of the initialization.
	 */
	Status = XMutex_CfgInitialize(mutex_ptr, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS)
	{
		return MTX_INIT_CONFIG;
	}
	return MTX_OK;
}


/****************************************************************************/
/**
*
* This function used to ask the status of the given Mutex ID.
*
* @param	id is the Mutex ID number.
*
* @return	Status of the given Mutex ID.
*
* @note		None.
*
*****************************************************************************/
BOOL MTX_IsLocked(U32 id)
{
	if(XMutex_IsLocked(mutex_ptr, id) != 0)
	{
		return TRUE;
	}
	return FALSE;
}


/****************************************************************************/
/**
*
* This function used to lock the given Mutex ID.
*
* @param	id is the Mutex ID number.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void MTX_Lock(U32 id)
{
	XMutex_Lock(mutex_ptr, id);
}


/****************************************************************************/
/**
*
* This function used to try to lock the given Mutex ID.
*
* @param	id is the Mutex ID number.
*
* @return	MTX_ALREADY_LOCKED: The given Mutex ID number is already locked.
* 			MTX_OK: locked the given Mutex ID number successfully.
*
* @note		None.
*
*****************************************************************************/
HRESULT MTX_TryLock(U32 id)
{
	if(XMutex_Trylock(mutex_ptr, id) != XST_SUCCESS)
	{
		return MTX_ALREADY_LOCKED;
	}
	return MTX_OK;
}


/****************************************************************************/
/**
*
* This function used to try to lock the given Mutex ID when considers the time.
*
* @param	id is the Mutex ID number.
*
* @param	timeout is the maximum of time to wait.
*
* @return	MTX_TIMEOUT: Time out.
* 			MTX_OK: locked the given Mutex ID number successfully.
*
* @note		None.
*
*****************************************************************************/
HRESULT MTX_TryLockWait(U32 id, U32 timeout)
{
	if(timeout == TIMEOUT_INFINITE)
	{
		MTX_Lock(id);
		return MTX_OK;
	}
	else
	{
		int t_cnt = 0;
		while ((XMutex_Trylock(mutex_ptr, id)) != XST_SUCCESS)
		{
			if(timeout != TIMEOUT_INFINITE)
			{
				t_cnt++;
				if(t_cnt >= timeout)
				{
					return MTX_TIMEOUT;
				}
			}
		}
		return MTX_OK;
	}
}


/****************************************************************************/
/**
*
* This function used to unlock the given Mutex ID.
*
* @param	id is the Mutex ID number.
*
* @return	MTX_NOT_OWNED: the given Mutex ID number is not owned.
* 			MTX_OK: unlocked the given Mutex ID number successfully.
*
* @note		None.
*
*****************************************************************************/
HRESULT MTX_Unlock(U32 id)
{
	if(XMutex_Unlock(mutex_ptr, id) != XST_SUCCESS)
	{
		return MTX_NOT_OWNED;
	}
	return MTX_OK;
}



