/********************************************************************
	Copyright(C)
	filename:	BaseServer.h
	created:	2015/2/28 17:24:12
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2015/2/28 17:24:12 Create file
	...

*********************************************************************/
#ifndef BaseServer_h__
#define BaseServer_h__

#include "common.h"
#include "network/netlib.h"

class CBaseServer
{
public:
    CBaseServer( MVOID );
    ~CBaseServer( MVOID );

public:
    MBOOL MainLoop( MVOID );

protected:
    static MVOID OnNetEvent( MBYTE EventId, net_handle_t nHandle, MVOID* pParam );

private:
    typedef std::list<net_handle_t> SocketList;
    typedef SocketList::iterator    SocketListIter;
    
    SocketList m_SocketSet;

};

#endif // BaseServer_h__