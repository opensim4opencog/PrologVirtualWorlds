// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef vwevents_h
#define vwevents_h

// this file should contain the names of all the commonly used strings for events

#define VW_HELP_EVENT_STR "Help"

// profile-related events
#define VW_BEFRIEND_EVENT_STR "Befriended"
#define VW_UNBEFRIEND_EVENT_STR "Unbefriended"
#define VW_NOTAFRIEND_EVENT_STR "NotAFriend"
#define VW_CANNOTMAKEYOURSELFAFRIEND_EVENT_STR "CannotMakeYourselfAFriend"
#define VW_ALREADYAFRIEND_EVENT_STR "AlreadyAFriend"
#define VW_PENDINGFRIENDSHIP_EVENT_STR "OnPendingFriendship"
#define VW_ALLOWFRIENDSHIP_EVENT_STR "OnAllowFriendship"
#define VW_DECLINEFRIENDSHIP_EVENT_STR "OnDeclineFriendship"

#define VW_IGNOREDUSER_EVENT_STR "IgnoredUser"
#define VW_UNIGNOREDUSER_EVENT_STR "UnignoredUser"
#define VW_IGNOREDBYUSER_EVENT_STR "IgnoredByUser"
#define VW_UNIGNOREDBYUSER_EVENT_STR "UnignoredByUser"

#define VW_PROFILECHANGED_EVENT_STR "OnProfileChanged"

// system notification events
#define VW_RECONNECT_EVENT_STR "OnReconnect"
#define VW_RECONNECTFAILED_EVENT_STR "ReconnectFailed"

#define VW_NAMECHANGED_EVENT_STR "OnNameChanged"
#define VW_GEOMETRYCHANGED_EVENT_STR "OnGeometryChanged"
#define VW_BACKGROUNDCOLORCHANGED_EVENT_STR "OnBackgroundColorChanged"
#define VW_UIELEMENTCHANGED_EVENT_STR "OnUIElementChanged"
#define VW_EXEMPLARS_CHANGED_EVENT_STR "OnExemplarsChanged"

#define VW_TELL_EVENT_STR "OnTell"
#define VW_ENTERINVENTORY_EVENT_STR "OnEnterInventory"
#define VW_LEAVEINVENTORY_EVENT_STR "OnLeaveInventory"
#define VW_ENTERROOM_EVENT_STR "OnEnterRoom"
#define VW_LEAVEROOM_EVENT_STR "OnLeaveRoom"
#define VW_CONTENTENTER_EVENT_STR "OnContentEnter"
#define VW_CONTENTLEAVE_EVENT_STR "OnContentLeave"

#define VW_CREATEROOM_EVENT_STR "OnCreateRoom"

#define VW_ONFIRSTCONNECT_EVENT_STR "OnFirstConnect"
#define VW_SHOWWELCOME_EVENT_STR "OnShowWelcome"

#define VW_ENTERWORLD_EVENT_STR "OnEnterWorld"
#define VW_LEAVEWORLD_EVENT_STR "OnLeaveWorld"

// Avatar events
#define VW_WEAR_EVENT_STR "OnWear"
#define VW_UNWEAR_EVENT_STR "OnUnWear"

#define VW_WIELD_EVENT_STR "OnWield"
#define VW_UNWIELD_EVENT_STR "OnUnWield"

#define VW_SHOWPROFILE_EVENT_STR "OnShowProfile"
#define VW_EDITPROFILE_EVENT_STR "OnEditProfile"
#define VW_WHISPER_EVENT_STR "OnWhisper"
#define VW_DOGESTURE_EVENT_STR "OnDoGesture"

#define VW_ONISAUTHORCHANGED_EVENT_STR "OnIsAuthorChanged"

// Host events
#define VW_LOGOFF_EVENT_STR "OnLogOff"
#define VW_HOSTSTATUSCHANGED_EVENT_STR "OnHostStatusChanged"
#define VW_SHOWLOGOFF_EVENT_STR "OnShowLogoffDialog"

// Thing events
#define VW_MOREABOUT_EVENT_STR "OnMoreAbout"
#define VW_EDITOBJECT_EVENT_STR "OnEditObject"


// Portal events
#define VW_SHOWTRANSPORT_EVENT_STR "OnShowTransportDialog"
#define VW_PORTALARRIVE_EVENT_STR "OnPortalArrive"
#define VW_PORTALDESTINATIONNOTSET_EVENT_STR "PortalDestinationNotSet"
#define VW_PORTALDESTINATIONINVALID_EVENT_STR "PortalDestinationInvalid"

// UI Events to popup HTML dialogs
#define VW_SHOWHTML_STR "ShowHTML"
#define VW_SHOWURL_STR "ShowURL"
#define VW_SHOWDIALOG_STR "OnShowDialog"

#define VW_STARTCOMPOSIT_EVENT_STR "OnStartSceneComposite"
#define VW_FINISHCOMPOSIT_EVENT_STR "OnFinishSceneComposite"

#endif

