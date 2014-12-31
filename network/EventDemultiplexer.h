/********************************************************************
	Copyright(C)
	filename:	EventDemultiplexer.h
	created:	2014/12/31 8:57:59
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2014/12/31 8:57:59 Create file
	...

*********************************************************************/
#ifndef EventDemultiplexer_h__
#define EventDemultiplexer_h__

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include "Reactor.h"
#include "util.h"

class CEventDemultiplexer
{
public:
    CEventDemultiplexer( void );
    ~CEventDemultiplexer( void );

public:
#ifdef _WIN32
    void RequestEvent( handle_type Handle, event_type Event );
    void UnrequestEvent( handle_type Handle, event_type Event );
    void WaitEvent( HandlerMap* pHandlerSet );
#endif

private:
#ifdef _WIN32
    fd_set  m_Readset;
    fd_set  m_Writeset;
    fd_set  m_Excepset;
#endif

    CThreadLock     m_SetLock;


};
#endif // EventDemultiplexer_h__