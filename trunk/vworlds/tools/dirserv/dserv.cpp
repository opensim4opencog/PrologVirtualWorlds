// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DServ.cpp : Implementation of CDServ
#include "stdafx.h"
#include <activeds.h>
#include <DirServ.h>
#include "DServ.h"

/////////////////////////////////////////////////////////////////////////////
// CDServ
/////////////////////////////////////////////////////////////////////////////
CDServ::CDServ()
{
	m_pWorldAttribs = new CDSAttribMap;
	ASSERT(m_pWorldAttribs);
}

HRESULT CDServ::FinalConstruct()
{
	HRESULT hr = S_OK;

	//load up default attribs for the user object.
	CAttrib attrib;

	//m_UserAttribs.AddAttrib(CComBSTR("CN"),CComBSTR("acctname"),CComVariant(""),TRUE);
	
//	hr=m_UserAttribs.AddAttrib(CComBSTR("sAMAccountName"),CComBSTR(""),CComVariant(""),TRUE);
//	if(FAILED(hr))
//		goto ERROR_ENCOUNTERED;

	//use this for guid.
/*	hr=m_UserAttribs.AddAttrib(CComBSTR("rfc822Mailbox"),CComBSTR("email"),CComVariant(""));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=m_UserAttribs.AddAttrib(CComBSTR("givenName"),CComBSTR("firstname"),CComVariant(""));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

//	hr=m_UserAttribs.AddAttrib(CComBSTR("location"),CComBSTR("address"),CComVariant(""));
//	if(FAILED(hr))
//		goto ERROR_ENCOUNTERED;

	hr=m_UserAttribs.AddAttrib(CComBSTR("comment"),CComBSTR("users"),CComVariant(""));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	/*hr=m_UserAttribs.AddAttrib(CComBSTR("E-mail-Addresses"),CComBSTR("email"),CComVariant(""));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=m_UserAttribs.AddAttrib(CComBSTR("Home-Directory"),CComBSTR("home computer"),CComVariant(""));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr=m_UserAttribs.AddAttrib(CComBSTR("comment"),CComBSTR("autobiography"),CComVariant(""));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=m_UserAttribs.AddAttrib(CComBSTR("customAttribute1"),CComBSTR(""),CComVariant("http://computer/c:\temp\myworld"));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	*/
//ERROR_ENCOUNTERED:
 	return hr;
}


CDServ::~CDServ()
{
	Disconnect();

	m_pWorldAttribs->Terminate();

	m_pWorldAttribs->Release();

	m_pWorldAttribs = NULL;
}

STDMETHODIMP CDServ::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IDServ,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CDServ::ConnectEx2(HWND hNotificationWindow, BSTR bstrLDAP)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=S_OK;

	hr=m_pWorldAttribs->ConnectToDS(
		bstrLDAP,
		NULL,
		NULL,
		hNotificationWindow);
	if(FAILED(hr)) 
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
 	return hr;
}


STDMETHODIMP CDServ::ConnectEx(BSTR bstrLocation, BSTR bstrAdmin, BSTR bstrPasswd,HWND hNotificationWindow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

/*	return m_UserAttribs.ConnectToDS(
		bstrLocation,
		bstrAdmin,
		bstrPasswd,hNotificationWindow);
*/
	return S_OK;
}


STDMETHODIMP CDServ::Disconnect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pWorldAttribs->Disconnect();

	return S_OK;
}

STDMETHODIMP CDServ::TestQueue()
{
//	m_UserAttribs.testQueue();
	return S_OK;
}

STDMETHODIMP CDServ::EnumerateUsers(HWND hNotificationWindow)
{
//	m_UserAttribs.EnumerateChildren(hNotificationWindow);

	return S_OK;
}

STDMETHODIMP CDServ::GetNextUser(BSTR * inworldalias, BSTR * emailname, BSTR * computername, 
								 BSTR * comment, BSTR *bstrCurrentWorld)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=S_OK;
