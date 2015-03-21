#include "util.h"

CThreadLock::CThreadLock( void )
{
#ifdef _WIN32
    InitializeCriticalSection( &m_CriticalSection );
#endif
}

CThreadLock::~CThreadLock( void )
{
#ifdef _WIN32
    DeleteCriticalSection( &m_CriticalSection );
#endif
}

void CThreadLock::Lock( void )
{
#ifdef _WIN32
    EnterCriticalSection( &m_CriticalSection );
#endif
}

void CThreadLock::Unlock( void )
{
#ifdef _WIN32
    LeaveCriticalSection( &m_CriticalSection );
#endif
}