// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef vwstudio_events_h
#define vwstudio_events_h

// this file should contain the names of all the commonly used strings for events


// editor notification events

//Fired by render view
#define VWSTUDIO_VIEWMODECHANGED_EVENT_STR "OnViewModeChanged"
#define VWSTUDIO_EDITMODECHANGED_EVENT_STR "OnEditModeChanged"
#define VWSTUDIO_TARGETOBJECTPROPERTYCHANGED_EVENT_STR "OnTargetObjectPropertyChanged"
#define VWSTUDIO_ZOOMLEVELCHANGED_EVENT_STR "OnZoomLevelChanged"
#define VWSTUDIO_ADDCURRENTTOOL_EVENT_STR "OnAddCurrentTool"
#define VWSTUDIO_CAMERACHANGED_EVENT_STR "OnCameraChanged"

//Fired by the render root
#define VWSTUDIO_DEVICECHANGED_EVENT_STR "OnDeviceChanged"

//Fired by Studio Exemplars 
#define VWSTUDIO_EDITINGMODECHANGED_EVENT_STR "OnEditingModeChanged"
#define VWSTUDIO_GRAVITYCHANGED_EVENT_STR "OnGravityChanged"
#define VWSTUDIO_COLLISIONDETECTIONCHANGED_EVENT_STR "OnCollisionDetectionChanged"
#define VWSTUDIO_SELECTIONLOCKCHANGED_EVENT_STR "OnSelectionLockChanged"
#define VWSTUDIO_CAMERAFOLLOWSOBJECTCHANGED_EVENT_STR "OnCameraFollowsObjectChanged"
#define VWSTUDIO_CENTERCAMERAONOBJECT_EVENT_STR "OnCenterCameraOnObject"

//Fired by Multimedia Exemplars 
#define VWSTUDIO_VISIBILITYCHANGED_EVENT_STR "OnVisibilityChanged"
#define VWSTUDIO_ISMOVEABLECHANGED_EVENT_STR "OnIsMoveableChanged"

//Fired by control manager
#define VWSTUDIO_ADDSELECTION_EVENT_STR "OnAddSelection"
#define VWSTUDIO_REMOVESELECTION_EVENT_STR "OnRemoveSelection"
#define VWSTUDIO_CLEARSELECTION_EVENT_STR "OnClearSelection"

//Fired by geometry/boundary tools
#define VWSTUDIO_SELECTIONPOSITIONCHANGED_EVENT_STR "OnPositionChanged"
#define VWSTUDIO_SELECTIONDIRECTIONCHANGED_EVENT_STR "OnDirectionChanged"
#define VWSTUDIO_SELECTIONSCALECHANGED_EVENT_STR "OnScaleChanged"
#define VWSTUDIO_BOUNDARYUPDATE_EVENT_STR "BoundaryUpdate"  //Hints boundary editor to refresh a boundary
#endif