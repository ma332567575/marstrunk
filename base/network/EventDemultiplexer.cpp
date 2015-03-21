#include "EventDemultiplexer.h"

using namespace Mars;

CEventDemultiplexer::CEventDemultiplexer( void )
{
    FD_ZERO(&m_Readset);
    FD_ZERO(&m_Writeset);
    FD_ZERO(&m_Excepset);
}

CEventDemultiplexer::~CEventDemultiplexer( void )
{

}

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

    int nRes    = select( 0, &ReadSet, &WriteSet, &ExcepSet, &Timeout );

    if ( nRes == SOCKET_ERROR || nRes == 0 )
    {
        return;
    }

    for ( MUI32 uIdx = 0; uIdx < ReadSet.fd_count; ++uIdx )
    {
        if ( !FD_ISSET( ReadSet.fd_array[uIdx], &ReadSet ) )
            continue;

        HandlerMapIter Iter = pHandlerSet->find( ReadSet.fd_array[uIdx] );
        if ( Iter == pHandlerSet->end( ) )
            continue;

        Iter->second->OnRead( );
    }

    for ( MUI32 uIdx = 0; uIdx < WriteSet.fd_count; uIdx++ )
    {
        if ( !FD_ISSET( WriteSet.fd_array[uIdx], &WriteSet ) )
            continue;

        HandlerMapIter Iter = pHandlerSet->find( WriteSet.fd_array[uIdx] );
        if ( Iter == pHandlerSet->end( ) )
            continue;

        Iter->second->OnWrite( );
    }

    for ( MUI32 uIdx = 0; uIdx < ExcepSet.fd_count; uIdx++ )
    {
        if ( !FD_ISSET( ExcepSet.fd_array[uIdx], &ExcepSet ) )
            continue;

        HandlerMapIter Iter = pHandlerSet->find( ExcepSet.fd_array[uIdx] );
        if ( Iter == pHandlerSet->end( ) )
            continue;

        Iter->second->OnError( );
    }

}

bool CEventDemultiplexer::IsAllUnrequest( handle_type Handle )
{
    if ( FD_ISSET( Handle, &m_Readset ) )
    {
        return false;
    }

    if ( FD_ISSET( Handle, &m_Writeset ) )
    {
        return false;
    }

    if ( FD_ISSET( Handle, &m_Excepset ) )
    {
        return false;
    }

    return true;
}
