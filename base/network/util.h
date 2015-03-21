/********************************************************************
	Copyright(C)
	filename:	util.h
	created:	2014/12/31 9:36:38
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2014/12/31 9:36:38 Create file
	...

*********************************************************************/
#ifndef util_h__
#define util_h__

#include "common.h"

class CThreadLock
{
public:
    CThreadLock( void );
    ~CThreadLock( void );
    void Lock( void );
    void Unlock( void );

private:
#ifdef _WIN32
    CRITICAL_SECTION m_CriticalSection;
#endif

};

#endif // util_h__