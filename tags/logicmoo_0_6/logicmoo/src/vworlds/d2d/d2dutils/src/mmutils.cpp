// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	MMUtils.cpp
// Author:	MM    (MM)
//
// History:
// -@- 4/12/96 (MM) created
// -@- 11/07/96 (MM) modified
//     combined debug stuff

#include "D2DUtils.h"

char g_rgchTmpBuf[nTMPBUFSIZE];

//
// Debug Stuff
//
void _MMStall(const char *szExp, const char *szFile, int nLine) {
	sprintf(g_rgchTmpBuf, "error: (%s) in %s at line %d\n", szExp, szFile, nLine);

#ifdef _WINDOWS
	OutputDebugString(g_rgchTmpBuf);
#endif
	fprintf(stderr, "%s", g_rgchTmpBuf);

	// hardcode breakpoint
#if defined(_DEBUG) && defined(_X86_)
	_asm { int 3 };
#else
	exit(1);
#endif
}

void _MMTrace(const char *szFmt, ...)
{
	_vsnprintf(g_rgchTmpBuf, nTMPBUFSIZE - 1, szFmt, (va_list) (&szFmt+1));

#if defined(_WINDOWS) && defined(_DEBUG)
	OutputDebugString(g_rgchTmpBuf);
#else
	fprintf(stderr, "%s", g_rgchTmpBuf);
#endif
}


#if defined(_WINDOWS)
#define xxx(MSGID) case MSGID: return #MSGID

const char *WinMsgIDToString(unsigned int uMsgID) {
	switch(uMsgID) {
		xxx(WM_NULL);
		xxx(WM_CREATE);
		xxx(WM_DESTROY);
		xxx(WM_MOVE);
		xxx(WM_SIZE);
		xxx(WM_ACTIVATE);
		xxx(WM_SETFOCUS);
		xxx(WM_KILLFOCUS);
		xxx(WM_ENABLE);
		xxx(WM_SETREDRAW);
		xxx(WM_SETTEXT);
		xxx(WM_GETTEXT);
		xxx(WM_GETTEXTLENGTH);
		xxx(WM_PAINT);
		xxx(WM_CLOSE);
		xxx(WM_QUERYENDSESSION);
		xxx(WM_QUIT);
		xxx(WM_QUERYOPEN);
		xxx(WM_ERASEBKGND);
		xxx(WM_SYSCOLORCHANGE);
		xxx(WM_ENDSESSION);
		xxx(WM_SHOWWINDOW);
		xxx(WM_WININICHANGE);
		xxx(WM_DEVMODECHANGE);
		xxx(WM_ACTIVATEAPP);
		xxx(WM_FONTCHANGE);
		xxx(WM_TIMECHANGE);
		xxx(WM_CANCELMODE);
		xxx(WM_SETCURSOR);
		xxx(WM_MOUSEACTIVATE);
		xxx(WM_CHILDACTIVATE);
		xxx(WM_QUEUESYNC);
		xxx(WM_GETMINMAXINFO);
		xxx(WM_PAINTICON);
		xxx(WM_ICONERASEBKGND);
		xxx(WM_NEXTDLGCTL);
		xxx(WM_SPOOLERSTATUS);
		xxx(WM_DRAWITEM);
		xxx(WM_MEASUREITEM);
		xxx(WM_DELETEITEM);
		xxx(WM_VKEYTOITEM);
		xxx(WM_CHARTOITEM);
		xxx(WM_SETFONT);
		xxx(WM_GETFONT);
		xxx(WM_SETHOTKEY);
		xxx(WM_GETHOTKEY);
		xxx(WM_QUERYDRAGICON);
		xxx(WM_COMPAREITEM);
		xxx(WM_COMPACTING);
		xxx(WM_WINDOWPOSCHANGING);
		xxx(WM_WINDOWPOSCHANGED);
		xxx(WM_POWER);
		xxx(WM_COPYDATA);
		xxx(WM_CANCELJOURNAL);
		xxx(WM_NOTIFY);
		xxx(WM_INPUTLANGCHANGEREQUEST);
		xxx(WM_INPUTLANGCHANGE);
		xxx(WM_TCARD);
		xxx(WM_HELP);
		xxx(WM_USERCHANGED);
		xxx(WM_NOTIFYFORMAT);
		xxx(WM_CONTEXTMENU);
		xxx(WM_STYLECHANGING);
		xxx(WM_STYLECHANGED);
		xxx(WM_DISPLAYCHANGE);
		xxx(WM_GETICON);
		xxx(WM_SETICON);
		xxx(WM_NCCREATE);
		xxx(WM_NCDESTROY);
		xxx(WM_NCCALCSIZE);
		xxx(WM_NCHITTEST);
		xxx(WM_NCPAINT);
		xxx(WM_NCACTIVATE);
		xxx(WM_GETDLGCODE);
		xxx(WM_NCMOUSEMOVE);
		xxx(WM_NCLBUTTONDOWN);
		xxx(WM_NCLBUTTONUP);
		xxx(WM_NCLBUTTONDBLCLK);
		xxx(WM_NCRBUTTONDOWN);
		xxx(WM_NCRBUTTONUP);
		xxx(WM_NCRBUTTONDBLCLK);
		xxx(WM_NCMBUTTONDOWN);
		xxx(WM_NCMBUTTONUP);
		xxx(WM_NCMBUTTONDBLCLK);
		xxx(WM_KEYDOWN);
		xxx(WM_KEYUP);
		xxx(WM_CHAR);
		xxx(WM_DEADCHAR);
		xxx(WM_SYSKEYDOWN);
		xxx(WM_SYSKEYUP);
		xxx(WM_SYSCHAR);
		xxx(WM_SYSDEADCHAR);
		xxx(WM_KEYLAST);
		xxx(WM_INITDIALOG);
		xxx(WM_COMMAND);
		xxx(WM_SYSCOMMAND);
		xxx(WM_TIMER);
		xxx(WM_HSCROLL);
		xxx(WM_VSCROLL);
		xxx(WM_INITMENU);
		xxx(WM_INITMENUPOPUP);
		xxx(WM_MENUSELECT);
		xxx(WM_MENUCHAR);
		xxx(WM_ENTERIDLE);
		xxx(WM_CTLCOLORMSGBOX);
		xxx(WM_CTLCOLOREDIT);
		xxx(WM_CTLCOLORLISTBOX);
		xxx(WM_CTLCOLORBTN);
		xxx(WM_CTLCOLORDLG);
		xxx(WM_CTLCOLORSCROLLBAR);
		xxx(WM_CTLCOLORSTATIC);
		xxx(WM_MOUSEMOVE);
		xxx(WM_LBUTTONDOWN);
		xxx(WM_LBUTTONUP);
		xxx(WM_LBUTTONDBLCLK);
		xxx(WM_RBUTTONDOWN);
		xxx(WM_RBUTTONUP);
		xxx(WM_RBUTTONDBLCLK);
		xxx(WM_MBUTTONDOWN);
		xxx(WM_MBUTTONUP);
		xxx(WM_MBUTTONDBLCLK);
		xxx(WM_PARENTNOTIFY);
		xxx(WM_ENTERMENULOOP);
		xxx(WM_EXITMENULOOP);
		xxx(WM_SIZING);
		xxx(WM_CAPTURECHANGED);
		xxx(WM_MOVING);
		xxx(WM_POWERBROADCAST);
		xxx(WM_DEVICECHANGE);
		xxx(WM_IME_SETCONTEXT);
		xxx(WM_IME_NOTIFY);
		xxx(WM_IME_CONTROL);
		xxx(WM_IME_COMPOSITIONFULL);
		xxx(WM_IME_SELECT);
		xxx(WM_IME_CHAR);
		xxx(WM_IME_KEYDOWN);
		xxx(WM_IME_KEYUP);
		xxx(WM_MDICREATE);
		xxx(WM_MDIDESTROY);
		xxx(WM_MDIACTIVATE);
		xxx(WM_MDIRESTORE);
		xxx(WM_MDINEXT);
		xxx(WM_MDIMAXIMIZE);
		xxx(WM_MDITILE);
		xxx(WM_MDICASCADE);
		xxx(WM_MDIICONARRANGE);
		xxx(WM_MDIGETACTIVE);
		xxx(WM_MDISETMENU);
		xxx(WM_ENTERSIZEMOVE);
		xxx(WM_EXITSIZEMOVE);
		xxx(WM_DROPFILES);
		xxx(WM_MDIREFRESHMENU);
		xxx(WM_CUT);
		xxx(WM_COPY);
		xxx(WM_PASTE);
		xxx(WM_CLEAR);
		xxx(WM_UNDO);
		xxx(WM_RENDERFORMAT);
		xxx(WM_RENDERALLFORMATS);
		xxx(WM_DESTROYCLIPBOARD);
		xxx(WM_DRAWCLIPBOARD);
		xxx(WM_PAINTCLIPBOARD);
		xxx(WM_VSCROLLCLIPBOARD);
		xxx(WM_SIZECLIPBOARD);
		xxx(WM_ASKCBFORMATNAME);
		xxx(WM_CHANGECBCHAIN);
		xxx(WM_HSCROLLCLIPBOARD);
		xxx(WM_QUERYNEWPALETTE);
		xxx(WM_PALETTEISCHANGING);
		xxx(WM_PALETTECHANGED);
		xxx(WM_HOTKEY);
		xxx(WM_PRINT);
		xxx(WM_PRINTCLIENT);
		xxx(WM_HANDHELDFIRST);
		xxx(WM_HANDHELDLAST);
		xxx(WM_AFXFIRST);
		xxx(WM_AFXLAST);
		xxx(WM_PENWINFIRST);
		xxx(WM_PENWINLAST);
		xxx(WM_APP);
		xxx(WM_USER);
default:
	return "Unknown MsgID";
	}
}

