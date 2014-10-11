/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   14:28:03
	filename: 	d:\marstrunk\filetransfer\mars_curl_def.h
	author:		Yves Ma
	
	purpose:	Mars curl data define. Mars curl global function
*********************************************************************/

#ifndef _MARS_CURL_DEF_H_
#define _MARS_CURL_DEF_H_

// curl_global_init parameter
#define MARS_CURL_GLOBAL_WIN32	CURL_GLOBAL_WIN32
#define MARS_CURL_GLOBAL_ALL	CURL_GLOBAL_ALL
#define MARS_CURL_GLOBAL_SSL	CURL_GLOBAL_SSL

/*
* NAME			GMarsCurlGlobalInit

* PARAMETER	flags: MARS_CURL_GLOBAL_WIN32	
MARS_CURL_GLOBAL_ALL	
MARS_CURL_GLOBAL_SSL	

* DESCRIPTION	Call once in the all lifetime of the process. 
Call once GMarsCurlGlobalInit and call once GMarsCurlGlobalCleanup.
*/
#define GMarsCurlGlobalInit( flags )		curl_global_init( flags );

/*
* NAME			GMarsCurlGlobalInit

* PARAMETER	

* DESCRIPTION	Call once in the all lifetime of the process. 
Call once GMarsCurlGlobalInit and call once GMarsCurlGlobalCleanup.
*/
#define GMarsCurlGlobalCleanup( )			curl_global_cleanup( );

// FiletransError
typedef enum
{
	MTRNAS_NONE = MARS_INVALID_VALUE,	// first enum
	MTRANS_OK,							// 
	MTRANS_INVALID_ARGUMENT,			// Invalid argument
	MTRANS_EASY_CURL_INITIAL_FAILD,		// Easy curl initialize faild
	MTRANS_ALREADY_DOWNLOAD,			// Already download
	MTRANS_CURL_INTERNAL_ERROR,			// Curl error. We can get the detail in log.
	MTRANS_LAST,						// last enum
} MFTransCode;

// ProxyData
typedef struct tagMFTransProxyArg
{
	MCHAR	szAddress[MARS_MAX_PATH];
	MCHAR	szUsername[MARS_MAX_PATH];
	MCHAR	szPassword[MARS_MAX_PATH];
} MarsFtransProxyArg;

#endif
//---------------------------------------------为了Linux，留个空行-----------------------------------------
