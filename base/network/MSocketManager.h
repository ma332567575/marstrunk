/********************************************************************
	Copyright(C)
	filename:	MSocketManager.h
	created:	2015/3/21 16:46:21
	author:		YvesMa(ma332567575@163.com)	
	Description: Socketπ‹¿Ì

	History:
	1. 2015/3/21 16:46:21 Create file
	...

*********************************************************************/
#ifndef MSocketManager_h__
#define MSocketManager_h__

namespace Mars
{
    class MCSocketManager
    {
    public:
        MCSocketManager( MVOID );
        ~MCSocketManager( MVOID );

    public:
        MVOID PushSocket( BaseSocket* pSocket );
        MVOID RemoveSocket( BaseSocket* pSocket );
        BaseSocket* FindSocket( net_handle_t nSocket );

    private:
        typedef std::map<net_handle_t, BaseSocket*> SocketMap;
        typedef SocketMap::iterator                 SocketIter;
        SocketMap   m_SocketMap;
    };
}

#endif // MSocketManager_h__