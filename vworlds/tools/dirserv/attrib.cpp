// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <activeds.h>
#include <DirServ.h>
#include "DServ.h"
#include "winmsgs.h"
#include <comdef.h>

const IID IID_IADs = {0xfd8256d0,0xfd15,0x11ce,{0xab,0xc4,0x02,0x60,0x8c,0x9e,0x75,0x53}};
const IID IID_IADsContainer = {0x001677d0,0xfd16,0x11ce,{0xab,0xc4,0x02,0x60,0x8c,0x9e,0x75,0x53}};

//////////////////////////////////////////////////////////////////////////////
//CAttrib map stuff.
//////////////////////////////////////////////////////////////////////////////
CString CDSAttribMap::Dump_T2()
{
	CString output;

#ifdef _DEBUG	
	output+="Number attribs: "+str(m_attribs.GetCount())+"\r\n";
	POSITION pos;
	
	for (pos=GetStartPosition(); pos!=NULL; )
	{
		CComBSTR strKey;
		CAttrib pvValue;

		GetNextAssoc(pos, strKey, pvValue);

		output+=CString("Key: ")+CString(strKey);
		if(pvValue.m_varValue.vt==VT_BSTR)
		{
			output+=", Value: "+CString(pvValue.m_varValue.bstrVal);
		}
		output+=CString("\r\n");
	}
#endif	
	return output;
}

void CDSAttribMap::RemoveAll_T2()
{
	m_attribs.RemoveAll();
}

void CDSAttribMap::RemoveAll()
{
	DSRequest req;
	req.setEvent(RequestRemoveAll);
	
	m_queue.put(req);
}

