/////////////////////////////////////////////////////////////////////////////
// Character

/*

  The character object will point toward a joint hierarchy, which is mirrored by a 
  vwframe hierarchy.

  The real question is whether or not to have a separate vwgeomar object. A wrapper
  of some kind will be necessary to supply the vwgeom interface methods. I could write a
  character object type in any case to provide that AND the actual articulated functionality.
  When the direct character comes along, we could always just write another wrapper which
  has the vwgeom stuff AND has a plug for the direct character....

  ...the other option is to provide a revamped vwgeomar object, which comes with a plug-in 
  for a character object. That would simplify the eventual incorporation of the direct
  character.

  The question is whether or not the Character object should have any VW-specific stuff in it.
  For the sake of finding root VWFrames and stuff like that it might be easier to say yes.
  Logically, though, I think No is the correct answer.

  I think I'll use the vwgeom-type strategy, but I'll call it something different. In that case,
  who deals with the setBehavior or setMotorController?

  Add getRoot and setRoot to interface of character.

  */

#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include <d3d.h>
#include <d3drm.h>
#include <resource.h>       // main symbols
#include <propbase.h>
#include <objimpl.h>
#include <atlctl.h>
#include "vwgeomar.h"

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_Character;

class ATL_NO_VTABLE CCharacter :
 	public IDispatchImpl<ICharacter, &IID_ICharacter, &LIBID_VWMMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCharacter, &CLSID_Character>,
	public IObjectSafetyImpl<CCharacter, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public ISupportErrorInfoImpl<&IID_ICharacter>
{
	DECLARE_AGGREGATABLE(CCharacter)

	CCharacter();
	~CCharacter();

	DECLARE_REGISTRY(CCharacter, _T("VWSYSTEM.Character.1"), _T("VWSYSTEM.Character"), IDS_CHARACTER_DESC, THREADFLAGS_BOTH)

	BEGIN_COM_MAP(CCharacter)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ICharacter)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IObjectSafety)
	END_COM_MAP()

	//Original interface

	STDMETHOD(put_Name)(BSTR *bstrName);
	STDMETHOD(get_Name)(BSTR **bstrName);
	STDMETHOD(JointNameToID)(BSTR bstrName, long *jointID);
	STDMETHOD(ParameterNameToID)(BSTR bstrName, long *jointID);
	STDMETHOD(GetJoint)(long jointID, IJoint **ij);
	STDMETHOD(AddJoint)(IJoint *ij);
	STDMETHOD(JointCount)(int *num);
	STDMETHOD(DeleteJoint)(long jointID);
	STDMETHOD(PutGuid)(long guid);
	STDMETHOD(GetGUID)(long *guid);
	STDMETHOD(GetNumDOFs)(int *num);

	//My additions

	STDMETHOD(GetRoot)(IJoint **joint);
	STDMETHOD(get_Behavior)(IBehavior **behave);
	STDMETHOD(put_Behavior)(IBehavior *behave);

	STDMETHOD(Destroy)();

private:

	BSTR *name;
	IJoint *root;
	long GUID;
	
	IBehavior	*behavior;
};
#endif