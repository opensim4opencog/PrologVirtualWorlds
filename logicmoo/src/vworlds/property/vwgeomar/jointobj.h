// Joint.h : Declaration of the joint object class

#ifndef __JOINT_H_
#define __JOINT_H_

#include <d3d.h>
#include <d3drm.h>
#include <resource.h>       // main symbols
#include <propbase.h>
#include <objimpl.h>
#include "vwgeomar.h"
#include "vwtempl.h"

/////////////////////////////////////////////////////////////////////////////
// Joint

#include <atlctl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_Joint;

//First, some defs...

typedef int DegreesOfFreedom;
#define XT 1
#define YT 2
#define ZT 4
#define XR 8
#define YR 16
#define ZR 32

typedef CVWArray<IJoint *, IJoint *> JointArray;

class ATL_NO_VTABLE CJoint :
 	public IDispatchImpl<IJoint, &IID_IJoint, &LIBID_VWMMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CJoint, &CLSID_Joint>,
	public IObjectSafetyImpl<CJoint, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public ISupportErrorInfoImpl<&IID_IJoint>
{

public:	

	static void			(*pfUpdateJoint)(IJoint *, int, double *, int *);

	DECLARE_AGGREGATABLE(CJoint)

	CJoint();
	~CJoint();

	BEGIN_COM_MAP(CJoint)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IJoint)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IObjectSafety)	//COM_INTERFACE_ENTRY_IMPL(IObjectSafety)
	END_COM_MAP()

	DECLARE_REGISTRY(CJoint, _T("VWSYSTEM.Joint.1"), _T("VWSYSTEM.Joint"), IDS_JOINT_DESC, THREADFLAGS_BOTH)

	STDMETHOD(SetID)(int newID);
	STDMETHOD(GetID)(int *curID);
	STDMETHOD(GetPosition)(int DOF, VARIANT *val);
	STDMETHOD(SetPosition)(int DOF, double position);
	STDMETHOD(SetDOF)(int DOF, double position);
	STDMETHOD(SetState)(double xt, double yt, double zt, double xr, double yr, double zr);
	STDMETHOD(SetDOFLimit)(int DOF, double min, double max);
	STDMETHOD(GetDOFLimit)(int DOF, VARIANT *min, VARIANT *max);
	STDMETHOD(UnconstrainDOF)(int DOF);
	STDMETHOD(AddDOF)(int DOF);
	STDMETHOD(SetParentOffset)(double x, double y, double z);
	STDMETHOD(GetParentOffset)(VARIANT *x, VARIANT *y, VARIANT *z);
	STDMETHOD(AddChild)(IJoint *ij);
	STDMETHOD(ChildCount)(int *num);
	STDMETHOD(GetChild)(int jointID, IJoint **ij);
	STDMETHOD(SetParent)(IJoint *ij);

	STDMETHOD(UpdateGeometry)();
	STDMETHOD(GetAppData)(void** data);
	STDMETHOD(SetAppData)(void* data);
//	STDMETHOD(SetUpdateJointFunction)(void *fncptr(IJoint *, DegreesOfFreedom, double[]));

	STDMETHOD(GetNumDOFs)(int *count);

	STDMETHOD(Destroy)();
	STDMETHOD(GetDOFs)(int *dofs);

	STDMETHOD(GetOrderDOFs)(int **dofs);

private:

	int ID;   // arbitrary way to refer to joint in actor

	bool CheckDOF(DegreesOfFreedom d);

	DegreesOfFreedom	dof;
	double				state[6];

    int					numChildren;
	int					numDOFs;
	void				* AppData;
    IJoint				* pParent;
	JointArray			*children;
	DegreesOfFreedom	order[6];
};

int DOFToIndex(DegreesOfFreedom d);

#endif