#include "BaseGame.h"

MVOID main( MVOID )
{
    CBaseGame* pGame = new CBaseGame;

    if ( !pGame )
        return;

    pGame->MainLoop( );

    delete pGame;
}