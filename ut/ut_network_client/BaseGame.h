/********************************************************************
	Copyright(C)
	filename:	BaseGame.h
	created:	2015/2/28 18:38:39
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2015/2/28 18:38:39 Create file
	...

*********************************************************************/
#ifndef BaseGame_h__
#define BaseGame_h__

#include "common.h"

class CBaseGame
{
public:
    CBaseGame( MVOID );
    ~CBaseGame( MVOID );

public:
    MVOID MainLoop( MVOID );

protected:
    static MVOID OnNetEvent( MBYTE EventId, net_handle_t nHandle, MVOID* pParam );

private:
    net_handle_t m_ChatHandle;

};
#endif // BaseGame_h__