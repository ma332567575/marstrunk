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

#include "mars_define.h"
#include "mars_curl_def.h"
#include "curl.h"

MINTERFACE CMarsIDownload
{
public:

	CMarsIDownload( MVOID ){ }
	~CMarsIDownload( MVOID ){ }

public:

	virtual MVOID Init( MVOID ) = 0;				// ��ʼ��
	virtual MVOID Unit( MVOID ) = 0;				// ����ʼ��

public:

	virtual MBOOL
	DoBlockDld
	(
		MARS_OUT	MARS_CURL_ERROR_CODE&	enCurlErrorCode,
		MARS_IN		MBOOL					bForce = MFALSE
	) = 0;												// ����ģʽ����

	virtual MVOID DoUnBlockDld( MVOID ) = 0;		// ������ģʽ����

};

#endif
//---------------------------------------------Ϊ��Linux����������-----------------------------------------
