/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   11:34:46
	filename: 	d:\marstrunk\filetransfer\main.cpp
	author:		Yves Ma
	
	purpose:	Test filetransfer
*********************************************************************/

#include <iostream>

#include "curl.h"
#include "mars_download_interface.h"
typedef struct curl_slist curlslist;

void ProxyUse( void )
{
	 CURL*		curl;
	 CURLcode	res;

	 curl		= curl_easy_init( );

	 if ( curl )
	 {
		 curl_easy_setopt( curl, CURLOPT_URL, "http://www.facebook.com" );
		 curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
		 curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L );

		 // Set proxy option
		 curl_easy_setopt( curl, CURLOPT_PROXY, "j32.jayproxy.org:3000" );
		 curl_easy_setopt( curl, CURLOPT_PROGRESSDATA, "Yvesma:6379" );

		 // Resolve the error CURLE_SSL_CACERT£¨no.60£©.I think it maybe not an error, because checking ssl certificate is nice action.
		 curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, FALSE );

		 // Set authentication option for choosing a safe way encode name and password!
		 // The fault is basic,and we can choose 'Basic','Digest','NTLM','Negotiate'
		 curl_easy_setopt( curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY );
		 // For set proxy authentication
		 curl_easy_setopt( curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY );
		 // ps: For convenience, you can use the 'CURLAUTH_ANY' define (instead of a list with specific types) which allows libcurl to use whatever method it wants. 

		 // Perform the transfer
		 res	= curl_easy_perform( curl );

		 if ( res != CURLE_OK )
			 std::cout << "Error: " << curl_easy_strerror( res );

		 curl_easy_cleanup( curl );
	 }
}

void HttpPosting( void )
{
	CURL* curl = NULL;
	CURLcode res;

	curl			= curl_easy_init( );

	// 1. First version is the simple POST, the most common version, that most HTML pages using the<form>tag uses.
	// We provide a pointer to the data and tell libcurl to post it all to the remote site:
	char data[256]	= {0};
	curl_easy_escape( curl, "iuser=123&ipass=curl", 0 );
	
	curl_easy_setopt( curl, CURLOPT_POSTFIELDS, data );
	curl_easy_setopt( curl, CURLOPT_URL, "http://xinke.uir.cn/admin/login.php" );
	/*
	struct curl_httppost* post	= NULL;
	struct curl_httppost* last	= NULL;

	curl_formadd( &post, &last, CURLFORM_COPYNAME, "userid", CURLFORM_COPYCONTENTS, "123", CURLFORM_END );
	curl_formadd( &post, &last, CURLFORM_COPYNAME, "pwd", CURLFORM_COPYCONTENTS, "123", CURLFORM_END );

	curl_easy_setopt( curl, CURLOPT_HTTPPOST, post );
	*/
	
	curl_easy_setopt( curl, CURLOPT_VERBOSE, 1L );
	// perform post way
	res				= curl_easy_perform( curl );

	//curl_easy_cleanup( curl );

	// use binary model to post
	// libcurl can't use strlen, so we must tell him how long the string.
	/*
	curlslist* headers	= NULL;
	headers						= curl_slist_append( headers, "Content-type: text/xml" );

	// Post binary data
	curl_easy_reset( curl );
	curl_easy_setopt( curl, CURLOPT_URL, "http://posthere.com" );
	curl_easy_setopt( curl, CURLOPT_POSTFIELDS, "binary stream" );
	curl_easy_setopt( curl, CURLOPT_POSTFIELDSIZE, 100L );
	curl_easy_setopt( curl, CURLOPT_HTTPHEADER, headers );

	res = curl_easy_perform( curl );

	curl_slist_free_all( headers );
	*/
	//curl_formfree( post );
	curl_easy_cleanup( curl );
}

void main( void )
{
	HttpPosting( );
}