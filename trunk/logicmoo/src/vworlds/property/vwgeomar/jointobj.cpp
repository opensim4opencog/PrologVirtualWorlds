/////////////////////////////////////////////////////////////////////////////
//
// jointobj.cpp : Implementation of Joint object and DLL registration.


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

void (*CJoint::pfUpdateJoint)(IJoint *, int, double *, int *);

CJoint::CJoint() {
	dof = 0;
	for (int c=0; c < 6; c++) {
		order[c] = 0;
		state[c] = 0.0;
	}

	ID  = 0;   //Does the argument make sense?

	numChildren =0;
	AppData = NULL;
    pParent = NULL;
	children = new JointArray;

	numDOFs = 0;
}

CJoint::~CJoint() {
	delete children;
}

STDMETHODIMP CJoint::SetID(int newID) {
	ID = newID;
	return S_OK;
}

STDMETHODIMP CJoint::GetID(int *curID) {
	*curID = ID;
	return S_OK;
}

STDMETHODIMP CJoint::GetPosition(DegreesOfFreedom DOF, VARIANT *val) {
	if (!CheckDOF(DOF)) return E_FAIL;
	val->dblVal = state[DOFToIndex(DOF)];
	return S_OK;
}

/*
Set the position of the given DOF and update the geometry to reflect that change.
  */
STDMETHODIMP CJoint::SetPosition(DegreesOfFreedom DOF, double position) {
	HRESULT hr;
	hr = SetDOF(DOF, position);
	if (SUCCEEDED(hr)) {
		pfUpdateJoint((IJoint *)this, dof, state, order);
	}
	return hr;
}

/*
Set the position of the DOF (and DO NOT update the geometry
  */
STDMETHODIMP CJoint::SetDOF(DegreesOfFreedom DOF, double position) {
	if (!CheckDOF(DOF)) return E_FAIL;
	if (DOF & dof) {
		int x = DOFToIndex(DOF);
		state[x] = position;
		return S_OK;
	}
	else return E_FAIL;		//that's not a degree of freedom on this joint
}

STDMETHODIMP CJoint::SetState(double xt, double yt, double zt, double xr, double yr, double zr) {
	state[0] = xt;
	state[1] = yt;
	state[2] = zt;
	state[3] = xr;
	state[4] = yr;
	state[5] = zr;

	return S_OK;
}

STDMETHODIMP CJoint::SetDOFLimit(DegreesOfFreedom DOF, double min, double max) {
	if (!CheckDOF(DOF)) return E_FAIL;
	// Not implemented yet, but just return a success.
	return S_OK;
}

STDMETHODIMP CJoint::GetDOFLimit(DegreesOfFreedom DOF, VARIANT *min, VARIANT *max) {
	if (!CheckDOF(DOF)) return E_FAIL;
	// Not implemented yet, but just return a success.
	return S_OK;
}

STDMETHODIMP CJoint::UnconstrainDOF(DegreesOfFreedom DOF) {
	if (!CheckDOF(DOF)) return E_FAIL;
	// Not implemented yet, but just return a success.
	return S_OK;
}

/*
Add a single degree of freedom to the current joint.
It updates the numDOFs variable to reflect the new DOFs.
The important thing: ORDER of the DOFs is vitally significant
*/
STDMETHODIMP CJoint::AddDOF(DegreesOfFreedom DOF) {
//	if (DOF > 63) return E_FAIL;
	if (!CheckDOF(DOF)) return E_FAIL;
	if (dof & DOF) return S_OK;
	dof |= DOF;

	order[numDOFs] = DOF;	
/*
	numDOFs=0;
	if (dof & 1) numDOFs++;
	if (dof & 2) numDOFs++;
	if (dof & 4) numDOFs++;
	if (dof & 8) numDOFs++;
	if (dof & 16) numDOFs++;
	if (dof & 32) numDOFs++;
*/
	numDOFs++;
	return S_OK;
}

