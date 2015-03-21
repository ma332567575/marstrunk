/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   14:41:50
	filename: 	d:\marstrunk\filetransfer\mars_download.cpp
	author:		Yves Ma
	
	purpose:	
*********************************************************************/

#include "mars_download.h"

std::tr1::shared_ptr<CMarsIDownload>
CMarsIDownload::CreateDownload( MVOID )
{
	return std::tr1::shared_ptr<CMarsIDownload>( new CMarsDownload );
}

CMarsDownload::CMarsDownload( MVOID )
{
	
}

CMarsDownload::~CMarsDownload( MVOID )
{

}

MVOID
CMarsDownload::Init( MVOID )
{
	// build-in type member variable defination
	m_DldLock				= MTRUE;
}

MVOID
CMarsDownload::Unit( MVOID )
{

}

MFTransCode
CMarsDownload::DoBlockDld
(
	 MARS_IN		MLPCSTR					szUrl,
	 MARS_IN		MLPCSTR					szFileName /*= MNULL*/,
	 MARS_IN		MBOOL					bVerbase /*= MTRUE*/,
	 MARS_IN		MBOOL					bRelocate /*= MTRUE*/,
	 MARS_IN		MarsFtransProxyArg*		ProxyArg /*= MNULL*/,
	 MARS_IN		MBOOL					bForce /*= MFALSE*/
)
{
	// 1. Check argument
	if ( !szUrl || !szUrl[0] )
		return MTRANS_INVALID_ARGUMENT;

	// 2. Check download lock
	if ( IsDownloadLock( ) )
		return MTRANS_ALREADY_DOWNLOAD;

	// 3. Create easy curl
	CURL* pEasyHandle		= MNULL;
	pEasyHandle				= curl_easy_init( );

	if ( !pEasyHandle )
		return MTRANS_EASY_CURL_INITIAL_FAILD;

	// 4. Set curl optional
	if ( szFileName && szFileName[0] )
	{

	}

	return MTRANS_OK;
}

MVOID
CMarsDownload::DoUnBlockDld( MVOID )
{

}

//---------------------------------------------为了Linux，留个空行-----------------------------------------
