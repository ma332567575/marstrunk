#include "EventDemultiplexer.h"

CEventDemultiplexer::CEventDemultiplexer( void )
{
#ifdef _WIN32
    FD_ZERO(&m_Readset);
    FD_ZERO(&m_Writeset);
    FD_ZERO(&m_Excepset);
#endif
}

CEventDemultiplexer::~CEventDemultiplexer( void )
{

}

#ifdef _WIN32
void CEventDemultiplexer::RequestEvent( handle_type Handle, event_type Event )
{
    if ( ( Event & event_read ) != 0 )
    {
        FD_SET( Handle, &m_Readset );
    }

    if ( ( Event & event_write ) != 0 )
    {
        FD_SET( Handle, &m_Writeset );
    }

    if ( ( Event & event_excep ) != 0 )
    {
        FD_SET( Handle, &m_Excepset );
    }
}

void CEventDemultiplexer::UnrequestEvent( handle_type Handle, event_type Event )
{
    if ( ( Event & event_read ) != 0 )
    {
        FD_CLR( Handle, &m_Readset );
    }

    if ( ( Event & event_write ) != 0 )
    {
        FD_CLR( Handle, &m_Writeset );
    }

    if ( ( Event & event_excep ) != 0 )
    {
        FD_CLR( Handle, &m_Excepset );
    }
}

void CEventDemultiplexer::WaitEvent( HandlerMap* pHandlerSet )
{
    if ( m_Readset.fd_count == 0 && m_Writeset.fd_count == 0 && m_Excepset.fd_count == 0 )
        return;

    fd_set ReadSet, WriteSet, ExcepSet;
    
    m_SetLock.Lock( );
    memcpy( &ReadSet, &m_Readset, sizeof( fd_set ) );
    memcpy( &WriteSet, &m_Writeset, sizeof( fd_set ) );
    memcpy( &ExcepSet, &m_Excepset, sizeof( fd_set ) );
    m_SetLock.Unlock( );

    timeval Timeout;
    Timeout.tv_sec  = 0;
    Timeout.tv_usec = 10 * 1000;

    int nRes    = select( 0, &m_Readset, &m_Writeset, &m_Excepset, &Timeout );

    if ( nRes == SOCKET_ERROR || nRes == 0 )
    {
        return;
    }

    for ( int nIdx = 0; nIdx < m_Readset.fd_count; nIdx++ )
    {
        HandlerMapIter Iter = pHandlerSet->find( m_Readset.fd_array[nIdx] );
        if ( Iter == pHandlerSet->end( ) )
            continue;

        Iter->second->OnRead( );
    }

    for ( int nIdx = 0; nIdx < m_Writeset.fd_count; nIdx++ )
    {
        HandlerMapIter Iter = pHandlerSet->find( m_Writeset.fd_array[nIdx] );
        if ( Iter == pHandlerSet->end( ) )
            continue;

        Iter->second->OnWrite( );
    }

    for ( int nIdx = 0; nIdx < m_Excepset.fd_count; nIdx++ )
    {
        HandlerMapIter Iter = pHandlerSet->find( m_Excepset.fd_array[nIdx] );
        if ( Iter == pHandlerSet->end( ) )
            continue;

        Iter->second->OnError( );
    }

}
#endif