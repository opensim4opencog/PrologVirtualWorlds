// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <stdafx.h>
#include "ctdata.h"
#include "ahistctl.h"

/////////////////////////////////////////////////////////////////////////////
// CTextData



CTextData::CTextData()
{
	m_lData	= 0;
	m_ToID		= -1;
	m_FromID	= -1;
	m_logType	= ltUnknown;
	m_logHint	= lhNone;

}


CTextData::CTextData(IThing *pFrom, IThing *pTo, BSTR bstr, long lval, LogType logType, long lhHint)
{
	m_lData		= lval;
	m_logType	= logType;
	m_logHint	= lhHint;

	m_bstrText	= bstr;

	m_ToID = m_FromID = -1;

	if ( pFrom )
	{
		BSTR bstr = NULL;
		pFrom->get_ID( &m_FromID );
		pFrom->get_Name( &bstr);
		m_bstrFromName = bstr;
		SAFEFREESTRING(bstr);
	}

	if ( pTo )
	{
		BSTR bstr = NULL;
		pTo->get_ID( &m_ToID );
		pTo->get_Name(&bstr);
		m_bstrToName = bstr;
		SAFEFREESTRING(bstr);
	}
}

CTextData::CTextData(const CTextData &that)
{
	m_logType	= that.m_logType;
	m_logHint	= that.m_logHint;
	m_lData		= that.m_lData;
	m_ToID		= that.m_ToID;
	m_FromID	= that.m_FromID;
	m_bstrText	= that.m_bstrText;
	m_bstrFromName = that.m_bstrFromName;
	m_bstrToName = that.m_bstrToName;
}


CTextData::~CTextData()
{
	// do nothing for now
	// REVIEW:  Do we have to release the BSTR?
};

CTextData &CTextData::operator=( const CTextData &that )
{
	m_lData		= that.m_lData;
	m_ToID		= that.m_ToID;
	m_FromID	= that.m_FromID;
	m_bstrText	= that.m_bstrText;
	m_logType	= that.m_logType;
	m_logHint	= that.m_logHint;
	m_bstrFromName = that.m_bstrFromName;
	m_bstrToName = that.m_bstrToName;
	return *this;
}

BOOL CTextData::Filter( long flags, CArray<long, long&> &WhisperList,CArray<long, long&> &IgnoreList, CStringList &PageList)
{
	BOOL bRet = (flags & m_logType)? TRUE:FALSE;

	// Is this directed at us? then override any type filtering
	bRet = (m_logHint & lhDirected) ? TRUE:bRet;
	 

	//Check the hints first
	if ( m_logHint & lhHidden ) 
		bRet = FALSE;

	if ( m_logHint & lhIgnored )
		bRet = FALSE;

	// Is there a ignore list?  if so check it
	int n = IgnoreList.GetSize();
	if (( n > 0 ) && bRet)
	{
		for(int i = 0; i<n && bRet; i++ )
		{
			if ( IgnoreList[i] == m_FromID )
				bRet = FALSE;
		}
	}

	// Is there a whisper list?  if so only return true if the from is one it.
	// The whipser should only respond to directed text
	n = WhisperList.GetSize();
	if (( n > 0 ) && bRet && (m_logHint & lhDirected ))
	{
		// The user is in the whisper list
		bRet = FALSE;
		for(int i = 0; i<n && !bRet; i++ )
		{
			if ( WhisperList[i] == m_FromID )
				bRet = TRUE;
		}
	}
	else
	{
		// handle page filtering
		n = PageList.GetCount();
		if ((n > 0) && bRet && (m_logHint & lhDirected))
		{
			bRet = FALSE;
			// The first word in the text is in the list
			CString	userName = (BSTR)m_bstrText;
			long	lSpace = userName.Find(' ');
			if (lSpace != -1)
			{
				userName = userName.Left(lSpace);
				userName.MakeUpper();
				if (PageList.Find(userName))
					bRet = TRUE;
			}
		}
	}

	return bRet;
}


//******************************************************
// Format
// The generic format commands - switches for all of the
// sub-types.

void CTextData::Format( IWorld *pWorld, CHistoryCtrl &rHistory)
{
//	BOOL bLocked = rHistory.LockWindowUpdate();
	switch ( m_logType )
	{
	case ltText:
		FormatText(pWorld, rHistory);
		break;

	case ltGesture:
		FormatGesture(pWorld, rHistory);
		break;

	case ltEvent:
		FormatEvent(pWorld, rHistory);
		break;

	case ltTrace:
	case ltReport:
		FormatTrace(pWorld, rHistory);
		break;
	}
//	if ( bLocked )
//		rHistory.UnlockWindowUpdate();
}

