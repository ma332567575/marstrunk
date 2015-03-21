#include "MSyncTool.h"

using namespace Mars;

MVOID Mars::MarsInitCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    InitializeCriticalSection( pCSection );
#endif
}

MVOID Mars::MarsDelCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    DeleteCriticalSection( pCSection );
#endif
}

MVOID Mars::MarsEnterCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    EnterCriticalSection( pCSection );
#endif
}

MVOID Mars::MarsLeaveCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    LeaveCriticalSection( pCSection );
#endif
}