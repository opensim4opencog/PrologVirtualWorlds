// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WorldHlp.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
//#include <vwstats.h>
#include <vwanim.h>
#include <inetfile.h>
#include <registry.h>
#include <dirserv.h>
#include "worldobj.h"
#include "thingobj.h"
#include "propsecu.h"
#include "scripts.h"
#include "global.h"
#include "reghelp.h"
#include "urlhelp.h"
#include "syshelp.h"
#include "reghelp.h"

EXTERN_C const CLSID CLSID_DServ;
EXTERN_C const IID IID_IDServ;
//EXTERN_C const CLSID CLSID_VWStatCol;
EXTERN_C const CLSID CLSID_VWAnimator;
EXTERN_C const CLSID CLSID_InternetFileManager;
EXTERN_C const CLSID CLSID_VWObjectRepository;
EXTERN_C const CLSID CLSID_AvatarProfile;

//#define TRACE_CALLSTACK VWT_NONE
#define TRACE_CALLSTACK VWT_METHOD

////////////////////////////////////////////////////////////////
// Parameters
// 

#define ENABLE_LOGGING

#define SUPPORT_SECURITY_OVERRIDE

// defining this makes SaveDatabase and SaveToScript fail if user isn't world owner
#define SUPPORT_WORLDOWNER_RESTRICTIONS

// must define XXX_NETWORK_DEBUG to enable XXX_PERF_DEBUG
// need to flip-flop TRACE_SEND and TRACE_RECV to see output

//#define SEND_NETWORK_DEBUG
//#define GENERAL_SEND_NETWORK_DEBUG
//#define SEND_PERF_DEBUG

//#define RECV_NETWORK_DEBUG
//#define RECV_PERF_DEBUG

//#define WORLD_PERF_DEBUG

// define to track connect/disconnect
//#define WORLD_USER_DEBUG

// use VWT_NONE to always get a trace

//#define TRACE_GLOBALPATHS VWT_NONE
#define TRACE_GLOBALPATHS VWT_METHOD

//#define TRACE_BYSTANDERS VWT_NONE
#define TRACE_BYSTANDERS VWT_METHOD

//#define TRACE_SEND VWT_NONE
#define TRACE_SEND VWT_METHOD

//#define TRACE_RECV VWT_NONE
#define TRACE_RECV VWT_METHOD

//#define TRACE_LOGGING VWT_NONE
#define TRACE_LOGGING VWT_METHOD

//#define TRACE_PROCESSMSG VWT_NONE
#define TRACE_PROCESSMSG VWT_METHOD

//#define TRACE_ROOTURL VWT_NONE
#define TRACE_ROOTURL VWT_METHOD

//#define TRACE_SENDCONTAINER VWT_NONE
#define TRACE_SENDCONTAINER VWT_METHOD

// from thingobj.cpp
HRESULT IsRoomInhabited(IThing* pRoom, VARIANT_BOOL* pbInhabited);

#define VALIDATE_WORLD		{ if (m_pWorld == NULL) return ReportWorldError(VWOBJECT_E_INVALIDWORLD); }

// IMPORTANT: the semantic for DISPPARAMS is to pack the structure in
// reverse order (actual param 0 goes in dispparam slot n - 1).
// For remoting, the dispparams are sent already packed correctly.

STDMETHODIMP CWorldObject::ProcessLogRecord(IUnMarshallBuffer *pUbuffer)
{
	HRESULT hr = S_OK;
	HRESULT senthr = S_OK;
	CComVariant var;
	unsigned char chMsg = 0;
	unsigned char chReply = 0;
	unsigned long ulVersion = 0L;
	VARIANT_BOOL bOldFreezeEvents = VARIANT_FALSE;
	VARIANT_BOOL bOldShouldRemote = VARIANT_FALSE;
#ifdef RECV_NETWORK_DEBUG
	DWORD dwTime = GetTickCount();
#endif

	hr = pUbuffer->get_ULong(&ulVersion);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (ulVersion != PROTOCOL_VERSION)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessLogRecord: client version number not same as server version number\n");
#endif
		hr = VWOBJECT_E_INVALIDCLIENTVERSION;
		goto ERROR_ENCOUNTERED;
	}

	hr = pUbuffer->get_Byte(&chReply);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pUbuffer->get_Byte(&chMsg);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "\nCWorldObject::ProcessLogRecord: message '%c'\n", chMsg);

	bOldFreezeEvents = m_bFreezeEvents;
	bOldShouldRemote = m_bShouldRemote;

	// don't fire any OLE or in-world events on changes
	m_bFreezeEvents = VARIANT_TRUE;

	// don't try to remote any commands (this also disables logging)
	m_bShouldRemote = VARIANT_FALSE;

	try
	{
		// shouldn't need this if we're persisting callstacks
#ifndef SUPPORT_SERVER_CALLSTACK
		CSetUID SetUID(m_pWorld, m_pGlobal, m_pGlobal, VARIANT_TRUE);
#endif

		hr = ProcessMessageExt(chMsg, pUbuffer, &senthr, &var);
	}
	catch (...)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessLogRecord: caught exception in ProcessMessageExt\n");
#endif

		hr = VWOBJECT_E_PROCESSMSGEXCEPTION;
	}

	if (SUCCEEDED(hr))
		hr = senthr;

#ifdef RECV_NETWORK_DEBUG
#ifdef RECV_PERF_DEBUG
	{
		DWORD dwNow = GetTickCount();

		VWREPORT(m_pWorld, VWR_PERFLOG, "Process Log Record,%d,%c,%d\n", dwNow, chMsg, dwNow - dwTime);
	}
#endif
#endif

	// NOTE: we ignore senthr and var from ProcessMessageExt

	// turn remoting back on (this also enables logging)
	m_bShouldRemote = bOldShouldRemote;

	// allow events
	m_bFreezeEvents = bOldFreezeEvents;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ProcessLogRecord: end\n");

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::ProcessMessage(IVWCommMessage* pmessage)
{
	HRESULT senthr = S_OK;
	HRESULT hr = S_OK;
	unsigned char chReply = 0;
	unsigned char chMsg = 0;
	IUnMarshallBuffer* pUbuffer = NULL;
	CComVariant var;
	unsigned long ulVersion = 0L;
	VARIANT_BOOL bOldShouldRemote = VARIANT_FALSE;
#ifdef RECV_NETWORK_DEBUG
	DWORD dwTime;
#endif

	m_bInProcessMessage = VARIANT_TRUE;

	// if server-side, get the connection out of the message
	if (m_bServerSide == VARIANT_TRUE)
	{
		hr = pmessage->GetConnection(&m_pVWConnection);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// cache user during ProcessMessage
		m_pUser = m_clist.FindConnection(m_pVWConnection);
		SAFEADDREF(m_pUser);
	}

	// get the message buffer
	hr = pmessage->GetData(&pUbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef RECV_NETWORK_DEBUG
	{
		DWORD lBufferLength = 0L;
		static long lCount = 0L;
		static DWORD lTotalLength = 0L;
		static DWORD dwLastTime = 0L;
		static DWORD dwTotalTime = 0L;
		DWORD dwDelta = 0L;

		dwTime = GetTickCount();

		if (dwLastTime != 0)
		{
			dwDelta = dwTime - dwLastTime;
			dwTotalTime += dwDelta;
		}

		dwLastTime = dwTime;

		if (SUCCEEDED(pUbuffer->get_Length(&lBufferLength)))
		{
			lTotalLength += lBufferLength;
			lCount++;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_RECV, "CWorldObject::ProcessMessage: time %d, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
				dwTime, lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif
		}
	}
#endif

	// version already checked by client code
	hr = pUbuffer->get_ULong(&ulVersion);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pUbuffer->get_Byte(&chReply);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pUbuffer->get_Byte(&chMsg);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	m_ucCurrentMessage = chMsg;

	// flip-flop remote/notify flags during client message processing
	if (m_bServerSide == VARIANT_FALSE)
	{
		bOldShouldRemote = m_bShouldRemote;

		m_bShouldRemote = VARIANT_FALSE;
	}

	try
	{
		hr = ProcessMessageExt(chMsg, pUbuffer, &senthr, &var);
	}
	catch (...)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessage: caught exception in ProcessMessageExt\n");
#endif

		// NOTE: set senthr and clear var so we can still do ReplyWithVARIANT
		senthr = hr = VWOBJECT_E_PROCESSMSGEXCEPTION;

		var.Clear();
	}

	if (SUCCEEDED(hr))
		hr = senthr;

	if (m_bServerSide == VARIANT_FALSE)
	{
		m_bShouldRemote = bOldShouldRemote;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ProcessMessage: message '%c', hr %x\n", chMsg, hr);

#ifdef RECV_NETWORK_DEBUG
	{
		DWORD dwNow = GetTickCount();

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_RECV, "CWorldObject::ProcessMessage: (done) time %d, message '%c', transaction %5.3f sec\n", 
			dwNow, chMsg, (double)(dwNow - dwTime)/1000.0);
#endif

#ifdef RECV_PERF_DEBUG
		VWREPORT(m_pWorld, VWR_PERFLOG, "Process Message,%d,%c,%d\n", dwNow, chMsg, dwNow - dwTime);
#endif
	}
#endif

	// if reply needed, send VARIANT
	// else, send NULL
	/* hr = */ ReplyWithVARIANT(pmessage, senthr, (chReply == 'R') ? &var : NULL);

ERROR_ENCOUNTERED:
	SAFERELEASE(pUbuffer);

	// if server-side, release the connection
	if (m_bServerSide == VARIANT_TRUE)
	{
		SAFERELEASE(m_pVWConnection);

		// release cached user
		SAFERELEASE(m_pUser);
	}

	m_ucCurrentMessage = ' ';

	m_bInProcessMessage = VARIANT_FALSE;

	return hr;
}

/*
	O - IThing byval
	P - IObjectProperty byval
	D - IMethod byval
	E - IThing (exemplar) byval
	W - global IThing (World) byval
	M - IModule byval

	o - IThing command
	p - IObjectProperty command
	d - IMethod command
	w - IWorld command
	m - IModule command
 */