#undef xxx
#endif

// Function: FloatEquals
//    Peform a "fuzzy" compare of two floating point numbers.  This relies
//  on the IEEE bit representation of floating point numbers.
int 
FloatEquals(float x1, float x2) {
	#define EXPMASK 		0x7f800000
	#define BITSOFPRECISION 19
	#define MANTBITS 		23
	#define EXPOFFSET		(BITSOFPRECISION<<MANTBITS)
	#define ZEROEPS 		3.8e-6F
	#define TINYEPS         1.E-35F

	if (x1 == x2) return 1;		// quick out on exact match
	
	float flEps;

	if ((x1 == 0.0f) || (x2 == 0.0f)) {
		flEps = ZEROEPS;
	} else {
		float maxX;

		SETMAX(maxX, x1, x2);
		unsigned int uExp = (*((unsigned int *) &maxX) & EXPMASK);
		if (uExp < EXPOFFSET)
			flEps = TINYEPS;
		else {
			uExp -= EXPOFFSET;
			flEps = *((float *) &uExp);
		}
	}
	return (((x1 + flEps) >= x2) && ((x1 - flEps) <= x2));
}

// Function: PrintBits
//    Print the bit pattern of a 32 bit number in using 1's and 0's
//  to the given file stream.
void PrintBits(FILE *pfs, BitVector uBits)
{
	for (unsigned int i = NUMBITS(BitVector) - 1; i != 0; i--)
		MMVERIFY(putc(char(((uBits >> i) & 1) + '0'), pfs) != EOF);
	MMVERIFY(putc(char((uBits & 1) + '0'), pfs) != EOF);
}
