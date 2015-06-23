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

#endif // mars_stdinclude_h__
