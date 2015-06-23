/********************************************************************
	Copyright(C)
	File Name: netlibimp.h
	Creation Data: 2015-06-18 01:16:56 PM
	Author: Yvesma(ma332567575@163.com)
	Description:    

	History:
	1. Create file  on 2015-06-18 01:16:56 PM
	...

*********************************************************************/
#ifndef netlibimp_h__
#define netlibimp_h__

#include "common.h"
#include "mars_stdinclude.h"
#include <sys/epoll.h>
#include "MSimpleBuffer.h"

using namespace Mars;

enum MarsSocketHandlerType
{
    Mars_Socket_Handler_Type_Server,
    Mars_Socket_Handler_Type_Client
};

class ISocketHandler
{
public:
    ISocketHandler( MVOID ) 
    {

    }
    virtual ~ISocketHandler( MVOID )
    {

    }

public:
    MI32 GetSocketType( MVOID )
    {
        return m_SocketType;
    }

    net_handle_t GetSocket( MVOID )
    {
        return m_Socket;
    }

protected:
    MI32            m_SocketType;
    net_handle_t    m_Socket;
};

class CServerSocketHandler : public ISocketHandler
{
public:
    CServerSocketHandler( MLPCSTR szIp, MUI16 uPort );
    virtual ~CServerSocketHandler( MVOID );

public:
    MBOOL Listen( MVOID );
    MBOOL OnListen( MVOID );

private:
    MCHAR m_LocalIp[MARS_NET_IP_STR_LENGTH];
    MUI16 m_ListenPort;

};

class CClientSocketHandler : public ISocketHandler
{
public:
    CClientSocketHandler( MLPCSTR szIp, MUI16 uPort );
    virtual ~CClientSocketHandler( MVOID );

public:
    MBOOL OnRecv( MVOID );
    MBOOL OnSend( MVOID );
    MBOOL Send( MPVOID pBuff, MI32 nLen );

private:
    MCHAR           m_ClientIp[MARS_NET_IP_STR_LENGTH];
    MUI16           m_ClientPort;

    MCSimpleBuffer  m_SendBuffer;
    MBOOL           m_Busy;

};

class IServer;
class CNetLibImp
{
public:
    CNetLibImp( );
    ~CNetLibImp( );

public:
    MBOOL Listen( MUI16* pPortList, MI32 nPortNum, IServer* pServer );
    MBOOL Send( net_handle_t handleSocket, MPVOID pBuf, MI32 nLen );
    MBOOL Close( net_handle_t handleSocket );
    MBOOL OneLoop( MVOID );
private:
    MBOOL AddEpollEvent( net_handle_t handleSocket );
    MBOOL DelEpollEvent( net_handle_t handleSocket );

private:
    MBOOL AddSocketHandler( ISocketHandler* pSocketHandler );
    MBOOL DelSocketHandler( ISocketHandler* pSocketHandler );
    MBOOL DelSocketHandler( net_handle_t handleSocket );

private:
    friend class CServerSocketHandler;
    friend class CClientSocketHandler;

private:
    typedef list<MUI16>             PortList;
    typedef PortList::iterator      PortListIter;

    typedef map<net_handle_t, ISocketHandler*>  SocketHandlerMap;
    typedef SocketHandlerMap::iterator          SocketHandlerMapIter;
    
private:
    IServer*            m_ServerPump;           // Event Pump
    PortList            m_PortSet;              // Listen port set
    SocketHandlerMap    m_SocketHandlerSet;     // Socket handler set
    net_handle_t        m_EpollFd;              // Epoll file description
    struct epoll_event  m_EpollWaitEvents[MARS_NET_MAX_EPOLL_SIZE];

};

#endif
