/////////////////////////////////////////////////////////////////////////////
//
// VWGeomA.cpp : Implementation of CVWGeomApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <d3drm.h>
#include <vwgeom.h>
#include <vwgcache.h>
//#include "..\vwframe\VWFrameO.h"
#include <inetfile.h>
#include <atlbase.h>
#include <VWGeomAr.h>
#include "jointobj.h"
#include "VWGeomA.h"
#include "charobj.h"
#include "PuppetOb.h"
#include <string.h>
#include <afx.h>

#define D2R(X) (3.14159*(X)/180.)
#define INIT_SAFERETURN HRESULT hr = S_OK;
#define SAFERETURN(hRes) \
if FAILED(hr = (hRes)) \
{ \
	goto ERROR_ENCOUNTERED; \
}
#define FORCE_SAFERETURN goto ERROR_ENCOUNTERED;
#define BEGIN_SAFERETURN ERROR_ENCOUNTERED:
#define END_SAFERETURN return hr;

void SetJointPositionD3D(IJoint *ij, DegreesOfFreedom DOF, double s[], int order[]);
//void (*CJoint::pfUpdateJoint)(IJoint *, int, double *);

CComBSTR CVWGeometryArticulated::m_bstrVWFrame("VWSYSTEM.Frame.1");

IMPLEMENT_OBJECTPROPERTY(CVWGeometryArticulated)

// VWGeomOb.cpp : Implementation of CVWGeomApp and DLL registration.

/////////////////////////////////////////////////////////////////////////////
//
CVWGeometryArticulated::CVWGeometryArticulated()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometryArticulated::CVWGeometryArticulated, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	top_frame = NULL;
	geom_frame = NULL;
	m_pVWGeometryCache = NULL;
	m_bOccluded = FALSE;
	character = NULL;
	m_bstrGeometryName = ::SysAllocString(BSTR(""));

	//Set the update function. 
	CJoint::pfUpdateJoint = SetJointPositionD3D;

	curID = -1;
}

CVWGeometryArticulated::~CVWGeometryArticulated()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometryArticulated::~CVWGeometryArticulated, this 0x%0x\n", this);
#endif
	Terminate();
}

/*
  Return the character object to which this articulated goemetry object points.
  */
STDMETHODIMP CVWGeometryArticulated::get_Character(ICharacter **ch){
	*ch = character;
	if (character) character->AddRef();
	return S_OK;
}

/*
  Set the character object
  */
STDMETHODIMP CVWGeometryArticulated::put_Character(ICharacter *ch){
	if (character!=NULL) character->Release();
	character = ch;
	character->AddRef();
	return S_OK;
}

