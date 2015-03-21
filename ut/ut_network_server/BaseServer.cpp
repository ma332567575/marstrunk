#include "BaseServer.h"
#include "network/global.h"

CBaseServer::CBaseServer( MVOID )
{

}

CBaseServer::~CBaseServer( MVOID )
{

}

MBOOL CBaseServer::MainLoop( MVOID )
{
    // 初始化WinSock
    MI16 wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    if ( WSAStartup(wVersionRequested, &wsaData) != 0 )
    {
        return MFALSE;
    }

    // 1. 监听IP和PORT
    if ( !netlib_listen( "127.0.0.1", 22550, OnNetEvent, this ) )
    {
        return MFALSE;
    }

    // 2. 开始循环
    while( MTRUE )
    {
        GetReactor( )->HandleEvents( );

        Sleep( 10 );
    }
    
    
    return MTRUE;
}

MVOID CBaseServer::OnNetEvent( MBYTE EventId, net_handle_t nHandle, MVOID* pParam )
{
    CBaseServer* pServer = (CBaseServer*)pParam;

    switch( EventId )
    {
    case net_event_newclient:
        {
            pServer->m_SocketSet.push_front( nHandle );
            break;
        }
    case net_event_read:
        {
            MCHAR pBuff[10];
            memset( pBuff, 0, 10 );

            netlib_recv( nHandle, pBuff, 10 );

            SocketListIter Iter = pServer->m_SocketSet.begin( );

            for ( ;Iter != pServer->m_SocketSet.end( ); ++Iter )
            {
                printf( "Broadcast message to %d. Message:%s \n", *Iter, pBuff );
                netlib_send( *Iter, pBuff, 10 );
            }
        }
    }
}
