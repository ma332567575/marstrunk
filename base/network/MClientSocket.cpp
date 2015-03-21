#include "MClientSocket.h"

using namespace Mars;

#define MARS_NETWORD_CLIENT_SOCKET_READ_SIZE        ( 1024 )

MCClientSocket::MCClientSocket( MVOID ):m_RecvCallback(MNULL), m_RecvCallbackData(MNULL), m_Busy(MFALSE)
{
    m_Type = MARS_SOCKET_TYPE_CLIENT;
}

MCClientSocket::~MCClientSocket( MVOID )
{/* */}

MVOID MCClientSocket::OnRead( MVOID )
{
    MUI64 u64Availed = 0;

    // ��ȡ״̬
    if ( ( ioctlsocket( m_Socket, FIONREAD, &u64Availed ) == SOCKET_ERROR ) || 
        ( u64Availed == 0 ) )
    {
        // TODO: �����ˣ��Ͽ����ӵĴ���
        return;
    }
     
    // ��ȡ���ݣ���BUFFER��
    while( TRUE )
    {
        MI64 n64AvailabeSize = m_RecvBuffer.GetAllocSize( ) - m_RecvBuffer.GetWrietSize( );

        // ����д
        if ( n64AvailabeSize < MARS_NETWORD_CLIENT_SOCKET_READ_SIZE )
        {
            m_RecvBuffer.ExpendBuffer( MARS_NETWORD_CLIENT_SOCKET_READ_SIZE );
        }

        // ����
        MI32 nRet = recv( m_Socket, m_RecvBuffer.GetBuffer( ), MARS_NETWORD_CLIENT_SOCKET_READ_SIZE );
        
        // û�ö�ȡ��
        if ( nRet == 0 )
        {
            break;
        }

        // SocketError
        if ( nRet == SOCKET_ERROR )
        {
            break;
        }

        // �ı��С
        m_RecvBuffer.ModWriteSize( nRet );
    }

    // �Ѷ��������ݷŵ��ص�����
    if ( m_RecvCallback )
    {
        m_RecvCallback( m_RecvBuffer.GetBuffer( ), m_RecvBuffer.GetWrietSize( ) );
    }

    // �ӵ�����������
    m_RecvBuffer.CleanBuffer( );
}

MVOID MCClientSocket::OnWrite( MVOID )
{
    
}

MBOOL MCClientSocket::Send( MVOID* pBuf, MI64 nLen )
{
    // 1. Socketæ�أ����ȸ㵽������
    if ( m_Busy )
    {
        m_SendBuffer.Write( pBuf, nLen );
        return MTRUE;
    }

    // 2. ���Է��Ͱ�
    MI64 n64Offset = 0;
    MI64 n64Remain = nLen;

    while( n64Remain > 0 )
    {
        MI64 n64SendSize = n64Remain;

        if ( n64SendSize > MARS_NET_MAX_SEND_SOCKET_BUF_SIZE )
        {
            n64SendSize = MARS_NET_MAX_SEND_SOCKET_BUF_SIZE;
        }
        
        MI64 nRet = send( m_Socket, (MPBYTE)(pBuf+n64Offset), n64SendSize, 0 );

        // û�÷�
        if ( nRet == 0 )
        {
            break;
        }

        // ����״̬
        if ( nRet == SOCKET_ERROR )
        {
            // �����У�������еȴ����ͼ���
            if ( IsBlock( WSAGetLastError( ) ) )
            {
                GetReactor( )->RegisterHandler( event_write, this );
            }

            break;
        }

        // ����ȥ�ˣ����ٷ���ֵ����������
        n64Offset   += nRet;
        n64Remain   -= nRet;
    }

    // û����ȥ�Ķ�������
    if ( n64Remain > 0 )
    {
        m_SendBuffer.Write( (MPBYTE)( pBuf + n64Offset), n64Remain );
        m_Busy = MTRUE;
    }

    return MTRUE;
}

MVOID MCClientSocket::OnWrite( MVOID )
{
    // �ɷ��������ˣ��Ͱѻ��巢��ȥ
    if ( !m_Busy )
    {
        return;
    }

    while( m_SendBuffer.GetWrietSize( ) > 0 )
    {
        MI64 n64SendSize = m_SendBuffer.GetWrietSize( );

        if ( n64SendSize > MARS_NET_MAX_SEND_SOCKET_BUF_SIZE )
        {
            n64SendSize = MARS_NET_MAX_SEND_SOCKET_BUF_SIZE;
        }

        MI64 nRet = send( m_Socket, (MPBYTE)(pBuf+n64Offset), n64SendSize, 0 );

        // û�÷�
        if ( nRet == 0 )
        {
            break;
        }

        // ����״̬
        if ( nRet == SOCKET_ERROR )
        {
            // �����У�������еȴ����ͼ���
            if ( IsBlock( WSAGetLastError( ) ) )
            {
                GetReactor( )->RegisterHandler( event_write, this );
            }

            break;
        }

        m_SendBuffer.Read( MNULL, nRet );
    }

    if ( m_SendBuffer.GetWrietSize( ) == 0 )
    {
        m_Busy = MFALSE;
    }
}
