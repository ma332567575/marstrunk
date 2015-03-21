#include "BaseCon.h"
#include "netlib.h"

static MVOID NetworkCb( MBYTE EventId, net_handle_t nHandle, MVOID* pCallbackData )
{
    CBaseCon* pCon = (CBaseCon*)pCallbackData;

//     switch( EventId )
//     {
//     case net_event_newclient:
// 
//     }
}

CBaseCon::CBaseCon( MVOID )
{
    
}

CBaseCon::~CBaseCon( MVOID )
{

}

MBOOL CBaseCon::Listen( MLPCSTR szIp, MUI16 uPort )
{
    if ( !szIp || !szIp[0] )
        return MFALSE;

    if ( uPort == 0 )
        return MFALSE;

    return netlib_listen( szIp, uPort, NetworkCb, this );
}