/*

  This constructs the hierarchy of frames and joints.

  For now, hard code them in. Maybe create a helper function for this.
  We also need to add a machanism to associate mesh files with joints.
  The best way is probably to add the VWFrames immediately in a helper.

  The single argument, xfile, gives the name of the .x file which defines the
  entire character skeleton and geometry.
*/
STDMETHODIMP CVWGeometryArticulated::Load(BSTR xFileName){

	CStdioFile xFile;
	CComObject<CPuppet> *pup;
	char buf[1024],*pbuf;
	char word[200];
	CString		str;
	HRESULT hr;
	BSTR		bstrFinalURL = NULL, bstrFinalPath = NULL;
	IInternetFileManager	*pInternetFileManager = NULL;
	static CComBSTR bstrInetfile("Inetfile");

	CComObject<CCharacter> *ch;
	CComObject<CCharacter>::CreateInstance(&ch);
	if (character) {
		character->Destroy();
		character->Release();
	}
	character = ch;
	character->AddRef();

	if (FAILED(hr=m_pWorld->get_Tool(bstrInetfile, (IUnknown **) &pInternetFileManager)))
		goto Cleanup_ParseFile;
 	if (FAILED(hr = pInternetFileManager->GetFileEx(xFileName, VARIANT_FALSE, &bstrFinalURL, &bstrFinalPath)))
	{
		m_pWorld->Report(CComBSTR(CString("Failed to download file: ") + CString(xFileName)), -1);
		goto Cleanup_ParseFile;
	}

	pbuf = &buf[0];
	if (!xFile.Open(CString(bstrFinalPath), CFile::modeRead | CFile::typeText)) return E_FAIL;

	// Find the HIERARCHY keyword
	while (xFile.ReadString(str) && str != "HIERARCHY");
	if (str != "HIERARCHY") {
		hr = E_FAIL;
		goto Cleanup_ParseFile;
	}

	//reset the joint ID's ....
	curID=-1;

	xFile.ReadString(str);
	pbuf = str.GetBuffer( 1024);
	sscanf(pbuf, "%s", word);
	if (strcmp(word, "ROOT")==0) hr = ParseJoint(pbuf, &xFile, -1);
	else return E_FAIL;

	if (FAILED(hr)) return hr;

	//This puppet stuff can definitely be moved into script.
	xFile.ReadString(str);
	pbuf = str.GetBuffer( 1024);
	sscanf(pbuf, "%s", word);
	if (strcmp(word, "PUPPET")==0) {		//add further behavior types
		BSTR bstrMotionFile;
		if (FAILED(pInternetFileManager->GetFileEx(CComBSTR(&pbuf[7]), VARIANT_FALSE, &bstrMotionFile, &bstrFinalPath)))
		{
			m_pWorld->Report(CComBSTR(CString("Failed to download file: ") + CString(&pbuf[7])), -1);
			goto Cleanup_ParseFile;
		}
		CComObject<CPuppet>::CreateInstance(&pup);
		pup->AddRef();
		character->put_Behavior((IBehavior *)pup);
		pup->AddMotion(bstrFinalPath);
		pup->Release();
	}

Cleanup_ParseFile:
	SAFEFREESTRING(bstrFinalURL);
	if (bstrFinalPath)
		SysFreeString(bstrFinalPath);
	SAFERELEASE(pInternetFileManager);
	if (!SUCCEEDED(hr)) {
		character->Destroy();
		character->Release();
		character=NULL;
	}
	
	return hr;
}

STDMETHODIMP CVWGeometryArticulated::ParseJoint(char *firstline, CStdioFile *xFile, int parent) {
	char buf[1024], *pbuf, jointName[50], jointFileName[50], word[50];
	float off1, off2, off3;
	int count, num, charCount;
	CString		str;
	int currentID;
	bool worked;
	int dof[6];

	pbuf = &buf[0];

	curID++;

	//first line
	sscanf(firstline, "%s %s %s", word, jointName, jointFileName);

	//open '{'
	xFile->ReadString(str);
	pbuf = str.GetBuffer(1024);
	sscanf(pbuf, "%1s", word);
	if (strcmp(word, "{")!=0) return E_FAIL;

	//offset
	xFile->ReadString(str);
	pbuf = str.GetBuffer(1024);
	count = sscanf(pbuf, "%s %f %f %f", word, &off1, &off2, &off3);
	if (count==0) return E_FAIL;
	if (count < 4 || strcmp(word, "OFFSET")!=0) return E_FAIL;

	//DOFS
	xFile->ReadString(str);
	charCount =0;
	pbuf = str.GetBuffer(1024);
	count = sscanf(pbuf, "%s %d%n", word, &num, &charCount);	//num = numDOFs
	if (count < 2) return E_FAIL;
	if (strcmp(word, "CHANNELS")!=0) return E_FAIL;
	//Loop for reading in DOFs
	int c =0;
	int numlines=0;
	int count2;
	while (c < num && numlines < num + 1) {
		count = sscanf(&pbuf[charCount], "%s%n", word, &count2);
		if (count<1) {		//goto the next line
			xFile->ReadString(str);
			pbuf = str.GetBuffer(1024);
			charCount = 0;
			numlines++;
		}
		else {		//found the DOF correctly --> add it in
			charCount+= count2;
			if (strcmp(word, "Xposition")==0) dof[c] = 1;
			else if (strcmp(word, "Yposition")==0) dof[c] = 2;
			else if (strcmp(word, "Zposition")==0) dof[c] = 4;
			else if (strcmp(word, "Xrotation")==0) dof[c] = 8;
			else if (strcmp(word, "Yrotation")==0) dof[c] = 16;
			else if (strcmp(word, "Zrotation")==0) dof[c] = 32;
			else return E_FAIL;
			c++;
		}
	}
	if (c!=num) return E_FAIL;

	//we can create the joint finally!
	//make a call to initjoint.
	currentID = curID;
	worked = InitJoint(currentID, dof, num, parent, CComBSTR(jointFileName), off1, off2, off3);

	if (!worked) return E_FAIL;

	//Child joints or close '}'
	while(true) {
		xFile->ReadString(str);
		pbuf = str.GetBuffer(1024);
		if (sscanf(pbuf, "%1s", word)==0) return E_FAIL;
		if (strcmp(word, "}")==0) {		//end
			return S_OK;
		}
		else {
			if (sscanf(pbuf, "%s", word)==0) return E_FAIL;
			if (strcmp(word, "JOINT")==0) {		//recurse
				ParseJoint(pbuf, xFile, currentID);
			}
			else return E_FAIL;
		}
	}
}

