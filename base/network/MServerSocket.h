/********************************************************************
	Copyright(C)
	filename:	MServerSocket.h
	created:	2015/3/20 18:01:30
	author:		YvesMa(ma332567575@163.com)	
	Description: 作为监听者的Socket类
                不对外暴露

	History:
	1. 2015/3/20 18:01:30 Create file
	...

*********************************************************************/
#ifndef MServerSocket_h__
#define MServerSocket_h__

#include "BaseSocket.h"

namespace Mars
{
    class MCServerSocket : public BaseSocket
    {
    public:
        MCServerSocket( MVOID );
        ~MCServerSocket( MVOID );

    public:
        MVOID OnRead( MVOID );                          // 有数据可读事件通知

    public:
        // 监听
        MBOOL Listen( MLPCSTR szIp, MUI16 uPort, RecvCallback pCallback, MPVOID pCallbackData );
        
    private:
        ListenCallback          m_ListenCallback;       // 监听回调函数
        MPVOID                  m_ListenCallbackData;   // 监听回调函数上下文数据
    };
}

#endif // MServerSocket_h__
