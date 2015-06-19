#include "netlib.h"
#include "netlibimp.h"

CNetLibImp* g_clsNetLibImp = new CNetLibImp(  );

MBOOL netlib_listen( MUI16* pPortList, MI32 nPortNum, IServer* pServer )
{
    return g_clsNetLibImp->Listen( pPortList, nPortNum, pServer );
}

MBOOL netlib_send( net_handle_t handleSocket, MVOID* pBuf, MI64 nLen )
{
    return  g_clsNetLibImp->Send( handleSocket, pBuf, nLen );
}

MBOOL netlib_close( net_handle_t handleSocket )
{
    return g_clsNetLibImp->Close( handleSocket );
}

MBOOL netlib_oneloop( MVOID )
{
    return g_clsNetLibImp->OneLoop(  );
}