/*

  This is a helper for the joint construction. It takes care of creating the frame, associating
  it with the joint, loading the geometry (given a filename), setting up hierarchical relations
  (e.g. links back to parents) etc.

  */
bool CVWGeometryArticulated::InitJoint(int id, int dof[], int numDOF, int parent, CComBSTR filename, 
				double poX, double poY, double poZ) {
	IJoint *jparent;
	HRESULT hr;
	bool retval = true;

	CComObject<CJoint> *ij;

	CComObject<CJoint>::CreateInstance(&ij);
	ij->AddRef();
	ij->SetID(id);
	if (parent>=0) {
		character->GetJoint(parent, &jparent);
		if (jparent!=NULL) jparent->AddChild(ij);
		else return false;
	}
	else {
		jparent=NULL;
		character->AddJoint(ij);
	}
	ij->SetParentOffset(poX, poY, poZ);
	for (int c=0; c < numDOF; c++) {
		ij->AddDOF(dof[c]);
	}

//--------Code to load the geometry and associate it with the appData of the joint-----------

	//We need to create a new transform node for each 
	IVWFrame *frame, *parentFrame, *geomFrame;

	m_pWorld->CreateObjectProperty(CVWGeometryArticulated::m_bstrVWFrame, (IObjectProperty**)&frame);

	//Construct hierarchy relations between this frame and parent.
	if (parent > -1 && jparent!=NULL) {	//this could be the root node.
		jparent->GetAppData((void**)&parentFrame);
		parentFrame->AddChild(frame);
		if (filename.Length() != 0) {
			hr = m_pWorld->CreateObjectProperty(CVWGeometryArticulated::m_bstrVWFrame, (IObjectProperty**)&geomFrame);
			hr = m_pVWGeometryCache->LoadGeometry(filename, &geomFrame);
			parentFrame->AddChild(geomFrame);
			geomFrame->Release();
		}
	}
	else frame->AddRef();

	ij->SetAppData((void *)frame);

//Set the d3d frame here
	double s[6];
	s[0]=poX;
	s[1]=poY;
	s[2]=poZ;
	s[3]=0.0;
	s[4]=0.0;
	s[5]=0.0;

	SetJointPositionD3D(ij, 63, s, dof);

	ij->Release();
	if (jparent!=NULL) jparent->Release();
	return retval;
}

STDMETHODIMP CVWGeometryArticulated::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometryArticulated::Terminate, this 0x%0x\n", this);
#endif

	CVWGeometryArticulatedBase::Terminate();

	SAFERELEASE(m_pVWGeometryCache);

	return S_OK;
}

