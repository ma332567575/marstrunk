/********************************************************************
	Copyright(C)
	filename:	mars_stdinclude.h
	created:	2015/3/15 11:28:49
	author:		YvesMa(ma332567575@163.com)	
	Description: 里面include一些标准头文件

	History:
	1. 2015/3/15 11:28:49 Create file
	...

*********************************************************************/
#ifndef mars_stdinclude_h__
#define mars_stdinclude_h__

#include "mars_define.h"

#include <map>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <fstream>
#include <algorithm>

using std::map;
using std::list;
using std::string;
using std::queue;
using std::vector;
using std::fstream;
using std::make_pair;

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
#define MMemcpy( pDst, nDstSize, pSrc, nCpySize )   memcpy_s( (pDst), (nDstSize), (pSrc), (nCpySize) )
#endif

#ifdef _WIN32
#define MMemmove( pDst, nDstSize, pSrc, nMoveSize ) memmove_s( (pDst), (nDstSize), (pSrc), (nMoveSize) )
#endif

#ifdef _WIN32
#define MZeroMemory( pBuff, nBufferSize ) memset( (pBuff), 0, nBufferSize )
#endif

#endif // mars_stdinclude_h__
