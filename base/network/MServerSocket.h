/********************************************************************
	Copyright(C)
	filename:	MServerSocket.h
	created:	2015/3/20 18:01:30
	author:		YvesMa(ma332567575@163.com)	
	Description: ��Ϊ�����ߵ�Socket��
                �����Ⱪ¶

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
        MVOID OnRead( MVOID );                          // �����ݿɶ��¼�֪ͨ

    public:
        // ����
        MBOOL Listen( MLPCSTR szIp, MUI16 uPort, RecvCallback pCallback, MPVOID pCallbackData );
        
    private:
        ListenCallback          m_ListenCallback;       // �����ص�����
        MPVOID                  m_ListenCallbackData;   // �����ص���������������
    };
}

#endif // MServerSocket_h__