/*
This method puts together the necessary T-node geometry-node hierarchy, through calls to load
  */
STDMETHODIMP CVWGeometryArticulated::ComposeGeometry(IVWFrame **ppvwf) {
	INIT_SAFERETURN
	IThing	*pThing = NULL;
	IJoint *jtemp;
	IVWFrame *ftemp;
	static CComBSTR bstrGeometryCache("VWGeometryCache");

	if (!m_pVWGeometryCache) {
		SAFERETURN(m_pWorld->get_Tool(bstrGeometryCache, (IUnknown**)&m_pVWGeometryCache));
	}

	//We need a handle on the D3D device we're going to be using.
	IDirect3DRM *m_pDirect3DRM;
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&m_pDirect3DRM)))
		goto ERROR_ENCOUNTERED;

	if (top_frame){			// Use existing 'T' node.
		*ppvwf = top_frame;
		top_frame->AddRef();
	}
	else {							// Create an empty 'T' node.
		//This is the top-level (empty) node
		SAFERETURN(m_pWorld->CreateObjectProperty(CVWGeometryArticulated::m_bstrVWFrame, (IObjectProperty**)&top_frame));
		SAFERETURN(m_pWorld->CreateObjectProperty(CVWGeometryArticulated::m_bstrVWFrame, (IObjectProperty**)&geom_frame));
		top_frame->AddChild(geom_frame);
		top_frame->put_GeometryObject((IVWGeometry*)this);

		float x, y, z, xu, yu, zu;
		geom_frame->GetOrientationEx(NULL,&x, &y, &z, &xu, &yu, &zu);
		geom_frame->SetOrientationEx(NULL,-x, y, -z, xu, yu, zu);

		//Q: Load the entire geometry structure! Where do we get the filename for the model?
		//A: It might have been set previously by a put_geometryname call. If not, we'll return, after
		//		simply creating the empty top-level frame. Oh, I guess we could also assign the 
		//		visual to it while we're at it.
		int xl = m_bstrGeometryName.Length();
		BSTR btemp = (BSTR)m_bstrGeometryName;
		if ((BSTR)m_bstrGeometryName != NULL && xl != 0) {
			hr = Load((BSTR)m_bstrGeometryName);
			if (SUCCEEDED(hr) && SUCCEEDED(character->GetRoot(&jtemp))) {
				hr = jtemp->GetAppData((void **)&ftemp);
				if (SUCCEEDED(hr)) hr = geom_frame->AddChild(ftemp);
				jtemp->Release();
			}
		}

		if (m_pSecurity)
		{
			m_pSecurity->get_Instance(&pThing);
			top_frame->put_Thing(pThing);
		}

		//Add the callback stuff -- this only needs to be done once, since the visual is
		//associated with the top-level frame, which sticks around even if we change 
		//the geometry.
		//Note thst the callback stuff is executed even if no filename was available at the
		//time.
		IDirect3DRMUserVisual *prlUvis;
		IDirect3DRMFrame *prlFrame;
		geom_frame->get_Frame3D(&prlFrame);
		ASSERT(prlFrame);
		//CHANGE! This is assuming that the behavior is a CPuppet.
		m_pDirect3DRM->CreateUserVisual(CVWGeometryArticulated::UpdateCallBack, (void*)this, &prlUvis);
		prlFrame->AddVisual(prlUvis);
		m_pDirect3DRM->Release();
		prlFrame->Release();

		//I'm not sure how this UserVisual will ever be released. I'll have to think about it.

		if (!SUCCEEDED(hr)) goto ERROR_ENCOUNTERED;
		top_frame->AddRef();
		*ppvwf = top_frame;
	}
ERROR_ENCOUNTERED:
//	SAFERELEASE(pThing);
	return hr;
}

/*
This method is called by the render thread to update the position of the character model.
  */
