/////////////////////////////////////////////////////////////////////////////
//
// charobj.cpp : Implementation of Character object and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <d3drm.h>
#include <vwgeom.h>
#include <vwgcache.h>
#include <inetfile.h>
#include <VWGeomAr.h>
#include "VWGeomA.h"
#include "vwgeomar.h"
#include "jointobj.h"
#include "afx.h"
#include "afxtempl.h"
#include "charobj.h"



CCharacter::CCharacter() {
	root=NULL;
	behavior = NULL;
}

CCharacter::~CCharacter() {
	//Do nothing for now
}

STDMETHODIMP CCharacter::put_Name(BSTR *bstrName) {
	name = bstrName;
	return S_OK;
}

STDMETHODIMP CCharacter::get_Name(BSTR **bstrName) {
	*bstrName = name;
	return S_OK;
}

STDMETHODIMP CCharacter::JointNameToID(BSTR bstrName, long *jointID) {
	return E_NOTIMPL;
}

STDMETHODIMP CCharacter::ParameterNameToID(BSTR bstrName, long *jointID) {
	return E_NOTIMPL;
}

/*
Recurses down the joint hierarchy looking for the joint with ID jointID.
  */
STDMETHODIMP CCharacter::GetJoint(long jointID, IJoint **ij) {
	HRESULT hr;
	if (root==NULL) return E_FAIL;
	hr = root->GetChild(jointID, (IJoint **)ij);
	if (FAILED(hr)) *ij = NULL;
	return hr;
}

/*
  For now, this simply sets the root. Children to the root can be added directly.
*/
STDMETHODIMP CCharacter::AddJoint(IJoint *ij) {
	root = ij;
	root->AddRef();
	return S_OK;
}

STDMETHODIMP CCharacter::JointCount(int *num) {
	int x;
	if (root==NULL) {
		*num=0;
		return S_OK;
	}
	root->ChildCount(&x);
	*num = x+1;
	return S_OK;
}

STDMETHODIMP CCharacter::DeleteJoint(long jointID) {
	//Now, why would you want to go and delete a joint, eh?
	return E_NOTIMPL;
}

STDMETHODIMP CCharacter::PutGuid(long guid) {
	GUID = guid;
	return S_OK;
}

STDMETHODIMP CCharacter::GetGUID(long *guid) {
	*guid = GUID;
	return S_OK;
}

STDMETHODIMP CCharacter::GetNumDOFs(int *num) {
	IJoint *ij;
	int total =0;
	int temp;
	long c = 0;
	while(true) {
		if (SUCCEEDED(GetJoint(c, &ij))) {
			ij->GetNumDOFs(&temp);
			total+=temp;
		}
		else {
			*num = total;
			return S_OK;
		}
		c++;
	}
}

//--------------------------My additions---------------------------

/*
  Returns the root of the joint hierarchy.
  */
STDMETHODIMP CCharacter::GetRoot(IJoint **ij) {
	*ij = root;
	root->AddRef();
	return S_OK;
}

STDMETHODIMP CCharacter::get_Behavior(IBehavior **behave) {
	*behave = behavior;
	if (behavior) behavior->AddRef();
	return S_OK;
}

STDMETHODIMP CCharacter::put_Behavior(IBehavior *behave) {
	if (behave) behave->AddRef();
	if (behavior) {
		behavior->put_Character(NULL);
		behavior->Release();
	}
	behavior = behave;
	behavior->put_Character(this);
	return S_OK;
}

STDMETHODIMP CCharacter::Destroy() {
	if (root) {
		root->Destroy();
		root->Release();
	}
	if (behavior) {
		behavior->put_Character(NULL);
		behavior->Release();
	}
	return S_OK;
}
