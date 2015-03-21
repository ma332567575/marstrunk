/********************************************************************
	Copyright(C)
	filename:	global.h
	created:	2015/1/14 16:48:43
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2015/1/14 16:48:43 Create file
	...

*********************************************************************/

#ifndef global_h__
#define global_h__

#include "singleton.h"
#include "Reactor.h"
#include "MSocketManager.h"

class CGlobal : public CSingleton<CGlobal>
{
public:
    CGlobal( void );
    ~CGlobal( void );

private:
    CReactor* m_pReactor;
    MCSocketManager* m_pSocketManager;
};

CReactor*
GetReactor( );

MCSocketManager*
GetSocketManager( MVOID );

#endif // global_h__