//******************************************************
// FormatEvent
// Formats the event messages

void CTextData::FormatEvent( IWorld *pWorld, CHistoryCtrl &rHistory)
{
	COLORREF	colorSay	= colorSay = rHistory.m_rgbEvent;
	int			eLOptions = CHistoryCtrl::eLBold;
	_bstr_t		quoteText		= "";
	BOOL		bPrintWhat		= TRUE;
	BOOL		bUserIsWho		= FALSE;
	BOOL		bUserIsWhat		= FALSE;

	switch ( m_lData )
	{
	case etWield:	
		quoteText = " wields ";
		bPrintWhat	= TRUE;
		break;

	case etUnWield:
		quoteText = " unwields ";
		bPrintWhat	= TRUE;
		break;

#ifdef OLDWAY
	// The following is now done via OnAction
	case etEnterRoom:
		quoteText = " enters the room.";
		bPrintWhat	= FALSE;
		break;

	case etLeaveRoom:
		quoteText = " leaves the room.";
		bPrintWhat	= FALSE;
		break;

	case etEnterInv:
		quoteText = " picks up ";
		bPrintWhat	= TRUE;
		break;

	case etLeaveInv:
		quoteText = " drops ";
		bPrintWhat	= TRUE;
		break;

#else
	case etEnterRoom:
	case etLeaveRoom:
	case etEnterInv:
	case etLeaveInv:
		return;

#endif

	case etConnect:	
		quoteText = " has connected.";
		bPrintWhat	= FALSE;
		break;

	case etDisconnect:
		quoteText = " has disconnected.";
		bPrintWhat	= FALSE;
		break;
	}

	// now print the event...
	if (m_FromID >=0 )
		rHistory.Log( m_bstrFromName, CHistoryCtrl::eLNewPara |eLOptions, colorSay);
	else
		eLOptions |= CHistoryCtrl::eLNewPara;

	rHistory.Log( quoteText, eLOptions, colorSay);

	if (bPrintWhat)
	{
		if (m_ToID >= 0)
		{
			rHistory.Log( m_bstrToName,	eLOptions, colorSay);
			rHistory.Log( _bstr_t("."),	eLOptions, colorSay);
		}

	}

}

//******************************************************
// FormatTrace
// Formats the trace messages

void CTextData::FormatTrace( IWorld *pWorld, CHistoryCtrl &rHistory)
{
	COLORREF	colorSay= rHistory.m_rgbTrace;
	int			eLOptions		= 0;
	_bstr_t		leftquote = "";

	// see if you could hear what was said...
	if (m_bstrText.length() != 0)
	{
		// Something was actually heard...
		// is it directect text? change color to white
		// special case options
		switch ( m_lData )
		{

		case VWR_ECHO:	// user's text
			leftquote = ">> ";
			eLOptions = CHistoryCtrl::eLBold;
			break;

		case VWR_EVAL:	// expression
			leftquote = " -->";
			break;

		case VWR_PROMPT:	// prompt 
			break;

		case VWR_ERROR:	// error
			leftquote = "***";
			break;
		
		case VWR_PERFLOG:	// performance log
			eLOptions = CHistoryCtrl::eLBold;
			break;

		default:
		break;
		}

		// now print the trace...
		rHistory.Log( leftquote, CHistoryCtrl::eLNewPara | CHistoryCtrl::eLBold |eLOptions, colorSay);
		rHistory.Log( m_bstrText,	eLOptions, colorSay, TRUE);
	}
}


//******************************************************
// FormatGesture
// Formats a gesture log record (ie, an emote)

void CTextData::FormatGesture( IWorld *pWorld, CHistoryCtrl &rHistory)
{
	COLORREF	colorSay = rHistory.m_rgbGesture;
	int			eLOptions = 0;
	_bstr_t		quoteText		= " ";
	
	// see if you could hear what was said...
	if (m_bstrText.length() != 0)
	{
		if ( m_FromID >= 0 )
		{
			rHistory.Log( m_bstrFromName ,CHistoryCtrl::eLBold | CHistoryCtrl::eLNewPara | eLOptions, colorSay);
			rHistory.Log( quoteText , eLOptions, colorSay);
		}
		
		// now print the rest of the text...
		rHistory.Log( m_bstrText, eLOptions, colorSay, TRUE);
	}
	
}


//******************************************************
// FormatText
// Formats a spoken text log record

