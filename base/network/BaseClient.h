/********************************************************************
	Copyright(C)
	filename:	BaseClient.h
	created:	2015/3/6 16:44:41
	author:		YvesMa(ma332567575@163.com)	
	Description:

	History:
	1. 2015/3/6 16:44:41 Create file
	...

*********************************************************************/

#ifndef BaseClient_h__
#define BaseClient_h__

#include "common.h"

class CBaseClient
{
public:
    CBaseClient( MVOID );
    ~CBaseClient( MVOID );

public:
    MBOOL OnMsg( DWORD dwMsg );


};

#endif // BaseClient_h__