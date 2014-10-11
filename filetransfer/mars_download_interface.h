/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   13:26:03
	filename: 	d:\marstrunk\filetransfer\mars_download_interface.h
	author:		Yves Ma
	
	purpose:	Download interface
	
	attention:
	1. The first basic rule is that you must never share a download or a upload object between
	   multiple threads.Only use one object in one thread at any time.
*********************************************************************/

#ifndef _MARS_DOWNLOAD_INTERFACE_H_
#define _MARS_DOWNLOAD_INTERFACE_H_

// c++ stand library
#include <memory>

// Third library -- libcurl
#include "curl.h"

// Our header file
#include "mars_define.h"
#include "mars_curl_def.h"

MINTERFACE CMarsIDownload
{
public:

	CMarsIDownload( MVOID ){ }
	~CMarsIDownload( MVOID ){ }

public:

	virtual MVOID Init( MVOID ) = 0;				// 初始化
	virtual MVOID Unit( MVOID ) = 0;				// 反初始化

public:

	static std::tr1::shared_ptr<CMarsIDownload>
	CreateDownload( MVOID );

public:

	virtual MFTransCode
	DoBlockDld
	(
		MARS_IN		MLPCSTR					szUrl,
		MARS_IN		MLPCSTR					szFileName = MNULL,
		MARS_IN		MBOOL					bVerbase = MTRUE,
		MARS_IN		MBOOL					bRelocate = MTRUE,
		MARS_IN		MarsFtransProxyArg*		ProxyArg = MNULL,
		MARS_IN		MBOOL					bForce = MFALSE

	) = 0;												// 阻塞模式下载

	virtual MVOID DoUnBlockDld( MVOID ) = 0;		// 非阻塞模式下载

};

#endif
//---------------------------------------------为了Linux，留个空行-----------------------------------------
