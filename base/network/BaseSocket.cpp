#include "BaseSocket.h"
#include "global.h"

BaseSocket::BaseSocket( void )
{
    m_State         = SOCKET_STATE_IDLE;
}

BaseSocket::~BaseSocket( void )
{

}

// SOCKET BaseSocket::Connect( const char* szIp, unsigned short uPort, NetCallback pCallback, MVOID* pCallbackData )
// {
//     // 1. 检查函数运行条件
//     if ( !szIp || !szIp[0] || uPort == 0 )
//         return INVALID_SOCKET;
//     
//     // 2. 创建socket
//     m_Socket = socket( AF_INET, SOCK_STREAM, 0 );
// 
//     if ( m_Socket == INVALID_SOCKET )
//         return INVALID_SOCKET;
// 
//     SetNonblock( m_Socket );
//     SetNoDelay( m_Socket );
// 
//     // 3. 连接某个地址
//     sockaddr_in ServerAddr;
//     SetServerAddr( szIp, uPort, &ServerAddr );
// 
//     int nRet = connect( m_Socket, (sockaddr*)&ServerAddr, sizeof( ServerAddr ) );
// 
//     if ( nRet == SOCKET_ERROR && (!IsBlock( WSAGetLastError( ) ) ) )
//     {
//         closesocket( m_Socket );
//         return INVALID_SOCKET;
//     }
// 
//     // 4. 设置状态
//     m_State = SOCKET_STATE_CONNECTING;
//     m_Callback = pCallback;
//     m_CallbackData = pCallbackData;
//     AddBaseSocket( this );
// 
//     // 5. 连接成功，通知reactor模型
//     GetReactor( )->RegisterHandler( event_read|event_write|event_excep, this );
//     
//     return m_Socket;
// }

int BaseSocket::Close( void )
{
    // 1. 通知reactor模型
    GetReactor( )->RemoveHandler( event_read | event_write | event_excep, this );

    GetSocketManager( )->RemoveSocket( this );
    
    // 2. 释放这个socket
    closesocket( m_Socket );

    return 0;
}

void BaseSocket::OnWrite( void )
{
    GetReactor( )->RemoveHandler( event_write, this );

    // 连接中的
    if ( m_State == SOCKET_STATE_CONNECTING )
    {
        MI32 nError = 0;
        MI32 nLen = sizeof( nError );
        getsockopt( m_Socket, SOL_SOCKET, SO_ERROR, (MCHAR*)&nError, &nLen );
        if ( nError )
        {
            if ( m_Callback )
            {
                m_Callback( net_event_close, m_Socket, m_CallbackData );
            }            
        }
        else
        {
            if ( m_Callback )
            {
                m_Callback( net_event_confirm, m_Socket, m_CallbackData );
            }

            m_State = SOCKET_STATE_CONNECTED;
        }
    }
    else
    {
        if ( m_Callback )
        {
            m_Callback( net_event_write, m_Socket, m_CallbackData );
        }
    }
}

void BaseSocket::SetServerAddr( const char* szIp, const unsigned short uPort, sockaddr_in* pAddr )
{
    // 1. 清空
    memset( pAddr, 0, sizeof( sockaddr_in ) );

    // 2. ipv4协议族
    pAddr->sin_family           = AF_INET;
    pAddr->sin_port             = htons( uPort );           // 网络字节序列转换
    pAddr->sin_addr.s_addr      = inet_addr( szIp );        // ipv4地址
    if ( pAddr->sin_addr.s_addr == INADDR_NONE )    // 尝试dns
    {
        hostent* pHost  = gethostbyname( szIp );
        
        if ( pHost == NULL )// dns失败了
        {
            return;
        }
        
        // dns suc,use dns result.
        pAddr->sin_addr.s_addr = *( unsigned int* )pHost->h_addr;
    }
}

void BaseSocket::SetNoDelay( SOCKET Socket )
{
    BOOL bDelay = TRUE;
    int nRet    = setsockopt( Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&bDelay, sizeof( bDelay ) );

    if ( nRet == SOCKET_ERROR )
    {

    }
}

void BaseSocket::SetNonblock( SOCKET Socket )
{
    unsigned long ulNonblock    = 1;
    int nRet                    = ioctlsocket( Socket, FIONBIO, &ulNonblock );
    if ( nRet == SOCKET_ERROR )
    {

    }
}

void BaseSocket::SetReuseAddr( SOCKET Socket )
{
    BOOL reuse = TRUE;
    MI32 nRet = setsockopt( Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

    if ( nRet == SOCKET_ERROR )
    {

    }
}

bool BaseSocket::IsBlock( int nErrCode )
{
    return ( ( nErrCode == WSAEINPROGRESS ) || ( nErrCode == WSAEWOULDBLOCK ) );
}
