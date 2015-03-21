/********************************************************************
	created:	2014/09/29
	created:	2014:9:29   13:28:36
	filename: 	d:\marstrunk\include\mars_define.h
	author:		Yves Ma
	
	purpose:	Type define
*********************************************************************/

#ifndef _MARS_DEFINE_H_
#define _MARS_DEFINE_H_

#define MINTERFACE		struct
#define MARS_IN
#define MARS_OUT

typedef long            MI64;
typedef unsigned long   MUI64;
typedef int				MI32;
typedef unsigned int	MUI32;
typedef short           MI16;
typedef unsigned short  MUI16;

typedef const char*		MLPCSTR;
typedef char*			MLPSTR;

typedef void			MVOID;
typedef void*           MPVOID;
typedef unsigned int	MBOOL;

typedef char			MCHAR;
typedef unsigned char	MBYTE;
typedef unsigned char*  MPBYTE;

#ifdef _WIN32
typedef size_t          MSIZE_T;
#endif

#define MNULL			(0)
#define MTRUE			(1)
#define MFALSE			(0)
#define MCONST          const

#define MARS_MAX_PATH		(256)
#define MARS_INVALID_VALUE	(-1)

#endif
