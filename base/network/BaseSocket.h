/********************************************************************
	Copyright(C)
	filename:	BaseSocket.h
	created:	2015/1/13 17:01:57
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2015/1/13 17:01:57 Create file
	...

*********************************************************************/

#ifndef BaseSocket_h__
#define BaseSocket_h__

#include "Reactor.h"
#include "common.h"

enum
{
    SOCKET_STATE_IDLE,
    SOCKET_STATE_LISTENING,
    SOCKET_STATE_CONNECTING,
    SOCKET_STATE_CONNECTED,
    SOCKET_STATE_CLOSING
};

enum
{
    MARS_SOCKET_TYPE_SERVER,
    MARS_SOCKET_TYPE_CLIENT,

    MARS_SOCKET_TYPE_NUM
};

class BaseSocket : public IEventHandler
{
public:
    BaseSocket( void );
    virtual ~BaseSocket( void );

public:
    // parent class method
    handle_type GetHandle( void ){ return m_Socket; }
    virtual void OnRead( void );
    virtual void OnWrite( void );
    virtual void OnError( void );

public:
    //SOCKET Connect( const char* szIp, unsigned short uPort, NetCallback pCallback, MVOID* pCallbackData );
    int Close( void );
    MBYTE GetType( MVOID ) MCONST
    { return m_Type; }

protected:
    void SetServerAddr( const char* szIp, const unsigned short uPort, sockaddr_in* pAddr );
    void SetNoDelay( SOCKET Socket );
    void SetNonblock( SOCKET Socket );
    void SetReuseAddr( SOCKET Socket );
    bool IsBlock( int nErrCode );

    void SetSocket( net_handle_t Socket )
    { m_Socket = Socket; }
    void SetState( unsigned char uState )
    { m_State = uState; }
    MVOID SetType( MBYTE nType )
    { 
        if ( nType < MARS_SOCKET_TYPE_SERVER || nType >= MARS_SOCKET_TYPE_NUM )
            return;

        m_Type = nType;
    }


private:
    SOCKET          m_Socket;       // SOCKET¾ä±úÖµ
    unsigned char   m_State;        // SOKCET×´Ì¬
    MBYTE           m_Type;         // SOCKETÀàÐÍ

};

BaseSocket* FindBaseSocket( net_handle_t Socket );

#endif // BaseSocket_h__