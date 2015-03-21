#include "netlib.h"
#include "BaseSocket.h"

MBOOL netlib_listen( MLPCSTR szIp, MUI16 uPort, ListenCallback pCallback, MVOID* pCallbackData )
{
    MCServerSocket* pSocket = new MCServerSocket( );

    if ( !pSocket )
        return MFALSE;

    return pSocket->Listen( szIp, uPort, pCallback, pCallbackData );
}

MBOOL netlib_send( net_handle_t Socket, MVOID* pBuf, MI64 nLen )
{
    BaseSocket* pSocket = GetSocketManager->FindSocket( Socket );

    if ( !pSocket )
        return MFALSE;

    if ( pSocket->GetType( ) != MARS_SOCKET_TYPE_CLIENT )
        return MFALSE;

    ((MCClientSocket*)pSocket)->Send( pBuf, nLen );

    return MTRUE;
}

MBOOL netlib_close( net_handle_t Socket )
{
    BaseSocket* pSocket = FindBaseSocket( Socket );

    if ( !pSocket )
        return FALSE;

    pSocket->Close( );

    return MTRUE;
}
