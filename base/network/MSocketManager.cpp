#include "MSocketManager.h"

using namespace Mars;

MCSocketManager::MCSocketManager( MVOID )
{/* */}

MCSocketManager::~MCSocketManager( MVOID )
{/* */}

MVOID MCSocketManager::PushSocket( BaseSocket* pSocket )
{
    m_SocketMap.insert( std::make_pair( pSocket->GetHandle(), pSocket ) );
}

MVOID MCSocketManager::RemoveSocket( BaseSocket* pSocket )
{
    m_SocketMap.erase( pSocket->GetHandle( ) );
}

BaseSocket* MCSocketManager::FindSocket( net_handle_t nSocket )
{
    SocketIter Iter = m_SocketMap.find( nSocket );

    if ( Iter == m_SocketMap.end( ) )
    {
        return MNULL;
    }

    return Iter->second;
}
