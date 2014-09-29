/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   11:34:46
	filename: 	d:\marstrunk\filetransfer\main.cpp
	author:		Yves Ma
	
	purpose:	Test filetransfer
*********************************************************************/

#include <iostream>

#include "curl.h"

void main( void )
{
	CURL*		curl;
	CURLcode	res;

	curl		= curl_easy_init( );

	if ( curl )
	{
		curl_easy_setopt( curl, CURLOPT_URL, "http://www.baidu.com" );
		curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );

		res		= curl_easy_perform( curl );

		if ( res != CURLE_OK )
			std::cout << "Error: " << curl_easy_strerror( res );

		curl_easy_cleanup( curl );
	}
}