int CVWGeometryArticulated::UpdateCallBack(LPDIRECT3DRMUSERVISUAL uvis,
									  void* arg,
									  D3DRMUSERVISUALREASON reason,
									  LPDIRECT3DRMDEVICE dev,
									  LPDIRECT3DRMVIEWPORT view)
{
	//I think all we'll do is to call the update time method of the behavior, if we have one.
	ICharacter *character;
	IBehavior *behavior;
	//the arg is a CVWGeometryArticulated
	((CVWGeometryArticulated *)arg)->get_Character(&character);
	if (character) {
		character->get_Behavior(&behavior);
		if (behavior) {
			behavior->Update(arg);
			behavior->Release();
		}
		character->Release();
	}
	return D3D_OK;
}

/*
We'll use this as the call to destroy the joint hierarchy. It will need to compose geometry
again before we can use it again.
  */
STDMETHODIMP CVWGeometryArticulated::ReleaseGeometry()
{
	if (!top_frame) return E_FAIL;

	if (character) {
		character->Destroy();
		character->Release();
		character=NULL;
	}

	// Use stack variables since this object may be deleted when we remove the VWFrame's
	// reference to it.
	IVWFrame *pvwfTransform = top_frame, *pvwfCachedGeometry = geom_frame;
	top_frame = NULL;
	geom_frame = NULL;

	pvwfTransform->put_GeometryObject(NULL);
	SAFERELEASE(pvwfTransform);
	SAFERELEASE(pvwfCachedGeometry);

	return S_OK;
}

STDMETHODIMP CVWGeometryArticulated::put_GeometryName(BSTR bstr)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWGeometryObject::put_GeometryName\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVWGeometryObject::put_GeometryName", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWGeometryObject::put_GeometryName: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		// Actually set the geomtry
		m_bstrGeometryName = bstr;

		IJoint *jtemp;
		IVWFrame *ftemp;

		if (top_frame)
		{
			//get rid of the existing character geometry.

			if (character) {
				character->GetRoot(&jtemp);
				jtemp->GetAppData((void**)&ftemp);
				geom_frame->DeleteChild(ftemp);
				jtemp->Release();
				character->Destroy();
				character->Release();
				character=NULL;
			}

			if ((BSTR)m_bstrGeometryName != NULL && m_bstrGeometryName.Length() != 0)
			{
				// Load the named geometry.
				hr = Load(bstr);
				if (!SUCCEEDED(hr)) goto ERROR_ENCOUNTERED;

				if (SUCCEEDED(character->GetRoot(&jtemp))) {
					hr = jtemp->GetAppData((void **)&ftemp);
					if (SUCCEEDED(hr)) {
						ftemp->SetPositionEx(geom_frame, 0.0f,0.0f,0.0f);
						hr = geom_frame->AddChild(ftemp);
					}
					jtemp->Release();
				}
			}
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstr;

		hr = SendObjectPropertyCommand(psbits, DISPID_GEOMETRY_GEOMETRYNAME, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			if (FAILED(hr = m_pParent->NotifyObjectPropertyChanged(this, glPutGeometryName, CComVariant(bstr))))
				goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWGeometryArticulated::get_GeometryName(BSTR* pbstr)
{
	*pbstr = m_bstrGeometryName.Copy();
	return S_OK;
}

STDMETHODIMP CVWGeometryArticulated::put_MotionFile(BSTR bstr) {
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWGeometryObject::put_MotionFile\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVWGeometryObject::put_MotionFile", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWGeometryObject::put_MotionFile: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		// Actually set the geomtry
		motionFile = bstr;

		IBehavior *b;
		IPuppet *p;
		if (character) {
			hr = character->get_Behavior(&b);
			if (SUCCEEDED(hr)) {
				if (b==NULL) {
					hr = CreateBehavior(CComBSTR("VWSYSTEM.Puppet.1"), (IBehavior**)&p);
					p->AddMotion(bstr);
				}
				else {
					hr = b->QueryInterface(IID_IPuppet, (void**)&p);
					if (SUCCEEDED(hr)) {
						p->AddMotion(bstr);
						p->Release();
					}
					b->Release();
				}
			}
		}

	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstr;

		// REVIEW: Just removed for testing.
//		hr = SendObjectPropertyCommand(psbits, DISPID_GEOMETRYARTICULATED_MOTIONFILE, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			if (FAILED(hr = m_pParent->NotifyObjectPropertyChanged(this, glPutGeometryName, CComVariant(bstr))))
				goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;

}

STDMETHODIMP CVWGeometryArticulated::get_MotionFile(BSTR* pbstr) {
	*pbstr = motionFile.Copy();
	return S_OK;
}


STDMETHODIMP CVWGeometryArticulated::get_Frame(IVWFrame **ppVWFrame)
{
	*ppVWFrame = top_frame;
	SAFEADDREF(*ppVWFrame);
	return S_OK;
}

STDMETHODIMP CVWGeometryArticulated::get_GeometryFrame(IVWFrame **ppVWFrame)
{
	*ppVWFrame = geom_frame;
	SAFEADDREF(*ppVWFrame);
	return S_OK;
}

/*
Not relevant for articulated geometry.
  */
STDMETHODIMP CVWGeometryArticulated::put_Frame(IVWFrame *pVWFrame)
{
	IThing *pThing = NULL;

	top_frame->put_GeometryObject(NULL);
	SAFERELEASE(top_frame);
	top_frame = pVWFrame;
	SAFEADDREF(pVWFrame);
	top_frame->put_GeometryObject((IVWGeometry*)this);
	if (m_pSecurity)
	{
		m_pSecurity->get_Instance(&pThing);
		top_frame->put_Thing(pThing);
	}
	SAFERELEASE(pThing);
	return S_OK;
}

/*
	I'm not sure if we need this, so I'll erase it for now.
  */
STDMETHODIMP CVWGeometryArticulated::put_Occluded(VARIANT_BOOL bOccluded)
{
	return S_OK;
}

STDMETHODIMP CVWGeometryArticulated::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);

	if (FAILED(hr = CVWGeometryArticulatedBase::Marshall(dwOptions, pbuffer )))
		return hr;

	hr = pbuffer->put_String(m_bstrGeometryName);

	return hr;
}

