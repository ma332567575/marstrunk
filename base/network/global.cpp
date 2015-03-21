#include "global.h"

DECLARE_SINGLETON_MEBER(CGlobal)

CGlobal::CGlobal( void )
{
    m_pReactor          = new CReactor( );
    m_pSocketManager    = new MCSocketManager( );
}

CGlobal::~CGlobal( void )
{
    delete m_pReactor;
    m_pReactor = 0;

    delete m_pSocketManager;
    m_pSocketManager = 0;
}

CReactor*
GetReactor( )
{
    return CGlobal::Instance( )->m_pReactor;
}

MCSocketManager*
GetSocketManager( MVOID )
{
    return CGlobal::Instance( )->m_pSocketManager;
}
