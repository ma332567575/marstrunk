#include "MSyncTool.h"

using namespace Mars;

MVOID Mars::MarsInitCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    InitializeCriticalSection( pCSection );
#else
    ::pthread_mutex_init( pCSection, MNULL );
#endif
}

MVOID Mars::MarsDelCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    DeleteCriticalSection( pCSection );
#else
    ::pthread_mutex_destroy( pCSection );
#endif
}

MVOID Mars::MarsEnterCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    EnterCriticalSection( pCSection );
#else
    ::pthread_mutex_lock( pCSection );
#endif
}

MVOID Mars::MarsLeaveCriticalSection( MARS_PCRITICAL_SECTION pCSection )
{
#ifdef _WIN32
    LeaveCriticalSection( pCSection );
#else
    ::pthread_mutex_unlock( pCSection );
#endif
}