/*	BOOL bResult=TRUE;
	CComBSTR alias,email,computer,strcomment,strstate;
	AttributeMap *pMap=NULL;
	CAttrib attrib;

	hr=m_UserAttribs.GetNextChild(&pMap);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	if(pMap==NULL) 
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	bResult=pMap->Lookup(CComBSTR("sAMAccountName"),attrib);
	ASSERT(attrib.m_varValue.vt==VT_BSTR);
	alias=attrib.m_varValue.bstrVal;
	if(!bResult)
	{
		goto ERROR_ENCOUNTERED;
	}

	bResult=pMap->Lookup(CComBSTR("E-mail-Addresses"),attrib);
	ASSERT(attrib.m_varValue.vt==VT_BSTR);
	email=attrib.m_varValue.bstrVal;
	if(!bResult)
	{
		goto ERROR_ENCOUNTERED;
	}

	bResult=pMap->Lookup(CComBSTR("Home-Directory"),attrib);
	ASSERT(attrib.m_varValue.vt==VT_BSTR);
	computer=attrib.m_varValue.bstrVal;
	if(!bResult)
	{
		goto ERROR_ENCOUNTERED;
	}
	
	bResult=pMap->Lookup(CComBSTR("comment"),attrib);
	ASSERT(attrib.m_varValue.vt==VT_BSTR);
	strcomment=attrib.m_varValue.bstrVal;
	if(!bResult)
	{
		goto ERROR_ENCOUNTERED;
	}

	bResult=pMap->Lookup(CComBSTR("customAttribute1"),attrib);
	ASSERT(attrib.m_varValue.vt==VT_BSTR);
	strstate=attrib.m_varValue.bstrVal;
	if(!bResult)
	{
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	*inworldalias=alias.Copy();
	*emailname=email.Copy();
	*computername=computer.Copy();
	*comment=strcomment.Copy();	
	*bstrCurrentWorld=strstate.Copy();
	
	if(pMap) delete pMap;

#ifdef _DEBUG
	afxDump<<email<<","<<computer<<","<<strcomment<<"\r\n";
#endif

*/
	return hr;
}

STDMETHODIMP CDServ::AddUser(BSTR EmailName, BSTR InWorldAlias, BSTR ComputerName, BSTR Comment, 
							 BSTR bstrCurrentWorld)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=S_OK;
/*
	//Not allowed!

	//m_UserAttribs.AddAttrib(CComBSTR("CN"),CComBSTR("account name"),EmailName,TRUE);

	//hr=m_UserAttribs.AddAttrib(CComBSTR("sAMAccountName"),CComBSTR(""),EmailName,TRUE);

	hr=m_UserAttribs.AddAttrib(CComBSTR("givenName"),CComBSTR("firstname"),InWorldAlias);

	//hr=m_UserAttribs.AddAttrib(CComBSTR("location"),CComBSTR("address"),ComputerName);
	
	hr=m_UserAttribs.AddAttrib(CComBSTR("comment"),CComBSTR("users"),Comment);

	hr=m_UserAttribs.UploadObjectToDS(CComBSTR("RTPerson"), 
								EmailName);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
*/
	return hr;
}

STDMETHODIMP CDServ::RemoveUser(BSTR InWorldAlias)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Delete user.
	//return m_UserAttribs.DeleteObjectFromDS(CComBSTR("RTPerson"),
	//				   InWorldAlias);

	return S_OK;
}

