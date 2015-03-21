#include "BaseServer.h"

void main( )
{
    CBaseServer* pService = new CBaseServer;

    if ( !pService )
        return;

    pService->MainLoop( );

    delete pService;

    return;
}