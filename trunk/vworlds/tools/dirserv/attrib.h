// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef ATTRIBHEADER
#define ATTRIBHEADER

#include <propbase.h>
#include<Afxmt.h>
#include "dqueue.h"
#include "..\..\vwcommon\global.h"

// V-Worlds optimized templates
#include <vwtempl.h>

//display msgbox.
void ShowError(CString errmsg);

typedef HRESULT (CALLBACK * ADsOpenObjectTYPE)( LPWSTR lpszPathName,	//The ADsPath name of the object
    LPWSTR lpszUserName,	//User name to use for credentials
    LPWSTR lpszPassword,	//Password to use for credentials
    DWORD dwReserved,	//Flags
    REFIID riid,	//IID of primary interface
    VOID FAR * FAR *pp);

typedef HRESULT (CALLBACK * ADsBuildEnumeratorTYPE)(
    IADsContainer *pADsContainer,
    IEnumVARIANT   **ppEnumVariant
    );

typedef HRESULT (CALLBACK * ADsFreeEnumeratorTYPE)(
    IEnumVARIANT *pEnumVariant
    );

typedef HRESULT (CALLBACK * ADsEnumerateNextTYPE)(
    IEnumVARIANT *pEnumVariant,
    ULONG         cElements,
    VARIANT FAR  *pvar,
    ULONG FAR    *pcElementsFetched
    );

//From the header adshlp.h:
typedef HRESULT (CALLBACK * ADsGetObjectTYPE)(
    LPWSTR lpszPathName,
    REFIID riid,
    VOID * * ppObject
    );
 
typedef HRESULT (CALLBACK * ADsBuildVarArrayStrTYPE)(
	LPWSTR* lppPathNames,
	DWORD dwPathNames,
	VARIANT* pVar
	);

typedef CVWMap<CBSTRAdapt<CComBSTR>,BSTR,CAttrib,CAttrib&> AttributeMap;

//Directory service states.
enum DSStatesType
{
	ADSINotInstalled = -2,
	ADSIMissingFunctions = -1,
	DSBeforeConnected = 0,
	DSConnected = 1,
	DSFailedConnect = 2,
	DSEmptyURL = 3
};

//Queue of people (and their attributes). Used for enumeration storage.
class CAttribMapQ : public CList< AttributeMap *, AttributeMap *>
{ 
public:
	CAttribMapQ() {InitializeCriticalSection(&m_critsec);}
	~CAttribMapQ() {
		EnterCriticalSection(&m_critsec);
			AttributeMap *pmap;				
			for (POSITION pos=GetHeadPosition(); pos!=NULL; )
			{
				pmap = GetNext(pos);

				if (pmap) 
				{
					delete pmap;
					pmap = NULL;
				}
			}
		LeaveCriticalSection(&m_critsec);
		DeleteCriticalSection(&m_critsec);
	}

    // Go to the end of the line
    void put( AttributeMap * newPerson )
	{ 
		EnterCriticalSection(&m_critsec);
			AddTail( newPerson ); 
		LeaveCriticalSection(&m_critsec);
	}        

    // Get first element in line
    AttributeMap* get()
	{ 
		AttributeMap *temp;
		EnterCriticalSection(&m_critsec);
			temp = IsEmpty() ? NULL : RemoveHead(); 
		LeaveCriticalSection(&m_critsec);
		
		return temp;
	}

private:
	//Guard access for m_Contents.
	CRITICAL_SECTION m_critsec;
}; 

//This reusable and general class marshalls a property map to/from an ADSI provider, whose
//server backends can range from ILS to NT5. The dirserv object uses this
//collection class for all the real work.
class CDSAttribMap 
{
public:
	//The following exposed functions can be called from any thread. They
	//forward all the real work to a background worker thread.

	CDSAttribMap();

	void Terminate(void);

	void AddRef(void) 
	{
		m_critsec.Lock();
			m_iRefCount++;
		m_critsec.Unlock();
	};
	void Release() 
	{
		m_critsec.Lock();
			--m_iRefCount;
			if(!m_iRefCount) 
			{
				m_critsec.Unlock();
				delete this;
				return;
			}
		m_critsec.Unlock();
	}

	CString dump(VARIANT_BOOL *pbIsConnected);

	//normal collection stuff. 
	//These fcns all return S_OK always unless otherwise noted.
	HRESULT RemoveAttrib(BSTR attrib);

	//an attribute is sensitive if it can only be modified on
	//the construction of the object on the DS. (things like the index)
	HRESULT AddAttrib(BSTR dsattrib, CAttrib attrib);
	HRESULT AddAttrib(BSTR dsattrib, BSTR, VARIANT,BOOL sens=FALSE);
	HRESULT AddAttrib(BSTR dsattrib, BSTR attrib,BSTR, BOOL sens=FALSE);

	//This is a synchronous operation. The HRESULT can be E_FAIL.
	//Returns when the queue catches up.
	HRESULT FindAttrib(BSTR attrib, CAttrib &outattrib);
	HRESULT FindAttrib(BSTR attrib,CComBSTR & out);

	void		RemoveAll();
	
	void testQueue() {m_queue.testQueue();}

	//Special DS stuff.=====================================================

	BOOL IsConnected(void) {if(m_pContainer) return TRUE; else return FALSE;}