STDMETHODIMP CDServ::AddWorld(BSTR bstrWorldGUID, BSTR bstrWorldURL, BSTR bstrWorldNoUsers, BSTR bstrInfoURL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=S_OK;

	//netmeeting stuff here.
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("port"),CComBSTR("port"),CComVariant(1503));

	hr=m_pWorldAttribs->AddAttrib(CComBSTR("ilsA39321630"),CComBSTR("ilsA39321630"),CComBSTR("1"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("ilsA26214430"),CComBSTR("ilsA26214430"),CComBSTR("0"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("ilsA32964638"),CComBSTR("ilsA32964638"),CComBSTR("0"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("ilsA32833566"),CComBSTR("ilsA32833566"),CComBSTR("1"));
	
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("securityToken"),CComBSTR("securityToken"),CComVariant(82249079));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("c"),CComBSTR("c"),CComVariant("US"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("sFlags"),CComBSTR("sFlags"),CComBSTR("1"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("rfc822Mailbox"),CComBSTR("rfc822Mailbox"),bstrWorldGUID);
	//hr=m_pWorldAttribs->AddAttrib(CComBSTR("surName"),CComBSTR("surName"),CComBSTR("Surname"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("givenName"),CComBSTR("givenName"),bstrWorldURL);
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("protocolMimeType"),CComBSTR("protocolMimeType"),CComBSTR("text/t120"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("mimeType"),CComBSTR("mimeType"),CComBSTR("text/iuls"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("smodop"),CComBSTR("smodop"),CComBSTR("3"));
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("ipAddress"),CComBSTR("ipAddress"),CComBSTR("1848260765"));

	//important ones below.
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("Guid"),CComBSTR("guid"),bstrWorldGUID, TRUE);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=m_pWorldAttribs->AddAttrib(CComBSTR("location"),CComBSTR("address"),bstrWorldURL);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr=m_pWorldAttribs->AddAttrib(CComBSTR("comment"),CComBSTR("users"),bstrWorldNoUsers);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=m_pWorldAttribs->AddAttrib(CComBSTR("surName"),CComBSTR("surName"), bstrInfoURL);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=m_pWorldAttribs->UploadObjectToDS(CComBSTR("RTPerson"), bstrWorldGUID);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
 	return hr;
}

STDMETHODIMP CDServ::RemoveWorld(BSTR WorldGUID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Delete user.
	return m_pWorldAttribs->DeleteObjectFromDS(CComBSTR("DynamicObject"),
					   CComBSTR("CN="+CString(WorldGUID)));
}


STDMETHODIMP CDServ::LogUserOn(BSTR InWorldAlias)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr=S_OK;

/*	m_UserAttribs.AddAttrib(CComBSTR("customAttribute1"),CComBSTR(""),CComVariant("InWorld"));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Write it back to the DS.
	hr=m_UserAttribs.UploadChanges();
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

 */	return hr;
}


STDMETHODIMP CDServ::LogUserOff(BSTR InWorldAlias)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT hr=S_OK;

/*	m_UserAttribs.AddAttrib(CComBSTR("customAttribute1"),CComBSTR(""),CComVariant("OutWorld"));
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Write it back to the DS.
	hr=m_UserAttribs.UploadChanges();
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
*/
 	return hr;

}

STDMETHODIMP CDServ::FindUser(BSTR inworldalias, BSTR * emailname, BSTR * computername, BSTR * comment, BSTR *bstrCurrentWorld)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr=S_OK;
	CComBSTR email,computer,strcomment,strstate;	

/*	hr=m_UserAttribs.DownloadObjectFromDS(CComBSTR("RTPerson"), inworldalias);
	if(FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}
	
	//TRACE(m_UserAttribs.Dump());

	hr=m_UserAttribs.FindAttrib(CComBSTR("E-mail-Addresses"),email);
	if(FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}

	hr=m_UserAttribs.FindAttrib(CComBSTR("Home-Directory"),computer);
	if(FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}
	
	hr=m_UserAttribs.FindAttrib(CComBSTR("comment"),strcomment);
	if(FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}

	hr=m_UserAttribs.FindAttrib(CComBSTR("customAttribute1"),strstate);
	if(FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	
	*emailname=email.Copy();
	*computername=computer.Copy();
	*comment=strcomment.Copy();	
	*bstrCurrentWorld=strstate.Copy();

#ifdef _DEBUG
	afxDump<<email<<","<<computer<<","<<strcomment<<"\r\n";
#endif
*/	return hr;
}


//////////////////////////////////////////////////////////////////////////////
//Utility fcns.
//////////////////////////////////////////////////////////////////////////////

void ShowError(CString errmsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

#ifdef _DEBUG	
	::MessageBox(NULL,errmsg,"VWorld Directory Service Module",MB_OK);
	afxDump<<"Directory Service module error: "+errmsg+"\r\n";
#endif
}

STDMETHODIMP CDServ::GetStatus(BSTR * bstrMsg, VARIANT_BOOL *pbIsDSConnected)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString output;	

	output=m_pWorldAttribs->dump(pbIsDSConnected);

	*bstrMsg=output.AllocSysString();

	if(m_pWorldAttribs->IsConnected())
		*pbIsDSConnected = VARIANT_TRUE;
	else 
		*pbIsDSConnected = VARIANT_FALSE;

	return S_OK;
}
