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

    // 获取状态
    if ( ( ioctlsocket( m_Socket, FIONREAD, &u64Availed ) == SOCKET_ERROR ) || 
        ( u64Availed == 0 ) )
    {
        // TODO: 错误了，断开连接的处理
        return;
    }
     
    // 读取数据，用BUFFER类
    while( TRUE )
    {
        MI64 n64AvailabeSize = m_RecvBuffer.GetAllocSize( ) - m_RecvBuffer.GetWrietSize( );

        // 不够写
        if ( n64AvailabeSize < MARS_NETWORD_CLIENT_SOCKET_READ_SIZE )
        {
            m_RecvBuffer.ExpendBuffer( MARS_NETWORD_CLIENT_SOCKET_READ_SIZE );
        }

        // 接收
        MI32 nRet = recv( m_Socket, m_RecvBuffer.GetBuffer( ), MARS_NETWORD_CLIENT_SOCKET_READ_SIZE );
        
        // 没得读取了
        if ( nRet == 0 )
        {
            break;
        }

        // SocketError
        if ( nRet == SOCKET_ERROR )
        {
            break;
        }

        // 改变大小
        m_RecvBuffer.ModWriteSize( nRet );
    }

    // 把读到的数据放到回调函数
    if ( m_RecvCallback )
    {
        m_RecvCallback( m_RecvBuffer.GetBuffer( ), m_RecvBuffer.GetWrietSize( ) );
    }

    // 扔掉读到的数据
    m_RecvBuffer.CleanBuffer( );
}

MVOID MCClientSocket::OnWrite( MVOID )
{
    
}

MBOOL MCClientSocket::Send( MVOID* pBuf, MI64 nLen )
{
    // 1. Socket忙呢，就先搞到缓存里
    if ( m_Busy )
    {
        m_SendBuffer.Write( pBuf, nLen );
        return MTRUE;
    }

    // 2. 尝试发送吧
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

        // 没得发
        if ( nRet == 0 )
        {
            break;
        }

        // 阻塞状态
        if ( nRet == SOCKET_ERROR )
        {
            // 阻塞中，加入队列等待发送即可
            if ( IsBlock( WSAGetLastError( ) ) )
            {
                GetReactor( )->RegisterHandler( event_write, this );
            }

            break;
        }

        // 发出去了，减少发送值，继续发送
        n64Offset   += nRet;
        n64Remain   -= nRet;
    }

    // 没法出去的都存起来
    if ( n64Remain > 0 )
    {
        m_SendBuffer.Write( (MPBYTE)( pBuf + n64Offset), n64Remain );
        m_Busy = MTRUE;
    }

    return MTRUE;
}

MVOID MCClientSocket::OnWrite( MVOID )
{
    // 可发送数据了，就把缓冲发出去
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

        // 没得发
        if ( nRet == 0 )
        {
            break;
        }

        // 阻塞状态
        if ( nRet == SOCKET_ERROR )
        {
            // 阻塞中，加入队列等待发送即可
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
