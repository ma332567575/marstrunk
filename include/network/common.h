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
// description: �ɶ��¼��ص������ṹ
// param:       pBuff--�ɶ����ݵĻ�����
// param:       pBuffSize--�ɶ����ݴ�С
// param:       pCallbackData--���ûص�����ʱָ����������
typedef void (*RecvCallback) ( MARS_IN MPBYTE pBuff, MARS_IN MUI32 pBuffSize, MARS_IN MPVOID pCallbackData );

//************
// description: �����¼��ص������ṹ
// param:       nHandle--��Client��Socketֵ
// param:       pCallbackData--���ü����¼��ص�����ָ����������
// param:       pRecvCallback--�����������������ø�Client���пɶ�����ʱ�Ļص�����
// param:       pRecvCallBackData--�����������������ø�Client�Ŀɶ����ݻص�������������
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
