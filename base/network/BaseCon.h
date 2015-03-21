/********************************************************************
	Copyright(C)
	filename:	BaseCon.h
	created:	2015/3/6 16:15:17
	author:		YvesMa(ma332567575@163.com)	
	Description: 服务器基类

	History:
	1. 2015/3/6 16:15:17 Create file
	...

*********************************************************************/

#ifndef BaseCon_h__
#define BaseCon_h__

#include "common.h"
class CBaseCon
{
public:
    CBaseCon( MVOID );
    virtual ~CBaseCon( MVOID );

public:
    MBOOL Listen( MLPCSTR szIp, MUI16 uPort );

};

#endif // BaseCon_h__