	//call first.
	HRESULT ConnectToDS(BSTR m_bstrContainerPath,BSTR m_bstrAdministrator,
		BSTR m_bstrAdminPassword,HWND hNotificationWindow);

	//call last.
	HRESULT Disconnect();

	//no side effects.
	HRESULT DeleteObjectFromDS(BSTR objectClass, BSTR objectKey);

	//HRESULT FindObjectFromDS(BSTR objectClass, BSTR objectKey, IADs **ppObject);

	//These associate the map /w the directory. 
		//Pass class (eg. "user"), key (eg. "myname"), and it'll marshall 
		//this object into DS. It retains this association.
		HRESULT UploadObjectToDS(BSTR objectClass, BSTR objectKey);

		//pass class (eg. "user"), key (eg. "myname"), and it'll marshall 
		//this object from DS to local. It retains this association.	
		HRESULT DownloadObjectFromDS(BSTR objectClass, BSTR objectKey);
	
		//after these calls, local prop changes are cached 
		//and can be uploaded with the following:
		HRESULT UploadChanges();

		//Starts downloading users async.
		HRESULT EnumerateChildren(HWND hNotificationWindow);

		HRESULT GetNextChild(AttributeMap **pmap) {
			*pmap=m_Contents.get(); 
			if(*pmap==NULL) m_bIsCurrentlyEnumerating=FALSE;
			else m_bIsCurrentlyEnumerating=TRUE;		
			return S_OK;
		}

		BOOL IsEnumerating() {return m_bIsCurrentlyEnumerating;}

		HRESULT MakeObjectDynamic(IADs *pObject);

private:
	~CDSAttribMap();

	//helper fcns.========================
	
	//LONG		GetCount() {return m_attribs.GetCount();}
	int m_iRefCount;

	//Traversal.
	POSITION	GetStartPosition() {return m_attribs.GetStartPosition();}
	void GetNextAssoc( POSITION& rNextPosition, CComBSTR& rKey, CAttrib & rValue )  {m_attribs.GetNextAssoc(rNextPosition,(CBSTRAdapt<CComBSTR>&)rKey,rValue);}


	// Background threadhandler thread
	static UINT BackgroundThread( void *pCDSAttribMap );
	
	Constructor_T2();

	//call first.
	HRESULT ConnectToDS_T2(BSTR m_bstrContainerPath,BSTR m_bstrAdministrator,
		BSTR m_bstrAdminPassword);

	//call last.
	HRESULT Disconnect_T2();

	//no side effects.
	HRESULT DeleteObjectFromDS_T2(BSTR objectClass, BSTR objectKey);

	HRESULT FindObjectFromDS_T2(BSTR objectClass, BSTR objectKey, IADs **ppObject);

	//These associate the map /w the directory. 
		//Pass class (eg. "user"), key (eg. "myname"), and it'll marshall 
		//this object into DS. It retains this association.
		HRESULT UploadObjectToDS_T2(BSTR objectClass, BSTR objectKey);

		//pass class (eg. "user"), key (eg. "myname"), and it'll marshall 
		//this object from DS to local. It retains this association.	
		HRESULT DownloadObjectFromDS_T2(BSTR objectClass, BSTR objectKey);
		
		HRESULT DownloadObjectFromDS_T2(IADs *pobject,AttributeMap *pOutputMap);
	
		//after these calls, local prop changes are cached 
		//and can be uploaded with the following:
		HRESULT UploadChanges_T2();

		HRESULT EnumerateChildren_T2();

	//Background thread destructor.
	HRESULT ShutDown_T2();

	HRESULT FindAttrib_T2(BSTR attrib,CAttrib &);

	HRESULT RemoveAttrib_T2(BSTR attrib);

	HRESULT AddAttrib_T2(BSTR dsattrib, CAttrib attrib);

	void		RemoveAll_T2();

	CString	Dump_T2();

	//Data ================================================
	CVWMap<CBSTRAdapt<CComBSTR>,BSTR,CAttrib,CAttrib&> m_attribs;

	IADsContainer *m_pContainer;
	IADs *m_pObject;

	//ADSI DLL.
	HINSTANCE m_hDSModule;

	//ADSI Helper fcns.
	ADsOpenObjectTYPE m_pOpenObjectFcn;
	ADsBuildEnumeratorTYPE m_pADsBuildEnumerator;
	ADsFreeEnumeratorTYPE m_pADsFreeEnumerator;
	ADsEnumerateNextTYPE m_pADsEnumerateNext;
	ADsGetObjectTYPE m_pADsGetObject;
	ADsBuildVarArrayStrTYPE m_pADsBuildVarArrayStr;

	//container stats.
	CComBSTR m_bstrContainerPath;	
	CComBSTR m_bstrAdministrator;
	CComBSTR m_bstrAdminPassword;

	//object stats.
	CComBSTR m_bstrObjectClass;
	CComBSTR m_bstrObjectKey;

	DSRequestQueue m_queue;

	// background thread
	CWinThread *m_pthreadComm;

	//Enumeration Data.
	CAttribMapQ m_Contents;

	HWND m_hNotificationWindow;

	BOOL m_bIsCurrentlyEnumerating;

	DSStatesType m_StateOfDS;

	CCriticalSection m_critsec;

};

#endif