STDMETHODIMP CVWGeometryArticulated::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	// free any alloced data
	m_bstrGeometryName.Empty();
	if (top_frame)
		top_frame->put_GeometryObject(NULL);
	SAFERELEASE(top_frame);
	SAFERELEASE(geom_frame);

	ASSERT(pbuffer);

	if (FAILED(hr = CVWGeometryArticulatedBase::UnMarshall( pbuffer )))
		return hr;

	hr = pbuffer->get_String(&m_bstrGeometryName.m_str);
	if (FAILED(hr))
		return hr;

	return hr;
}

STDMETHODIMP CVWGeometryArticulated::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	CString strOut;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;
											
	strOut.Format(	"\tSet %s = World.CreateGeometry(\"%s\")\n",
					(LPCTSTR) CString(bstrContext),
					(LPCTSTR) CString(m_bstrGeometryName));

	*pbstrOut = strOut.AllocSysString();
	*pbAsVal = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CVWGeometryArticulated::CreateBehavior(BSTR progID, IBehavior **behavior) {

	CLSID clsid;
	HRESULT hr;

	if (!SUCCEEDED(hr = CLSIDFromProgID(CComBSTR(progID), &clsid))) return hr;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IBehavior, (void **)behavior);
	return hr;	
}


/*
	GESTURE STUFF
  */


STDMETHODIMP CVWGeometryArticulated::GetNamedGestures(IPropertyList **ppGestureNameList) {
	return E_NOTIMPL;
}