STDMETHODIMP CJoint::SetParentOffset(double x, double y, double z) {
	state[0] = x;
	state[1] = y;
	state[2] = z;

	return S_OK;
}

STDMETHODIMP CJoint::GetParentOffset(VARIANT *x, VARIANT *y, VARIANT *z) {
	x->dblVal = state[0];
	y->dblVal = state[1];
	z->dblVal = state[2];
	return S_OK;
}

/*
Add a child to the current joint
A failure indicates either that a child was added twice or the child was NULL
*/
STDMETHODIMP CJoint::AddChild(IJoint *ij) {
	numChildren++;
	ij->AddRef();
	children->Add(ij);
	ij->SetParent(this);
	return S_OK;
}

/*
Return the number of children of this joint
  */
STDMETHODIMP CJoint::ChildCount(int *num) {
	int total=0;
	int sub;
	for (int c=0; c < numChildren; c++) {
		total++;
		children->GetAt(c)->ChildCount(&sub);
		total+=sub;
	}
	*num = total;
	return S_OK;
}

/*
Return a child by ID. Recurses down the joint hierarchy.
A failure indicates that the requested ID is not among the children of the current joint.
*/
STDMETHODIMP CJoint::GetChild(int jointID, IJoint **ij) {
	if (jointID == ID) {
		*ij = this;
		((IJoint *)this)->AddRef();
		return S_OK;
	}
	IJoint *itemp;
	for (int c=0; c < numChildren; c++) {
		itemp = children->GetAt(c);
		if (itemp->GetChild(jointID, ij)==S_OK) return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CJoint::SetParent(IJoint *ij) {
	if (pParent!=NULL) pParent->Release();
	pParent=ij;
	pParent->AddRef();
	return S_OK;
}

//For geometry update callbacks.
/*
This strikes me as a really ugly solution.
Other option: define an abstract base class which the joint can contain a pointer to.
I don't know if that would gain us anything at all.
  */

/*
This method synchronizes its state with the state of its geometry frame and
recurses to all of its children. This need only be called once per cycle on the 
root node.
*/
STDMETHODIMP CJoint::UpdateGeometry() {
	((IJoint *)this)->AddRef();
	pfUpdateJoint((IJoint *)this, dof, state, order);
	for (int c =0; c < numChildren; c++) {
		children->GetAt(c)->UpdateGeometry();
	}
	((IJoint *)this)->Release();
	return S_OK;
}

STDMETHODIMP CJoint::GetAppData(void** data) {
	*data = AppData;
	return S_OK;
}

STDMETHODIMP CJoint::SetAppData(void* data) {
	AppData = data;
	return S_OK;
}

STDMETHODIMP CJoint::GetNumDOFs(int *count) {
	*count = numDOFs;
	return S_OK;
}

STDMETHODIMP CJoint::Destroy() {
	int c;
	numChildren=0;
	IJoint *jtemp;
	if (pParent!=NULL) pParent->Release();
	for (c=0; c < numChildren; c++) {
		jtemp = children->GetAt(c);
		jtemp->Destroy();
		jtemp->Release();
	}
	children->RemoveAll();	
	numChildren=0;
	return S_OK;
}

STDMETHODIMP CJoint::GetDOFs(int *dofs) {
	*dofs = dof;
	return S_OK;
}

STDMETHODIMP CJoint::GetOrderDOFs(int **dofs) {
	*dofs = order;
	return S_OK;
}


//------------------------Helpers-------------------------


/*
Returns the index [0 5] corresonding to the index of the degree of freedom in the
STATE array represented by the DegreesOfFreedom d.

  This is easy. Just return the log-base2 of the d...
*/
int DOFToIndex(DegreesOfFreedom d) {
	return (int)(log(0.1+(double)d)/log(2.0));
}

/*
Make sure that d is a correct DOF (i.e., it's a power of two and it's < 2^5
Use [AC]'s trick.
  */
bool CJoint::CheckDOF(DegreesOfFreedom d) {
	return (!(d-1 & d) && (d < 33));
}
