// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _CTDATA_
#define _CTDATA_

#include "stdafx.h"
#include <vwobject.h>
#include <afxtempl.h>
#include <propbase.h>
#include <comdef.h>

class CHistoryCtrl;

#define FILTER_TEXT 0x1L
#define FILTER_GESTURES 0x2L
#define FILTER_EVENTS 0x4L
#define FILTER_TRACE  0x8L
#define FILTER_REPORT	0x10L
#define FILTER_SELECTION 0x20L

#define FILTER_ALL 0xFFFFFFFFL
#define FILTER_NONE 0
#define FILTER_DEFAULT 7L


typedef enum _LogHint
{
	lhNone		= 0x00,
	lhFriend	= 0x01,
	lhHidden	= 0x02,
	lhIgnored	= 0x04,
	lhDirected	= 0x08,
} LogHint;


typedef enum _LogType
{
	ltUnknown = 0,
	ltText		= FILTER_TEXT,
	ltGesture	= FILTER_GESTURES,
	ltEvent		= FILTER_EVENTS,
	ltTrace		= FILTER_TRACE,
	ltReport	= FILTER_REPORT,
} LogType;

typedef enum _EventType
{
	etUnknown		= -1,
	etEnterRoom		= 0,
	etLeaveRoom		= 1,
	etEnterInv		= 2,
	etLeaveInv		= 3,
	etConnect		= 4,
	etDisconnect	= 5,
	etWield			= 6,
	etUnWield		= 7
} EventType;


class CTextData
{
public:
	// member vars
	long		m_lData;
	long		m_ToID;
	long		m_FromID;
	bstr_t		m_bstrText;
	bstr_t		m_bstrToName;
	bstr_t		m_bstrFromName;
	LogType		m_logType;
	long		m_logHint;
	
public:
	// member functions
	CTextData(void);
	~CTextData();
	CTextData(IThing *pFrom, IThing *pTo, BSTR bstr = NULL, long lval = 0, LogType logType = ltText, long lhHint = lhNone);
	CTextData(const CTextData &that);
	CTextData &operator=( const CTextData &that );

	BOOL Filter( long flags,CArray<long, long&> &WhisperList,CArray<long, long&> &IgnoreList, CStringList &PageList);
	void Format( IWorld *pWorld, CHistoryCtrl &pHistory);
	void FormatText( IWorld *pWorld, CHistoryCtrl &pHistory);
	void FormatEvent( IWorld *pWorld, CHistoryCtrl &pHistory);
	void FormatGesture( IWorld *pWorld, CHistoryCtrl &pHistory);
	void FormatTrace( IWorld *pWorld, CHistoryCtrl &pHistory);
};

#endif