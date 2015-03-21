#include "Reactor.h"
#include "EventDemultiplexer.h"

using namespace Mars;

namespace Mars
{
    class ReactorImp
    {
    public:
        ReactorImp( void ){/**/}
        ~ReactorImp( void ){/**/}

    public:
        void RegisterHandler( event_type Event, IEventHandler* pHandler );
        void RemoveHandler( event_type Event, IEventHandler* pHandler );
        void HandleEvents( void );

    private:
        HandlerMap              m_HandlerSet;
        CEventDemultiplexer     m_EventDemultiplexer;

    };

}

CReactor::CReactor( void )
{
    m_ReactorImp    = new ReactorImp;
}

CReactor::~CReactor( void )
{
    if ( m_ReactorImp )
        delete m_ReactorImp;
}

void CReactor::RegisterHandler( event_type Event, IEventHandler* pHandler )
{
    m_ReactorImp->RegisterHandler( Event, pHandler );
}

void CReactor::RemoveHandler( event_type Event, IEventHandler* pHandler )
{
    m_ReactorImp->RemoveHandler( Event, pHandler );
}

void CReactor::HandleEvents( void )
{
    m_ReactorImp->HandleEvents( );
}

void ReactorImp::RegisterHandler( event_type Event, IEventHandler* pHandler )
{
    handle_type nHandle = pHandler->GetHandle( );

    if ( m_HandlerSet.find( nHandle ) == m_HandlerSet.end( ) )
    {
        m_HandlerSet[nHandle]   = pHandler;
    }

    m_EventDemultiplexer.RequestEvent( nHandle, Event );
}

void ReactorImp::RemoveHandler( event_type Event, IEventHandler* pHandler )
{
    handle_type nHandle = pHandler->GetHandle( );

    if ( m_HandlerSet.find( nHandle ) == m_HandlerSet.end( ) )
        return;

    m_EventDemultiplexer.UnrequestEvent( nHandle, Event );

    if ( !m_EventDemultiplexer.IsAllUnrequest( nHandle ) )
        return;

    m_HandlerSet.erase( nHandle );
}

void ReactorImp::HandleEvents( void )
{
    m_EventDemultiplexer.WaitEvent( &m_HandlerSet );
}
