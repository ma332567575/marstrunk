/********************************************************************
	Copyright(C)
	filename:	Reactor.h
	created:	2014/12/30 17:16:32
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2014/12/30 17:16:32 Create file
	...

*********************************************************************/
#ifndef Reactor_h__
#define Reactor_h__

#include <map>
enum EventType
{
    event_read  = 0x01,
    event_write = 0x02,
    event_excep = 0x04
};

typedef int handle_type;
typedef unsigned int event_type;

class IEventHandler
{
public:
    virtual handle_type GetHandle( void ) = 0;
    virtual void OnRead( void ){/**/};
    virtual void OnWrite( void ){/**/};
    virtual void OnError( void ){/**/};

protected:
    IEventHandler( void );
    virtual ~IEventHandler( void );

};

typedef std::map<handle_type, IEventHandler*>   HandlerMap;
typedef HandlerMap::iterator                    HandlerMapIter;

class ReactorImp;
class CReactor
{
public:
    CReactor( void );
    ~CReactor( void );

public:
    void HandleEvents( void );
    
    void RegisterHandler( event_type Event, IEventHandler* pHandler );
    
    void RemoveHandler( event_type Event, IEventHandler* pHandler );

private:
    ReactorImp*              m_ReactorImp;

};

#endif // Reactor_h__