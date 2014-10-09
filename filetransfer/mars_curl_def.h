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

#define MARS_CURL_ERROR_CODE	CURLcode

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

#endif
//---------------------------------------------为了Linux，留个空行-----------------------------------------
