// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//Editor info for render view.

enum CameraMode 
{
	PERSPECTIVE = 0,
	TOP,
	RIGHT,
	FRONT,
};

enum EditingMode 
{
	EDIT_OFF = 0,
	EDIT_ON,
	EDIT_BOUNDARIES,
};

enum ToolEventCode
{
	TOOLEVENT_3DOBJECTTRANSLATED = 0,		//0
	TOOLEVENT_3DOBJECTROTATED,				//1
	TOOLEVENT_3DOBJECTSCALED,				//2
	TOOLEVENT_2DOBJECTTRANSLATED,			//3
	TOOLEVENT_2DOBJECTROTATED,				//4
	TOOLEVENT_2DOBJECTSCALED,				//5
	TOOLEVENT_2DMOUSEPOSITION,				//6
	TOOLEVENT_BOUNDARYUPDATE,				//7
	TOOLEVENT_BOUNDARYUPDATEBYINDEX,		//8
};

enum BOUNDARYUPDATE_HINT
{
	BOUNDARYUPDATE_ADD = 0,
	BOUNDARYUPDATE_REMOVE,
	BOUNDARYUPDATE_ADDSELECTION,
	BOUNDARYUPDATE_REMOVESELECTION,
	BOUNDARYUPDATE_CLEARSELECTION,
	BOUNDARYUPDATE_UPDATEINFO,
	BOUNDARYUPDATE_UPDATESCREENINFO = 6,
};
