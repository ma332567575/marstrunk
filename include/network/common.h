/********************************************************************
	Copyright(C)
	filename:	common.h
	created:	2015/1/14 16:56:36
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2015/1/14 16:56:36 Create file
	...

*********************************************************************/

#ifndef common_h__
#define common_h__

#include "mars_define.h"

#ifdef WIN32

#include <WinSock2.h>
#include <Windows.h>

#else

#include <sys/types.h>
#include <errno.h>
#include <unistd.h>


#endif

// TODO: Insert the following code into Mars namespace
#ifdef WIN32
typedef SOCKET             net_handle_t;
#else
typedef int32_t            net_handle_t;
#endif

//************
// description: 可读事件回调函数结构
// param:       pBuff--可读内容的缓冲区
// param:       pBuffSize--可读内容大小
// param:       pCallbackData--设置回调函数时指定的上下文
typedef void (*RecvCallback) ( MARS_IN MPBYTE pBuff, MARS_IN MUI32 pBuffSize, MARS_IN MPVOID pCallbackData );

//************
// description: 监听事件回调函数结构
// param:       nHandle--新Client的Socket值
// param:       pCallbackData--设置监听事件回调函数指定的上下文
// param:       pRecvCallback--传出参数，用于设置该Client的有可读数据时的回调函数
// param:       pRecvCallBackData--传出参数，用于设置该Client的可读数据回调函数的上下文
typedef void (*ListenCallback) ( MARS_IN net_handle_t nHandle, MARS_IN MVOID* pCallbackData, MARS_OUT RecvCallback& pRecvCallback, MARS_OUT MPVOID*& pRecvCallBackData );

#define MARS_NET_SELECT_NUM                 ( 64 )
#define MARS_NET_MAX_SEND_SOCKET_BUF_SIZE   ( 128 * 1024 )
#define MARS_NET_IP_STR_LENGTH              ( 64 ) 
#define MARS_NET_MAX_EPOLL_SIZE             ( 9999 )

enum NetEvent
{
    net_event_newclient,
    net_event_close,
    net_event_read,
    net_event_write,
    net_event_confirm,
};

#endif // common_h__