HRESULT CDSAttribMap::FindAttrib(BSTR attrib, CAttrib &outattrib)
{
	HRESULT hr=S_OK;
	DSRequest req;	

	req.FindAttrib(attrib);

	DSRequest returnvalue=m_queue.putSync(req);

	//Wait until 2nd thread is done. Then go on.
	
	if(FAILED(returnvalue.getHR()))
		goto ERROR_ENCOUNTERED;		
	
	outattrib=returnvalue.returnedAttrib();

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CDSAttribMap::FindAttrib_T2(BSTR attrib, CAttrib &outattrib)
{
	if (m_attribs.Lookup(attrib, outattrib))
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT CDSAttribMap::FindAttrib(BSTR attribdescr,CComBSTR & out)
{
	HRESULT hr=S_OK;
	DSRequest req;
	CAttrib attrib;	

	req.FindAttrib(attribdescr);

	DSRequest returnvalue=m_queue.putSync(req);

	//Wait until 2nd thread is done. Then go on.
	
	if(FAILED(returnvalue.getHR()))
		goto ERROR_ENCOUNTERED;		
	
	attrib=returnvalue.returnedAttrib();

	if(attrib.m_varValue.vt==VT_BSTR)
	{
		out=attrib.m_varValue.bstrVal;		
		#ifdef _DEBUG
			afxDump<<"FindAttrib: ("<<attribdescr<<") "<<attrib.m_varValue.bstrVal<<"\r\n";
		#endif
	}
	else
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;			
	}
	
ERROR_ENCOUNTERED:
	return hr;	
}

HRESULT CDSAttribMap::AddAttrib(BSTR dsattrib, BSTR VWorldAttrib, BSTR Value, BOOL sens)
{
	return AddAttrib(dsattrib,VWorldAttrib,CComVariant(Value),sens);
}

//the core fcn.
HRESULT CDSAttribMap::AddAttrib(BSTR dsattrib, CAttrib attrib)
{
	DSRequest req;
	req.AddAttrib(dsattrib,attrib);

	m_queue.put(req);
	return S_OK;
}

HRESULT CDSAttribMap::AddAttrib_T2(BSTR dsattrib, CAttrib InAttrib)
{
	HRESULT hr=S_OK;
	HRESULT hr2=S_OK;

	CAttrib attrib;
	hr2=FindAttrib_T2(dsattrib,attrib);
	if(SUCCEEDED(hr2))
	{
		// [km]: removed TRACE since it can't be filtered out
		#if 0 // def _DEBUG
			afxDump<<"Key: "<<dsattrib<<". ";
			if(attrib.m_varValue.vt==VT_BSTR)
				afxDump<<"Value: "<<attrib.m_varValue.bstrVal<<".\r\n";
		#endif

		if(attrib.m_bIsSensitive && !InAttrib.m_bIsSensitive) return E_FAIL;
	}
	
	m_attribs.SetAt(dsattrib,InAttrib);	

	if(m_pObject)
	{
		HRESULT hr;
		if(!InAttrib.m_bIsSensitive)
		{
			hr=m_pObject->Put(dsattrib,InAttrib.m_varValue);
			if(FAILED(hr))
				goto ERROR_ENCOUNTERED;		
		}
		else
		{
			hr=E_FAIL;
			goto ERROR_ENCOUNTERED;
		}			
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CDSAttribMap::AddAttrib(BSTR dsattrib, BSTR VWorldAttrib, VARIANT Value,
							BOOL sens)
{
 	CAttrib attrib;
	
	attrib.m_bstrVWorldAttrib=VWorldAttrib;
	attrib.m_varValue=Value;
	attrib.m_bIsSensitive=sens;

	DSRequest req;
	req.AddAttrib(dsattrib,attrib);

	m_queue.put(req);

	return S_OK;
}

HRESULT CDSAttribMap::ConnectToDS(BSTR ContainerPath,BSTR Administrator,
		BSTR AdminPassword,HWND hNotificationWindow)
{
	m_hNotificationWindow=hNotificationWindow;
	DSRequest req;
	req.setEvent(RequestConnect);
	req.Connect(ContainerPath,Administrator,AdminPassword);
	m_queue.put(req);
	
	return S_OK;
}

HRESULT CDSAttribMap::ConnectToDS_T2(BSTR ContainerPath,BSTR Administrator,
		BSTR AdminPassword)
{
	HRESULT hr=S_OK;

	if(CString(ContainerPath).GetLength()==0)
	{
		m_StateOfDS = DSEmptyURL;
		return S_OK;
	}

	if(m_pContainer) return E_FAIL;
	if(m_pObject) return E_FAIL;

	m_bstrContainerPath=ContainerPath;
	m_bstrAdministrator=Administrator;
	m_bstrAdminPassword=AdminPassword;

	// Bind to the known container.
	ASSERT(m_pADsGetObject);

	if(!m_pADsGetObject) return E_FAIL;

	hr = (*m_pADsGetObject)(
		m_bstrContainerPath,
		IID_IADsContainer,
		(void **)&m_pContainer
    );

/*	hr=(*m_pOpenObjectFcn)(m_bstrContainerPath,NULL,NULL,
		0,	//Flags
		IID_IADsContainer,	//IID of primary interface
		(void**)&m_pContainer	//Interface on that object
		);
	
	hr=(*m_pOpenObjectFcn)(m_bstrContainerPath,m_bstrAdministrator,m_bstrAdminPassword,
		0,	//Flags
		IID_IADsContainer,	//IID of primary interface
		(void**)&m_pContainer	//Interface on that object
		);
*/
	if(FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

	if(m_hNotificationWindow)
		//ASSERT
		(::PostMessage(m_hNotificationWindow,WM_CONNECTED_TO_DS,NULL,NULL));

ERROR_ENCOUNTERED:

	if(FAILED(hr))
	{
		m_StateOfDS = DSFailedConnect;
	}
	else
	{
		m_StateOfDS = DSConnected;
	}


	return hr;
}

CDSAttribMap::CDSAttribMap()
{
	DSRequest req;

	CWinThread *pThread = NULL;
	m_pContainer=NULL;
	m_pObject=NULL;
	m_hNotificationWindow=NULL;
	m_bIsCurrentlyEnumerating=FALSE;

	m_iRefCount = 1;

	//maybe should FreeLibrary at some point. doesn't matter much, though (auto-freed anyway). [ch].	
	m_hDSModule=LoadLibrary("activeds.dll");
	if(m_hDSModule==NULL) 
	{
//		AfxMessageBox("The system can't find activeds.dll. We recommend you install it.");	
		m_StateOfDS = ADSINotInstalled;
		goto ERROR_ENCOUNTERED;
	}
	else
	{
		m_pOpenObjectFcn=(ADsOpenObjectTYPE)GetProcAddress(m_hDSModule,"ADsOpenObject"); 
		if(m_pOpenObjectFcn==NULL)
		{
			m_StateOfDS = ADSINotInstalled;
//			AfxMessageBox("The system can't find the fcn ADsOpenObject in activeds.dll. We recommend you install the current version.");
			goto ERROR_ENCOUNTERED;
		}

		m_pADsBuildEnumerator=(ADsBuildEnumeratorTYPE)GetProcAddress(m_hDSModule,"ADsBuildEnumerator"); 
		if(m_pADsBuildEnumerator==NULL)
		{
			m_StateOfDS = ADSIMissingFunctions;
//			AfxMessageBox("The system can't find the fcn m_pADsBuildEnumerator in activeds.dll. We recommend you install the current version.");
			goto ERROR_ENCOUNTERED;
		}

		m_pADsEnumerateNext=(ADsEnumerateNextTYPE)GetProcAddress(m_hDSModule,"ADsEnumerateNext"); 
		if(m_pADsEnumerateNext==NULL)
		{
			m_StateOfDS = ADSIMissingFunctions;
//			AfxMessageBox("The system can't find the fcn ADsEnumerateNext in activeds.dll. We recommend you install the current version.");
			goto ERROR_ENCOUNTERED;
		}

		m_pADsFreeEnumerator=(ADsFreeEnumeratorTYPE)GetProcAddress(m_hDSModule,"ADsFreeEnumerator"); 
		if(m_pADsFreeEnumerator==NULL)
		{
			m_StateOfDS = ADSIMissingFunctions;
//			AfxMessageBox("The system can't find the fcn ADsFreeEnumerator in activeds.dll. We recommend you install the current version.");
			goto ERROR_ENCOUNTERED;
		}
		
		m_pADsGetObject=(ADsGetObjectTYPE)GetProcAddress(m_hDSModule,"ADsGetObject"); 
		if(m_pADsGetObject==NULL)
		{
			m_StateOfDS = ADSIMissingFunctions;
//			AfxMessageBox("The system can't find the fcn ADsGetObject in activeds.dll. We recommend you install the current version.");
			goto ERROR_ENCOUNTERED;
		}

		m_pADsBuildVarArrayStr=(ADsBuildVarArrayStrTYPE)GetProcAddress(m_hDSModule,"ADsBuildVarArrayStr"); 
		if(m_pADsBuildVarArrayStr==NULL)
		{
			m_StateOfDS = ADSIMissingFunctions;
//			AfxMessageBox("The system can't find the fcn ADsGetObject in activeds.dll. We recommend you install the current version.");
			goto ERROR_ENCOUNTERED;
		}

		
		
	}

	m_StateOfDS = DSBeforeConnected;

	// if everything works, create this thread. Otherwise, 
	// all api calls should fizzle, but not crash!
	
	// start thread to handle communications
	pThread = AfxBeginThread( CDSAttribMap::BackgroundThread, this, THREAD_PRIORITY_NORMAL );

	ASSERT(pThread);

	m_queue.SetWorkerThread(pThread->m_nThreadID, pThread->m_hThread);

	// pause this thread to wait for other thread to initialize.
//	req.InitWorkerThread();
//	m_queue.putSync(req, TRUE);

ERROR_ENCOUNTERED:
	//noop.
	;
}

void CDSAttribMap::Terminate(void)
{
	//ask commthread to shut down.
	DSRequest req(DSShutDownCommThread);
	
	TRACE("CDSAttribMap: In Terminate.\n");

	//pass it this request.
	m_queue.putSync(req, FALSE);
	
	//Wait until 2nd thread is done. Then go on.
	
	//The Commthread is gone now.
	m_pthreadComm=NULL;
}

CDSAttribMap::~CDSAttribMap()
{
}

CString CDSAttribMap::dump(VARIANT_BOOL *pbIsConnected) 
{
	switch(m_StateOfDS)
	{
		case DSBeforeConnected:
			*pbIsConnected = VARIANT_TRUE;
			return "Attempting to connect to Directory Server...\r\n";//delete this for now. +m_queue.dump();
		case DSConnected:
			ASSERT(m_pContainer);
			if(m_pContainer)
			{
				*pbIsConnected = VARIANT_TRUE;
				return "Connected to Directory Server.\r\n";//delete this for now. +m_queue.dump();
			}
		case DSFailedConnect:
			*pbIsConnected = VARIANT_FALSE;
			return "Not connected. Invalid Directory Server address.\r\n";//delete this for now.+m_queue.dump();
		case DSEmptyURL:
			*pbIsConnected = VARIANT_FALSE;
			return "Not connected. Directory Server address not entered.\r\n";//delete this for now.+m_queue.dump();
		case ADSINotInstalled:
			*pbIsConnected = VARIANT_FALSE;
			return "Not connected. Active Directory Services are not installed.\r\n";//delete this for now.+m_queue.dump();
		case ADSIMissingFunctions:
			*pbIsConnected = VARIANT_FALSE;
			return "Not connected. Activeds.dll is missing functions. Wrong version.\r\n";//delete this for now.+m_queue.dump();
		default:
			ASSERT(0);
			return "";
	}
}

HRESULT CDSAttribMap::Disconnect()
{	
	DSRequest req(RequestDisconnect);
	
	//pass it this request.
	m_queue.put(req);
	
	return S_OK;
}

HRESULT CDSAttribMap::Disconnect_T2()
{
	SAFERELEASE(m_pContainer);
	SAFERELEASE(m_pObject);

	m_bstrContainerPath=CComBSTR("");
	m_bstrAdministrator=CComBSTR("");
	m_bstrAdminPassword=CComBSTR("");

	m_bstrObjectClass=CComBSTR("");
	m_bstrObjectKey=CComBSTR("");

	m_attribs.RemoveAll();

	return S_OK;
}

HRESULT CDSAttribMap::UploadChanges()
{
	DSRequest req(RequestUploadChanges);

	//pass it this request.
	m_queue.put(req);
	
	return S_OK;
}

HRESULT CDSAttribMap::UploadChanges_T2()
{
	return m_pObject->SetInfo();
}

HRESULT CDSAttribMap::UploadObjectToDS(BSTR objectClass, 
								BSTR objectKey)
{
	DSRequest req;
	req.UploadObjectToDS(objectClass, objectKey);

	//pass it this request.
	m_queue.put(req);
	
	return S_OK;
}

HRESULT CDSAttribMap::MakeObjectDynamic(IADs *pObject)
{
	SAFEARRAY *pArray = NULL;
	HRESULT hr = S_OK;    
/*  
	[T.O.]: Replacing this stuff with the ADsBuildVarArrayStr
			wrapper calls because it doesn't work on Win2k any more
			[filed bug with win2k folks]

	SAFEARRAYBOUND rgsabound[1];    
	long index[1];
	VARIANT var;

	CComVariant varMember("RTPerson");
	CComVariant varDynamic("DynamicObject");

	
	::VariantInit(&var);

	rgsabound[0].lLbound = 0;
    rgsabound[0].cElements = 2;

    pArray = SafeArrayCreate(VT_VARIANT, 1, rgsabound);    
	if(pArray == NULL){
        hr = E_OUTOFMEMORY;        
		ASSERT(0);
		goto ERROR_ENCOUNTERED;
	}

	index[0]=0;
	hr = SafeArrayPutElement(pArray, index, &varMember);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
    
	index[0]=1;
	hr = SafeArrayPutElement(pArray, index, &varDynamic);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	var.vt = VT_ARRAY | VT_BSTR;
	var.parray = pArray;
	hr = pObject->Put(CComBSTR("ObjectClass"), var);
*/
	if(!m_pADsBuildVarArrayStr) return E_FAIL;

	// Build a Variant of array type, using the specified string array.
	LPOLESTR pwszArray[] = {L"RTPerson",L"DynamicObject"};
	DWORD dwArrayItems = sizeof(pwszArray)/sizeof(LPOLESTR);
	VARIANT vArray;
	VariantInit(&vArray);
	hr = (*m_pADsBuildVarArrayStr)(pwszArray, dwArrayItems, &vArray);

	hr = pObject->Put(CComBSTR("ObjectClass"), vArray);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
//	if(pArray) SafeArrayDestroy(pArray);
	VariantClear(&vArray);
	
	return hr;
}

HRESULT CDSAttribMap::UploadObjectToDS_T2(BSTR objectClass, 
								BSTR objectKey)
{
	HRESULT hr=S_OK;
	POSITION pos;
	CComBSTR strKey;
	CAttrib attrib;
	IADs *pObject=NULL;
	IADs *pNewObject=NULL;
	
	IDispatch *pDisp=NULL;
	IDispatch *pNewDisp=NULL;


	if(!m_pContainer) return E_FAIL;

	if(m_pObject && 
		objectClass==m_bstrObjectClass &&
		objectKey==m_bstrObjectKey)
	{

		hr=UploadChanges_T2();
	}
	else
	{
		hr=FindObjectFromDS_T2(objectClass,objectKey,&pObject);
		if(FAILED(hr))
		{
			hr=m_pContainer->Create(objectClass,
				CComBSTR("CN="+CString(objectKey)), &pDisp);				   				
			if(FAILED(hr))
				goto ERROR_ENCOUNTERED;
	
			hr=pDisp->QueryInterface(IID_IADs,(void **)&pObject);
			if(FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// change class.
			hr = MakeObjectDynamic(pObject);
			if(FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			// Set mandatory properties.
			for (pos=GetStartPosition(); pos!=NULL; )
			{
				GetNextAssoc(pos, strKey, attrib);

// do this no matter what now: if(attrib.m_bIsSensitive)
				{
					#if 0 // def _DEBUG
						afxDump<<"Key:"<<strKey<<".";
						if(attrib.m_varValue.vt==VT_BSTR)
						{
							afxDump<<"Value(string):"<<attrib.m_varValue.bstrVal<<".\r\n";
						}
					#endif
					hr=pObject->Put(strKey,attrib.m_varValue);			
					if(FAILED(hr))
						goto ERROR_ENCOUNTERED;		
				}
			}

			// Write it back to the DS. (Create the object in the DS).
			hr=pObject->SetInfo();
			if(FAILED(hr))
				goto ERROR_ENCOUNTERED;

		}
		
		m_bstrObjectClass=objectClass;
		m_bstrObjectKey=objectKey;

		//workaround.
/*		hr=m_pContainer->GetObject(objectClass,
						   objectKey,
						   &pNewDisp);
		if(FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr=pNewDisp->QueryInterface(IID_IADs,(void **)&pNewObject);
		if(FAILED(hr))
			goto ERROR_ENCOUNTERED;

		//hr=pNewObject->GetInfo();

		// Set other properties.
		for (pos=GetStartPosition(); pos!=NULL; )
		{
			GetNextAssoc(pos, strKey, attrib);

			if(!attrib.m_bIsSensitive)
			{
				//Print the params.
			#if 0 // def _DEBUG
				afxDump<<"Key:"<<strKey<<".";
				if(attrib.m_varValue.vt==VT_BSTR)
				{
					afxDump<<"Value(string):"<<attrib.m_varValue.bstrVal<<".\r\n";
				}
			#endif
						
				//DS can't set empty strings.
				if(attrib.m_varValue.vt==VT_BSTR && (
					CComBSTR(attrib.m_varValue.bstrVal).Length()==0))
				{
					hr=pNewObject->PutEx(ADS_PROPERTY_CLEAR,strKey,attrib.m_varValue);			
					//if(FAILED(hr))
					//	goto ERROR_ENCOUNTERED;		
					//E_ADS_PROPERTY_NOT_FOUND might happen but is okay..
				}
				else
				{
					hr=pNewObject->Put(strKey,attrib.m_varValue);			
					//if(FAILED(hr))
					//	goto ERROR_ENCOUNTERED;		
				}
				// Write it back to the DS. 
				// DEBUG (inefficient). Be tolerant of mistakes here.
				//hr=
				hr=pNewObject->SetInfo();
				//if(FAILED(hr))
				//	goto ERROR_ENCOUNTERED;
			}
			
		}
		
		//if it got all the way here, ignore errors.
		hr=S_OK;

		m_pObject=pNewObject;
		SAFEADDREF(pNewObject);
*/	}
	 
ERROR_ENCOUNTERED:
	SAFERELEASE(pObject);
	SAFERELEASE(pNewObject);
	SAFERELEASE(pDisp);
	SAFERELEASE(pNewDisp);
	
 	return hr;
}

HRESULT CDSAttribMap::DownloadObjectFromDS(BSTR objectClass, BSTR objectKey)
{
	DSRequest req;
	req.DownloadObjectFromDS(objectClass, objectKey);

	//pass it this request.
	DSRequest returnvalue=m_queue.putSync(req);
	
	return returnvalue.getHR();
}

HRESULT CDSAttribMap::DownloadObjectFromDS_T2(IADs *pObject, AttributeMap *pOutputMap)
{
	POSITION pos;
	CComBSTR strKey;
	CAttrib attrib;
	HRESULT hr=S_OK,hr2=S_OK;
	
	ASSERT(pOutputMap);

	// Get properties.
	for (pos=GetStartPosition(); pos!=NULL; )
	{
		GetNextAssoc(pos, strKey, attrib);

		TRACE("Property: "+CString(strKey)+"\r\n");
		hr2=pObject->Get(strKey,&attrib.m_varValue);
		if(FAILED(hr2))
			attrib.m_varValue=CComVariant("Property not set.");
		//change the value.
		pOutputMap->SetAt(strKey,attrib); 
	}

//ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CDSAttribMap::DownloadObjectFromDS_T2(BSTR objectClass, BSTR objectKey)
{
	HRESULT hr=S_OK,hr2=S_OK;
	
	IADs *pObject=NULL;
	IDispatch *pDisp=NULL;

	if(!m_pContainer) return E_FAIL;
	if(m_pObject) 
	{
		SAFERELEASE(m_pObject);		
	}

	m_bstrObjectClass=objectClass;
	m_bstrObjectKey=objectKey;

	hr=m_pContainer->GetObject(objectClass,
					   objectKey,
					   &pDisp);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr=pDisp->QueryInterface(IID_IADs,(void **)&pObject);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=DownloadObjectFromDS_T2(pObject,&m_attribs);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	m_pObject=pObject;
	SAFEADDREF(m_pObject);
	
ERROR_ENCOUNTERED:
	SAFERELEASE(pObject);
	SAFERELEASE(pDisp);

 	return hr;
}

//no side effects.
HRESULT CDSAttribMap::FindObjectFromDS_T2(BSTR objectClass, BSTR objectKey, IADs **ppObject)
{
	HRESULT hr=S_OK;
	IDispatch *pDisp=NULL;

	if(!m_pContainer) return E_FAIL;

	hr=m_pContainer->GetObject(objectClass,
		   objectKey,(IDispatch**)&pDisp);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr=pDisp->QueryInterface(IID_IADs,(void **)ppObject);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pDisp);

 	return hr;

}

//no side effects.
HRESULT CDSAttribMap::DeleteObjectFromDS(BSTR objectClass, BSTR objectKey)
{
	HRESULT hr=S_OK;
	DSRequest req;

	req.DeleteObjectFromDS(objectClass,objectKey);

	m_queue.put(req);

//ERROR_ENCOUNTERED:
	return hr;
}

//no side effects.
HRESULT CDSAttribMap::DeleteObjectFromDS_T2(BSTR objectClass, BSTR objectKey)
{
	if(!m_pContainer) return E_FAIL;
	
	
	return m_pContainer->Delete(objectClass,
		   objectKey);
}

HRESULT CDSAttribMap::ShutDown_T2()
{
	Disconnect_T2();

	return S_OK;
}

// main communications loop.
UINT CDSAttribMap::BackgroundThread( void *pmap )
{
	HRESULT hr=S_OK;
	
	//the this ptr not accessible from here..
	CDSAttribMap *pCDSAttribMap= (CDSAttribMap *) pmap;	
	pCDSAttribMap->AddRef();

	if (FAILED(CoInitialize(NULL)))
	{
		TRACE("CoInitialize in comm thread failed\n");
		ASSERT(0);
		SAFERELEASE(pCDSAttribMap);
		return 1;
	}

	// let request m_queue know who to notify
	pCDSAttribMap->m_queue.SetWorkerThread();

	// Create the tools
	for(;;)
	{
		// block on an event, waiting for action
		pCDSAttribMap->m_queue.WaitUntilNotEmpty();

		// handle all m_queue messages
		for (;;)
		{	
			DSRequest req=pCDSAttribMap->m_queue.get();

			// if empty, break out and wait again
			if (req.getEvent() == DSEMPTY)
				break;

			switch (req.getEvent())
			{
				case DSSyncInitCommThread:
					{
						//Main thread will wait until the following line completes.
						DSRequest returnValue;
						
						//set the return's hr.
						returnValue.setHR(S_OK);

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pCDSAttribMap->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case DSShutDownCommThread:
					{
						TRACE("DSShutDownCommThread: Received shutdown comm thread request.\n");
						
						//Clean up this thread and all objects in it.
						hr=pCDSAttribMap->ShutDown_T2();
						
						//Main thread should wait until the following line completes.
						DSRequest returnValue;

						returnValue.setHR(hr);
						
						//sets required internal variables.
						returnValue.putSync(req);						
						
						pCDSAttribMap->m_queue.FireSignalToPutSync(returnValue);

						pCDSAttribMap->m_queue.Terminate();

						SAFERELEASE(pCDSAttribMap);
						//Quit COMMThread.
						return 0;
						break;
					}
				case DSWindowsMessage:
					{
						// dispatch the message.
						::DispatchMessage(&(req.getMSG()));
						break;
					}
				case RequestConnect:
					{
						BSTR a,b,c;
						req.getConnect(&a,&b,&c);
						
						hr=pCDSAttribMap->ConnectToDS_T2(a,b,c);

						SAFEFREESTRING(a);
						SAFEFREESTRING(b);
						SAFEFREESTRING(c);
						break;
					}
				case RequestRemoveAll:
					{
						pCDSAttribMap->RemoveAll_T2();

						break;
					}
				case RequestDisconnect:
					{
						hr=pCDSAttribMap->Disconnect_T2();

						break;
					}
				case RequestUploadChanges:
					{
						hr=pCDSAttribMap->UploadChanges_T2();
						break;
					}
				case RequestUpload:
					{
						BSTR a,b;
					
						req.getUploadObjectToDS(&a,&b);
						hr=pCDSAttribMap->UploadObjectToDS_T2(a,b);
							
						SAFEFREESTRING(a);
						SAFEFREESTRING(b);
						break;
					}
				case RequestDownload:
					{
						BSTR a,b;
					
						req.getDownloadObjectFromDS(&a,&b);
						hr=pCDSAttribMap->DownloadObjectFromDS_T2(a,b);
							
						DSRequest returnValue;

						returnValue.setHR(hr);
						//sets required internal variables.
						returnValue.putSync(req);						

						SAFEFREESTRING(a);
						SAFEFREESTRING(b);

						pCDSAttribMap->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case RequestFindAttrib:
					{
						BSTR a;
						DSRequest returnValue;
						CAttrib attrib;

						req.getFindAttrib(&a);
						returnValue.setHR(pCDSAttribMap->FindAttrib_T2(a,attrib));
						returnValue.setReturnedAttrib(attrib);
						//sets required internal variables.
						returnValue.putSync(req);						

						SAFEFREESTRING(a);					
						
						pCDSAttribMap->m_queue.FireSignalToPutSync(returnValue);
						break;
					}
				case RequestDeleteObject:
					{
						BSTR a,b;
					
						req.getDeleteObjectFromDS(&a,&b);
						hr=pCDSAttribMap->DeleteObjectFromDS_T2(a,b);
							
						SAFEFREESTRING(a);
						SAFEFREESTRING(b);
						break;
					}
				case RequestAddAttrib:
					{
						BSTR descr;
						CAttrib attrib;
						req.getAddAttrib(&descr,attrib);

						hr=pCDSAttribMap->AddAttrib_T2(descr,attrib);
						
						SAFEFREESTRING(descr);
						break;
					}
				case RequestTestQueue:
					{
						DSRequest returnValue;
						HRESULT hr2;
					
						hr2=req.getTestQueue();
						returnValue.setHR(hr2);
						//sets required internal variables.
						returnValue.putSync(req);						

						Sleep(2000);
						pCDSAttribMap->m_queue.FireSignalToPutSync(returnValue);
						break;
					}
				case RequestEnumerate:
					{
						hr=pCDSAttribMap->EnumerateChildren_T2();
						break;
					}
				default:
					{
						TRACE("ERROR: Background DS thread terminating\n");
						ASSERT(FALSE);
						SAFERELEASE(pCDSAttribMap);
						return 0;
					}
			}
			if(FAILED(hr))
			{
				#ifdef DEBUG_DLTHREAD
				TRACE("The request <"+req.dump()+"> failed.\n");
				hr=S_OK;
				#endif
			}
			
		}
	}
}

HRESULT CDSAttribMap::EnumerateChildren(HWND hNotificationWindow)
{
	m_hNotificationWindow=hNotificationWindow;

	DSRequest req;
	req.Enumerate();

	//pass in this request.
	m_queue.put(req);
	
	return S_OK;
}

HRESULT CDSAttribMap::EnumerateChildren_T2()
{
	if(!m_pContainer || !m_hNotificationWindow) return E_FAIL;

	IEnumVARIANT *pEnum=NULL;
	BSTR bstrMemberName=NULL;
	HRESULT hr=S_OK;
	ULONG ulItemsRetrieved=1;	
	VARIANT varContents[1];	
	IADs *pCurrItem=NULL;
	CString temp;

	m_Contents.RemoveAll();

	VariantInit(&varContents[0]);

	// Get an enumerator.  ADsBuildEnumerator is a helper
	// function supplied by ADSI (not linked in).
	hr = m_pADsBuildEnumerator(m_pContainer,&pEnum);
	if(FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

	//
	// Enumerate through all the children of the container.
	// ADsEnumerateNext is a helper function supplied by ADSI.
	// See Appendix B for more details.
	//
	while (SUCCEEDED(hr) && ulItemsRetrieved==1)
	{
		hr = m_pADsEnumerateNext(pEnum,
							 1,
							 varContents,
							 &ulItemsRetrieved);
		if (SUCCEEDED(hr) && ulItemsRetrieved==1)
		{
			ASSERT(varContents[0].vt==VT_DISPATCH);
			
			hr = varContents[0].pdispVal->QueryInterface( IID_IADs, (void**)&pCurrItem);
			if(FAILED(hr)) 
				goto ERROR_ENCOUNTERED;
			
			//Release dispatch.
			VariantClear( &varContents[0] );
			
			//Deal /w pCurrItem. Extract data, put it into queue.
			{
				AttributeMap *pMap=new AttributeMap;
				hr = pCurrItem->get_Name(&bstrMemberName);
				if(FAILED(hr)) 
					goto ERROR_ENCOUNTERED_2;
				
#ifdef _DEBUG
				afxDump<<"DS Item: "<<bstrMemberName<<"\r\n";
#endif
				
				//String format: "CS = mynamehere".
				temp=CString(bstrMemberName);

				hr = DownloadObjectFromDS_T2(pCurrItem, pMap);
				if(FAILED(hr)) 
					goto ERROR_ENCOUNTERED_2;
				
				m_Contents.put(pMap);
				
				pMap=NULL;
				
				if(!m_bIsCurrentlyEnumerating)
					//ASSERT
					(::PostMessage(m_hNotificationWindow,WM_NEW_ITEMS_PENDING,NULL,NULL));

ERROR_ENCOUNTERED_2:
				if(FAILED(hr) && pMap) delete pMap;
			}

			SAFEFREESTRING(bstrMemberName);
			SAFERELEASE(pCurrItem);
		}
	}

	//ASSERT
	(::PostMessage(m_hNotificationWindow,WM_NO_NEW_ITEMS_PENDING,NULL,NULL));

	//no more items left is okay.
	hr=S_OK;

ERROR_ENCOUNTERED:
	
	m_pADsFreeEnumerator(pEnum);
	SAFEFREESTRING(bstrMemberName);
	SAFERELEASE(pCurrItem);
	VariantClear( &varContents[0] );

	return hr;
}

//////////////////////////////////////////////////////////////////////////////
//END: CAttrib map stuff.
//////////////////////////////////////////////////////////////////////////////
