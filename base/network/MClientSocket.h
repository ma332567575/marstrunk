/********************************************************************
	Copyright(C)
	filename:	MClientSocket.h
	created:	2015/3/21 11:43:22
	author:		YvesMa(ma332567575@163.com)	
	Description: �ͻ���Socket��
                �����Ⱪ¶

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
        // �����ݿɶ�
        MVOID OnRead( MVOID );
        // �����ݿ�д
        MVOID OnWrite( MVOID );
        // �д���
        MVOID OnError( MVOID );

    public:
        // ���ý������ݻص�����
        MVOID SetRecvCallback( RecvCallback pCallback )
        { m_RecvCallback = pCallback; }

        // ���ý������ݻص�����������������
        MVOID SetRecvCallbackData( MPVOID pData )
        { m_RecvCallbackData = pData; }

        // ������Ϣ
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
