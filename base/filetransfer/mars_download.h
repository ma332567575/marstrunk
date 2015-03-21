/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   14:31:10
	filename: 	d:\marstrunk\filetransfer\mars_download.h
	author:		Yves Ma
	
	purpose:	Download interface realization
				Guarantee Block model and Unblock model can't process at the same time!
*********************************************************************/

#ifndef _MARS_DOWNLOAD_H_
#define _MARS_DOWNLOAD_H_

#include "mars_download_interface.h"

class CMarsDownload : public CMarsIDownload
{
public:

	CMarsDownload( MVOID );
	~CMarsDownload( MVOID );

public:

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
	);

	virtual MVOID 
	DoUnBlockDld( MVOID );

private:

	MVOID Init( MVOID );
	MVOID Unit( MVOID );

private:

/*
 * NAME				Relative lock funcitons

 * DESCRIPTION		The lock function guarantee one download model run at the same time.

*/
	MVOID
	LockDownload( MVOID )
	{
		m_DldLock		= MTRUE;
	}

	MVOID
	UnLockDownload( MVOID )
	{
		m_DldLock		= MFALSE;
	}

	MBOOL
	IsDownloadLock( MVOID )
	{
		return m_DldLock;
	}

private:

	// Block download data

	// Common data
	MBOOL			m_DldLock;			// Guarantee running by one model
};

#endif
//---------------------------------------------为了Linux，留个空行-----------------------------------------
