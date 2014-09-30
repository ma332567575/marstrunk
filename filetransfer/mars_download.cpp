/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   14:41:50
	filename: 	d:\marstrunk\filetransfer\mars_download.cpp
	author:		Yves Ma
	
	purpose:	
*********************************************************************/

#include "mars_download.h"

CMarsDownload::CMarsDownload( MVOID )
{
	m_BlockHandle			= MNULL;
	m_BlockHandle			= curl_easy_init( );

	if ( !m_BlockHandle )
	{
		// Todo: Write error log: Init easy curl failed!
	}

	m_DldLock				= MTRUE;
}

CMarsDownload::~CMarsDownload( MVOID )
{
	if ( m_BlockHandle != MNULL )
		curl_easy_cleanup( m_BlockHandle );
}

MVOID
CMarsDownload::Init( MVOID )
{
	
}

MVOID
CMarsDownload::Unit( MVOID )
{

}

MBOOL
CMarsDownload::DoBlockDld
(
	MARS_OUT	MARS_CURL_ERROR_CODE&	enCurlErrorCode,
	MARS_IN		MBOOL					bForce /* = MFALSE */
)
{
	if ( IsDownloadLock( ) )
		return MFALSE;

	return MTRUE;
}

MVOID
CMarsDownload::DoUnBlockDld( MVOID )
{

}

//---------------------------------------------为了Linux，留个空行-----------------------------------------
