#include "BaseGame.h"
#include "network/global.h"

CBaseGame::CBaseGame( MVOID )
{

}

CBaseGame::~CBaseGame( MVOID )
{
    netlib_close( m_ChatHandle );
}

MVOID CBaseGame::MainLoop( MVOID )
{
    // 初始化WinSock
    MI16 wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;

    if ( WSAStartup(wVersionRequested, &wsaData) != 0 )
    {
        return;
    }

    // 连接服务器
    m_ChatHandle = netlib_connect( "127.0.0.1", 22550, OnNetEvent, this );

    if ( m_ChatHandle == MARS_INVALID_VALUE )
    {
        return;
    }

    while( TRUE )
    {
        MBOOL bRst = netlib_send( m_ChatHandle, (MVOID*)"123", 4 );
        GetReactor( )->HandleEvents( );

        Sleep( 1000 );
    }

    netlib_close( m_ChatHandle );
}

MVOID CBaseGame::OnNetEvent( MBYTE EventId, net_handle_t nHandle, MVOID* pParam )
{

}