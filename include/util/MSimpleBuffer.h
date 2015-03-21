/********************************************************************
	Copyright(C)
	filename:	MSimpleBuffer.h
	created:	2015/3/21 14:00:53
	author:		YvesMa(ma332567575@163.com)	
	Description: BUFFER读取写入类

	History:
	1. 2015/3/21 14:00:53 Create file
	...

*********************************************************************/
#ifndef MSimpleBuffer_h__
#define MSimpleBuffer_h__

#include "mars_stdinclude.h"

namespace Mars
{
    class MCSimpleBuffer
    {
    public:
        MCSimpleBuffer( MVOID );
        ~MCSimpleBuffer( MVOID );

    public:
        MI64 GetWrietSize( MVOID )
        { return m_WriteSize; }

        MI64 GetAllocSize( MVOID )
        { return m_AllocSize; }

        MPBYTE GetBuffer( MVOID )
        { return m_Buffer; }

        MBOOL ExpendBuffer( MI64 n64Size );

        MI64 Write( MPVOID pBuf, MI64 nSize );

        MI64 Read( MPVOID pBuf, MI64 nSize );

        MI64 ModWriteSize( MI64 n64ModValue )
        {
            m_WriteSize += n64ModValue;
        }

        MVOID CleanBuffer( MVOID );

    private:
        MPBYTE       m_Buffer;       // 读写BUFFER
        MI64        m_AllocSize;    // 已经分配的BUFFER大小
        MI64        m_WriteSize;    // 已经写入的BUFFER大小

    };
}

#endif // MSimpleBuffer_h__