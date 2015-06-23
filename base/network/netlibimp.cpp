#include "netlibimp.h"
#include "netlib.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern CNetLibImp* g_clsNetLibImp;
 
CServerSocketHandler::CServerSocketHandler( MLPCSTR szIp, MUI16 uPort )
{
    MMemcpy( m_LocalIp, MARS_NET_IP_STR_LENGTH, szIp, MARS_NET_IP_STR_LENGTH );
    m_ListenPort = uPort;
}

CServerSocketHandler::~CServerSocketHandler( MVOID )
{

}

MBOOL CServerSocketHandler::Listen( MVOID )
{
    // 1. Create server file descrition
    net_handle_t Fd = socket( PF_INET, SOCK_STREAM, 0 );

    if ( Fd == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    // 2. Set nonblock attr
    MI32 nRet = fcntl( Fd, F_SETFL, fcntl( Fd, F_GETFD, 0 ) | O_NONBLOCK );

    if ( nRet == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    // 3. Set buffer size
    MI32 nBuffSize = 174760;
    setsockopt( Fd, SOL_SOCKET, SO_RCVBUF, ( MCONST MI32* )&nBuffSize, sizeof( MI32 ) );
    setsockopt( Fd, SOL_SOCKET, SO_SNDBUF, ( MCONST MI32* )&nBuffSize, sizeof( MI32 ) );

    // 4. Bind
    struct sockaddr_in addrSock;
    memset( &addrSock, 0, sizeof( addrSock ) );
    addrSock.sin_family = PF_INET;
    addrSock.sin_port   = htons( m_ListenPort );

    if ( m_LocalIp == MNULL || MStrcmp( m_LocalIp, "" ) == 0 )
    {
        addrSock.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        addrSock.sin_addr.s_addr = inet_addr( m_LocalIp );
    }

    MI32 nReuseFlag = 1;
    MI32 nReuseFlagLen  = sizeof( int );

    setsockopt( Fd, SOL_SOCKET, SO_REUSEADDR, &nReuseFlag, nReuseFlagLen );

    nRet = bind ( Fd, ( struct sockaddr* )&addrSock, sizeof( addrSock ) );

    if ( nRet != 0 )
    {
        return MFALSE;
    }

    // 5. Listen
    nRet = listen( Fd, 20 );

    if ( nRet == -1 )
    {
        return MFALSE;
    }

    // 6. Insert into epoll
    if ( !g_clsNetLibImp->AddEpollEvent( Fd ) )
    {
        return MFALSE;
    }

    // 7. Set member variable
    m_Socket        = Fd;
    m_SocketType    = Mars_Socket_Handler_Type_Server;

    return MTRUE;
}

MBOOL CServerSocketHandler::OnListen( MVOID )
{
    // 1. Do accept
    struct sockaddr_in addrClient;
    socklen_t lenClient     = sizeof( addrClient );
    net_handle_t ClientFd   = accept( m_Socket, ( struct sockaddr* ) &addrClient, &lenClient );

    if ( lenClient < 0 )
    {
        if ( errno == EAGAIN )
        {
            // TODO: Think over it
            return MFALSE;
        }
        else
        {
            return MFALSE;
        }
    }

    // TODO: Think adding a max client socket num
    
    // 2. Get client address info
    MLPSTR pszClientAddr    = inet_ntoa( addrClient.sin_addr );
    MUI16 unClientPort      = ntohs( addrClient.sin_port  );

    // 3. Notify the IServer
    if ( !g_clsNetLibImp->m_ServerPump )
    {
        close( ClientFd );
        return MFALSE;
    }

    if ( !g_clsNetLibImp->m_ServerPump->OnListen( m_ListenPort, pszClientAddr, unClientPort, ClientFd ) )
    {
        close( ClientFd );
        return MFALSE;
    }

    // 4. Set socket attribution
    MI32 nRet = fcntl( ClientFd, F_SETFL, fcntl( ClientFd, F_GETFD, 0 ) | O_NONBLOCK );

    if ( nRet == MARS_INVALID_VALUE )
    {
        close( ClientFd );
        return MFALSE;
    }

    // 5. Add epoll event
    g_clsNetLibImp->AddEpollEvent( ClientFd );

    // 6. Create Client Socket handler
    CClientSocketHandler* pClientHandler    = MNULL;
    pClientHandler                          = new CClientSocketHandler( pszClientAddr, unClientPort );

    if ( !pClientHandler )
    {
        close( ClientFd );
        g_clsNetLibImp->DelEpollEvent( ClientFd );
        return MFALSE;
    }

    // Insert client socket handler into the socket handler set
    if ( !g_clsNetLibImp->AddSocketHandler( pClientHandler ) )
    {
        close( ClientFd );
        g_clsNetLibImp->DelEpollEvent( ClientFd );
        return MFALSE;
    }

    return MTRUE;
}

CClientSocketHandler::CClientSocketHandler( MLPCSTR szIp, MUI16 uPort )
{
    MMemcpy( m_ClientIp, MARS_NET_IP_STR_LENGTH, szIp, MARS_NET_IP_STR_LENGTH );
    m_ClientPort = uPort;
    m_Busy = MFALSE;
}

CClientSocketHandler::~CClientSocketHandler( MVOID )
{

}

MBOOL CClientSocketHandler::OnRecv( MVOID )
{
    // Fixed data
    // 10 KB
    static MBYTE szBuff[10240];
    memset( szBuff, 0, 10240 );

    MI32 nLen = ::recv( m_Socket, szBuff, 10240, 0 );

    if ( nLen > 0 )
    {
        g_clsNetLibImp->m_ServerPump->OnReceive( m_Socket, szBuff, nLen );
    }
    else if ( nLen == 0 )//Close
    {
        g_clsNetLibImp->m_ServerPump->OnClose( m_Socket );
    }
    else // Error
    {
        g_clsNetLibImp->m_ServerPump->OnClose( m_Socket );
    }

    return MTRUE;    
}

MBOOL CClientSocketHandler::OnSend( MVOID )
{
    if ( !m_Busy )
    {
        return MTRUE;
    }

    while( m_SendBuffer.GetWriteSize(  ) > 0 )
    {
        MI32 nSendSize = m_SendBuffer.GetWriteSize(  );
        if ( nSendSize > 64 * 1024 )
        {
            nSendSize = 64 * 1024;
        }    

        MI32 nRet = send( m_Socket, (MPBYTE)( m_SendBuffer.GetBuffer(  ) ), nSendSize, 0 );

        if ( nRet == 0 )
        {
            break;
        }

        // Block
        if ( nRet < 0 )
        {
            if ( errno == EINPROGRESS || errno == EAGAIN )
            {
                
            }
            break;
        }

        m_SendBuffer.Read( MNULL, nRet );
    }

    if ( m_SendBuffer.GetWriteSize(  ) == 0 )
    {
        m_Busy = MFALSE;
    }

    return MTRUE;
}

MBOOL CClientSocketHandler::Send( MPVOID pBuff, MI32 nLen )
{
    // 1. Socket is busy, write buff to cache
    if ( m_Busy )
    {
        m_SendBuffer.Write( pBuff, nLen );
        return MTRUE;
    }

    MI32 nOffset = 0;
    MI32 nRemain = nLen;

    while ( nRemain > 0 )
    {
        MI32 nSendSize = nRemain;
        // TODO: Replace max buf size with macro
        if ( nSendSize > 64 * 1024 )
        {
            nSendSize = 64 * 1024;
        }

        MI32 nRet = send( m_Socket, (MPBYTE)(pBuff + nOffset), nSendSize, 0 );

        if ( nRet == 0 )
        {
            break;
        }

        // block
        if ( nRet < 0 )
        {
            // TODO: Error is block then cache
            if ( errno == EINPROGRESS || errno == EAGAIN )
            {
                
            }
            break;
        }

        nOffset += nRet;
        nRemain -= nRet;
        
    }

    // Cache remain data
    if ( nRemain > 0 )
    {
        m_SendBuffer.Write( (MPBYTE)( pBuff + nOffset ), nRemain );
        m_Busy = MTRUE;
    }
    return MTRUE;
}

CNetLibImp::CNetLibImp( ):m_ServerPump(MNULL), m_EpollFd( MARS_INVALID_VALUE )
{
    
}

CNetLibImp::~CNetLibImp( ) {

}

MBOOL CNetLibImp::Listen( MUI16* pPortList, MI32 nPortNum, IServer* pServer )
{
    // 1. Check arg
    // 1.1 Is already running?
    if ( m_ServerPump )
    {
        return MFALSE;
    }

    // 1.2 pServer cant null
    if ( !pServer )
    {
        return MFALSE;
    }

    // 1.3 Port number must be bigger than 0 
    if ( nPortNum <= 0 )
    {
        return MFALSE;
    }

    // 2. Filter the port from exist port and portlist arg.
    PortList portlistHelper;
    for ( MI32 nPortIdx = 0; nPortIdx < nPortNum; ++nPortNum )
    {
        PortListIter findIter = std::find( m_PortSet.begin(  ), m_PortSet.end(  ), pPortList[nPortIdx] );

        if ( findIter != m_PortSet.end(  ) )
        {
            continue;
        }

        portlistHelper.push_back( pPortList[nPortIdx] );
        m_PortSet.push_back( pPortList[nPortIdx] );
    }

    // 3. Create epoll interface
    m_EpollFd = epoll_create( MARS_NET_MAX_EPOLL_SIZE );

    if ( m_EpollFd == -1 )
    {
        return MFALSE;
    }

    // 4. Listen port
    PortListIter ExistPortIter = portlistHelper.begin(  );

    for ( ; ExistPortIter != portlistHelper.end(  ); ++ExistPortIter )
    {
        // TODO: Add a function that get local ip.
        CServerSocketHandler* pServerHandler = MNULL;
        pServerHandler = new CServerSocketHandler( "127.0.0.1", *ExistPortIter );

        if ( pServerHandler == MNULL )
        {
            // Leak memory
            return MFALSE;
        }

        if ( !pServerHandler->Listen(  ) )
        {
            return MFALSE;
        }

        // Insert into sockethandler map.
        if ( !AddSocketHandler( pServerHandler ) )
        {
            // Add failed
            return MFALSE; 
        }
    }

    m_ServerPump = pServer;

    return MTRUE;
}

MBOOL CNetLibImp::Send( net_handle_t handleSocket, MPVOID pBuf, MI32 nLen )
{
    // 1. Find the socket handler
    SocketHandlerMapIter Iter = m_SocketHandlerSet.find( handleSocket ); 

    if ( Iter == m_SocketHandlerSet.end(  ) )
    {
        return MFALSE;
    }

    // Check socket handler type
    if ( Iter->second->GetSocketType(  ) != Mars_Socket_Handler_Type_Client )
    {
        return MFALSE;
    }

    // 2. Call socket handler send method
    // force transfer
    CClientSocketHandler* pHandler = reinterpret_cast<CClientSocketHandler*> (Iter->second);
    return pHandler->Send( pBuf, nLen );
}

MBOOL CNetLibImp::Close( net_handle_t handleSocket )
{
    return MTRUE;
}

MBOOL CNetLibImp::OneLoop( MVOID )
{
    // 1. Check epoll fd
    if ( m_EpollFd == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    // 2. Get available fd count
    int nValidFdCount       = m_SocketHandlerSet.size(  );
    int nAvailableFdCount   = epoll_wait( m_EpollFd, m_EpollWaitEvents, nValidFdCount, -1 );
    
    // 3. Check available fd count
    if ( nAvailableFdCount == MARS_INVALID_VALUE )
    {
        if ( errno == EINTR )
        {
            return MTRUE;
        }
        else
        {
            return MFALSE;
        }
    }

    // Time out
    if ( nAvailableFdCount == 0 )
    { 
        // TODO: Deal timeout 
    }

    // Trigger read | write callback
    for ( MI32 nIdx = 0; nIdx < nAvailableFdCount; ++nIdx )
    {
        net_handle_t Fd = m_EpollWaitEvents[nIdx].data.fd;

        SocketHandlerMapIter Iter = m_SocketHandlerSet.find( Fd );

        // The socket is not in the handler set
        if ( Iter == m_SocketHandlerSet.end(  ) )
        {
            continue;
        }

        ISocketHandler* pISocketHandler = Iter->second;

        // Switch sockethandler type
        switch( Iter->second->GetSocketType(  ) )
        {
            case Mars_Socket_Handler_Type_Server:
                {
                    // Constraint convert
                    CServerSocketHandler* pServerHandler = reinterpret_cast<CServerSocketHandler*>( Iter->second );
                    
                    pServerHandler->OnListen(  );
                }
                break;
            case Mars_Socket_Handler_Type_Client:
                {
                    CClientSocketHandler* pClientHandler = reinterpret_cast<CClientSocketHandler*>( Iter->second );

                    // Write
                    if ( m_EpollWaitEvents[nIdx].events & EPOLLOUT )
                    {
                        pClientHandler->OnRecv(  );
                    }
                    // Read
                    if ( m_EpollWaitEvents[nIdx].events & EPOLLIN )
                    {
                        pClientHandler->OnSend(  );
                    }

                }
                break;
            default:
                {
                    //TODO: Deal the default case
                }
                break;
        }
    } 

    return MTRUE;
}

MBOOL CNetLibImp::AddEpollEvent( net_handle_t handleSocket )
{
    // 1. Check epoll fd
    if ( m_EpollFd == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    struct epoll_event ev;
    ev.events   = EPOLLIN;
    ev.data.fd  = handleSocket;
    
    if ( epoll_ctl( m_EpollFd, EPOLL_CTL_ADD, handleSocket, &ev ) == -1 )
    {
        return MFALSE;
    }

    return MTRUE;
}

MBOOL CNetLibImp::DelEpollEvent( net_handle_t handleSocket )
{
    if ( m_EpollFd == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    if (  epoll_ctl( m_EpollFd, EPOLL_CTL_DEL, handleSocket, MNULL ) == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    return MTRUE;
}

MBOOL CNetLibImp::AddSocketHandler( ISocketHandler* pSocketHandler )
{
    // 1. Check arg
    if ( !pSocketHandler )
    {
        return MFALSE;
    }

    // 2. Check whether the socket already exists?
    net_handle_t handleSocket = pSocketHandler->GetSocket(  );

    if ( handleSocket == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    SocketHandlerMapIter Iter = m_SocketHandlerSet.find( handleSocket );

    if ( Iter != m_SocketHandlerSet.end(  ) )
    {
        return MFALSE;
    }

    // 3. Insert data into set
    m_SocketHandlerSet.insert( std::make_pair( handleSocket, pSocketHandler ) );

    return MTRUE;
}

MBOOL CNetLibImp::DelSocketHandler( ISocketHandler* pSocketHandler )
{
    if ( !pSocketHandler )
    {
        return MFALSE;
    }

    net_handle_t handleSocket = pSocketHandler->GetSocket(  );

    return DelSocketHandler( handleSocket );
}

MBOOL CNetLibImp::DelSocketHandler( net_handle_t handleSocket )
{
    if ( handleSocket == MARS_INVALID_VALUE )
    {
        return MFALSE;
    }

    m_SocketHandlerSet.erase( handleSocket );

    return MTRUE;
}