STDMETHODIMP CWorldObject::ProcessMessageExt(unsigned char ch, IUnMarshallBuffer* pUbuffer, HRESULT* precvhr, VARIANT* pvar)
{
	IThing* pthing = NULL;
	IModule* pmodule = NULL;
	IObjectProperty* pprop = NULL;
	IMethod* pmethod = NULL;
	IDispatch* pdispatch = NULL;
	HRESULT hr = S_OK;
	CComVariant var;
	CComBSTR bstrTemp;
	DISPPARAMS dispparams;
	DISPID dispid = DISPID_UNKNOWN;
	WORD wFlags = 0;
	EXCEPINFO excepinfo;
	unsigned int argerr = 0;
	DISPID dispidNamedArgs = DISPID_PROPERTYPUT;
#ifdef RECV_NETWORK_DEBUG
	DWORD dwStart = 0;
#endif
	CSetUID* pSetUID = NULL;
	IThing* pOldThis = NULL;
	IThing* pOldCaller = NULL;
	IThing* pOldSuper = NULL;

	// NOTE: need save off old ptrs for callstack stuff
	/* hr = */ get_This(&pOldThis);
	/* hr = */ get_Caller(&pOldCaller);
	/* hr = */ get_Super(&pOldSuper);

	memset(&excepinfo, 0x0, sizeof(EXCEPINFO));

	if (precvhr)
		*precvhr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ProcessMessageExt\n");

	hr = InitializeDISPPARAMS(&dispparams, 0);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: if we send callstacks server-to-client, can we get rid of this?

#ifndef SUPPORT_SERVER_CALLSTACK
	// if we're on the client side, set the security context to indicate that
	// we're handling a server side message and should tolerate top level security violations
	if (m_bServerSide == VARIANT_FALSE)
	{
		pSetUID = new CSetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

		if (pSetUID == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessageExt: failed to allocate CSetUID\n");

			hr = VWOBJECT_E_INVALIDSECURITY;
			goto ERROR_ENCOUNTERED;
		}
	}
#endif

	if (isupper(ch))
	{
		if (ch == 'W')
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: marshalled world object\n");

#ifdef RECV_NETWORK_DEBUG
#ifdef RECV_PERF_DEBUG
			VWREPORT(m_pWorld, VWR_PERFLOG, "ProcessMessageExt,%d,World\n", GetTickCount());
#endif
#endif

			hr = UnMarshallThing(pUbuffer, &pthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			ASSERT(pthing);

			// callback to setup state after global object is received
			hr = OnReceiveWorld();
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else if (ch == 'O')
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: marshalled object\n");

			hr = UnMarshallThing(pUbuffer, &pthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			ASSERT(pthing);

#ifdef RECV_NETWORK_DEBUG
#ifdef RECV_PERF_DEBUG
			{
				CComBSTR bstrName("n/a");

				if (pthing)
					pthing->get_Name(&bstrName.m_str);

				VWREPORT(m_pWorld, VWR_PERFLOG, "ProcessMessageExt,%d,Thing,%s\n", GetTickCount(), CString(bstrName));
			}
#endif
#endif

		}
		else if (ch == 'E')
		{
			IPropertyMap* pExemplars = NULL;

			// unmarshall exemplar
			hr = UnMarshallThing(pUbuffer, &pthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			ASSERT(pthing);

			// unmarshall exemplar type
			hr = pUbuffer->get_String(&bstrTemp.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: marshalled exemplar, name (%s)\n", CString(bstrTemp));
#endif

#ifdef RECV_NETWORK_DEBUG
#ifdef RECV_PERF_DEBUG
			VWREPORT(m_pWorld, VWR_PERFLOG, "ProcessMessageExt,%d,Exemplar,%s\n", GetTickCount(), CString(bstrTemp));
#endif
#endif

			hr = get_Exemplars(&pExemplars);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			m_bLocalMode = VARIANT_TRUE;

			{
				CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

				// add exemplar to exemplar map
				hr = pExemplars->put_Thing(bstrTemp, pthing);
			}

			m_bLocalMode = VARIANT_FALSE;

			SAFERELEASE(pExemplars);

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else if (ch == 'M')
		{
			IPropertyMap* pModules = NULL;
			IPropertyList* pModuleNames = NULL;

			hr = UnMarshallObjectProperty(pUbuffer, (IObjectProperty**)&pmodule);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			ASSERT(pmodule);

			// unmarshall module name
			hr = pUbuffer->get_String(&bstrTemp.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: marshalled module, name (%s)\n", CString(bstrTemp));
#endif

#ifdef RECV_NETWORK_DEBUG
#ifdef RECV_PERF_DEBUG
			VWREPORT(m_pWorld, VWR_PERFLOG, "ProcessMessageExt,%d,Module,%s\n", GetTickCount(), CString(bstrTemp));
#endif
#endif

			hr = get_Modules(&pModules);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			m_bLocalMode = VARIANT_TRUE;

			{
				CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

				// add module to module map
				hr = pModules->put_ObjectProperty(bstrTemp, pmodule);
			}

			m_bLocalMode = VARIANT_FALSE;

			SAFERELEASE(pModules);

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
//			ASSERT(FALSE);

			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessageExt: unknown message type (%c)\n", ch);
			hr = VWOBJECT_E_UNKNOWNMESSAGETYPE;
			goto ERROR_ENCOUNTERED;
		}
	}
	else
	{
		if (ch == 'o')
		{
#ifdef SUPPORT_SERVER_CALLSTACK
			hr = ReadCommandCallstack(pUbuffer, &pSetUID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
#endif

			hr = UnMarshallThing(pUbuffer, &pthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: IThing command\n");

			pdispatch = (IDispatch*)pthing;
		}
		else if (ch == 'p')
		{
#ifdef SUPPORT_SERVER_CALLSTACK
			hr = ReadCommandCallstack(pUbuffer, &pSetUID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
#endif

			// unmarshal IThing
			hr = UnMarshallThing(pUbuffer, &pthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// unmarshal property name
			hr = pUbuffer->get_String(&bstrTemp.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
			{
				CComBSTR bstrName("n/a");

				if (pthing)
					pthing->get_Name(&bstrName.m_str);

				VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: IObjectProperty command, thing (%s), property name (%s)\n", CString(bstrName), CString(bstrTemp));
			}
#endif

			// grab object property from IThing
			hr = pthing->get_ObjectProperty(bstrTemp, &pprop);
			if (FAILED(hr))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessageExt: failed to lookup object property (%s)\n", CString(bstrTemp));
#endif
				goto ERROR_ENCOUNTERED;
			}

			pdispatch = (IDispatch*)pprop;
		}
		else if (ch == 'd')
		{
#ifdef SUPPORT_SERVER_CALLSTACK
			hr = ReadCommandCallstack(pUbuffer, &pSetUID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
#endif

			// unmarshal IThing
			hr = UnMarshallThing(pUbuffer, &pthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// unmarshal method name
			hr = pUbuffer->get_String(&bstrTemp.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
			{
				CComBSTR bstrName("n/a");

				if (pthing)
					pthing->get_Name(&bstrName.m_str);

				VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: IMethod command, thing (%s), method (%s)\n", CString(bstrName), CString(bstrTemp));
			}
#endif

			// grab method from IThing
			hr = pthing->get_MethodExt(bstrTemp, HashKey<BSTR>(bstrTemp), NULL, &pmethod);
			if (FAILED(hr))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessageExt: failed to lookup method (%s)\n", CString(bstrTemp));
#endif
				goto ERROR_ENCOUNTERED;
			}

			pdispatch = (IDispatch*)pmethod;
		}
		else if (ch == 'm')
		{
#ifdef SUPPORT_SERVER_CALLSTACK
			hr = ReadCommandCallstack(pUbuffer, &pSetUID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
#endif

			// unmarshal module name
			hr = pUbuffer->get_String(&bstrTemp.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: IModule command, name (%s)\n", CString(bstrTemp));

			hr = get_Module(bstrTemp, &pmodule);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			pdispatch = (IDispatch*)pmodule;
		}
		else if (ch == 'w')
		{
#if 0 // def SUPPORT_SERVER_CALLSTACK
			hr = ReadCommandCallstack(pUbuffer, &pSetUID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
#endif

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: IWorld method\n");

			pdispatch = (IDispatch*)GetUnknown();
		}
		else
		{
//			ASSERT(FALSE);

			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessageExt: unknown message type (%c)\n", ch);
			hr = VWOBJECT_E_UNKNOWNMESSAGETYPE;
			goto ERROR_ENCOUNTERED;
		}

		// unmarshal DISPID
		hr = pUbuffer->get_Long(&dispid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// unmarshal wFlags
		hr = pUbuffer->get_UShort(&wFlags);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// unmarshal args
		hr = UnMarshallDISPPARAMS(pUbuffer, &dispparams);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessageExt: UnMarshallDISPPARAMS, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}

//		if (m_bShouldRemote == VARIANT_TRUE)
//		{
			// figure out if we're processing a property change request
			if (ch == 'o')
			{		   
				// DISPID_THING_REMOTEPROPERTY is always a synchronous
				// DISPATCH_PROPERTYGET operation and therefore isn't bounced back to clients
				if (dispid == DISPID_THING_INVOKEMETHODINT || 
					dispid == DISPID_THING_FIREEVENTINT ||
//					dispid == DISPID_THING_FINDWITHININT ||
					dispid == DISPID_THING_MOVEINTOINT ||
//					dispid == DISPID_THING_GIVEINT ||
					dispid == DISPID_THING_REMOTEPROPERTY)
				{
					m_bInPropChange = VARIANT_FALSE;
				}
				else if (dispid == DISPID_THING_ADDPROPERTYINT ||
						dispid == DISPID_THING_REMOVEPROPERTY ||
						dispid == DISPID_THING_ADDMETHODINT ||
						dispid == DISPID_THING_REMOVEMETHOD ||
						dispid == DISPID_THING_INSTANCEPROPERTY)
				{
					m_bInPropChange = VARIANT_TRUE;
				}
				else if (dispid == DISPID_THING_PROPERTY ||
						dispid == DISPID_THING_METHOD ||
						dispid == DISPID_THING_EXEMPLARINT)
				{
					// NOTE: should always be true
					if (wFlags == DISPATCH_PROPERTYPUT)
						m_bInPropChange = VARIANT_TRUE;
					else
					{
						ASSERT(FALSE);
						m_bInPropChange = VARIANT_FALSE;
					}
				}
			}
			else if (ch == 'p')
				m_bInPropChange = VARIANT_TRUE;
			else if (ch == 'd')
				m_bInPropChange = VARIANT_TRUE;
			else if (ch == 'm')
				m_bInPropChange = VARIANT_TRUE;
			else if (ch == 'w')
				m_bInPropChange = VARIANT_FALSE;
//		}
//		else
//			m_bInPropChange = VARIANT_FALSE;

#ifdef _DEBUG
		{
			long id = NULL_OBJECT;

			if (pthing)
				pthing->get_ID(&id);

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: command '%c', id %d, dispid %d, wFlags %x, prop change %s\n", 
				ch, id, dispid, wFlags, m_bInPropChange ? "yes" : "no");
		}
#endif

#ifdef RECV_NETWORK_DEBUG
		{
			CString strCommand;
			CString strMethodName("n/a");
			CComBSTR bstrName("n/a");

			dwStart = GetTickCount();

			if (pthing)
				pthing->get_Name(&bstrName.m_str);

			if (ch == 'o')
			{
				strCommand = "Thing";

				if (dispid == DISPID_THING_INVOKEMETHODINT)
					strMethodName = dispparams.rgvarg[1].bstrVal;
			}
			else if (ch == 'p')
				strCommand = "ObjectProperty";
			else if (ch == 'd')
				strCommand = "Method";
			else if (ch == 'm')
				strCommand = "Module";
			else if (ch == 'w')
				strCommand = "World";

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_RECV, "CWorldObject::ProcessMessageExt: time %d, %s command, name (%s), dispid %d, method (%s)\n", 
				dwStart, strCommand, bstrTemp ? CString(bstrTemp) : CString(bstrName), dispid, strMethodName);
#endif

#ifdef RECV_PERF_DEBUG
			VWREPORT(m_pWorld, VWR_PERFLOG, "ProcessMessageExt,%d,%s Command,%s,%d,%s\n", 
				dwStart, strCommand, bstrTemp ? CString(bstrTemp) : CString(bstrName), dispid, strMethodName);
#endif
		}
#endif

		if (pdispatch != NULL)
		{
			// invoke method
			if (wFlags == DISPATCH_PROPERTYPUT)
			{
				dispparams.rgdispidNamedArgs = &dispidNamedArgs;
				dispparams.cNamedArgs = 1;
			}

			hr = pdispatch->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, wFlags,
				&dispparams, (wFlags == DISPATCH_PROPERTYPUT) ? NULL : pvar, &excepinfo, &argerr);
		
			if (wFlags == DISPATCH_PROPERTYPUT)
			{
				dispparams.rgdispidNamedArgs = NULL;
				dispparams.cNamedArgs = 0;
			}
		}
		else
		{
			// REVIEW: shouldn't ever happen
			ASSERT(FALSE);

			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ProcessMessageExt: NULL IDispatch* ptr to invoke\n");
			hr = VWOBJECT_E_INVOKEEXCEPTION;
		}

		// reset flag
		m_bInPropChange = VARIANT_FALSE;

#ifdef RECV_NETWORK_DEBUG
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_RECV, "CWorldObject::ProcessMessageExt: (done) time %d, invocation %5.3f sec\n", 
			GetTickCount(), (double)(GetTickCount() - dwStart)/1000.0);
#endif
#endif

		if (hr == DISP_E_EXCEPTION)
		{
			HRESULT excephr = ResultFromScode(excepinfo.scode);
			CComBSTR bstrName(bstrTemp);
			CString strOtherName("n/a");
			CString strCommand;
			long lID = 0L;

			if ((BSTR)bstrTemp == NULL)
			{
				if (pthing)
					pthing->get_Name(&bstrName.m_str);
			}

			if (pthing)
				pthing->get_ID(&lID);

			if (bstrName.Length() == 0)
				bstrName = "<unknown>";

			if (ch == 'o')
			{
				strCommand = "thing";

				if (dispid == DISPID_THING_INVOKEMETHODINT)
					strOtherName = dispparams.rgvarg[1].bstrVal;
				else if (dispid == DISPID_THING_ADDPROPERTYINT)
					strOtherName = dispparams.rgvarg[7].bstrVal;
				else if (dispid == DISPID_THING_PROPERTY)
					strOtherName = dispparams.rgvarg[1].bstrVal;
				else if (dispid == DISPID_THING_ADDMETHODINT)
					strOtherName = dispparams.rgvarg[4].bstrVal;
				else if (dispid == DISPID_THING_METHOD)
					strOtherName = dispparams.rgvarg[1].bstrVal;
				else if (dispid == DISPID_THING_INSTANCEPROPERTY)
					strOtherName = dispparams.rgvarg[1].bstrVal;
			}
			else if (ch == 'p')
			{
				CComBSTR bstrPropertyName("n/a");

				strCommand = "objectproperty";

				if (pprop)
				{
					IThing* pParent = NULL;
					/* hr = */ pprop->get_Parent(&pParent);
					if (pParent)
						pParent->get_Name(&bstrName.m_str);
					SAFERELEASE(pParent);

					pprop->get_PropertyName(&bstrPropertyName.m_str);

					strOtherName = bstrPropertyName;
				}
			}
			else if (ch == 'd')
				strCommand = "method";
			else if (ch == 'm')
				strCommand = "module";
			else if (ch == 'w')
				strCommand = "world";

			if (precvhr)
				*precvhr = excephr;

			// absorb DISP_E_EXCEPTION error if we're handing back HRESULT
			hr = S_OK;

//			ASSERT(FALSE);

			VWREPORT(m_pWorld, VWR_ERROR, "Error occurred while processing %s command, id %d, %s.%s, dispid %d, hr %x, %s\n", 
				strCommand, lID, CString(bstrName), strOtherName, dispid, excephr, CString(excepinfo.bstrDescription));
		}
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		CComBSTR bstrName(bstrTemp);
		CString strOtherName("n/a");
		CString strCommand;
		long lID = 0L;

		if ((BSTR)bstrTemp == NULL)
		{
			if (pthing)
				pthing->get_Name(&bstrName.m_str);
		}

		if (pthing)
			pthing->get_ID(&lID);

		if (bstrName.Length() == 0)
			bstrName = "<unknown>";

		if (ch == 'o')
		{
			strCommand = "thing";

			if (dispid == DISPID_THING_INVOKEMETHODINT)
				strOtherName = dispparams.rgvarg[1].bstrVal;
			else if (dispid == DISPID_THING_ADDPROPERTYINT)
				strOtherName = dispparams.rgvarg[7].bstrVal;
			else if (dispid == DISPID_THING_PROPERTY)
				strOtherName = dispparams.rgvarg[1].bstrVal;
			else if (dispid == DISPID_THING_ADDMETHODINT)
				strOtherName = dispparams.rgvarg[4].bstrVal;
			else if (dispid == DISPID_THING_METHOD)
				strOtherName = dispparams.rgvarg[1].bstrVal;
			else if (dispid == DISPID_THING_INSTANCEPROPERTY)
				strOtherName = dispparams.rgvarg[1].bstrVal;
		}
		else if (ch == 'p')
		{
			CComBSTR bstrPropertyName("n/a");

			strCommand = "objectproperty";

			if (pprop)
			{
				IThing* pParent = NULL;
				/* hr = */ pprop->get_Parent(&pParent);
				if (pParent)
					pParent->get_Name(&bstrName.m_str);
				SAFERELEASE(pParent);

				pprop->get_PropertyName(&bstrPropertyName.m_str);

				strOtherName = bstrPropertyName;
			}
		}
		else if (ch == 'd')
			strCommand = "method";
		else if (ch == 'm')
			strCommand = "module";
		else if (ch == 'w')
			strCommand = "world";

//		ASSERT(FALSE);

		VWREPORT(m_pWorld, VWR_ERROR, "Error occurred while processing %s command, id %d, %s.%s, dispid %d, hr %x\n", 
			strCommand, lID, CString(bstrName), strOtherName, dispid, hr);
	}

	TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

	if (pSetUID)
	{
		delete pSetUID;
		pSetUID = NULL;
	}

	// NOTE: need restore saved ptrs for callstack stuff
	/* hr = */ put_This(pOldThis);
	SAFERELEASE(pOldThis);

	/* hr = */ put_Caller(pOldCaller);
	SAFERELEASE(pOldCaller);

	/* hr = */ put_Super(pOldSuper);
	SAFERELEASE(pOldSuper);

	SAFERELEASE(pthing);
	SAFERELEASE(pmodule);
	SAFERELEASE(pprop);
	SAFERELEASE(pmethod);

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_PROCESSMSG, "CWorldObject::ProcessMessageExt: end\n");

	return hr;
}

HRESULT CWorldObject::ReplyWithVARIANT(IVWCommMessage *pmess, HRESULT replyhr, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bSync = VARIANT_FALSE;
	IVWCommConnection *pconn = NULL;

	if (FAILED(pmess->GetSynchronous(&bSync)) || bSync == VARIANT_FALSE)
	{
//		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "Warning: Replying to non synchronous message\n");
		return S_OK; // E_FAIL;
	}

	// get a reply buffer
	hr = pmess->GetConnection(&pconn);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pMarshBuf->put_Long(replyhr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (SUCCEEDED(replyhr) && pvar != NULL)
	{
		hr = MarshallProperty(*pvar, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK, m_pMarshBuf);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ReplyWithVARIANT\n");

	hr = pmess->Reply(m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();
	SAFERELEASE(pconn);

	return hr;
}

// Dialog box mungus - very scary...
char szPassword[80]; // receives name of item to delete.  
BOOL CALLBACK DeleteItemProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)    
	{ 
	case WM_COMMAND:            
		switch (LOWORD(wParam))
		{ 
		case IDOK: 
			if (!GetDlgItemText(hwndDlg, IDC_EDIT_PASS, szPassword, 80)) 
				*szPassword=0;                      // Fall through.  
		case IDCANCEL:                     
			EndDialog(hwndDlg, wParam); 
			return TRUE;             
		}
	}
	return FALSE; 
} 

STDMETHODIMP CWorldObject::Connect(BSTR bstrName, BSTR bstrPassword, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	CComBSTR bstrProfile;

	if (m_pThingCaller != NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Connect: only callable from external script\n");
		hr = VWOBJECT_E_EXTERNALCALLONLY;
		goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::Connect\n");

	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Connect: outgoing pointer is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppthing = NULL;

	if (m_bUserConnected == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Connect: already connected\n");
		return ReportWorldError(VWOBJECT_E_ALREADYCONNECTED);
	}

	// Grab profile from registry
	// returns S_OK w/ an empty pProfile if something fails.
	// (tolerant of an empty registry)
	hr = LoadRegistryIntoProfile(bstrName, &bstrProfile.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_bServerSide == VARIANT_FALSE)
		{
			VARIANT	varProfile;

			varProfile.vt = VT_BLOB;
			varProfile.bstrVal = bstrProfile;

			hr = ConnectInt(bstrName, bstrPassword, varProfile, ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else 
		{
			// no-op: shouldn't be called directly on server
			ASSERT(FALSE);
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_BSTR;
		dispparams.rgvarg[2].bstrVal = bstrName;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrPassword;

		dispparams.rgvarg[0].vt = VT_BLOB;
		dispparams.rgvarg[0].bstrVal = bstrProfile.m_str;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_CONNECTINT, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;

			if (SUCCEEDED(senthr))
			{
				ASSERT(var.vt == VT_DISPATCH);

				*ppthing = (IThing*)var.pdispVal;
				SAFEADDREF(*ppthing);

				if (m_clist.AddConnection(m_pVWConnection, *ppthing) == FALSE)
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Connect: AddConnection failed\n");
					hr = VWOBJECT_E_CONNECTIONERROR;
					goto ERROR_ENCOUNTERED;
				}

				// if on client, cache user
				if (m_bServerSide == VARIANT_FALSE)
				{
#ifdef _DEBUG
					{
						CComBSTR bstrSockAddr;

						/* hr = */ m_pVWConnection->get_SocketAddress(&bstrSockAddr.m_str);

						VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::Connect: server ip address (%s)\n", CString(bstrSockAddr));
					}
#endif
		
					m_pUser = *ppthing;
					SAFEADDREF(m_pUser);
				}

				NotifyUserConnect(*ppthing);

				m_bUserConnected = VARIANT_TRUE;
			}
			else if ((senthr == VWOBJECT_E_INVALIDPASSWORD) && (m_bServerSide == VARIANT_FALSE))
			{
				if (::DialogBox(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDD_PASSWORD), NULL, (DLGPROC)DeleteItemProc) ==IDOK)
				{ 
                    CComBSTR	tempbstr = szPassword;
					hr = Connect(bstrName, tempbstr, ppthing);
				}
			}
		}
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}


STDMETHODIMP CWorldObject::ConnectInt(BSTR bstrName, BSTR bstrPassword, VARIANT varProfile, IThing** ppthing)
{
	HRESULT hr = S_OK;
	CComVariant varGUID;
	BSTR bstrProfile = NULL;
	IAvatarProfile* pProfile = NULL;
	IUnMarshallBuffer* pbuffer = NULL;
	long lProfSize;
	VARIANT_BOOL bRoaming = VARIANT_FALSE;
	static CComBSTR bstrRoaming("EnableRoamingProfiles");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ConnectInt\n");

	// unmarshal the profile
	// 1. Create unmarshal buffer and profile object
	hr = CreateUnMarshallBuffer(&pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = CreateObjectPropertyExt(CLSID_AvatarProfile, NULL, (IObjectProperty**)&pProfile);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = m_pGlobal->get_BOOL(bstrRoaming, &bRoaming);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if world doesn't support roaming profiles, don't use the passed-in profile .vwc file
	if (bRoaming == VARIANT_TRUE)
	{
		// 2.  set the contents of the unmarshal buffer to the BSTR
		if ((varProfile.vt == VT_BLOB) || (varProfile.vt == VT_BSTR))
			bstrProfile = varProfile.bstrVal;

		if (bstrProfile)
		{
			lProfSize = ::SysStringByteLen(bstrProfile);

			pbuffer->CopyTo((unsigned char *)bstrProfile, lProfSize);

			// 3.  unmarshal it
			hr = pProfile->UnMarshall(pbuffer);
		}

		if (FAILED(hr) || bstrProfile == NULL)
		{
			// for some reason, we couldn't read the profile -
			// stick in a default one..
			pProfile->Reset();

			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::ConnectInt: failed to unmarshall profile\n");

			hr = S_OK;
		}
	}

	// NOTE: FindOrCreateAvatar will send *ppthing to client
	hr = FindOrCreateAvatar(bstrName, bstrPassword, pProfile, ppthing);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectInt: FindOrCreateAvatar failed\n");
		goto ERROR_ENCOUNTERED;
	}

	if (m_bServerSide == VARIANT_TRUE)
	{
		hr = SendWorld(soClient);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectInt: SendWorld failed\n");
			goto ERROR_ENCOUNTERED;
		}

		hr = SendExemplars();
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectInt: SendExemplars failed\n");
			goto ERROR_ENCOUNTERED;
		}
	}

#if 0
	/* hrOptional = */ MoveProfileToDS(*ppthing, pProfile);
#endif

	hr = ConnectAvatar(*ppthing);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectInt: ConnectAvatar failed\n");
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfile);
	SAFERELEASE(pbuffer);

	return ReportWorldError(hr);
}

HRESULT CWorldObject::DecorateAlias(BSTR bstrAlias, BSTR* pbstrNewAlias)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bIsConnected = VARIANT_FALSE;
	IThing* pAvatar = NULL;
	long count = 1;
	CComBSTR bstrCurrentAlias;
	CComBSTR bstrBaseAlias;

	ASSERT(bstrAlias);
	if (bstrAlias == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DecorateAlias: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}
	
	ASSERT(pbstrNewAlias);
	if (pbstrNewAlias == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DecorateAlias: outgoing pointer (BSTR*) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	*pbstrNewAlias = NULL;

	/*
	// no need for lowercase
	bstrBaseAlias = NormalizeString(bstrAlias);
	*/

	bstrBaseAlias = bstrAlias;
	bstrCurrentAlias = bstrBaseAlias;
	
	// REVIEW: just find unused avatar name, don't look at isConnected flag

	// if an avatar doesn't exist with this name, use this name
	while (SUCCEEDED(get_Avatar(bstrCurrentAlias, &pAvatar)))
	{
//		/* hr = */ pAvatar->get_BOOL(m_bstrIsConnected, &bIsConnected);

		SAFERELEASE(pAvatar);

		// if we found an avatar with this name but it isn't connected, then use this name
//		if (bIsConnected == VARIANT_FALSE)
//			break;

		bstrCurrentAlias = bstrBaseAlias;
		bstrCurrentAlias += VWBSTRFromLong(count);

		count++;
	}
	
	*pbstrNewAlias = bstrCurrentAlias.Copy();

ERROR_ENCOUNTERED:
	return hr;
}

const VARIANT kVarNone = {VT_ERROR, 0, 0, 0, DISP_E_MEMBERNOTFOUND};

// '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// FindOrCreateAvatar
// Creates an avatar or returns an already existing one.

HRESULT CWorldObject::FindOrCreateAvatar(BSTR bstrName, BSTR bstrPassword, IAvatarProfile *pProfile, IThing** ppThing)
{
	HRESULT			hr = S_OK;
	CComBSTR		bstrGUID;
	VARIANT_BOOL	bValid = VARIANT_FALSE;
	VARIANT_BOOL	bFirstTime = VARIANT_FALSE;
	VARIANT			varFirstTime;
	VARIANT_BOOL	bIsValid = VARIANT_FALSE;
	VARIANT_BOOL	bSecurityEnabled = VARIANT_FALSE;
	VARIANT_BOOL	bRoaming = VARIANT_FALSE;
	static CComBSTR bstrProfileStatic("Profile");
	static CComBSTR bstrNameStatic("PreferredName");
	static CComBSTR	kbstrFirstTime("FirstTimeUser");
	static CComBSTR bstrRoaming("EnableRoamingProfiles");
	
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::FindOrCreateAvatar\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::FindOrCreateAvatar: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(bstrPassword);
	if (bstrPassword == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::FindOrCreateAvatar: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(pProfile);
	if (pProfile == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::FindOrCreateAvatar: incoming argument (IAvatarProfile*) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(ppThing);
	if (ppThing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::FindOrCreateAvatar: outgoing pointer (IThing**) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}
	
	*ppThing = NULL;

	// Find whether roaming profiles are enabled
	hr = m_pGlobal->get_BOOL(bstrRoaming, &bRoaming);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// If EnableRoamingProfiles == True
	if (SUCCEEDED(hr) && (bRoaming == VARIANT_TRUE))
	{
		// Try to get GUID key from profile.
		hr = pProfile->IsValid(m_bstrGUID, &bValid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Login by name (GUID key not found)
		if (SUCCEEDED(hr) && bValid == VARIANT_FALSE)
		{
			hr = LoginByName(bstrName, bstrPassword, ppThing, &bstrGUID.m_str, &bFirstTime);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// Login by GUID (Found GUID key in profile)
		else
		{
			hr = LoginByGUID(bstrName, bstrPassword, pProfile, ppThing, &bstrGUID.m_str, &bFirstTime);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}
	// EnableRoamingProfiles == False
	else 
	{
		hr = LoginByName(bstrName, bstrPassword, ppThing, &bstrGUID.m_str, &bFirstTime);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Assign avatar to profile
	hr = pProfile->put_Avatar(*ppThing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// If logging in by GUID, put the key value
	if (bValid == VARIANT_TRUE)
	{
 		// push in name
		hr = pProfile->put_KeyValue(bstrNameStatic, CComVariant(bstrName));
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// push in GUID
		hr = pProfile->put_KeyValue(m_bstrGUID, CComVariant(bstrGUID));
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	// Logging in by name, add GUID
	else
	{
 		// add in name (private key)
		hr = pProfile->AddKey(bstrNameStatic, CComVariant(bstrName), kVarNone, CComVariant(kKeyPrivate), kVarNone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// add in GUID (private key)
		hr = pProfile->AddKey(m_bstrGUID, CComVariant(bstrGUID), kVarNone, CComVariant(kKeyPrivate), kVarNone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// REVIEW: AddDefaultKeys already done in LoadRegistryIntoProfile
		// NOW - Here we actually fill in the default info for the avatar
		/*
		hr = InvokeAddDefaultKeys(*ppThing, pProfile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		*/
	}

	hr = (*ppThing)->IsValidProperty(bstrProfileStatic, &bValid);
	if (FAILED(hr) || !bValid)
		goto ERROR_ENCOUNTERED;	

	{
		CSetUID SetUID(this, *ppThing, *ppThing);

		hr = (*ppThing)->put_ObjectProperty(bstrProfileStatic, pProfile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = InvokeAddDefaultKeys(*ppThing, pProfile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// finally, set the profile flag that indicates first time user or not
	varFirstTime.vt = VT_BOOL;
	varFirstTime.boolVal = bFirstTime;
	
	pProfile->IsValid(kbstrFirstTime, &bIsValid);
	
	if (bIsValid == VARIANT_TRUE)
		pProfile->put_KeyValue(kbstrFirstTime, varFirstTime);
	else
		pProfile->AddKey(kbstrFirstTime, varFirstTime, kVarNone, CComVariant(kKeyPrivate), kVarNone);
  
ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		// free instance
		SAFERELEASE(*ppThing);
	}

	return hr;
}

// '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// LoginByName
// You are logging in without a GUID (or profile) 
// Name is what identifies an avatar uniquely
// Create new GUID and add to avatar

HRESULT CWorldObject::LoginByName(BSTR bstrName, BSTR bstrPassword, IThing** ppThing, BSTR* pBstrGUID, VARIANT_BOOL* bFirstTime)
{
	HRESULT			hr = S_OK;
	CComBSTR		bstrNewName;
	CLSID			newClsid;
	CComBSTR		bstrAvatarPassword;
	VARIANT_BOOL	bSecurityEnabled = VARIANT_FALSE;
	static CComBSTR bstrIsComplete("IsComplete");

	// Find out if avatar already exists
	hr = get_Avatar(bstrName, ppThing);
	
	// Name is found (avatar has already logged into world)
	if (SUCCEEDED(hr) && *ppThing)
	{
		VARIANT_BOOL bIsComplete = VARIANT_FALSE;

		hr = (*ppThing)->get_BOOL(bstrIsComplete, &bIsComplete);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bIsComplete == VARIANT_TRUE)
		{
			hr = CheckAvatarConnectionAndPassword(bstrPassword, *ppThing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			// It's incomplete (profile has reference to avatar)
			// Fill in ppThing( since name is unique, shouldn't be decorated) 
			// Create GUID for Avatar

			hr = CoCreateGuid(&newClsid);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			*pBstrGUID = VWBSTRFromCLSID(newClsid).Copy();

			hr = CreateAvatarExtInPlace(bstrName, bstrPassword, *pBstrGUID, *ppThing, &bstrNewName.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			// Set the flag to notify the user that s/he is new
			*bFirstTime = VARIANT_TRUE;	
		}
	}

	// Name not found (avatar doesn't exist, create new one)
	else
	{
		// Create GUID for Avatar
		hr = CoCreateGuid(&newClsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		*pBstrGUID = VWBSTRFromCLSID(newClsid).Copy();

		// bstrName is preferred name of new avatar
		// returns avatar in ppThing and new name in bstrNewName
		hr = CreateAvatarExt(bstrName, bstrPassword, *pBstrGUID, ppThing, &bstrNewName.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Set the flag to notify the user that s/he is new
		*bFirstTime = VARIANT_TRUE;	
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		// free instance
		SAFERELEASE(*ppThing);
	}
	return hr;
}

// '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// LoginByGUID
// GUID is what identifies an avatar uniquely
// Do checks for GUID in this world - 
// If avatar name already exists, decorate

HRESULT CWorldObject::LoginByGUID(BSTR bstrName, BSTR bstrPassword, IAvatarProfile *pProfile, IThing** ppThing, BSTR* pBstrGUID, VARIANT_BOOL* bFirstTime)
{
	HRESULT			hr = S_OK;
	CComVariant		varGUID;
	CComBSTR		bstrAvatarPassword;
	CComBSTR		bstrNewName;
	static CComBSTR bstrIsComplete("IsComplete");

	/* hr = */ pProfile->get_KeyValue(m_bstrGUID, &varGUID);

	if (varGUID.vt != VT_BSTR)
	{
		hr = E_FAIL; // REVIEW: need error code
		goto ERROR_ENCOUNTERED;
	}

	// Get GUID from profile key
	*pBstrGUID = CComBSTR(varGUID.bstrVal).Copy();

	// Lookup avatar in GUID map
	hr = get_AvatarByGUID(*pBstrGUID, ppThing);
	
	if (SUCCEEDED(hr) && *ppThing)
	{
		VARIANT_BOOL bIsComplete;

		hr = (*ppThing)->get_BOOL(bstrIsComplete, &bIsComplete);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bIsComplete == VARIANT_TRUE)
		{
			hr = CheckAvatarConnectionAndPassword(bstrPassword, *ppThing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// REVIEW: don't decorate avatar name on connect (Bob1 should stay as Bob1)
#if 0
			// NOTE: profile referenced a GUID which does exist in this world
			// Password is OK, check if preferred alias is unused, return decorated name
			hr = DecorateAlias(bstrName, &bstrNewName.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// Force in the decorated name
			{
				CSetUID SetUID(this, *ppThing, *ppThing, VARIANT_TRUE);

				hr = (*ppThing)->put_String(CThingObject::m_bstrName, bstrNewName);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}
#endif
		}
		else
		{
			// ppThing has GUID, but not name

			// It's incomplete (profile has reference to avatar)
			// Fill in ppThing, decorate name to bstrNewName
			hr = CreateAvatarExtInPlace(bstrName, bstrPassword, *pBstrGUID, *ppThing, &bstrNewName.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			// Set the flag to notify the user that s/he is new
			*bFirstTime = VARIANT_TRUE;	
		}
	}
	else
	{
		// NOTE: profile referenced a GUID which doesn't exist in this world - 
		// Use the GUID to create a profile in this world
		// Create a new avatar -  bstrName is preferred name of new avatar
		// Returns avatar in ppThing and new name in bstrNewName
		hr = CreateAvatarExt(bstrName, bstrPassword, *pBstrGUID, ppThing, &bstrNewName.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Set the flag to notify the user that s/he is new
		*bFirstTime = VARIANT_TRUE;	
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		// free instance
		SAFERELEASE(*ppThing);
	}
	return hr;
}

// '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// CheckAvatarConnectionAndPassword
// Checks if avatar already connected and
// if password passed in matches one in profile

HRESULT CWorldObject::CheckAvatarConnectionAndPassword(BSTR bstrPassword, IThing* pThing) 
{
	HRESULT hr = S_OK;
	CComBSTR bstrAvatarName;
	CComBSTR bstrAvatarPassword;
	VARIANT_BOOL bIsConnected = VARIANT_FALSE;
	VARIANT_BOOL bSecurityEnabled = VARIANT_TRUE;

	if (pThing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CheckAvatarConnectionAndPassword: failed to add connection property\n");
		return E_INVALIDARG;
	}

	/* hr = */ pThing->get_Name(&bstrAvatarName.m_str);

	/* hr = */ pThing->get_BOOL(m_bstrIsConnected, &bIsConnected);

	// If connected, fail with ALREADYCONNECTED
	if (bIsConnected == VARIANT_TRUE)
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CWorldObject::CheckAvatarConnectionAndPassword: avatar (%s) is already connected\n", CString(bstrAvatarName));

		hr = VWOBJECT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}

	// Else check security, password
	/* hr = */ m_pWorld->get_EnableSecurity(&bSecurityEnabled);

	if (bSecurityEnabled == VARIANT_TRUE)
	{
		// Get password from avatar
		hr = pThing->get_String(m_bstrPassword, &bstrAvatarPassword.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Need password check, logging in by name only
		if (!CompareElements(&bstrPassword, &bstrAvatarPassword.m_str))
		{
			// Password check has failed, report accordingly
			VWREPORT(m_pWorld, VWR_ERROR, "CWorldObject::CheckAvatarConnectionAndPassword: invalid password for avatar (%s)\n", CString(bstrAvatarName));

			hr = VWOBJECT_E_INVALIDPASSWORD;
			goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CWorldObject::ConnectAvatar(IThing* pthing)
{
	HRESULT hr = S_OK;
	long lUsers;
	static CComBSTR bstrIPAddress("IPAddress");

	if (m_bServerSide == VARIANT_TRUE)
	{
		IThing* pContainer = NULL;
		CComBSTR bstrSockAddr;

		// attach connection to avatar
		hr = pthing->AddPropertyExt(m_bstrConnection, CComVariant(m_pVWConnection),
			PSBIT_NOTREMOTED | PSBIT_NOTLOGGED | PSBIT_INTERNALLYADDED | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_UNKNOWN, IID_IUnknown, NULL);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: failed to add connection property\n");
			goto ERROR_ENCOUNTERED;
		}

		/* hr = */ m_pVWConnection->get_SocketAddress(&bstrSockAddr.m_str);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::ConnectAvatar: client ip address (%s)\n", CString(bstrSockAddr));
#endif

		hr = pthing->AddPropertyExt(bstrIPAddress, CComVariant(bstrSockAddr),
			PSBIT_NOTREMOTED | PSBIT_NOTLOGGED | PSBIT_INTERNALLYADDED | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: failed to set IPAddress property\n");
			goto ERROR_ENCOUNTERED;
		}
		
		if (SUCCEEDED(pthing->get_Container(&pContainer)) && pContainer)
		{
			VARIANT_BOOL bInhabited = VARIANT_FALSE;

			// update client with avatar's container and container's contents (incl. avatar)
			hr = SendContainer(soClient, pContainer);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: SendContainer failed\n");
				goto ERROR_ENCOUNTERED_2;
			}

			// NOTE: avatar is connecting to its container; check container
			// for logged-in avatars; if none, activate room recursively
			hr = IsRoomInhabited(pContainer, &bInhabited);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: IsRoomInhabited failed\n");
				goto ERROR_ENCOUNTERED_2;
			}

			if (bInhabited == VARIANT_FALSE)
			{
#ifdef WORLD_USER_DEBUG
				{
					CComBSTR bstrContainer("n/a");

					if (pContainer)
						/* hr = */ pContainer->get_Name(&bstrContainer.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::ConnectAvatar: activating %s\n", 
						CString(bstrContainer));
				}
#endif

				hr = ActivateRoom(pContainer);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: ActivateRoom failed\n");
					goto ERROR_ENCOUNTERED_2;
				}
			}

ERROR_ENCOUNTERED_2:
			SAFERELEASE(pContainer);

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			// REVIEW: if we don't have a container, send the object itself to the client
			// There are some changes that have been made to the avatar (e.g. the addition
			// of a profile) that have yet to be marshaled to the client.  They 
			// normally get marshaled as a side effect of marshaling the conainer (above).
			// If there is no container, however, the client never receives the changes.
			// The best fix for this might be to reorder some code in ConnectInt() and
			// FindOrCreateAvatar() so that the changes (the addition of the profile) are 
			// successfully marshaled earlier.  These changes are failing to be marshaled
			// because the avatar doesn't have a connection property or because the
			// exemplars have yet to be marshaled to the client (when the client receives
			// the change, it rejects it because it can't validate the property name).
			hr = SendThing(soClient, pthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// update client with avatar's contents
		hr = SendContents(soClient, pthing, VARIANT_TRUE);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: SendContents failed\n");
			goto ERROR_ENCOUNTERED;
		}

		/* hr = */ IncrementUserCount(&lUsers);

#ifdef WORLD_PERF_DEBUG
		{
			CComBSTR bstrUser;

			if (pthing)
				pthing->get_Name(&bstrUser.m_str);

			VWREPORT(m_pWorld, VWR_PERFLOG, "User Connect,%d,%s,%s,%d\n",
				GetTickCount(), CString(bstrSockAddr), CString(bstrUser), lUsers);
		}
#endif
	}

#ifdef WORLD_USER_DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::ConnectAvatar: adding connection (0x%x) to m_clist\n", 
		(DWORD)m_pVWConnection);
#endif

	// add avatar to connection list
	if (m_clist.AddConnection(m_pVWConnection, pthing) == FALSE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: AddConnection failed\n");
		hr = VWOBJECT_E_CONNECTIONERROR;
		goto ERROR_ENCOUNTERED;
	}

	{
		CSetUID SetUID(this, pthing, pthing);

		// add isConnected property to avatar
		hr = pthing->AddPropertyExt(m_bstrIsConnected, CComVariant((bool)VARIANT_TRUE),
			PSBIT_NOTLOGGED | PSBIT_INTERNALLYADDED | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: failed to add IsConnected flag\n");
			goto ERROR_ENCOUNTERED;
		}
	}

	if (m_bServerSide == VARIANT_FALSE)
		m_bUserConnected = VARIANT_TRUE;

	// cache user
	m_pUser = pthing;
	SAFEADDREF(m_pUser);

	hr = FireConnectAvatar(pthing);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConnectAvatar: FireConnectAvatar failed\n");
		goto ERROR_ENCOUNTERED;
	}

	// if on server, don't leave user cached
	if (m_bServerSide == VARIANT_TRUE)
		SAFERELEASE(m_pUser);

	NotifyUserConnect(pthing);

#ifdef WORLD_USER_DEBUG
	{
		CComBSTR bstrUser("n/a");

		/* hr = */ pthing->get_Name(&bstrUser.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::ConnectAvatar: connecting %s\n", 
			CString(bstrUser));
	}
#endif

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		// remove from connection list
		m_clist.RemoveConnection(m_pVWConnection);

		if (m_bServerSide == VARIANT_FALSE)
			SAFERELEASE(m_pUser);
	}
	
	return hr;
}

STDMETHODIMP CWorldObject::Disconnect(void)
{
	if (m_bUserConnected == VARIANT_FALSE)
		return VWOBJECT_S_ALREADYDISCONNECTED;

	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

	if (m_pThingCaller != NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Disconnect: only callable from external script\n");
		hr = VWOBJECT_E_EXTERNALCALLONLY;
		goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::Disconnect\n");

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	// NOTE: should only get called on client since DisconnectInt called on server
	SaveRoamingProfile(m_pUser);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_bServerSide == VARIANT_FALSE)
		{
			hr = DisconnectInt();

			if (m_bUserConnected == VARIANT_TRUE)
			{
//				NotifyUserDisconnect(m_pUser);

//				m_clist.RemoveConnection(m_pVWConnection);

				// released cached user
				SAFERELEASE(m_pUser);

				m_bUserConnected = VARIANT_FALSE; 
			}

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else 
		{
			// no-op: shouldn't be called directly on server
			ASSERT(FALSE);
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		if (m_bUserConnected == VARIANT_TRUE)
		{
			HRESULT senthr = S_OK;
			CComVariant var;
			DISPPARAMS dispparams;

			// package up args into DISPPARAMS struct
			hr = InitializeDISPPARAMS(&dispparams, 0);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = SendWorldCommand(VARIANT_FALSE, DISPID_WORLD_DISCONNECTINT, DISPATCH_METHOD, &dispparams, &senthr, &var);

			TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

			// REVIEW: should we always clean up no matter what HRESULT we get?
			// right now, we cleanup if the SendWorldCommand fails, or if it fully succeeds (both cli & srv)
			if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED) 
			{
				hr = senthr;
			}

			NotifyUserDisconnect(m_pUser);

			m_clist.RemoveConnection(m_pVWConnection);

			SAFERELEASE(m_pUser);

			m_bUserConnected = VARIANT_FALSE; 
		}
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::DisconnectInt()
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DisconnectInt\n");

	if (m_pUser) 
	{
#ifdef WORLD_USER_DEBUG
		{
			CComBSTR bstrUser("n/a");

			/* hr = */ m_pUser->get_Name(&bstrUser.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::DisconnectInt: disconnecting %s\n", 
				CString(bstrUser));
		}
#endif
		hr = DisconnectAvatarInt(m_pUser);
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::DisconnectInt: Failed calling DisconnectAvatarInt \n");
	}

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::DisconnectClient(IThing* pAvatar)
{
	HRESULT hr = S_OK;

	SaveRoamingProfile(m_pUser);

	NotifyUserDisconnect(m_pUser);

	m_clist.RemoveConnection(m_pVWConnection);

	SAFERELEASE(m_pUser);

	m_bUserConnected = VARIANT_FALSE; 

	return hr;
}

STDMETHODIMP CWorldObject::DisconnectAvatar(IThing* pAvatar)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK; 
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DisconnectAvatar\n");
	
	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = DisconnectAvatarInt(pAvatar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pAvatar;

		hr = SendWorldCommand(VARIANT_FALSE, DISPID_WORLD_DISCONNECTAVATAR, DISPATCH_METHOD, &dispparams, &senthr);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;
		}
	}
ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::DisconnectAvatarInt(IThing* pAvatar)
{
	HRESULT hr = S_OK;
	IThing* pContainer = NULL;
	CComVariant varConnection;
	IVWCommConnection* pConnection = NULL;
	long lUsers;
	VARIANT_BOOL bValid = VARIANT_FALSE;
	static CComBSTR bstrIPAddress("IPAddress");

	// how to check that user isn't disconnecting him/herself?  in host?

	// Get connection if on server, OK to be NULL if you're not on a server
	if (m_bServerSide == VARIANT_TRUE)
	{
		/* hr = */ pAvatar->IsValidProperty(m_bstrConnection, &bValid);

		// if connection prop doesn't exist, just kick out early
		if (bValid == VARIANT_FALSE)
			return S_OK;

		hr = pAvatar->get_Property(m_bstrConnection, &varConnection);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: failed to get connection, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}
		pConnection = (IVWCommConnection*) varConnection.punkVal;
		ASSERT(pConnection);
		SAFEADDREF(pConnection);
	}

	hr = FireDisconnectAvatar(pAvatar);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: FireDisconnectAvatar failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}
	
	// Set connection property to false before firing Deactivate event, if last avatar is logging off
	{
		CSetUID SetUID(m_pWorld, pAvatar, pAvatar, VARIANT_TRUE);

		hr = pAvatar->put_BOOL(m_bstrIsConnected, VARIANT_FALSE);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: failed to change IsConnected property to VARIANT_FALSE, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}
	}

	// NOTE: must remove connection prop *after* firing disconnect
	// event, otherwise c/s messaging won't happen correctly
	if (m_bServerSide == VARIANT_TRUE)
	{
		VARIANT_BOOL bInhabited = VARIANT_FALSE;

		hr = pAvatar->get_Container(&pContainer);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: failed to get user's container, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}

		if (pContainer != NULL)
		{

			// NOTE: avatar is disconnecting from its container; check container
			// for logged-in avatars; if none, deactivate room recursively
			hr = IsRoomInhabited(pContainer, &bInhabited);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: IsRoomInhabited failed, hr %x\n", hr);
				goto ERROR_ENCOUNTERED;
			}

			if (bInhabited == VARIANT_FALSE)
			{
#ifdef WORLD_USER_DEBUG
				{
					CComBSTR bstrContainer("n/a");

					if (pContainer)
						/* hr = */ pContainer->get_Name(&bstrContainer.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::DisconnectAvatarInt: deactivating %s\n", 
						CString(bstrContainer));
				}
#endif

				hr = DeactivateRoom(pContainer);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: DeactivateRoom failed, hr %x\n", hr);
					goto ERROR_ENCOUNTERED;
				}
			}
		}

		/* hr = */ DecrementUserCount(&lUsers);

#ifdef WORLD_PERF_DEBUG
		{
			CComBSTR bstrAvatar;

			if (pAvatar)
				pAvatar->get_Name(&bstrAvatar.m_str);

			CComBSTR bstrSockAddr;

			/* hr = */ pConnection->get_SocketAddress(&bstrSockAddr.m_str);

			VWREPORT(m_pWorld, VWR_PERFLOG, "User Disconnect,%d,%s,%s,%d\n",
				GetTickCount(), CString(bstrSockAddr), CString(bstrUser), lUsers);
		}
#endif
	}

	// Now, if we're kicking someone off, need to call DisconnectClient
	if (m_pUser != pAvatar)
	{
		DISPPARAMS dispparams;

		hr = WriteCommandHeader(m_pMarshBuf, 'N', 'w');
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// marshal DISPID
		hr = m_pMarshBuf->put_Long(DISPID_WORLD_DISCONNECTCLIENT);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// marshal wFlags
		hr = m_pMarshBuf->put_UShort(DISPATCH_METHOD);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		// marshal DISPPARAMS
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pAvatar;

		hr = MarshallDISPPARAMS(&dispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = SendCommandExt(soAvatar, pAvatar, m_pMarshBuf, VARIANT_FALSE, VARIANT_FALSE, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: failed to remove connection property, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}
	}

	// Do disconnect

	if (m_bServerSide == VARIANT_TRUE)
	{
		CSetUID SetUID(m_pWorld, pAvatar, pAvatar, VARIANT_TRUE);

		hr = pAvatar->RemoveProperty(m_bstrConnection);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: failed to remove connection property, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}

		hr = pAvatar->RemoveProperty(bstrIPAddress);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DisconnectAvatarInt: failed to remove IPAddress property, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		} 
	}

	/* hrOptional = */ DeleteThingFromDS(pAvatar);

#ifdef WORLD_USER_DEBUG
	{
		CComBSTR bstrAvatar("n/a");

		/* hr = */ pAvatar->get_Name(&bstrAvatar.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::DisconnectAvatarInt: finished disconnecting %s\n", 
			CString(bstrUser));
	}
#endif

ERROR_ENCOUNTERED:
	{
		// make sure we *always* remove the IsConnected property
		CSetUID SetUID(m_pWorld, pAvatar, pAvatar, VARIANT_TRUE);

		/* hr = */ pAvatar->RemoveProperty(m_bstrIsConnected);
	}

	NotifyUserDisconnect(pAvatar);

	// remove connection from connection map
	m_clist.RemoveConnection(pConnection);

	SAFERELEASE(pConnection);
	SAFERELEASE(pContainer);

	m_pMarshBuf->Rewind();

#ifdef WORLD_USER_DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::DisconnectAvatarInt: removing connection (0x%x) from m_clist\n", 
		(DWORD)pConnection);
#endif

	return hr;
}

STDMETHODIMP CWorldObject::DisconnectConnection(IVWCommConnection* pVWConnection)
{
	HRESULT hr = S_OK;
	IVWCommConnection* pVWOld = NULL;
	IThing* pOldUser = m_pUser;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DisconnectConnection\n");

	pVWOld = m_pVWConnection;
	SAFEADDREF(pVWOld);

	// stash connection ptr
	m_pVWConnection = pVWConnection;
	SAFEADDREF(m_pVWConnection);

	if (m_bServerSide == VARIANT_TRUE)
	{
		// stash user ptr
		m_pUser = m_clist.FindConnection(m_pVWConnection);

		// disconnect user from world
		/* hr = */ DisconnectInt();

		// restore user ptr
		m_pUser = pOldUser;
	}
	else
	{
		if (m_bUserConnected == VARIANT_TRUE)
		{
			NotifyUserDisconnect(m_pUser);

#ifdef WORLD_USER_DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::DisconnectConnection: removing connection (0x%x) from m_clist\n", 
				(DWORD)m_pVWConnection);
#endif

			m_clist.RemoveConnection(m_pVWConnection);

			// released cached user
			SAFERELEASE(m_pUser);

			m_bUserConnected = VARIANT_FALSE; 
		}
	}

	// restore connection ptr (should have been NULL)
	SAFERELEASE(m_pVWConnection);
	m_pVWConnection = pVWOld;

	SAFERELEASE(pVWOld);

	return hr;
}

STDMETHODIMP CWorldObject::Reconnect(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword)
{
	VALIDATE_WORLD;

    HRESULT hr = S_OK;
	
	if (m_pThingCaller != NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Reconnect: only callable by user or from external script\n");
		hr = VWOBJECT_E_EXTERNALCALLONLY;
		goto ERROR_ENCOUNTERED;
	}

	hr = ReconnectExt(bstrURL, bstrName, bstrPassword);

ERROR_ENCOUNTERED:
    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::ReconnectExt(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword)
{
	VALIDATE_WORLD;

    HRESULT hr = S_OK;
	CComBSTR bstrCurrentServerName;
	CComBSTR bstrCurrentWorldName;
	CComBSTR bstrNewServerName;
	CComBSTR bstrNewWorldName;
	IThing* pGlobal = NULL;
	
	// Can only reconnect from client-side
	if (m_bServerSide == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Reconnect: can only reconnect from client-side\n");
		hr = VWOBJECT_E_ONLYRECONNECTFROMCLIENT;
		goto ERROR_ENCOUNTERED;
	}

	// Get current server and world names
	hr = get_Global(&pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->get_String(CComBSTR("ServerName"), &bstrCurrentServerName.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = pGlobal->get_String(CComBSTR("WorldName"), &bstrCurrentWorldName.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Get new server and world names
	hr = ParseURLHelper(bstrURL, &bstrNewWorldName.m_str, NULL, &bstrNewServerName.m_str, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Check that we're not reconnecting to the same world
	// See if ServerName and WorldName are the same
	if ((::CompareElements(&bstrCurrentServerName.m_str, &bstrNewServerName.m_str)) && (::CompareElements(&bstrCurrentWorldName.m_str, &bstrNewWorldName.m_str)))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::Reconnect: cannot reconnect to the same world\n");
		hr = VWOBJECT_E_NORECONNECTTOSAMEWORLD;
		goto ERROR_ENCOUNTERED;
	}

	// flush profile to disk on reconnect (before we attempt the reconnect)
	SaveRoamingProfile(m_pUser);

	{
#if !defined(TIMING_DEBUG)
		START_EVENT(CWorldObject, IVWObjectSite)
		        hr = pEvent->OnUserReconnect(bstrURL, bstrName, bstrPassword);
		FINISH_EVENT()
#endif
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);

    return ReportWorldError(hr);
}

HRESULT CWorldObject::AddGlobalPaths()
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IThing* pGlobal = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::AddGlobalPaths\n");

	hr = get_Global(&pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = pGlobal->AddPropertyExt(CComBSTR("ContentPath"), CComVariant(CComBSTR("")),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("DirectoryServicePage"), CComVariant(CComBSTR("")),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_HIDDEN | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("WorldContentPath"), CComVariant(CComBSTR("")),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("AvatarGraphicsPath"), CComVariant(CComBSTR("")),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("WorldAvatarGraphicsPath"), CComVariant(CComBSTR("")),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("HelpPath"), CComVariant(CComBSTR("")),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("WorldPath"), CComVariant(CComBSTR("")),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("IsSDKInstalled"), CComVariant((bool)VARIANT_FALSE),
		PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);

	return hr;
}

STDMETHODIMP CWorldObject::UpdateGlobalPaths()
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	static CComBSTR bstrFile("file://");
	CComBSTR bstrContentPath, bstrAvatarGraphicsPath, bstrHelpPath, bstrDSPage;
	CComBSTR bstrTempPath, bstrWorldName;
	CComBSTR bstrWorldContentPath, bstrWorldAvatarGraphicsPath;
	CComBSTR bstrWorldPath;
	CComBSTR bstrRoot, bstrRootURLPath;
	IInternetFileManager* pinf = NULL;
	IThing* pGlobal = NULL;
	VARIANT_BOOL vBool = VARIANT_FALSE;

	hr = get_Global(&pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
		// must override security to set these constants
		CSetUID SetUID(m_pWorld, pGlobal, pGlobal, VARIANT_TRUE);
		
		hr = FindContentPath(&bstrContentPath.m_str);
		if (SUCCEEDED(hr))
		{
			bstrTempPath = bstrFile;
			bstrTempPath += bstrContentPath;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: ContentPath %s\n", CString(bstrTempPath));
#endif
			hr = pGlobal->put_Property(CComBSTR("ContentPath"), CComVariant(bstrTempPath));
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put ContentPath property, hr %x\n", hr);
				goto ERROR_ENCOUNTERED;
			}
		}
		
		// Compose the world-specific content path, e.g., \Local Content\Worlds\Plazas
		hr = pGlobal->get_String(CComBSTR("WorldName"),&bstrWorldName.m_str);
		if ((SUCCEEDED(hr)) && (bstrWorldName.Length()>0))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: WorldName %s\n", CString(bstrWorldName));
#endif

			bstrWorldContentPath = bstrTempPath;
			bstrWorldContentPath += "Worlds\\";
			bstrWorldContentPath += bstrWorldName;
			bstrWorldContentPath += "\\";
		}
		else
			bstrWorldContentPath = bstrTempPath;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: WorldContentPath %s\n", CString(bstrWorldContentPath));
#endif

		// REVIEW: add property no matter what, if no WorldName property, just set it to be same as ContentPath
		hr = pGlobal->put_Property(CComBSTR("WorldContentPath"), CComVariant(bstrWorldContentPath));
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put WorldContentPath property, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}

		hr = FindAvatarGraphicsPath(&bstrAvatarGraphicsPath.m_str);
		if (SUCCEEDED(hr))
		{
			bstrTempPath = bstrFile;
			bstrTempPath += bstrAvatarGraphicsPath;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: AvatarGraphicsPath %s\n", CString(bstrTempPath));
#endif

			hr = pGlobal->put_Property(CComBSTR("AvatarGraphicsPath"), CComVariant(bstrTempPath));
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put AvatarGraphicsPath property, hr %x\n", hr);
				goto ERROR_ENCOUNTERED;
			}
		}
		// Compose the world-specific content path, e.g., \Local Content\Worlds\Plazas\Avatar Graphics
		if ((SUCCEEDED(hr)) && (bstrWorldContentPath.Length()>0))
		{
			bstrWorldAvatarGraphicsPath = bstrWorldContentPath;
			bstrWorldAvatarGraphicsPath += "Avatar Graphics\\";
		}

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: WorldAvatarGraphicsPath %s\n", CString(bstrWorldAvatarGraphicsPath));
#endif

		// REVIEW: add property no matter what, if no WorldName property, just set it to be same as AvatarGraphics path
		hr = pGlobal->put_Property(CComBSTR("WorldAvatarGraphicsPath"), CComVariant(bstrWorldAvatarGraphicsPath));
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put WorldAvatarGraphicsPath property, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}

		// Create property for HelpPath
		hr = FindHelpPath(&bstrHelpPath.m_str);
		if (SUCCEEDED(hr))
		{
			//bstrTempPath = bstrFile;
			bstrTempPath = bstrHelpPath;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: HelpPath %s\n", CString(bstrTempPath));
#endif

			hr = pGlobal->put_Property(CComBSTR("HelpPath"), CComVariant(bstrTempPath));
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put HelpPath property, hr %x\n", hr);
				goto ERROR_ENCOUNTERED;
			}
		}
		
		// Create property for world path
		hr = FindWorldPath(&bstrWorldPath.m_str);
		if (SUCCEEDED(hr))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: WorldPath %s\n", CString(bstrWorldPath));
#endif
			hr = pGlobal->put_Property(CComBSTR("WorldPath"), CComVariant(bstrWorldPath));
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put WorldPath property, hr %x\n", hr);
				goto ERROR_ENCOUNTERED;
			}
		}

		// compose directory service page path
		hr = FindDirectoryServicePage(&bstrDSPage.m_str);
		if (SUCCEEDED(hr))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: DirectoryServicePage %s\n", CString(bstrDSPage));
#endif
			hr = pGlobal->put_Property(CComBSTR("DirectoryServicePage"), CComVariant(bstrDSPage));
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put DirectoryServicePage property, hr %x\n", hr);
				goto ERROR_ENCOUNTERED;
			}
		}

		// Create property for SDK Install BOOL
		hr = GetIsSDK(&vBool);
		if (SUCCEEDED(hr))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: IsSDKInstalled %s\n", vBool == VARIANT_TRUE ? "yes" : "no");
#endif
			hr = pGlobal->put_Property(CComBSTR("IsSDKInstalled"), CComVariant((bool)vBool));
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: failed to put IsSDKInstalled property, hr %x\n", hr);
				goto ERROR_ENCOUNTERED;
			}
		}

		// update the Inetfile tool as per the global property in the world
		// Get the global object, get the RootURL property, get the
		// Inetfile tool and set it to the value of the RootURL property.
		// Don't err out if any of this fails.
		if (SUCCEEDED(pGlobal->get_String(CComBSTR("RootURL"), &bstrRoot.m_str)))
		{
			CComBSTR *pbstrRootURLPath = new CComBSTR();
			hr = ComposeRootURLPath(m_pWorld, pbstrRootURLPath);
			bstrRootURLPath = *pbstrRootURLPath;

			if (SUCCEEDED(hr))
			{
				bstrRootURLPath += bstrRoot;

				if (SUCCEEDED(get_ToolExt(CComBSTR("Inetfile"), (IUnknown**)&pinf)) && pinf)
				{
#ifdef _DEBUG
					VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::UpdateGlobalPaths: RootURL %s\n", CString(bstrRootURLPath));
#endif

					hr = pinf->put_RootURL(bstrRootURLPath);

					SAFERELEASE(pinf);
				}
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);

	return hr;
}

#if 0
HRESULT CWorldObject::PutRootURL()
{
	HRESULT hr = S_OK;
	IThing *pglobal = NULL;
	CComBSTR bstrRoot;
	IInternetFileManager *pinf = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ROOTURL, "CWorldObject::PutRootURL\n");

	// update the Inetfile tool as per the global property in the world
	// Get the global object, get the RootURL property, get the
	// Inetfile tool and set it to the value of the RootURL property.
	// Don't err out if any of this fails.
	if (SUCCEEDED(get_Global(&pglobal)))
		if (SUCCEEDED(get_ToolExt(CComBSTR("Inetfile"), (IUnknown**)&pinf)))
			if (SUCCEEDED(pinf->get_RootURL(&bstrRoot.m_str)))
				pglobal->put_String(CComBSTR("RootURL"), bstrRoot);

//ERROR_ENCOUNTERED:
	SAFERELEASE(pglobal);
	SAFERELEASE(pinf);

	return hr;
}
#endif

STDMETHODIMP CWorldObject::OnOpenWorld(VARIANT_BOOL bPersist)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::OnOpenWorld, server-side %s, persist %s\n", 
		m_bServerSide  ? "yes" : "no", bPersist ? "yes" : "no");

	hr = CreateTools();
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bPersist == VARIANT_TRUE)
	{
		/* hr = */ AddGlobalPaths();

		/* hr = */ UpdateGlobalPaths();
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::OnCreateWorld()
{
	HRESULT hr = S_OK;
	IModule* pModule = NULL;
	long currentID = WORLD_OBJECT;

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::OnCreateWorld\n");

	hr = CreateWorldObject();
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// commit the database to write out the WORLD_OBJECT record
	hr = m_pDb->Commit();
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = CreateTools();
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create system exemplars
	hr = m_pWorld->CreateCOMModule(CComBSTR("System"), CComBSTR("VWSYSTEM.SystemEx.1"), MODULE_CLIENT | MODULE_SERVER, &pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// commit the database to write out the WORLD_OBJECT record	(again)
	hr = m_pDb->Commit();
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	/* hr = */ AddGlobalPaths();

	/* hr = */ UpdateGlobalPaths();

ERROR_ENCOUNTERED:
	SAFERELEASE(pModule);

	return hr;
}

HRESULT CWorldObject::OnReceiveWorld()
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_GLOBALPATHS, "CWorldObject::OnReceiveWorld\n");

	/* hr = */ AddGlobalPaths();

	/* hr = */ UpdateGlobalPaths();

	return hr;
}

HRESULT CWorldObject::CreateWorldObject()
{
	HRESULT hr = S_OK;
	long currentID = WORLD_OBJECT;
	static CComBSTR bstrGlobal("Global");

	// create embedded IThing for world global stuff
	hr = CreateObjectInt(currentID, &m_pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = LogCreateObject(currentID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	currentID++;

	hr = m_pGlobal->put_IsStubInt(VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->put_TypeInt(bstrGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#if 0
	// assign NULL owner
	hr = m_pGlobal->AddPropertyExt(CThingObject::m_bstrOwner, CComVariant((IDispatch *) NULL),
									PSBIT_HIDDEN | PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IThing, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// assign NULL container
	hr = m_pGlobal->AddPropertyExt(CThingObject::m_bstrContainer, CComVariant((IDispatch *) NULL),
									PSBIT_HIDDEN | PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IThing, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set name property (Global)
	hr = m_pGlobal->AddPropertyExt(CThingObject::m_bstrName, CComVariant("Global"),
									PSBIT_HIDDEN | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYADDED | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set description property
	hr = m_pGlobal->AddPropertyExt(CThingObject::m_bstrDescription, CComVariant("Global object"),
									PSBIT_HIDDEN | PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYADDED | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->AddPropertyExt(m_bstrCurrentID, CComVariant(currentID, VT_I4),
									PSBIT_HIDDEN | PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_NOTREMOTED | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


	// Add references to member property maps and property lists to Global
	// Must do a "get" afterwards because the list is cloned on the Add

	// Add Avatars
	hr = CreatePropertyMap(m_pWorld, &m_pAvatars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->AddPropertyExt(m_bstrAvatars, CComVariant(m_pAvatars),
									PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_HIDDEN | /* PSBIT_NOTREMOTED | */ PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyMap, NULL);

	SAFERELEASE(m_pAvatars);

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->get_ObjectProperty(m_bstrAvatars, (IObjectProperty**)&m_pAvatars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Add GUIDs
	hr = CreatePropertyMap(m_pWorld, &m_pGUIDs);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = m_pGlobal->AddPropertyExt(m_bstrGUIDs, CComVariant(m_pGUIDs),
								   PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyMap, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	SAFERELEASE(m_pGUIDs);

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->get_ObjectProperty(m_bstrGUIDs, (IObjectProperty**)&m_pGUIDs);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Add Exemplars
	hr = CreatePropertyMap(m_pWorld, &m_pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->AddPropertyExt(m_bstrExemplars, CComVariant(m_pExemplars),
									PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_HIDDEN | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyMap, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	SAFERELEASE(m_pExemplars);

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->get_ObjectProperty(m_bstrExemplars, (IObjectProperty**)&m_pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Add Modules
	hr = CreatePropertyMap(m_pWorld, &m_pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->AddPropertyExt(m_bstrModules, CComVariant(m_pModules), 
									PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_HIDDEN | PSBIT_BYVAL | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyMap, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	SAFERELEASE(m_pModules);

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->get_ObjectProperty(m_bstrModules, (IObjectProperty**)&m_pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Add ModuleNameList
	hr = CreatePropertyList(m_pWorld, &m_pModuleNameList);

	hr = m_pGlobal->AddPropertyExt(CComBSTR("ModuleNameList"), CComVariant(m_pModuleNameList), 
									PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyList, NULL);

	SAFERELEASE(m_pModuleNameList);

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->get_ObjectProperty(CComBSTR("ModuleNameList"), (IObjectProperty**)&m_pModuleNameList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CWorldObject::CreateTools()
{
	HRESULT hr = S_OK;
//	IVWStatCol *pIStats = NULL;
	IVWAnimator *pAnim = NULL;
	IInternetFileManager *pInetfile = NULL;

	// if InitializeTools not called, create tool map
	if (!m_pTools)
	{
		hr = CreatePropertyMap(m_pWorld, &m_pTools);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

#if 0
	// create statistics tool for this world.
	
	// REVIEW: server side only?
//	if (m_bServerSide == VARIANT_TRUE)
//	{
		hr = CreateToolExt(CComBSTR("Statistics"), CLSID_VWStatCol, (IUnknown**)&pIStats);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pIStats->Initialize(m_pWorld);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
//	}
#endif

	hr = CreateToolExt(CComBSTR("Inetfile"), CLSID_InternetFileManager, (IUnknown**)&pInetfile);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: Maybe we should move this code into some world-specific script.

	// create animation tool
	hr = CreateToolExt(CComBSTR("Animator"), CLSID_VWAnimator, (IUnknown**)&pAnim);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pAnim->Initialize(m_pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create client side tools
	if (m_bServerSide == VARIANT_FALSE)
	{
	}

ERROR_ENCOUNTERED:
//	SAFERELEASE(pIStats);
	SAFERELEASE(pAnim);
	SAFERELEASE(pInetfile);

	return hr;
}

STDMETHODIMP CWorldObject::get_Database(BSTR* pbstr)
{
	*pbstr = m_bstrDatabase.Copy();

	return S_OK;
}

STDMETHODIMP CWorldObject::OpenDatabase(BSTR bstrDatabase, VARIANT_BOOL bCreate, VARIANT_BOOL bOverwrite)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	if (m_pDb)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::OpenDatabase: closing existing repository, %s\n", CString(m_bstrDatabase));

		m_pDb->CloseWorld();
		SAFERELEASE(m_pDb);
	}

	if (bstrDatabase == NULL)
		m_bstrDatabase.Empty();
	else
	{
		hr = ValidateWorldNameExt(bstrDatabase);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::OpenDatabase: invalid world database name (%s)\n", CString(bstrDatabase));
			goto ERROR_ENCOUNTERED;
		}

		// canonicalize the database name
		hr = CanonDatabaseName(bstrDatabase, &m_bstrDatabase.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// get repository
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::OpenDatabase: opening repository, %s\n", CString(m_bstrDatabase));

	hr = CoCreateInstance(CLSID_VWObjectRepository, NULL, CLSCTX_INPROC_SERVER, IID_IVWObjectRepository, (LPVOID *) &m_pDb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pDb->Initialize(this, m_bstrDatabase);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pDb->OpenWorld(bCreate, bOverwrite);
	if (FAILED(hr))	
	{
		if (hr == VWODB_E_INVALIDDBRECORD)
		{
			VWREPORT(m_pWorld, VWR_ERROR, "CWorldObject::OpenDatabase: failed with invalid db record, committing from memory to disk\n");

			// REVIEW: do we want to commit the partially read .vwc file?
			hr = m_pDb->Commit();
		}

		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		if (m_pDb)
		{
			m_pDb->CloseWorld();
			SAFERELEASE(m_pDb);
		}
	}

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::SaveDatabase()
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

	if (m_pThingCaller != NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SaveDatabase: only callable from external script\n");
		hr = VWOBJECT_E_EXTERNALCALLONLY;
		goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SaveDatabase\n");

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

#ifdef SUPPORT_WORLDOWNER_RESTRICTIONS
	hr = CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SaveDatabase: must be world owner to save database\n");
		goto ERROR_ENCOUNTERED;
	}
#endif

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = m_pDb->Commit();

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 0);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = SendWorldCommand(VARIANT_FALSE, DISPID_WORLD_SAVECHECKPOINT, DISPATCH_METHOD, &dispparams, &senthr);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;
		}
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::SaveDatabaseAs(BSTR bstrNewName)
{
	VALIDATE_WORLD;
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	CComBSTR bstrCanonicalizedFilename;
	CComBSTR bstrOldWorldName, bstrOldGUID, bstrInfoURL, bstrRegPath, bstrRegName;
	CLSID clsid;

	if (m_pThingCaller != NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SaveDatabaseAs: only callable from external script\n");
		hr = VWOBJECT_E_EXTERNALCALLONLY;
		goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SaveDatabase\n");

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

#ifdef SUPPORT_WORLDOWNER_RESTRICTIONS
	hr = CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SaveDatabaseAs: must be world owner to save database\n");
		goto ERROR_ENCOUNTERED;
	}
#endif

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = ValidateWorldNameExt(bstrNewName);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SaveDatabaseAs: invalid world database name (%s)\n", CString(bstrNewName));
			goto ERROR_ENCOUNTERED;
		}

		ASSERT(m_pGlobal);

		//stash old guid and worldname.
		hr = m_pGlobal->get_String(CComBSTR("GUID"), &bstrOldGUID.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = m_pGlobal->get_String(CComBSTR("WorldName"), &bstrOldWorldName.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		//Create new guid. set new world with new guid and worldname.
		hr = CoCreateGuid(&clsid);
		if (FAILED(hr))
		{
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}

		{
			CSetUID SetUID(m_pWorld, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			hr = m_pGlobal->put_String(CComBSTR("GUID"), VWBSTRFromCLSID(clsid));
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;
			}

			hr = m_pGlobal->put_String(CComBSTR("WorldName"), bstrNewName);
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;
			}
		}

		//Save database.
		hr = CanonDatabaseName(bstrNewName, &bstrCanonicalizedFilename.m_str);
		if (FAILED(hr))
		{
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}

		hr = m_pDb->CommitAs(bstrCanonicalizedFilename+".vwc");
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		{
			CSetUID SetUID(m_pWorld, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			//put old worldname, guid into current world.
			hr = m_pGlobal->put_String(CComBSTR("GUID"), bstrOldGUID);
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;
			}

			hr = m_pGlobal->put_String(CComBSTR("WorldName"), bstrOldWorldName);
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;
			}
		}

		//Do registry work.
		//reregister the new world with a new worldname/guid.
		hr = FindWorldInRegistry(bstrOldGUID, &bstrRegPath.m_str, &bstrRegName.m_str);
		if (bstrRegPath.m_str == NULL)
		{
			hr = E_FAIL;
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}

		hr = GetWorldInfoUrlFromRegistry(bstrRegPath, &bstrInfoURL.m_str);
		if (FAILED(hr))
		{
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}

		hr = AddWorldToRegistry(VWBSTRFromCLSID(clsid), bstrNewName, NULL);
		if (FAILED(hr))
		{
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}

		hr = RegisterWorldInDS(bstrNewName, bstrInfoURL, FALSE);		
		if (FAILED(hr))
		{
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{	HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrNewName;

		hr = SendWorldCommand(VARIANT_FALSE, DISPID_WORLD_SAVECHECKPOINTAS, DISPATCH_METHOD, &dispparams, &senthr);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;
		}
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::SaveToScript(BSTR bstrFilename)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	IThing* pWorldOwner = NULL;

	if (m_pThingCaller != NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SaveToScript: only callable from external script\n");
		hr = VWOBJECT_E_EXTERNALCALLONLY;
		goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SaveToScript\n");

#ifdef SUPPORT_WORLDOWNER_RESTRICTIONS
	hr = CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SaveToScript: must be world owner to save database\n");
		goto ERROR_ENCOUNTERED;
	}
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = SaveToScriptInt(bstrFilename);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrFilename;

		hr = SendWorldCommand(VARIANT_FALSE, DISPID_WORLD_SAVETOSCRIPT, DISPATCH_METHOD, &dispparams, &senthr);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pWorldOwner);

	return ReportWorldError(hr);
}

HRESULT CWorldObject::SaveToScriptInt(BSTR bstrFilename)
{
	HRESULT hr = S_OK;
	IThing *pThing = NULL;
	IThing *pThingExemplar = NULL;
	IThing *pThingContainer = NULL;
	IPropertyList *pModuleNames = NULL;
	// IPropertyMap *pModMap = NULL;
	IModule *pMod = NULL;
	long iObj, cObj;
	long lID, lContainerID;
	CStdioFile file;
	CString strFilename(bstrFilename);
	CString strOut;
	CString str;
	CComVariant var;
	VARIANT_BOOL bDone;
	VARIANT_BOOL bAsVal;
	VARIANT_BOOL bAvatar;
	VARIANT_BOOL bExemplar;
	VARIANT_BOOL bStub;
	CComBSTR bstrContext;
	IThing* pWorldOwner = NULL;

	hr = m_pGlobal->get_Owner(&pWorldOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pGlobal->get_Long(m_bstrCurrentID, &cObj);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	TRY
	{
		// open output file
		if (!file.Open(	strFilename, 
							CFile::modeCreate		|	
							CFile::modeReadWrite	|
							CFile::shareExclusive	|
							CFile::typeText ))
		{
			// REVIEW: error codes
			hr = VWOBJECT_E_FILEREADERROR;
			goto ERROR_ENCOUNTERED;
		}

		// write out header
		// REVIEW: Should probably put all this stuff in resources
		file.WriteString("Sub AutoRun\n\n");

		// write out dims and initialization
		strOut.Format(	"\tDim ObjArray(%d)\n"
						"\tDim ObjProp, ObjProp_t, ObjProp_t_t, ObjProp_t_t_t\n"
						"\tDim Method, AvatarName\n\n", cObj);
		file.WriteString(strOut);

		// write out module loads
		file.WriteString("\t' Load modules\n");

		// loop through all modules
		hr = get_ModuleNameList(&pModuleNames);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pModuleNames->FirstItem(NULL, &var, &bDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		while (bDone!=VARIANT_TRUE)
		{
			// assure we've got what we need
			if (var.vt != VT_BSTR)
			{
				// REVIEW: error code
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}

			// REVIEW: need to update this list when new Core exemplars are added
			CComBSTR bstrOut;
			CString strName(var.bstrVal);

			// NOTE: these need to be all lowercase, so modulenamelist case-sensitivity doesn't screw up 

			// trim off trailing # character
			strName = strName.Left(strName.GetLength() - 1);

			CComBSTR bstrName(strName);

			if (strName=="thing"		||
				strName=="artifact"		||
				strName=="avatar"		||
				strName=="room"			||
				strName=="portal"		||
				strName=="iwportal"		||
				strName=="statemachine" ||
				strName=="host"			||
				strName=="system"		||
				strName=="webpage")
			{
				goto NEXTMODULE;
			}
			
			// fetch appropriate module
			hr = get_Module(bstrName, &pMod);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;


			// tell module to marshal itself out (goes to a string)
			hr = pMod->MarshallToScript(MARSHALL_BYVALUE, NULL, &bAsVal, &bstrOut.m_str);
			if (FAILED(hr))
			{
				// write out error
				str.Format("\t' Error trying to marshall out module: \"%s\"\n", (LPCTSTR)strName);
				file.WriteString(str);
			}
			else
			{
				ASSERT(bAsVal==VARIANT_TRUE);

				if (bstrOut != NULL)
				{
					str.Format("\tcall %s\n", (LPCTSTR) CString(bstrOut));
					file.WriteString(str);
				}
			}

NEXTMODULE:
			// free stuff
			var.Clear();
			SAFERELEASE(pMod);

			// on to the next one
			hr = pModuleNames->NextItem(NULL, &var, &bDone);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		file.WriteString(	"\tWorld.Trace \"Created modules\"\n\n"
							"\t' Pre-create all objects\n\n");

		// write out objects (skipping the world object - object #1)
		for (iObj=1; iObj<cObj; iObj++)
		{
			CComBSTR bstrType;
			CComBSTR bstrName;

			// skip global
			if (iObj==WORLD_OBJECT)
				continue;

			hr = m_pDb->GetObject(iObj, &pThing);
			if (FAILED(hr))
			{
				// A deleted object
				hr = S_OK;
				continue;
			}

			// see if it's a stub
			hr = pThing->get_IsStub(&bStub);
			if (FAILED(hr))
			{
				ASSERT(FALSE);
				hr = S_OK;
				continue;
			}

			if (bStub==VARIANT_TRUE)
			{
				// a stub

				// get the object type
				hr = pThing->get_Type(&bstrType.m_str);
				if (FAILED(hr))
				{
					// shouldn't fail, but tolerate
					strOut.Format("\t' Skipping stub object: #%d. Could not determine type.\n", iObj);
					hr = S_OK;
					goto CREATE_NEXTOBJECT;
				}
				strOut.Format("\tSet ObjArray(%d) = World.CreateInstance(\"\", \"%s\", Nothing) ' a stub\n", iObj, (LPCTSTR)CString(bstrType));
				str.Format("\tObjArray(%d).IsStub = True\n");
				strOut += str;
				goto CREATE_NEXTOBJECT;
			}

			// see if it's an exemplar
			hr = pThing->get_IsExemplar(&bExemplar);
			if (FAILED(hr))
			{
				// if we can't get this flag, it's probably a cocooned
				// oobject - treat is as a non-exemplar
				bExemplar = VARIANT_FALSE;
				hr = S_OK;
			}

			// if the object is an exemplar, just write out assignment
			if (bExemplar==VARIANT_TRUE)
			{
				// get the object type
				hr = pThing->get_Type(&bstrType.m_str);
				if (FAILED(hr))
				{
					strOut.Format("\t' Skipping exemplar #%d - could not get its Type\n", iObj);
					hr = S_OK;
					goto CREATE_NEXTOBJECT;
				}

				// format and output
				strOut.Format("\tSet ObjArray(%d) = World.Exemplar(\"%s\")\n", iObj, (LPCTSTR) CString(bstrType));

			}
			else
			{
				// fixup world owner to be user when script is run
				if (pThing == pWorldOwner)
				{
					strOut.Format( "\tSet ObjArray(%d) = World.User\n", iObj);
					goto CREATE_NEXTOBJECT;
				}

				hr = IsAvatar(pThing, &bAvatar);
				if (SUCCEEDED(hr) && bAvatar == VARIANT_TRUE)
				{
					/* hr = */ pThing->get_Name(&bstrName.m_str);

					CComBSTR bstrPassword;
					/* hr = */ pThing->get_String(m_bstrPassword, &bstrPassword.m_str);

					CComBSTR bstrGUID;
					/* hr = */ pThing->get_String(m_bstrGUID, &bstrGUID.m_str);

					strOut.Format( "\tAvatarName = World.CreateAvatar(\"%s\", \"%s\", \"%s\")\n\tSet ObjArray(%d) = World.Global.Avatars.Property(AvatarName)\n", 
						(LPCTSTR) CString(bstrName), (LPCTSTR) CString(bstrPassword), (LPCTSTR) CString(bstrGUID), iObj);
					goto CREATE_NEXTOBJECT;
				}

				// get the object name
				hr = pThing->get_Name(&bstrName.m_str);
				if (FAILED(hr))
				{
					strOut.Format("\t' Skipping object #%d - could not get its Name\n", iObj);
					hr = S_OK;
					goto CREATE_NEXTOBJECT;
				}

				// get the exemplar out of the object
				hr = pThing->get_Container(&pThingContainer);
				if (FAILED(hr))
				{
					ASSERT(FALSE);
					strOut.Format("\t' Could not get Container from object #%d\n", iObj);
					hr = S_OK;
					goto CREATE_NEXTOBJECT;
				}

				lContainerID = NULL_OBJECT;

				if (pThingContainer)
				{
					/// get the id out of the exemplar
					hr = pThingContainer->get_ID(&lContainerID);
					if (FAILED(hr))
					{
						ASSERT(FALSE);
						strOut.Format("\t' Could not get id of Container from object #%d\n", iObj);
						SAFERELEASE(pThingContainer);
						hr = S_OK;
						goto CREATE_NEXTOBJECT;
					}
				}

				// get the exemplar out of the object
				hr = pThing->get_Exemplar(&pThingExemplar);
				if (FAILED(hr))
				{
					ASSERT(FALSE);
					strOut.Format("\t' Could not get Exemplar from object #%d\n", iObj);
					SAFERELEASE(pThingContainer);
					hr = S_OK;
					goto CREATE_NEXTOBJECT;
				}

				if (pThingExemplar==NULL)
				{
					// no exemplar
					if (lContainerID == NULL_OBJECT)
					{
						strOut.Format( "\tSet ObjArray(%d) = World.CreateInstance(\"%s\", Nothing, Nothing)\n",
										iObj,
										(LPCTSTR) CString(bstrName));
					}
					else
					{
						strOut.Format( "\tSet ObjArray(%d) = World.CreateInstance(\"%s\", Nothing, ObjArray(%d))\n",
										iObj,
										(LPCTSTR) CString(bstrName),
										lContainerID);
					}

					hr = S_OK;
				}
				else
				{
					/// get the id out of the exemplar
					hr = pThingExemplar->get_ID(&lID);
					if (FAILED(hr))
					{
						ASSERT(FALSE);
						strOut.Format("\t' Could not get id of Exemplar from object #%d\n", iObj);
						SAFERELEASE(pThingContainer);
						SAFERELEASE(pThingExemplar);
						hr = S_OK;
						goto CREATE_NEXTOBJECT;
					}

					if (lContainerID == NULL_OBJECT)
					{
						// Create an empty instance
						strOut.Format(	"\tSet ObjArray(%d) = World.CreateInstance(\"%s\", ObjArray(%d), Nothing)\n", 
										iObj, 
										(LPCTSTR) CString(bstrName), 
										lID);
					}
					else
					{
						// Create an empty instance
						strOut.Format(	"\tSet ObjArray(%d) = World.CreateInstance(\"%s\", ObjArray(%d), ObjArray(%d))\n", 
										iObj, 
										(LPCTSTR) CString(bstrName), 
										lID,
										lContainerID);
					}

					// free stuff
					SAFERELEASE(pThingExemplar);
				}

				SAFERELEASE(pThingContainer);
			}

CREATE_NEXTOBJECT:
			file.WriteString(strOut);

			// release stuff
			SAFERELEASE(pThing);
		}

		file.WriteString(	"\tWorld.Trace \"Done creating objects\"\n\n"
							"\t' Initialize objects\n\n");

		// do object details now
		for (iObj=1; iObj<cObj; iObj++)
		{
			CComBSTR bstrName;
			CComBSTR bstrOut;

			hr = m_pDb->GetObject(iObj, &pThing);
			if (FAILED(hr))
			{
				// A deleted object
				hr = S_OK;
				continue;
			}

			// skip world owner
			if (pThing == pWorldOwner)
				goto INIT_NEXTOBJECT;

			// generate the context
			if (iObj == WORLD_OBJECT)
				bstrContext = "World.Global";
			else
			{
				str.Format("ObjArray(%d)", iObj);
				bstrContext = str;
			}

			// see if it's an exemplar
			if (iObj == WORLD_OBJECT)
				bExemplar = VARIANT_FALSE;
			else
			{
				hr = pThing->get_IsExemplar(&bExemplar);
				if (FAILED(hr))
				{
					// probably cocooned thing
					bExemplar = VARIANT_FALSE;
					hr = S_OK;
				}
			}

			// if the object is an exemplar, skip it
			// REVIEW: This is the essence of the scripting solution!
			// it is essential that no changes have been made to the
			// exemplars (i.e. that they're restored by the addition
			// of the modules that create them
			if (bExemplar == VARIANT_FALSE)
			{
				// get the object name
				hr = pThing->get_Name(&bstrName.m_str);
				if (FAILED(hr))
				{
					strOut.Format("\t' Skipping object #%d - could not get its name\n", iObj);
					file.WriteString(strOut);
					hr = S_OK;
					goto INIT_NEXTOBJECT;
				}

				// write out a nice comment			
				str.Format("\t' Initialize: %s\n", (LPCTSTR) CString(bstrName));
				file.WriteString(str);

				// tell object to write itself out
				hr = pThing->MarshallToScript(MARSHALL_BYVALUE, bstrContext, &bAsVal, &bstrOut.m_str);
				if (FAILED(hr))
				{
					strOut.Format("\t' Failed to marshall object \"%s\"\n", (LPCSTR) CString(bstrName));
					file.WriteString(strOut);

					hr = S_OK;
					goto INIT_NEXTOBJECT;
				}

				if (bstrOut!=NULL)
				{
					str = bstrOut;

					file.WriteString(str);
					file.WriteString("\n");
				}
			}

INIT_NEXTOBJECT:
			SAFERELEASE(pThing);
		}

		// write out footer
		file.WriteString(	"\tWorld.Trace \"Done initializing objects\"\n\n"
							"\tWorld.SaveDatabase\n"
							"\tWorld.Trace \"Done saving database\"\n\n"
							"\tWorld.Trace \"Done restoring world from script\"\n\n"
							"End Sub\n");

	}
	CATCH (CFileException, e)
	{
		// REVIEW: need better error code
		hr = E_FAIL;
	}
	END_CATCH

ERROR_ENCOUNTERED:

	file.Close();

	SAFERELEASE(pWorldOwner);
	SAFERELEASE(pMod);
	SAFERELEASE(pThing);
	SAFERELEASE(pThingExemplar);
	SAFERELEASE(pThingContainer);
	SAFERELEASE(pModuleNames);

	return hr;
}

STDMETHODIMP CWorldObject::Synchronize(void)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::Synchronize\n");

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		// no-op
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 0);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = SendWorldCommand(VARIANT_FALSE, DISPID_WORLD_SYNCHRONIZE, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
		
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED) 
		{
			hr = senthr;
		}
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

HRESULT CWorldObject::LogCommand(long lID, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

#ifdef ENABLE_LOGGING
	if (m_bLogging == VARIANT_TRUE)
	{
#ifdef _DEBUG
		DWORD lBytes = 0;

		ASSERT(lID != NULL_OBJECT);

		/* hr = */ pbuffer->get_Length(&lBytes);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD /* TRACE_LOGGING */, "CWorldObject::LogCommand: object ID %d, %d bytes\n", lID, lBytes);
#endif

		hr = m_pDb->WriteLogRecord(lID, pbuffer);
	}
#endif

	return hr;
}

HRESULT CWorldObject::LogCreateObject(long lID)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::LogCreateObject: id %d\n", lID);

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		return hr;

	dispparams.rgvarg[0].vt = VT_I4;
	dispparams.rgvarg[0].lVal = lID;

	hr = LogWorldCommand(DISPID_WORLD_CREATEOBJECTINT, DISPATCH_METHOD, &dispparams);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	return hr;
}

HRESULT CWorldObject::LogDeleteObject(IThing* pthing)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	if (pthing == NULL)
		return E_INVALIDARG;

#ifdef _DEBUG
	long lID = 0L;
	/* hr = */ pthing->get_ID(&lID);
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::LogDeleteObject: id %d\n", lID);
#endif

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		return hr;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pthing;

	hr = LogWorldCommand(DISPID_WORLD_DELETEOBJECTINT, DISPATCH_METHOD, &dispparams);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	return hr;
}

STDMETHODIMP CWorldObject::LogThingCommand(IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;
	long lID = NULL_OBJECT;

	if (pthing == NULL)
		return E_INVALIDARG;

	/* hr = */ pthing->get_ID(&lID);

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::LogThingCommand: command 'o', id %d, dispid %d, wFlags %x\n", lID, dispid, wFlags);

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'o');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef SUPPORT_SERVER_CALLSTACK
	hr = WriteCommandCallstack(m_pMarshBuf /* , VARIANT_TRUE */);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// marshal IThing (by ref)
	hr = MarshallThing(pthing, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPID
	hr = m_pMarshBuf->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = m_pMarshBuf->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = MarshallDISPPARAMS(pdispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = LogCommand(lID, m_pMarshBuf);

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::LogWorldCommand(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::LogWorldCommand: command 'w', dispid %d, wFlags %x\n", dispid, wFlags);

	// kick out early if logging not turned on
	if (m_bLogging == VARIANT_FALSE)
		return S_OK;

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'w');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPID
	hr = m_pMarshBuf->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = m_pMarshBuf->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = MarshallDISPPARAMS(pdispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = LogCommand(WORLD_OBJECT, m_pMarshBuf);

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

HRESULT CWorldObject::LogModule(IModule* pmodule, BSTR bstrName)
{
	HRESULT hr = S_OK;
	long lType = mcScript;

	if (pmodule == NULL)
		return E_INVALIDARG;

	// kick out early if logging not turned on
	if (m_bLogging == VARIANT_FALSE)
		return S_OK;

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'M');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	/* hr = */ pmodule->get_ModuleType(&lType);

	// marshall IModule by val
	hr = MarshallObjectProperty(pmodule, (lType == mcScript) ? clsidScriptModule : clsidCOMModule, 
		MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshall module name
	hr = m_pMarshBuf->put_String(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::LogModule: command 'M', type %d, name %s\n", lType, CString(bstrName));
#endif

	hr = LogCommand(WORLD_OBJECT, m_pMarshBuf);

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendExemplar(long lval, IThing* pthing, BSTR bstr)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendExemplar\n");

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'E');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal IThing by val
	hr = MarshallThing(pthing, MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshall exemplar type
	hr = m_pMarshBuf->put_String(bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				lTotalLength += lBufferLength;
				lCount++;

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendExemplar: mode %s, exemplar (%s), %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					ModeToCString(lval), CString(bstr), 
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, pthing, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

HRESULT CWorldObject::SendModules(void)
{
	VARIANT_BOOL bAllDone = VARIANT_FALSE;
	CComVariant var;
	IModule* pmodule = NULL;
	HRESULT hr = S_OK;
	BSTR bstr = NULL;
	IPropertyMap* pModules = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendModules\n");

	hr = get_Modules(&pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// get first property
	hr = pModules->FirstItem(&bstr, &var, &bAllDone);

	while (bAllDone == VARIANT_FALSE && SUCCEEDED(hr)) 
	{
		ASSERT(var.vt == VT_DISPATCH);

		// get the connection property
		pmodule = (IModule*)var.pdispVal;

		hr = SendModule(soClient, pmodule, bstr);

		// clean up alloced memory
		var.Clear();
		pmodule = NULL;
		SAFEFREESTRING(bstr);

		if (FAILED(hr))
			break;

		// get next property
		hr = pModules->NextItem(&bstr, &var, &bAllDone);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pModules);

	return hr;
}

HRESULT CWorldObject::SendExemplars(void)
{
	VARIANT_BOOL bAllDone = VARIANT_FALSE;
	CComVariant var;
	IThing* pthing = NULL;
	HRESULT hr = S_OK;
	BSTR bstr = NULL;
	IPropertyMap* pExemplars = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendExemplars\n");

	hr = get_Exemplars(&pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// get first property
	hr = pExemplars->FirstItem(&bstr, &var, &bAllDone);

	while (bAllDone == VARIANT_FALSE && SUCCEEDED(hr)) 
	{
		ASSERT(var.vt == VT_DISPATCH);

		pthing = (IThing*)var.pdispVal;

		hr = SendExemplar(soClient, pthing, bstr);

		// clean up alloced memory
		var.Clear();
		pthing = NULL;
		SAFEFREESTRING(bstr);

		if (FAILED(hr))
			break;

		// get next property
		hr = pExemplars->NextItem(&bstr, &var, &bAllDone);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplars);

	return hr;
}

HRESULT CWorldObject::SendToAvatars(VARIANT_BOOL bExcludeUser, IMarshallBuffer* pbuffer)
{
	VARIANT_BOOL bAllDone = VARIANT_FALSE;
	CComVariant var, varConn;
	IThing* pthing = NULL;
	IVWCommConnection* pconn = NULL;
	HRESULT hr = S_OK;
	IPropertyMap* pAvatars = NULL;
	IThing* pUser = NULL;

	/* hr = */ get_User(&pUser);

#ifdef _DEBUG
	{
		CComBSTR bstrUserName("n/a");

		if (pUser)
			pUser->get_Name(&bstrUserName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "\nCWorldObject::SendToAvatars: exclusive %s, user (%s), %s\n", 
			bExcludeUser ? "yes" : "no", CString(bstrUserName), m_bServerSide ? "server" : "client");
	}
#endif

	hr = get_Avatars(&pAvatars);
	if (FAILED(hr))
	{
		// NOTE: we need to tolerate this because the avatar list might
		// not exist yet.
		if (hr==VWOBJECT_E_PROPERTYNOTEXIST)
			hr = S_OK;
		goto ERROR_ENCOUNTERED;
	}

	// for each avatar, get connection prop
	// if not null, send async to it

	// get first property
	hr = pAvatars->FirstItem(NULL, &var, &bAllDone);

	while (bAllDone == VARIANT_FALSE && SUCCEEDED(hr)) 
	{
		ASSERT(var.vt == VT_DISPATCH);

		// get the connection property
		pthing = (IThing*)var.pdispVal;
		ASSERT(pthing);
		
		if (bExcludeUser == VARIANT_FALSE || pthing != pUser)
		{
			hr = SendToAvatar(pthing, pbuffer);
			// review, for now just trace if failed, we don't want to go to ERROR_ENCOUNTERED without releasing the memory
			if (FAILED(hr))
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::SendToAvatars: failed on SendToAvatar, hr=%x", hr); 
		}

		// clean up alloced memory
		var.Clear();
		varConn.Clear();
		pconn = NULL;
		pthing = NULL;

		if (FAILED(hr))
			break;

		// get next property
		hr = pAvatars->NextItem(NULL, &var, &bAllDone);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pAvatars);
	SAFERELEASE(pUser);

	return hr;
}

HRESULT CWorldObject::SendToAvatar(IThing* pAvatar, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	CComVariant varConnection;
	IVWCommConnection* pConnection;

	if (SUCCEEDED(pAvatar->get_Property(m_bstrConnection, &varConnection)))
		{
			pConnection = (IVWCommConnection*)varConnection.punkVal;
			ASSERT(pConnection);

			// if connection is found in connection list,
			// meaning that an avatar has logged in under
			// this connection, then send the message on the connection
			if (m_clist.FindConnection(pConnection) != NULL)
			{
				// send it async
				hr = pConnection->SendAsync(pbuffer);

				// absorb invalid socket errors
				if (hr == VWCOMM_E_INVALIDSOCKET)
					hr = S_OK;
			}
		}

	return hr;
}

HRESULT CWorldObject::SendToBystanders(IThing* pwho, VARIANT_BOOL bExcludeUser, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	int i, iCount = 0;
	IThing* pthing = NULL;
	IVWCommConnection* pconn = NULL;
	IVWCommConnection** rgpconn = NULL;
	int cconn=0;
	IPropertyList* pContents = NULL;
	CComVariant varConn;
	IThing* pUser = NULL;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	/* hr = */ get_User(&pUser);

#ifdef _DEBUG
	{
		CComBSTR bstrName("n/a");
		CComBSTR bstrUserName("n/a");

		pwho->get_Name(&bstrName.m_str);

		if (pUser)
			pUser->get_Name(&bstrUserName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "\nCWorldObject::SendToBystanders: exclusive %s, root (%s), user (%s), %s\n", 
			bExcludeUser ? "yes" : "no", CString(bstrName), CString(bstrUserName), m_bServerSide ? "server" : "client");
	}
#endif

	/* hr = */ AddContentsToBystanders(pwho);

	/* hr = */ AddContainerToBystanders(pwho);

	iCount = m_bystanderList.GetSize();

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::SendToBystanders: %d bystanders found\n", iCount);

	// allocate an array of connections (may not get filled)
	// REVIEW: use NEW (if can figure out how to do **
	rgpconn = (IVWCommConnection **) malloc(iCount * sizeof(IVWCommConnection *)); 
	if (rgpconn == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	// for each bystander, get connection prop
	// if not null add it to the connection array
	for (i = 0; i < iCount; i++)
	{
		pthing = m_bystanderList[i];
		ASSERT(pthing);

#ifdef _DEBUG
		{
			CComBSTR bstrName("n/a");
			pthing->get_Name(&bstrName.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::SendToBystanders: send to (%s), skip %s\n", 
				CString(bstrName), (bExcludeUser == VARIANT_TRUE && pthing == pUser) ? "yes" : "no");
		}
#endif

		if (pthing == NULL || (bExcludeUser == VARIANT_TRUE && pthing == pUser))
			continue;

		if (SUCCEEDED(pthing->get_Property(m_bstrConnection, &varConn)))
		{
			pconn = (IVWCommConnection*)varConn.punkVal;
			ASSERT(pconn);

			// add connection to array
			rgpconn[cconn++] = pconn;
		}
#ifdef SENDTOCLIENTIFNOCONNECTIONANDNULLUSER
		else
		{
			// REVIEW: Will we get some false positives here?.
			// if we can't get the connection property, this may be because
			// we're working on the current user's avatar and that's we're
			// not done initializing it yet
			if (pthing==pUser || pUser==NULL)
			{
				// add connection to array
				rgpconn[cconn++] = m_pVWConnection;
			}
		}
#endif

		// clean up alloced memory
		varConn.Clear();
		pconn = NULL;
	}

	// now do the batch send
	if (cconn > 0)
	{
		// can use any IVWCommConnection object for this
		pconn = rgpconn[0];

		hr = pconn->SendAsyncExt(pbuffer, cconn, rgpconn);

		// absorb invalid socket errors
		if (hr == VWCOMM_E_INVALIDSOCKET)
			hr = S_OK;
	}
	
ERROR_ENCOUNTERED:
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::SendToBystanders: done\n");

	m_bystanderList.RemoveAll();

	if (rgpconn!=NULL)
		free(rgpconn);

	SAFERELEASE(pUser);

	return hr;
}

HRESULT CWorldObject::AddContainerToBystanders(IThing* pthis)
{
	HRESULT hr = S_OK;
	IThing* pcontainer = NULL;

	// REVIEW: error?
	if (pthis == NULL)
		return S_OK;

#ifdef _DEBUG
	{
		CComBSTR bstrName;
		pthis->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContainerToBystanders: root (%s)\n", CString(bstrName));
	}
#endif

	hr = pthis->get_Container(&pcontainer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pcontainer != NULL)
	{
		VARIANT_BOOL bIsOpen = VARIANT_FALSE;
		VARIANT_BOOL bIsPrivateOpen = VARIANT_FALSE;

		/* hr = */ pcontainer->get_BOOL(CThingObject::m_bstrIsOpen, &bIsOpen);

#ifdef _DEBUG
		{
			CComBSTR bstrName;
			pcontainer->get_Name(&bstrName.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContainerToBystanders: container of root (%s)\n", CString(bstrName));
		}
#endif

		// TODO: look up pcontainer in m_pVWConnection map for 'private open'

		if (bIsOpen == VARIANT_TRUE || bIsPrivateOpen == VARIANT_TRUE)
		{
#ifdef _DEBUG
			{
				CComBSTR bstrName;
				pcontainer->get_Name(&bstrName.m_str);

				VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContainerToBystanders: container (%s) %s open\n", 
					CString(bstrName), (bIsPrivateOpen == VARIANT_TRUE) ? "is privately" : "is");
			}
#endif

			/* hr = */ AddContentsToBystanders(pcontainer);

			/* hr = */ AddContainerToBystanders(pcontainer);
		}
		else
		{
			VARIANT_BOOL bIsNoticeable = VARIANT_FALSE;

			/* hr = */ pthis->get_BOOL(CThingObject::m_bstrIsNoticeable, &bIsNoticeable);

			if (bIsNoticeable == VARIANT_TRUE)
			{
#ifdef _DEBUG
				{
					CComBSTR bstrName;
					CComBSTR bstrName2;

					pthis->get_Name(&bstrName.m_str);
					pcontainer->get_Name(&bstrName2.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContainerToBystanders: root (%s) is noticeable within container (%s)\n", CString(bstrName), CString(bstrName2));
				}
#endif

				/* hr = */ AddContentsToBystanders(pcontainer);

				/* hr = */ AddContainerToBystanders(pcontainer);
			}
			else
			{
#ifdef _DEBUG
				{
					CComBSTR bstrContainerName;
					pcontainer->get_Name(&bstrContainerName.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContainerToBystanders: container (%s) is closed, adding container and its contents\n", CString(bstrContainerName));
				}
#endif

				// container is closed, can't propagate upwards so add container
				// to bystanders along with its contents

				/* hr = */ AddContentsToBystanders(pcontainer);

				/* hr = */ AddThingToBystanders(pcontainer);
			}
		}

		SAFERELEASE(pcontainer);
	}
	else
	{
#ifdef _DEBUG
		{
			CComBSTR bstrName;
			pthis->get_Name(&bstrName.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContainerToBystanders: root (%s) has no container, adding root\n", CString(bstrName));
		}
#endif

		// if no container, add root to bystander list since its container won't

		/* hr = */ AddThingToBystanders(pthis);
	}

ERROR_ENCOUNTERED:
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContainerToBystanders: done\n");

	return hr;
}

HRESULT CWorldObject::AddContentsToBystanders(IThing* pcontainer)
{
	HRESULT hr = S_OK;
	IPropertyList* pContents = NULL;

#ifdef _DEBUG
	{
		CComBSTR bstrName;
		pcontainer->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContentsToBystanders: container (%s)\n", 
			CString(bstrName));
	}
#endif

	hr = pcontainer->get_Contents(&pContents);

	if (SUCCEEDED(hr) && pContents != NULL)
	{
		IThing* pthing = NULL;
		long propcnt = 0;

		VARIANT_BOOL bIsOpen = VARIANT_FALSE;
		VARIANT_BOOL bIsPrivateOpen = VARIANT_FALSE;

		/* hr = */ pcontainer->get_BOOL(CThingObject::m_bstrIsOpen, &bIsOpen);

		// TODO: look up pcontainer in m_pVWConnection map for 'private open'

		hr = pContents->get_Count(&propcnt);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContentsToBystanders: %d items in contents\n", propcnt);

		for (int i = 0; i < propcnt && SUCCEEDED(hr); i++)
		{
			if (SUCCEEDED(pContents->get_Thing(i, &pthing)) && pthing)
			{
#ifdef _DEBUG
				{
					CComBSTR bstrName;
					pthing->get_Name(&bstrName.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContentsToBystanders: content (%s)\n", CString(bstrName));
				}
#endif

				if (bIsOpen == VARIANT_TRUE || bIsPrivateOpen == VARIANT_TRUE)
				{
#ifdef _DEBUG
					{
						CComBSTR bstrName;
						pcontainer->get_Name(&bstrName.m_str);

						VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContentsToBystanders: container (%s) %s open\n", 
							CString(bstrName), (bIsPrivateOpen == VARIANT_TRUE) ? "is privately" : "is");
					}
#endif

					/* hr = */ AddThingToBystanders(pthing);

//					/* hr = */ AddContentsToBystanders(pthing);
				}
				else
				{
					VARIANT_BOOL bIsNoticeable = VARIANT_FALSE;

					/* hr = */ pthing->get_BOOL(CThingObject::m_bstrIsNoticeable, &bIsNoticeable);

					if (bIsNoticeable == VARIANT_TRUE)
					{
#ifdef _DEBUG
						{
							CComBSTR bstrName;
							CComBSTR bstrName2;

							pthing->get_Name(&bstrName.m_str);
							pcontainer->get_Name(&bstrName2.m_str);

							VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContentsToBystanders: thing (%s) is noticeable within container (%s)\n", CString(bstrName), CString(bstrName2));
						}
#endif

						/* hr = */ AddThingToBystanders(pthing);

//						/* hr = */ AddContentsToBystanders(pthing);
					}
					else
					{
#ifdef _DEBUG
						{
							CComBSTR bstrName;
							pcontainer->get_Name(&bstrName.m_str);

							VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContentsToBystanders: container (%s) is closed\n", CString(bstrName));
						}
#endif
					}
				}

				SAFERELEASE(pthing);
			}
		}

		SAFERELEASE(pContents);
	}

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddContentsToBystanders: done\n");

	return hr;
}

HRESULT CWorldObject::AddThingToBystanders(IThing* pthing)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bAvatar = VARIANT_FALSE;

	hr = IsAvatar(pthing, &bAvatar);
	if (SUCCEEDED(hr) && bAvatar == VARIANT_TRUE)
	{
		hr = m_bystanderList.Add(pthing);
	}

#ifdef _DEBUG
	{
		CComBSTR bstrName;
		pthing->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_BYSTANDERS, "CWorldObject::AddThingToBystanders: thing (%s) %s added\n", 
			CString(bstrName), (bAvatar == VARIANT_TRUE) ? "was" : "wasn't");
	}
#endif

	return S_OK;
}

STDMETHODIMP CWorldObject::RegisterInterest(IThing* pthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::RegisterInterest\n");
#endif

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RegisterInterest: incoming argument (IThing*) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		// register pthing in m_pVWConnection map

		// REVIEW: Is this the best way to do this?
		hr = pthing->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_TRUE);
		if (FAILED(hr)) 
			goto ERROR_ENCOUNTERED;

		hr = SendContainer(soBystanders, pthing);
		if (FAILED(hr)) 
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthing;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_REGISTERINTEREST, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::UnregisterInterest(IThing* pthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnregisterInterest\n");
#endif

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnregisterInterest: incoming argument (IThing*) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		// unregister pthing in m_pVWConnection map

		// REVIEW: Is this the best way to do this?
		hr = pthing->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_FALSE);
		if(FAILED(hr)) goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthing;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_UNREGISTERINTEREST, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

HRESULT CWorldObject::SendContainer(long lval, IThing* pcontainer)
{
	HRESULT hr = S_OK;

	if (m_bServerSide == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendContainer: shouldn't be called from client-side\n");
		return VWOBJECT_E_ONLYSERVERSIDE;
	}

#ifdef _DEBUG
	{
		CComBSTR bstrName;
		pcontainer->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "\nCWorldObject::SendContainer: mode %s, root (%s)\n", 
			ModeToCString(lval), CString(bstrName));
	}
#endif

	hr = SendContents(lval, pcontainer, VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = SendThing(lval, pcontainer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendContainer: done\n");

	return hr;
}

HRESULT CWorldObject::SendContents(long lval, IThing* pcontainer, VARIANT_BOOL bForceOpen)
{
	HRESULT hr = S_OK;
	IPropertyList* pContents = NULL;
	VARIANT_BOOL bIsOpen = VARIANT_FALSE;
	VARIANT_BOOL bIsPrivateOpen = VARIANT_FALSE;
	VARIANT_BOOL bIsNoticeable = VARIANT_FALSE;

	if (m_bServerSide == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendContents: shouldn't be called from client-side\n");
		return VWOBJECT_E_ONLYSERVERSIDE;
	}

#ifdef _DEBUG
	{
		CComBSTR bstrName;
		pcontainer->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendContents: mode %s, container (%s)\n", 
			ModeToCString(lval), CString(bstrName));
	}
#endif

	// update client with container's contents
	if (SUCCEEDED(pcontainer->get_Contents(&pContents)) && pContents)
	{
		IThing* pthing = NULL;
		long propcnt = 0;

		/* hr = */ pcontainer->get_BOOL(CThingObject::m_bstrIsOpen, &bIsOpen);

		// TODO: look up pthing in m_pConnection map for 'private open' flag

		hr = pContents->get_Count(&propcnt);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendContents: %d items in contents\n", propcnt);

		for (int i = 0; i < propcnt && SUCCEEDED(hr); i++)
		{
			if (SUCCEEDED(pContents->get_Thing(i, &pthing)) && pthing)
			{
#ifdef _DEBUG
				{
					CComBSTR bstrName;
					pthing->get_Name(&bstrName.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendContents: content (%s)\n", CString(bstrName));
				}
#endif

				// 1. test if sending object to originating client, or if container is open
				if (bForceOpen == VARIANT_TRUE || bIsOpen == VARIANT_TRUE || bIsPrivateOpen == VARIANT_TRUE)
				{
#ifdef _DEBUG
					{
						CComBSTR bstrName;
						pcontainer->get_Name(&bstrName.m_str);

						VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendContents: container (%s) %s open\n", 
							CString(bstrName), (bIsPrivateOpen == VARIANT_TRUE) ? "is privately" : ((bForceOpen == VARIANT_TRUE) ? "is forced" : "is"));
					}
#endif

					hr = SendThing(lval, pthing);
	
					if (SUCCEEDED(hr))
						hr = SendContents(lval, pthing, VARIANT_FALSE);
				}
				else 
				{
					/* hr = */ pthing->get_BOOL(CThingObject::m_bstrIsNoticeable, &bIsNoticeable);

					// 2. test if object is marked as noticeable within container
					if (bIsNoticeable == VARIANT_TRUE)
					{
#ifdef _DEBUG
						{
							CComBSTR bstrName;
							CComBSTR bstrName2;

							pthing->get_Name(&bstrName.m_str);
							pcontainer->get_Name(&bstrName2.m_str);

							VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendContents: thing (%s) is noticeable within container (%s)\n", CString(bstrName), CString(bstrName2));
						}
#endif

						hr = SendThing(lval, pthing);
					
						if (SUCCEEDED(hr))
							hr = SendContents(lval, pthing, VARIANT_FALSE);
					}
					else
					{
#ifdef _DEBUG
						{
							CComBSTR bstrName;

							pcontainer->get_Name(&bstrName.m_str);

							VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendContents: container (%s) is closed\n", CString(bstrName));
						}
#endif
					}
				}

				SAFERELEASE(pthing);
			}
		}

		SAFERELEASE(pContents);
	}

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendContents: done\n");

	return hr;
}

HRESULT CWorldObject::SendCommandExt(long lval, IThing* pthing, IMarshallBuffer* pbuffer, VARIANT_BOOL bSync, VARIANT_BOOL bReply, HRESULT* precvhr, VARIANT* pvar)
{
	HRESULT hr = S_OK;

	// clear out outgoing data
	if (precvhr)
		*precvhr = S_OK;

	if (pvar)
		::VariantClear(pvar);

	// ignore remoting request if not connected to server (ConnectLocal mode),
	// or if local mode set
	if (m_bConnected == VARIANT_FALSE || m_bLocalMode == VARIANT_TRUE)
		return VWOBJECT_S_NOTREMOTED;

#ifdef GENERAL_SEND_NETWORK_DEBUG
	{
		DWORD lBufferLength = 0L;
		static long lCount = 0L;
		static DWORD lTotalLength = 0L;
		static DWORD dwLastTime = 0L;
		static DWORD dwTotalTime = 0L;
		DWORD dwTime = GetTickCount();
		DWORD dwDelta = 0L;

		if (dwLastTime != 0)
		{
			dwDelta = dwTime - dwLastTime;
			dwTotalTime += dwDelta;
		}

		dwLastTime = dwTime;

		if (SUCCEEDED(pbuffer->get_Length(&lBufferLength)))
		{
			CComBSTR bstrName("n/a");

			lTotalLength += lBufferLength;
			lCount++;

			if (pthing)
				pthing->get_Name(&bstrName.m_str);

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendCommandExt: %s, %s, sync %s, reply %s, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
				CString(bstrName), ModeToCString(lval), 
				(bSync == VARIANT_TRUE) ? "yes" : "no",
				(bReply == VARIANT_TRUE) ? "yes" : "no",
				lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif

#ifdef SEND_PERF_DEBUG
			VWREPORT(m_pWorld, VWR_PERFLOG, "Send Packet,%d,%s,%d\n",
				dwTime, ModeToCString(lval), lBufferLength);
#endif
		}
	}
#endif

	if (m_bServerSide == VARIANT_FALSE)
	{
		if (lval == soServer)
			hr = SendToConnection(pbuffer, bSync, bReply, precvhr, pvar);
		else if (lval == soAvatar)
			hr = SendToAvatar(pthing, pbuffer);
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendCommandExt: invalid destination, client-side, destination %d\n", lval);
			hr = VWOBJECT_E_INVALIDDESTINATION;
		}
	}
	else
	{
		if (lval == soClient)
			hr = SendToConnection(pbuffer, bSync, bReply, precvhr, pvar);
		else if (lval == soBystanders)
			hr = SendToBystanders(pthing, VARIANT_FALSE, pbuffer);
		else if (lval == soBystandersExclUser)
			hr = SendToBystanders(pthing, VARIANT_TRUE, pbuffer);
		else if (lval == soAvatars)
			hr = SendToAvatars(VARIANT_FALSE, pbuffer);
		else if (lval == soAvatarsExclUser)
			hr = SendToAvatars(VARIANT_TRUE, pbuffer);
		else if (lval == soAvatar)
			hr = SendToAvatar(pthing, pbuffer);
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendCommandExt: invalid destination, server-side, destination %d\n", lval);
			hr = VWOBJECT_E_INVALIDDESTINATION;
		}
	}

	return hr;
}

HRESULT CWorldObject::SendToConnection(IMarshallBuffer* pbuffer, VARIANT_BOOL bSync, VARIANT_BOOL bReply, HRESULT* precvhr, VARIANT* pvar)
{
	HRESULT hr = S_OK;

	if (m_pVWConnection == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendToConnection: invalid connection\n");
		return VWOBJECT_E_INVALIDCONNECTION;
	}

	// send buffer to active connection
	if (bSync == VARIANT_TRUE)
	{
		IUnMarshallBuffer* pbufReply = NULL;

		hr = m_pVWConnection->SendSync(pbuffer, &pbufReply);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED_1;

		if (precvhr != NULL)
		{
			hr = pbufReply->get_Long((long *)precvhr);

			// if reply (VARIANT) expected, pull it out of the stream
			if (bReply == VARIANT_TRUE && SUCCEEDED(*precvhr) && pvar != NULL)
			{
				hr = UnMarshallProperty(pbufReply, pvar);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED_1;
			}
		}

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pbufReply);
	}
	else
	{
		hr = m_pVWConnection->SendAsync(pbuffer);
	}

	// absorb invalid socket errors
	if (hr == VWCOMM_E_INVALIDSOCKET)
		hr = S_OK;

	return hr;
}

STDMETHODIMP CWorldObject::SendObjectPropertyCommandExt(VARIANT_BOOL bReply, VARIANT_BOOL bSync, long lFlags,
	IObjectProperty* pprop, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* precvhr, VARIANT* pvar)
{
	IThing* pthing = NULL;
	BSTR bstr = NULL;
	long lID = NULL_OBJECT;
	long lval;
	HRESULT hr = S_OK;

	hr = pprop->get_Parent(&pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pprop->get_PropertyName(&bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// pthing and bstr can be NULL

	hr = WriteCommandHeader(m_pMarshBuf, bReply ? 'R' : 'N', 'p');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef SUPPORT_SERVER_CALLSTACK
	hr = WriteCommandCallstack(m_pMarshBuf /* , m_bServerSide ? VARIANT_FALSE : VARIANT_TRUE */);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// marshal IThing by ref
	hr = MarshallThing(pthing, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshall property name
	hr = m_pMarshBuf->put_String(bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPID
	hr = m_pMarshBuf->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = m_pMarshBuf->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = MarshallDISPPARAMS(pdispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	/* hr = */ pthing->get_ID(&lID);

	if (lFlags & PSBIT_NOTREMOTED)
		m_bLocalMode = VARIANT_TRUE;

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
		lval = GetMarshallingMode(pthing, lID, VARIANT_FALSE);

#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				CComBSTR bstrParent("n/a");

				lTotalLength += lBufferLength;
				lCount++;

				if (pthing)
					pthing->get_Name(&bstrParent.m_str);

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendObjectPropertyCommandExt: %s, parent (%s), property (%s), dispid %d, wFlags %x, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n"
, 
					ModeToCString(lval), CString(bstrParent), CString(bstr), dispid, wFlags,
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif

#ifdef SEND_PERF_DEBUG
				VWREPORT(m_pWorld, VWR_PERFLOG, "Send ObjectProperty Command,%d,%s,%s,%d,%d\n",
					dwTime, CString(bstrParent), CString(bstr), dispid, lBufferLength);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, pthing, m_pMarshBuf, bSync, bReply, precvhr, pvar);
	}

	if (lFlags & PSBIT_NOTREMOTED)
		m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (!(lFlags & PSBIT_NOTLOGGED))
	{
#ifdef _DEBUG
		long lID = 0L;
		/* hr = */ pthing->get_ID(&lID);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::SendObjectPropertyCommandExt: command 'p', id %d, name %s, dispid %d, wFlags %x\n", 
			lID, CString(bstr), dispid, wFlags);
#endif

		hr = LogCommand(lID, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pthing);
	SAFEFREESTRING(bstr);

	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendWorld(long lval)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendWorld\n");

	if (m_bServerSide == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendWorld: shouldn't be called from client-side\n");
		return VWOBJECT_E_ONLYSERVERSIDE;
	}

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'W');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal IThing by val
	hr = MarshallThing(m_pGlobal, MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				lTotalLength += lBufferLength;
				lCount++;

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendWorld: mode %s, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					ModeToCString(lval),
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, m_pGlobal, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendThingByID(long lval, long lID)
{
	HRESULT hr = S_OK;
	IThing* pObject = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendThingByID\n");

	if (m_bServerSide == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendThingByID: shouldn't be called from client-side\n");
		return VWOBJECT_E_ONLYSERVERSIDE;
	}

	hr = get_ObjectByID(lID, &pObject);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = SendThing(lval, pObject);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pObject);

	return hr;
}

STDMETHODIMP CWorldObject::SendThing(long lval, IThing* pthing)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendThing\n");

	if (m_bServerSide == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendThing: shouldn't be called from client-side\n");
		return VWOBJECT_E_ONLYSERVERSIDE;
	}

#ifdef _DEBUG
	{
		CComBSTR bstrName("n/a");
		if (pthing)
			pthing->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_SENDCONTAINER, "CWorldObject::SendThing: mode %s, object (%s)\n", 
			ModeToCString(lval), CString(bstrName));
	}
#endif

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'O');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal IThing by val
	hr = MarshallThing(pthing, MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				CComBSTR bstrName("n/a");
				long lID = NULL_OBJECT;

				lTotalLength += lBufferLength;
				lCount++;

				if (pthing)
				{
					pthing->get_Name(&bstrName.m_str);
					pthing->get_ID(&lID);
				}

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendThing: mode %s, thing (%s), id %d, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					ModeToCString(lval), CString(bstrName), lID,
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, pthing, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendThingCommandExt(VARIANT_BOOL bReply, VARIANT_BOOL bSync, long lFlags, 
	IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* precvhr, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	long lID = NULL_OBJECT;
	long lval;
	VARIANT_BOOL bPropChange = VARIANT_FALSE;

	// if not a property change, don't log it
	if (dispid != DISPID_THING_INVOKEMETHODINT && 
		dispid != DISPID_THING_FIREEVENTINT &&
//		dispid != DISPID_THING_FINDWITHININT &&
		dispid != DISPID_THING_MOVEINTOINT &&
		dispid != DISPID_THING_REMOTEPROPERTY)
		bPropChange = VARIANT_TRUE;

	hr = WriteCommandHeader(m_pMarshBuf, bReply ? 'R' : 'N', 'o');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef SUPPORT_SERVER_CALLSTACK
	hr = WriteCommandCallstack(m_pMarshBuf /* , m_bServerSide ? VARIANT_FALSE : VARIANT_TRUE */);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// marshal IThing (by ref)
	hr = MarshallThing(pthing, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPID
	hr = m_pMarshBuf->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = m_pMarshBuf->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = MarshallDISPPARAMS(pdispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	/* hr = */ pthing->get_ID(&lID);

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE && 
		!(lFlags & PSBIT_NOTREMOTED && dispid != DISPID_THING_ADDPROPERTYINT))
	{
		lval = GetMarshallingMode(pthing, lID, bPropChange ? VARIANT_FALSE : VARIANT_TRUE);

#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				CComBSTR bstrThis("n/a");
				CComBSTR bstrMethodName("n/a");

				lTotalLength += lBufferLength;
				lCount++;

				if (pthing)
					pthing->get_Name(&bstrThis.m_str);

				if (dispid == DISPID_THING_INVOKEMETHODINT)
					bstrMethodName = pdispparams->rgvarg[1].bstrVal;
				
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendThingCommandExt: time %d, %s, this (%s), id %d, lFlags %x, dispid %d, wFlags %x, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					dwTime, ModeToCString(lval), CString(bstrThis), lID, lFlags, dispid, wFlags,
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif

#ifdef SEND_PERF_DEBUG
				VWREPORT(m_pWorld, VWR_PERFLOG, "Send Thing Command,%d,%s,%d,%s,%d\n",
					dwTime, CString(bstrThis), dispid, CString(bstrMethodName), lBufferLength);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, pthing, m_pMarshBuf, bSync, bReply, precvhr, pvar);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// only log change commands (except instanceproperty), not invokes
	if (bPropChange && dispid != DISPID_THING_INSTANCEPROPERTY && !(lFlags & PSBIT_NOTLOGGED))
	{
#ifdef _DEBUG
		CString strOtherName("n/a");

		if (dispid == DISPID_THING_INVOKEMETHODINT)
			strOtherName = pdispparams->rgvarg[1].bstrVal;
		else if (dispid == DISPID_THING_ADDPROPERTYINT)
			strOtherName = pdispparams->rgvarg[7].bstrVal;
		else if (dispid == DISPID_THING_PROPERTY)
			strOtherName = pdispparams->rgvarg[1].bstrVal;
		else if (dispid == DISPID_THING_ADDMETHODINT)
			strOtherName = pdispparams->rgvarg[4].bstrVal;
		else if (dispid == DISPID_THING_METHOD)
			strOtherName = pdispparams->rgvarg[1].bstrVal;

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::SendThingCommandExt: command 'o', id %d, %s, dispid %d, wFlags %x\n", 
			lID, strOtherName, dispid, wFlags);
#endif

		hr = LogCommand(lID, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendModule(long lval, IModule* pmodule, BSTR bstrName)
{
	HRESULT hr = S_OK;
	long lType = mcScript;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendModule\n");

	if (m_bServerSide == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::SendModule: shouldn't be called from client-side\n");
		return VWOBJECT_E_ONLYSERVERSIDE;
	}

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'M');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pmodule)
		/* hr = */ pmodule->get_ModuleType(&lType);

	// marshall IModule by val
	hr = MarshallObjectProperty(pmodule, (lType == mcScript) ? clsidScriptModule : clsidCOMModule, 
		MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshall module name
	hr = m_pMarshBuf->put_String(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				lTotalLength += lBufferLength;
				lCount++;

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendModule: mode %s, module (%s), type %s, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					ModeToCString(lval), CString(bstrName), (lType == mcScript) ? "script" : "COM",
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, m_pGlobal, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendModuleCommand(IModule* pmodule, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;
	long lval;
	CComBSTR bstr;
	VARIANT_BOOL bPropChange = VARIANT_FALSE;

	if (pmodule == NULL)
		return E_INVALIDARG;

	// if not a property change, don't log it
	if (dispid != DISPID_MODULE_RELOAD)
		bPropChange = VARIANT_TRUE;

	hr = pmodule->get_ModuleName(&bstr.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'm');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef SUPPORT_SERVER_CALLSTACK
	hr = WriteCommandCallstack(m_pMarshBuf /* , m_bServerSide ? VARIANT_FALSE : VARIANT_TRUE */);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// marshall module name
	hr = m_pMarshBuf->put_String(bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPID
	hr = m_pMarshBuf->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = m_pMarshBuf->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = MarshallDISPPARAMS(pdispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
		lval = (m_bServerSide == VARIANT_TRUE) ? (m_bInPropChange ? soAvatarsExclUser : soAvatars): soServer;

#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				lTotalLength += lBufferLength;
				lCount++;

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendModuleCommand: %s, module (%s), dispid %d, wFlags %x, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					ModeToCString(lval), CString(bstr), dispid, wFlags,
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, m_pGlobal, m_pMarshBuf);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bPropChange == VARIANT_TRUE)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::SendModuleCommand: command 'm', name %s, dispid %d, wFlags %x\n", 
			CString(bstr), dispid, wFlags);
#endif

		hr = LogCommand(WORLD_OBJECT, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendWorldCommandExt(VARIANT_BOOL bReply, VARIANT_BOOL bSync, 
	DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* precvhr, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendWorldCommandExt\n");

	hr = WriteCommandHeader(m_pMarshBuf, bReply ? 'R' : 'N', 'w');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#if 0 // def SUPPORT_SERVER_CALLSTACK
	hr = WriteCommandCallstack(m_pMarshBuf /* , m_bServerSide ? VARIANT_FALSE : VARIANT_TRUE */);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// marshal DISPID
	hr = m_pMarshBuf->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = m_pMarshBuf->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = MarshallDISPPARAMS(pdispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = get_User(&pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				CComBSTR bstrUser("n/a");

				lTotalLength += lBufferLength;
				lCount++;

				if (pUser)
					pUser->get_Name(&bstrUser.m_str);

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendWorldCommandExt: user (%s), sync %s, reply %s, dispid %d, wFlags %x, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					CString(bstrUser), (bSync == VARIANT_TRUE) ? "yes" : "no", (bReply == VARIANT_TRUE) ? "yes" : "no", dispid, wFlags,
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif

#ifdef SEND_PERF_DEBUG
				VWREPORT(m_pWorld, VWR_PERFLOG, "Send World Command,%d,%d,%d\n",
					dwTime, dispid, lBufferLength);
#endif
			}
		}
#endif

		hr = SendCommandExt(soServer, pUser, m_pMarshBuf, bSync, bReply, precvhr, pvar);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pUser);

	m_pMarshBuf->Rewind();

	return hr;
}

STDMETHODIMP CWorldObject::SendMethodCommand(IMethod* pmethod, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
{
	IThing* pthing = NULL;
	BSTR bstr = NULL;
	HRESULT hr = S_OK;
	long lval;
	long lID = NULL_OBJECT;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::SendMethodCommand\n");

	hr = pmethod->get_Parent(&pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pmethod->get_PropertyName(&bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = WriteCommandHeader(m_pMarshBuf, 'N', 'd');
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef SUPPORT_SERVER_CALLSTACK
	hr = WriteCommandCallstack(m_pMarshBuf /* , m_bServerSide ? VARIANT_FALSE : VARIANT_TRUE */);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// marshal IThing by ref
	hr = MarshallThing(pthing, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK, m_pMarshBuf);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshall method name
	hr = m_pMarshBuf->put_String(bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPID
	hr = m_pMarshBuf->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = m_pMarshBuf->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = MarshallDISPPARAMS(pdispparams, m_pMarshBuf, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	/* hr = */ pthing->get_ID(&lID);

	if (m_bConnected == VARIANT_TRUE && m_bLocalMode == VARIANT_FALSE)
	{
		lval = GetMarshallingMode(pthing, lID, VARIANT_FALSE);

#ifdef SEND_NETWORK_DEBUG
		{
			DWORD lBufferLength = 0L;
			static long lCount = 0L;
			static DWORD lTotalLength = 0L;
			static DWORD dwLastTime = 0L;
			static DWORD dwTotalTime = 0L;
			DWORD dwTime = GetTickCount();
			DWORD dwDelta = 0L;

			if (dwLastTime != 0)
			{
				dwDelta = dwTime - dwLastTime;
				dwTotalTime += dwDelta;
			}

			dwLastTime = dwTime;

			if (SUCCEEDED(m_pMarshBuf->get_Length(&lBufferLength)))
			{
				CComBSTR bstrParent("n/a");

				lTotalLength += lBufferLength;
				lCount++;

				if (pthing)
					pthing->get_Name(&bstrParent.m_str);

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_SEND, "CWorldObject::SendMethodCommand: %s, parent (%s), property (%s), dispid %d, wFlags %x, %d bytes, avg %d bytes/buffer, total %d bytes, %d buffers, delta %7.5f sec\n", 
					ModeToCString(lval), CString(bstrParent), CString(bstr), dispid, wFlags,
					lBufferLength, (long)((double)lTotalLength / (double)lCount), lTotalLength, lCount, (double)dwDelta/1000.0);
#endif
			}
		}
#endif

		hr = SendCommandExt(lval, pthing, m_pMarshBuf);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
		long lID = 0L;
		/* hr = */ pthing->get_ID(&lID);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_LOGGING, "CWorldObject::SendMethodCommand: command 'd', id %d, name %s, dispid %d, wFlags %x\n", 
			lID, CString(bstr), dispid, wFlags);
#endif

		hr = LogCommand(lID, m_pMarshBuf);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pthing);
	SAFEFREESTRING(bstr);

	return hr;
}

STDMETHODIMP CWorldObject::WriteCommandHeader(IMarshallBuffer* pBuffer, unsigned char ucReply, unsigned char ucCommand)
{
	HRESULT hr = S_OK;

	// write out protocol version
	hr = pBuffer->put_ULong(PROTOCOL_VERSION);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pBuffer->put_Byte(ucReply);  
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pBuffer->put_Byte(ucCommand);  
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

#ifdef SUPPORT_SERVER_CALLSTACK
STDMETHODIMP CWorldObject::WriteCommandCallstack(IMarshallBuffer* pBuffer /* , VARIANT_BOOL bSendCallstack */)
{
	HRESULT hr = S_OK;
//	IThing* pInstance = NULL;
//	IThing* pExemplar = NULL;
	DWORD dwOptions = MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK;
	IThing* pCaller = NULL;

	// NOTE: can have security context from method invocation, or if we're within a CSetUID

	if (/* bSendCallstack == VARIANT_FALSE || */ m_pSecurityContext == NULL && m_pUser == NULL)
	{
		hr = pBuffer->put_Byte(0);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_CALLSTACK, "CWorldObject::WriteCommandCallstack: (no context)\n");
#endif
	}
	else
	{
		VARIANT_BOOL bOverride = VARIANT_FALSE;
		CPropertySecurity* pContext = (CPropertySecurity*)m_pSecurityContext;
		IThing* pInstance = NULL;
		IThing* pExemplar = NULL;
		IThing* pThis = NULL;

		hr = pBuffer->put_Byte(1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

//		m_pSecurityContext->get_Instance(&pInstance);
//		m_pSecurityContext->get_Exemplar(&pExemplar);

//		m_pSecurityContext->get_BitState(PSBIT_SYSTEMOVERRIDE, &bOverride);

		// NOTE: these are weak references
		if (pContext)
		{
			bOverride = ((long)pContext->m_ulBits & PSBIT_SYSTEMOVERRIDE) == PSBIT_SYSTEMOVERRIDE ? VARIANT_TRUE : VARIANT_FALSE;

			pInstance = pContext->m_pInstance;
			pExemplar = pContext->m_pExemplar;
			pCaller = m_pThingCaller;
			pThis = m_pThingThis;
		}
		else
		{
			// if no security context, write out essentially a CSetUID(this, m_pUser, m_pUser)
			// (this is how it will be recreated on the client-side)
			pInstance = m_pUser;
			pExemplar = m_pUser;
			pCaller = m_pUser;
			pThis = m_pUser;
		}

#ifdef _DEBUG
		{
			CComBSTR bstrInstance("n/a"), bstrExemplar("n/a");
			CComBSTR bstrThis("n/a"), bstrCaller("n/a"), bstrSuper("n/a");
			long lInstanceID = 0L, lExemplarID = 0L, lThisID = 0L;
			long lCallerID = 0L, lSuperID = 0L;

			if (pInstance)
			{
				pInstance->get_Name(&bstrInstance.m_str);
				pInstance->get_ID(&lInstanceID);
			}
			if (pExemplar)
			{
				pExemplar->get_Name(&bstrExemplar.m_str);
				pExemplar->get_ID(&lExemplarID);
			}
			if (pThis)
			{
				pThis->get_Name(&bstrThis.m_str);
				pThis->get_ID(&lThisID);
			}
			if (pCaller)
			{
				pCaller->get_Name(&bstrCaller.m_str);
				pCaller->get_ID(&lCallerID);
			}
			if (m_pThingSuper)
			{
				m_pThingSuper->get_Name(&bstrSuper.m_str);
				m_pThingSuper->get_ID(&lSuperID);
			}

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_CALLSTACK, "CWorldObject::WriteCommandCallstack: override (%s), instance (%s:%d), exemplar (%s:%d), this (%s:%d), caller (%s:%d), super (%s:%d)\n",
				bOverride ? "yes" : "no", CString(bstrInstance), lInstanceID, CString(bstrExemplar), lExemplarID,
				CString(bstrThis), lThisID, CString(bstrCaller), lCallerID, CString(bstrSuper), lSuperID);
		}
#endif

		hr = MarshallProperty(CComVariant((bool)bOverride), dwOptions, pBuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = MarshallObjectProperty(pInstance, clsidThing, dwOptions, pBuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = MarshallObjectProperty(pExemplar, clsidThing, dwOptions, pBuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = MarshallObjectProperty(pThis, clsidThing, dwOptions, pBuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = MarshallObjectProperty(m_pThingSuper, clsidThing, dwOptions, pBuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// NOTE: always pass caller (if NULL, pass user), so receiving server/clients have context of who initiated the change
	hr = MarshallObjectProperty(pCaller, clsidThing, dwOptions, pBuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		ASSERT(FALSE);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::WriteCommandCallstack: failed marshalling callstack, hr %x\n", hr);
	}

//	SAFERELEASE(pInstance);
//	SAFERELEASE(pExemplar);

	return hr;
}

HRESULT CWorldObject::ReadCommandCallstack(IUnMarshallBuffer* pUBuffer, CSetUID** ppSetUID)
{
	HRESULT hr = S_OK;
	IThing* pInstance = NULL;
	IThing* pExemplar = NULL;
	IThing* pThis = NULL;
	IThing* pCaller = NULL;
	IThing* pSuper = NULL;
	unsigned char bByte = 0;
	CComVariant var;
	VARIANT_BOOL bOverride = VARIANT_FALSE;

	// NOTE: doesn't save old this, caller, super ptrs; need to do that in caller

	if (ppSetUID == NULL)
		return E_POINTER;

	*ppSetUID = NULL;

	hr = pUBuffer->get_Byte(&bByte);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if no security context marshalled out, don't create CSetUID
	if (bByte == 0)
	{
//		VWTRACE(m_pWorld, "VWOBJECT", TRACE_CALLSTACK, "CWorldObject::ReadCommandCallstack: (no context)\n");
		goto READ_CALLER;
	}

	ASSERT(bByte == 1);

	hr = UnMarshallProperty(pUBuffer, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (var.vt != VT_BOOL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ReadCommandCallstack: incorrect variant type on bFromServer flag\n");

		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	bOverride = var.boolVal;

	hr = UnMarshallObjectProperty(pUBuffer, (IObjectProperty**)&pInstance);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = UnMarshallObjectProperty(pUBuffer, (IObjectProperty**)&pExemplar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	(*ppSetUID) = new CSetUID(this, pInstance, pExemplar, bOverride);

	if ((*ppSetUID) == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ReadCommandCallstack: failed to create CSetUID\n");

		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	hr = UnMarshallObjectProperty(pUBuffer, (IObjectProperty**)&pThis);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = UnMarshallObjectProperty(pUBuffer, (IObjectProperty**)&pSuper);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	/* hr = */ put_This(pThis);
	/* hr = */ put_Super(pSuper);

READ_CALLER:
	hr = UnMarshallObjectProperty(pUBuffer, (IObjectProperty**)&pCaller);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	/* hr = */ put_Caller(pCaller);

#ifdef _DEBUG
	{
		CComBSTR bstrInstance("n/a"), bstrExemplar("n/a");
		CComBSTR bstrThis("n/a"), bstrCaller("n/a"), bstrSuper("n/a");

		long lInstanceID = 0L, lExemplarID = 0L, lThisID = 0L;
		long lCallerID = 0L, lSuperID = 0L;

		if (pInstance)
		{
			pInstance->get_Name(&bstrInstance.m_str);
			pInstance->get_ID(&lInstanceID);
		}
		if (pExemplar)
		{
			pExemplar->get_Name(&bstrExemplar.m_str);
			pExemplar->get_ID(&lExemplarID);
		}
		if (m_pThingThis)
		{
			m_pThingThis->get_Name(&bstrThis.m_str);
			m_pThingThis->get_ID(&lThisID);
		}
		if (m_pThingCaller)
		{
			m_pThingCaller->get_Name(&bstrCaller.m_str);
			m_pThingCaller->get_ID(&lCallerID);
		}
		if (m_pThingSuper)
		{
			m_pThingSuper->get_Name(&bstrSuper.m_str);
			m_pThingSuper->get_ID(&lSuperID);
		}

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_CALLSTACK, "CWorldObject::ReadCommandCallstack: override (%s), instance (%s:%d), exemplar (%s:%d), this (%s:%d), caller (%s:%d), super (%s:%d)\n",
			bOverride ? "yes" : "no", CString(bstrInstance), lInstanceID, CString(bstrExemplar), lExemplarID,
			CString(bstrThis), lThisID, CString(bstrCaller), lCallerID, CString(bstrSuper), lSuperID);
	}
#endif

ERROR_ENCOUNTERED:
	SAFERELEASE(pThis);
	SAFERELEASE(pCaller);
	SAFERELEASE(pSuper);

	if (FAILED(hr))
	{
		if (*ppSetUID)
		{
			delete (*ppSetUID);
			*ppSetUID = NULL;
		}

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ReadCommandCallstack: failed unmarshalling callstack, hr %x\n", hr);
	}

	SAFERELEASE(pInstance);
	SAFERELEASE(pExemplar);

	return hr;
}
#endif

// BEGIN: Helper fcns./////////////////////////////////////////////////////////////////

HRESULT CWorldObject::DeleteThingFromDS(IThing *pAvatar)
{
#if 0
	HRESULT hr = S_OK;
	IDispatch * pDisp=NULL;
	IDServ *pDServ=NULL;
	CComBSTR bstrName, bstrURL;
	
	ASSERT(pAvatar);
	if (pAvatar == NULL)
		return E_INVALIDARG;

	/* hr = */ pAvatar->get_Name(&bstrName.m_str);

	/* hr = */ m_pGlobal->get_String(CComBSTR("WorldURL"), &bstrURL.m_str);

	hr = get_ToolExt(CComBSTR("DService"), (IUnknown**)&pDisp);
	if (SUCCEEDED(hr))
	{
		hr = pDisp->QueryInterface(IID_IDServ, (void **) &pDServ);
		if (SUCCEEDED(hr))
		{
			CComBSTR bstrServer, bstrIndex;
			
			hr = ParseURLHelper(bstrURL, NULL, &bstrServer.m_str, NULL);
			
			bstrIndex = bstrName;

			if (SUCCEEDED(hr))
			{
				bstrIndex += "@";
				bstrIndex += bstrServer;
			}

			hr = pDServ->RemoveUser(bstrIndex);
		}
	}

//ERROR_ENCOUNTERED:
	SAFERELEASE(pDisp);
	SAFERELEASE(pDServ);

	return hr;
#endif

	return S_OK;
}

HRESULT CWorldObject::MoveProfileToDS(IThing *pAvatar, IAvatarProfile *pProfile)
{
#if 0
	HRESULT hr = S_OK;
	CComBSTR bstrEmail("Email not provided"), bstrURL, bstrComment("No comment"), bstrName;
	IDispatch* pDisp = NULL;
	IDServ* pDServ = NULL;
	
	if (pAvatar == NULL)
		return E_INVALIDARG;

	// pProfile can be NULL

	hr = pAvatar->get_Name(&bstrName.m_str);
	
	if (pProfile)
	{
		hr = pProfile->get_String(CComBSTR("Email"), &bstrEmail.m_str);

		hr = pProfile->get_String(CComBSTR("Description"), &bstrComment.m_str);
	}

	// to do: look "WorldURL" up from resources.
	hr = m_pGlobal->get_String(CComBSTR("WorldURL"), &bstrURL.m_str);

	// to do: put "DService" in resources.
	hr = get_ToolExt(CComBSTR("DService"), &pDisp);
	if (SUCCEEDED(hr))
	{
		hr = pDisp->QueryInterface(IID_IDServ, (void **) &pDServ);
		if (SUCCEEDED(hr))
		{
			CString strFileName, strServer;
			int nPort;
			CComBSTR bstrIndex(bstrName);
			
			hr = ParseURLHelper(CString(bstrURL), strFileName, strServer, nPort);			
			
			if (SUCCEEDED(hr))
			{
				bstrIndex += CComBSTR("@");
				bstrIndex += CComBSTR(strServer);
			}

			hr = pDServ->AddUser(bstrEmail, bstrIndex, CComBSTR(""), bstrComment, bstrURL);		
		}
	}

//ERROR_ENCOUNTERED:
	SAFERELEASE(pDisp);
	SAFERELEASE(pDServ);

	return hr;
#endif

	return S_OK;
}

HRESULT CWorldObject::LoadRegistryIntoProfile(BSTR bstrName, BSTR* pbstrProfile)
{
	HRESULT hr = S_OK;
	IUnMarshallBuffer *pbuffer = NULL;
	CComBSTR bstrPath;

	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::LoadRegistryIntoProfile: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (pbstrProfile == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::LoadRegistryIntoProfile: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstrProfile = NULL;
	
	hr = FindAvatarInRegistry(bstrName, &bstrPath.m_str);
	if (SUCCEEDED(hr) && bstrPath.Length != 0)
	{
		// 1.  Create an unmarshall buffer
		hr = CreateUnMarshallBuffer(&pbuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		// 2.  fill it in from the disk file
		hr = pbuffer->ReadFromDisk(bstrPath);
		if (SUCCEEDED(hr))
		{
			// 3.  Copy it to a BSTR
			DWORD	dwLen = 0;
			char	*pBuffer = NULL;
			CString	strTemp;

			pbuffer->get_Length(&dwLen);
			pBuffer = strTemp.GetBuffer(dwLen);
			pbuffer->get_Bytes((unsigned char *)pBuffer, dwLen);

			*pbstrProfile = SysAllocStringByteLen(pBuffer, dwLen);

			strTemp.ReleaseBuffer();
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::LoadRegistryIntoProfile: failed to read profile from disk\n");

			// shouldn't return failure, just return NULL BSTR
			hr = S_OK;
		}
	}
	else
	{
		// shouldn't return failure, just return NULL BSTR
		hr = S_OK;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pbuffer);

	return hr;
}

HRESULT CWorldObject::SaveRoamingProfile(IThing *pUser)
{
	HRESULT hr = S_OK;
	IAvatarProfile *pProfile = NULL;
	CComBSTR bstrPath;
	CComBSTR bstrFinalPath;
	CComBSTR bstrName;
	CComVariant varName;
	VARIANT_BOOL bRoaming = VARIANT_FALSE;
	static CComBSTR	bstrRoaming("EnableRoamingProfiles");
	static CComBSTR bstrNameStatic("PreferredName");
	static CComBSTR	bstrProfileStatic("Profile");

	if (pUser == NULL)
		return E_INVALIDARG;

	hr = m_pGlobal->get_BOOL(bstrRoaming, &bRoaming);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if world doesn't support roaming profiles, don't write out profile .vwc file
	if (bRoaming == VARIANT_FALSE)
		goto ERROR_ENCOUNTERED;

	hr = pUser->get_ObjectProperty(bstrProfileStatic, (IObjectProperty **)&pProfile);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pProfile == NULL)
	{
		hr = E_FAIL; // REVIEW: need error code
		goto ERROR_ENCOUNTERED;
	}

	hr = pProfile->get_KeyValue(bstrNameStatic, &varName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (varName.vt == VT_BSTR)
		bstrName = varName.bstrVal;
	else
	{
		ASSERT(FALSE);
		hr = VWOBJECT_E_INVALIDPROFILE;
		goto ERROR_ENCOUNTERED;
	}


	hr = FindAvatarInRegistry(bstrName, &bstrPath.m_str);
	if (FAILED(hr))
	{
		hr = FindAvatarPath(&bstrPath.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		bstrFinalPath = bstrPath;
		bstrFinalPath += bstrName;
		bstrFinalPath += ".vwc";

		hr = pProfile->WriteToDisk(bstrFinalPath);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = AddAvatarToRegistry(bstrName, bstrFinalPath);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;		
	}
	else
	{
		hr = pProfile->WriteToDisk(bstrPath);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfile);

	return hr;
}

STDMETHODIMP CWorldObject::CheckPolicy(IThing *pThing, unsigned long ulFlags, VARIANT var, VARIANT_BOOL *pbAccessOk)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	if (pbAccessOk == NULL)
		return ReportWorldError(E_POINTER);

	// initialize
	*pbAccessOk = VARIANT_FALSE;

	hr = CheckPolicyExt(pThing, ulFlags, var);

	if (SUCCEEDED(hr))
		*pbAccessOk = VARIANT_TRUE;

	return S_OK;
}

STDMETHODIMP CWorldObject::CheckPolicyExt(IThing *pThing, unsigned long ulFlags, VARIANT var)
{
	HRESULT hr = S_OK;
	IThing *pOwner = NULL;
	IThing *pUser = NULL;
	IThing *pWorldOwner = NULL;
	IThing *pCaller  = NULL;
	IThing *pCallersOwner = NULL;
	IPropertyList *pList = NULL;
	VARIANT_BOOL bWizard = VARIANT_FALSE;
	static CComBSTR bstrOwner("Owner");

	// Flags are defined in vwobject.idl
#ifdef _DEBUG
	CString strFlags = "(";
	strFlags += ulFlags & CP_WIZARD ? "w":"-";
	strFlags += ulFlags & CP_OWNER ? "o":"-";
	strFlags += ulFlags & CP_WORLDOWNER ? "wo":"-";
	strFlags += ulFlags & CP_CALLER ? "c":"-";
	strFlags += ")";
#endif

	// check in order of simplest to most complex cases

	// get the current caller (user if caller is NULL)
	pCaller = (m_pThingCaller) ? m_pThingCaller : m_pUser;
	SAFEADDREF(pCaller);

	// NULL caller means trivially accept (would only occur with NULL user)
	if (pCaller == NULL)
		goto ERROR_ENCOUNTERED;

	if (ulFlags & CP_OWNER)
	{
		if (pThing == NULL)
		{
			hr = E_INVALIDARG;
			goto ERROR_ENCOUNTERED;
		}

		pOwner = ((CThingObject*)pThing)->m_pOwner;
		SAFEADDREF(pOwner);

		// if owner is NULL
		if (pOwner == NULL)
		{
			VARIANT_BOOL bIsAvatar = VARIANT_FALSE;

			// check if object is an avatar
			if (SUCCEEDED(IsAvatar(pThing, &bIsAvatar)))
			{
				// if not an avatar, make us check for world owner
				if (bIsAvatar == VARIANT_FALSE)
					ulFlags |= CP_WORLDOWNER;
			}
		}
		// else, if we have a valid owner and if caller is object's owner, accept
		else if (pCaller == pOwner)
			goto ERROR_ENCOUNTERED;
	}

	// if caller is a wizard, accept
	if (ulFlags & CP_WIZARD)
	{
		if (SUCCEEDED(pCaller->get_IsWizard(&bWizard)))
		{
			if (bWizard == VARIANT_TRUE)
				goto ERROR_ENCOUNTERED;
		}
	}

	// If caller is a world owner, accept
	if (ulFlags & CP_WORLDOWNER)
	{
		if (m_pGlobal)
		{
			pWorldOwner = ((CThingObject*)m_pGlobal)->m_pOwner;
			SAFEADDREF(pWorldOwner);

			if ((pCaller != NULL) && (pWorldOwner != NULL))
			{
				if (pCaller == pWorldOwner)
					goto ERROR_ENCOUNTERED;
			}
			else if (pWorldOwner == NULL)
				goto ERROR_ENCOUNTERED;
		}

	}

	// If the caller's owner is the same as the object's owner, accept
	// (Case of vending machine or other objects that create other object and modify them)
	if (ulFlags & CP_CALLER)
	{
		if (pThing == NULL)
			return E_INVALIDARG;

		pOwner = ((CThingObject*)pThing)->m_pOwner;
		SAFEADDREF(pOwner);

		if (pCaller)
		{
			pCallersOwner = ((CThingObject*)pCaller)->m_pOwner;
			SAFEADDREF(pCallersOwner);

			// Make sure owners exist (not case of 2 avatars), then check if same
			if (pOwner && pCallersOwner)
			{
				if (pOwner == pCallersOwner)
					goto ERROR_ENCOUNTERED;
			}
		}
	}

	// removed, not used
#if 0
	// if the thing is downstream (a sub-exemplar of) var
	if (ulFlags & CP_SUBEXEMPLAR)
	{
		if (pThing == NULL)
			return E_INVALIDARG;

		// make sure we're got an IDispatch
		if ((var.vt == VT_DISPATCH) && (var.pdispVal != NULL))
		{
			// trace back up the exemplar chain
			CComPtr<IThing> pExemplar = pThing;
			CComPtr<IThing> pNextExemplar;
			while (pExemplar)
			{
				// if we hit it, we're done
				if (pExemplar == var.pdispVal)
					goto ERROR_ENCOUNTERED;

				// Otherwise, try to get the next one in the chain
				if (FAILED(pExemplar->get_Exemplar(&pNextExemplar)))
					break;

				pExemplar = pNextExemplar;
				pNextExemplar.Release();
			}
		}
	}

	// only remaining case is the "list" case
	if (ulFlags & CP_LIST)
	{
		// check to assure a list was passed in
		if (var.vt == VT_DISPATCH && var.pdispVal != NULL)
		{
			if (SUCCEEDED(var.pdispVal->QueryInterface(IID_IPropertyList, (void **)&pList)))
			{
				// see if the user is in the list
				long lVal;

				if (SUCCEEDED(pList->Find(CComVariant((IDispatch *)pUser), &lVal)))
				{
					if (lVal != -1)
						goto ERROR_ENCOUNTERED;
				}
			}
		}
	}
#endif

#ifdef SUPPORT_SECURITY_OVERRIDE
	{
		VARIANT_BOOL bSecurityEnabled = VARIANT_TRUE;

		/* hr = */ m_pWorld->get_EnableSecurity(&bSecurityEnabled);

		if (bSecurityEnabled == VARIANT_FALSE)
			goto ERROR_ENCOUNTERED;
	}
#endif

	// all tests fail
	hr = VWOBJECT_E_PERMISSION_DENIED;

#ifdef _DEBUG
	{
		CComBSTR bstrOwner("n/a");
		CComBSTR bstrThing("n/a");
		CComBSTR bstrUser("n/a");
		CComBSTR bstrCaller("n/a");

		if (pOwner)
			pOwner->get_Name(&bstrOwner.m_str);
		if (pThing)
			pThing->get_Name(&bstrThing.m_str);
		if (pUser)
			pUser->get_Name(&bstrUser.m_str);
		if (pCaller)
			pCaller->get_Name(&bstrCaller.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::CheckPolicyExt: permission denied, ulFlags %s, owner (%s), source (%s), user (%s), caller (%s)\n", 
			strFlags, CString(bstrOwner), CString(bstrThing), CString(bstrUser), CString(bstrCaller));
	}
#endif

ERROR_ENCOUNTERED:
	SAFERELEASE(pWorldOwner);
	SAFERELEASE(pUser);
	SAFERELEASE(pOwner);
	SAFERELEASE(pList);
	SAFERELEASE(pCaller);
	SAFERELEASE(pCallersOwner);

	return hr;
}

STDMETHODIMP CWorldObject::GetStatus(BSTR *pbstr)
{
	CString strStatus;
	IThing* pThis = NULL;

	if (pbstr == NULL)
		return E_POINTER;

	// collect status information
	if (m_bInProcessMessage == VARIANT_TRUE)
		strStatus = "\t";
	else
	{
		strStatus = "\tIdle";
		goto FINISH_STATUS;
	}

	// Get info about any running method
	// Do this in an EH block in case any pointers go bogus
	try
	{
		unsigned char ucMsg = m_ucCurrentMessage;
		pThis = m_pThingThis;
		SAFEADDREF(pThis);

		switch (ucMsg)
		{
		case 'E':
			strStatus += "Receiving Exemplar";
			break;
		case 'O':
			strStatus += "Receiving Instance";
			break;
		case 'M':
			strStatus += "Receiving Module";
			break;
		case 'W':
			strStatus += "Receiving World";
			break;
		case 'o':
			strStatus += "Processing Thing command";
			break;
		case 'p':
			strStatus += "Processing Object Property command";
			break;
		case 'm':
			strStatus += "Processing Module command";
			break;
		case 'd':
			strStatus += "Processing Method command";
			break;
		case 'w':
			strStatus += "Processing World command";
			break;
		default:
			break;
		}

		if (pThis != NULL)
		{
			// add object id
			long lID = NULL_OBJECT;

			if (SUCCEEDED(pThis->get_ID(&lID)))
			{
				CString strID;
				strID.Format(", id %d", lID);
				strStatus += strID;
			}

			strStatus += ", ";

			// add name of this
			BSTR bstrName;
			if (SUCCEEDED(pThis->get_Name(&bstrName)) && bstrName)
			{
				CString strName(bstrName);
				strStatus += strName;
				::SysFreeString(bstrName);
			}
			else
				strStatus += "object";

			// only show method name, if it's a thing command
			// (because of timing, we might be onto the next command already)
			if (ucMsg == 'o')
			{
				// get the current method name
				BSTR bstrMethodName;
				if (SUCCEEDED(pThis->get_CurrentMethodName(&bstrMethodName)) && bstrMethodName)
				{
					CString strMethodName(bstrMethodName);

					if (strMethodName.GetLength() != 0)
					{
						strStatus += ".";
						strStatus += strMethodName;
					}

					::SysFreeString(bstrMethodName);
				}
			}
		}
	}
	catch(...)
	{
		strStatus = "???";
	}
	
FINISH_STATUS:
	// crlf at end
	strStatus += "\r\n";

	SAFERELEASE(pThis);

	*pbstr = strStatus.AllocSysString();

	return S_OK;
}
 
//////////////////////////////////

#include "version.h"

#if (rmm < 10)
#define rmmpad "0"
#else
#define rmmpad
#endif

#define VER_STR1(a,b,c)		#a ".00." rmmpad #b "." #c
#define VER_STR2(a,b,c)		VER_STR1(a,b,c)
#define VER_VERSION_STR		VER_STR2(rmj,rmm,rup)

STDMETHODIMP CWorldObject::get_Version(BSTR* pbstrVersion)
{
	if (pbstrVersion == NULL)
		return E_POINTER;

	*pbstrVersion = NULL;

	CString strVersion = VER_VERSION_STR;

	*pbstrVersion = strVersion.AllocSysString();

	return S_OK;
}