void CTextData::FormatText( IWorld *pWorld, CHistoryCtrl &rHistory)
{
	int			eLOptions = 0;
	_bstr_t		leftquote = "\"";
	_bstr_t		rightquote = "\"";
	COLORREF colorArray[] = {
						rHistory.m_rgbSay,//tcSay = 0,	
						rHistory.m_rgbEmote,//tcEmote = 1,	
						rHistory.m_rgbThink,//tcThink = 2,	
						rHistory.m_rgbWhisper,//tcWhisper = 3,	
						rHistory.m_rgbAnnounce,//tcAnnounce = 4,
						rHistory.m_rgbGesture,//tcGesture = 5,
						rHistory.m_rgbShout,//tcShout = 6,
						rHistory.m_rgbReport,//tcReport = 7,
						rHistory.m_rgbEvent,//tcAction = 8,
						};

	ASSERT( m_lData >= 0 );
	ASSERT( m_lData <= tcAction );

	COLORREF colorSay = colorArray[m_lData];

	if (m_logHint & lhDirected)
		colorSay = rHistory.m_rgbDirect;

	//if (m_lData == tcWhisper )
	//	eLOptions |= CHistoryCtrl::eLItalic;

	// see if you could hear what was said...
	if (m_bstrText.length() == 0)
	{
		// couldn't hear what was said...
		static _bstr_t strSay[] = {
						" is speaking",
						" is doing something",
						" is thinking something",
						" is whispering something",
						" is announcing something", 
						" is gesturing.",
						" is shouting something",
						" is reporting something",
						" is doing something" };

		if ( m_FromID >= 0 )
		{
			rHistory.Log( m_bstrFromName ,CHistoryCtrl::eLBold | CHistoryCtrl::eLNewPara | eLOptions, colorSay);
			rHistory.Log( strSay[m_lData] , CHistoryCtrl::eLBold | eLOptions, colorSay);
		}
		else
		{ 
			// need to add a newline
			// REVIEW:  This is an erroneous case, right???
			rHistory.Log( _bstr_t(""),CHistoryCtrl::eLNewPara , colorSay);
		}

		// now the to...
		//if (( m_ToID >= 0 ) &&
		//	(( m_lData == tcWhisper ) ||
		//	( m_lData == tcReport)))
		if( m_ToID >= 0)
		{
			rHistory.Log( _bstr_t(" to ") + m_bstrToName + _bstr_t("."),CHistoryCtrl::eLBold |eLOptions, colorSay);	
		}
		else	// just need a trailing period...
			rHistory.Log( _bstr_t("."),eLOptions, colorSay);
	}
	else
	{
		// Something was actually heard...
		// is it directect text? change color to white
		// special case options
		switch ( m_lData )
		{
			case tcThink:
				leftquote = ".oO(";
				rightquote = ")";
				break;

			case tcReport:
				leftquote = "";
				rightquote = "";
				break;

			case tcAnnounce:
			case tcEmote:
			case tcAction:
				leftquote = "";
				rightquote = "";
				eLOptions = CHistoryCtrl::eLBold;
				break;

			break;
		}
		
		if ( (m_FromID >= 0) && (m_lData != tcAction) )
		{
				// someone else said it - user their name
			static _bstr_t strOtherSay[] = {
							" says ",
							" ",
							" thinks ",
							" whispers ",
							" announces ", 
							" gestures ",
							" shouts " ,
							" reports " };

			rHistory.Log( m_bstrFromName ,CHistoryCtrl::eLBold | CHistoryCtrl::eLNewPara | eLOptions,colorSay);
			rHistory.Log( strOtherSay[m_lData] ,CHistoryCtrl::eLBold | eLOptions, colorSay);
		}
		else
		{ 
			// no one said it - could be a page!
			if (m_lData == tcWhisper)
			{
				rHistory.Log( m_bstrText ,CHistoryCtrl::eLBold | CHistoryCtrl::eLNewPara | eLOptions, colorSay);
				return;
			}
			else
			{
				rHistory.Log( _bstr_t(""), CHistoryCtrl::eLNewPara, colorSay);
			}
		}

		// if this is directed (ie, a whisper or a report)
		// put in the "TO" line
		if ((( m_ToID >= 0 ) &&
			((m_lData == tcWhisper ) ||
			 (m_lData == tcReport))) && (m_lData != tcAction))
		{
			rHistory.Log( _bstr_t("to ")+m_bstrToName+_bstr_t(" "), CHistoryCtrl::eLBold |eLOptions, colorSay);
		}
		
		// now print the rest of the text...
		rHistory.Log( leftquote, eLOptions, colorSay);
		rHistory.Log( m_bstrText,eLOptions, colorSay, TRUE);
		rHistory.Log( rightquote,eLOptions, colorSay);
	}	
}

