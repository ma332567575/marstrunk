/********************************************************************
	Copyright(C)
	filename:	MSyncTool.h
	created:	2015/3/15 16:24:13
	author:		YvesMa(ma332567575@163.com)	
	Description: �߳�ͬ������

	History:
	1. 2015/3/15 16:24:13 Create file
	...

*********************************************************************/
#ifndef MSyncTool_h__
#define MSyncTool_h__

#include "mars_stdinclude.h"

namespace Mars
{
    //----------------------------------------------------------
    //����Ϊ�ٽ���������װ
#ifdef _WIN32

    typedef CRITICAL_SECTION MARS_CRITICAL_SECTION;
    typedef PCRITICAL_SECTION MARS_PCRITICAL_SECTION;

#else
#endif
    
    MVOID MarsInitCriticalSection( MARS_PCRITICAL_SECTION pCSection );

    MVOID MarsDelCriticalSection( MARS_PCRITICAL_SECTION pCSection );

    MVOID MarsEnterCriticalSection( MARS_PCRITICAL_SECTION pCSection );

    MVOID MarsLeaveCriticalSection( MARS_PCRITICAL_SECTION pCSection );
}

#endif // MSyncTool_h__