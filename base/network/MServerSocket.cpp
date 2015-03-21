#include "MServerSocket.h"
#include "MClientSocket.h"

using namespace Mars;

MCServerSocket::MCServerSocket( MVOID ): m_ListenCallback(MNULL), m_ListenCallbackData(MNULL)
{
    m_Type = MARS_SOCKET_TYPE_SERVER;
}

MCServerSocket::~MCServerSocket( MVOID )
{ /**/ }

MVOID MCServerSocket::OnRead( MVOID )
{
    SOCKET Socket = 0;
    sockaddr_in PeerAddr;
    MI32 nAddrLen = sizeof( sockaddr_in );

    // 非阻塞的accept
    while( ( Socket = accept( m_Socket, (sockaddr*)&PeerAddr, &nAddrLen ) ) != INVALID_SOCKET )
    {
        // 监听者这一端的socket
        // 创建ClientSocket
        MCClientSocket* pSocket = new MCClientSocket( );

        MUI32 uIp = ntohl( PeerAddr.sin_addr.s_addr );
        MUI16 uPort = ntohs( PeerAddr.sin_port );

        // Socket属性设置
        SetNoDelay( Socket );
        SetNonblock( Socket );

        pSocket->SetSocket( Socket );
        pSocket->SetState( SOCKET_STATE_CONNECTED );
        
        //AddBaseSocket( pSocket );
        // 设置到Reactor模型
        GetReactor( )->RegisterHandler( event_read | event_excep, pSocket );

        // 添加到全局Socket列表
        GetSocketManager( )->PushSocket( pSocket );

        // 从监听回调里面取出ClientSocket接收数据的回调函数
        RecvCallback pClientRecvCallback = MNULL;
        MPVOID pClientCallbackData = MNULL;
        if ( m_ListenCallback )
        {
            m_ListenCallback( Socket, m_ListenCallbackData, pClientRecvCallback, pClientCallbackData );
        }
        
        // 设置取出来的回调函数
        pSocket->SetRecvCallback( pClientRecvCallback );
        pSocket->SetRecvCallbackData( pClientCallbackData );

    }
}

MBOOL MCServerSocket::Listen( MLPCSTR szIp, MUI16 uPort, RecvCallback pCallback, MPVOID pCallbackData )
{
    // 1. 函数运行条件检查
    if ( szIp == MNULL || !szIp[0] || uPort == 0 )
        return MFALSE;

    // 2. 创建用于监听的socket
    m_Socket = socket( AF_INET, SOCK_STREAM, 0 );

    if ( m_Socket == INVALID_SOCKET )
    {
        return MFALSE;
    }

    SetReuseAddr( m_Socket );
    SetNonblock( m_Socket );

    // 3. 绑定监听地址
    sockaddr_in ServerAddr;
    SetServerAddr( szIp, uPort, &ServerAddr );

    MI32 nRet    = bind( m_Socket, (sockaddr*)&ServerAddr, sizeof( ServerAddr ) );

    if ( nRet == SOCKET_ERROR )
    {
        closesocket( m_Socket );
        return MFALSE;
    }

    // 4. 监听
    nRet        = listen( m_Socket, MARS_NET_SELECT_NUM );

    if ( nRet == SOCKET_ERROR )
    {
        closesocket( m_Socket );
        return MFALSE;
    }

    // 5. 设置socket状态
    m_State = SOCKET_STATE_LISTENING;
    m_ListenCallback = pCallback;
    m_ListenCallbackData = pCallbackData;

    // 6. 通知reactor模型
    GetSocketManager( )->PushSocket( this );
    GetReactor( )->RegisterHandler( event_read|event_excep, this );

    return MTRUE;
}