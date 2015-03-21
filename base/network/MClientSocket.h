/********************************************************************
	Copyright(C)
	filename:	MClientSocket.h
	created:	2015/3/21 11:43:22
	author:		YvesMa(ma332567575@163.com)	
	Description: 客户端Socket类
                不对外暴露

	History:
	1. 2015/3/21 11:43:22 Create file
	...

*********************************************************************/
#ifndef MClientSocket_h__
#define MClientSocket_h__

#include "BaseSocket.h"
#include "MSimpleBuffer.h"

namespace Mars
{
    class MCClientSocket : public BaseSocket
    {
    public:
        MCClientSocket( MVOID );
        ~MCClientSocket( MVOID );

    public:
        // 有数据可读
        MVOID OnRead( MVOID );
        // 有数据可写
        MVOID OnWrite( MVOID );
        // 有错误
        MVOID OnError( MVOID );

    public:
        // 设置接收数据回调函数
        MVOID SetRecvCallback( RecvCallback pCallback )
        { m_RecvCallback = pCallback; }

        // 设置接收数据回调函数的上下文数据
        MVOID SetRecvCallbackData( MPVOID pData )
        { m_RecvCallbackData = pData; }

        // 发送消息
        MBOOL Send( MVOID* pBuf, MI64 nLen );

    private:
        RecvCallback            m_RecvCallback;
        MPVOID                  m_RecvCallbackData;
        MCSimpleBuffer          m_RecvBuffer;
        MCSimpleBuffer          m_SendBuffer;
        MBOOL                   m_Busy;

    };
}

#endif // MClientSocket_h__