STDMETHODIMP CVWGeometryArticulated::put_CurrentGesture(int iGesture) {
	IBehavior *b;
	IPuppet *p;
	HRESULT hr;
	if (character) {
		character->get_Behavior(&b);
		if (b) {	//we need to query for the interface...
			hr = b->QueryInterface(IID_IPuppet, (void**)&p);
			if (SUCCEEDED(hr)) {
				hr = p->put_Animation(iGesture);
				p->Start();
				p->Release();
				return hr;
			}
			b->Release();
		}
	}
	return E_FAIL;
}

STDMETHODIMP CVWGeometryArticulated::get_CurrentGesture(int* piGesture) {
	IBehavior *b;
	IPuppet *p;
	HRESULT hr;
	if (character) {
		character->get_Behavior(&b);
		if (b) {	//we need to query for the interface...
			hr = b->QueryInterface(IID_IPuppet, (void**)&p);
			if (SUCCEEDED(hr)) {
				hr = p->get_Animation(piGesture);
				p->Release();
				return hr;
			}
			b->Release();
		}
	}
	return E_FAIL;
}

STDMETHODIMP CVWGeometryArticulated::get_NamedGesture(BSTR bstrGesture, int *pGestureNum)
{
	IBehavior *b;
	IPuppet *p;
	HRESULT hr;
	if (character) {
		character->get_Behavior(&b);
		if (b) {	//we need to query for the interface...
			hr = b->QueryInterface(IID_IPuppet, (void**)&p);
			if (SUCCEEDED(hr)) {
				hr = p->GetGestureIndex(bstrGesture, pGestureNum);
				p->Release();
				return hr;
			}
			b->Release();
		}
	}
	return E_FAIL;
}

STDMETHODIMP CVWGeometryArticulated::get_GestureText(BSTR bstrGesture, BSTR *pbstrText)
{
	IBehavior *b;
	IPuppet *p;
	int pnum;
	HRESULT hr;
	if (character) {
		character->get_Behavior(&b);
		if (b) {	//we need to query for the interface...
			hr = b->QueryInterface(IID_IPuppet, (void**)&p);
			if (SUCCEEDED(hr)) {
				hr = p->GetGestureIndex(bstrGesture, &pnum);
				if (SUCCEEDED(hr)) p->GetGestureText(pnum, pbstrText);
				p->Release();
				return hr;
			}
			b->Release();
		}
	}
	return E_FAIL;

}

//This is not implemented for articulated geometry
/*
STDMETHODIMP CVWGeometryArticulated::get_ThumbnailName(BSTR* pbstrThumbnailName) {
	*pbstrThumbnailName = ::SysAllocString(BSTR(""));
	return S_OK;
}

*/
/////////////////////////////////////////////////////////////////////////////
//// end implementation of vwgeom
////////////////////////////////////////////////////////////////////////////


void SetJointPositionD3D(IJoint *ij, DegreesOfFreedom DOF, double s[], int order[])
{
	IVWFrame *ftemp = NULL;
	IDirect3DRMFrame *pd3drmf = NULL;

	ij->GetAppData((void**)&ftemp);
	ftemp->get_Frame3D(&pd3drmf);
	if (pd3drmf==NULL) return;

	pd3drmf->AddTranslation(D3DRMCOMBINE_REPLACE, (float)s[0], (float)s[1], (float)s[2]);

	for (int c=0; c < 6; c++) {
		if (order[c]==0) goto cleanup;
		if (order[c]==8) pd3drmf->AddRotation(D3DRMCOMBINE_BEFORE, 1.0f, 0.0f, 0.0f, (float)D2R(-s[3]));
		else if (order[c]==16) pd3drmf->AddRotation(D3DRMCOMBINE_BEFORE, 0.0f, 1.0f, 0.0f, (float)D2R(-s[4]));
		else if (order[c]==32) pd3drmf->AddRotation(D3DRMCOMBINE_BEFORE, 0.0f, 0.0f, 1.0f, (float)D2R(s[5]));
	}

cleanup:
	SAFERELEASE(pd3drmf);
}
