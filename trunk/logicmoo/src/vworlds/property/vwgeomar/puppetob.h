// Puppet.h : Declaration of the Character object class

#ifndef __PUPPET_H_
#define __PUPPET_H_

#include <resource.h>       // main symbols
#include <propbase.h>
#include <objimpl.h>
#include "vwgeomar.h"
#include "vwtempl.h"
#include <sys/types.h>
#include <sys/timeb.h>

/////////////////////////////////////////////////////////////////////////////
// The Puppet Behavior

#include <atlctl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_Puppet;

typedef double* JointFrame;
typedef CVWArray<JointFrame *, JointFrame *> MotionArray;
typedef CVWArray<int, int> IntArray;
typedef CVWArray<float, float> FloatArray;
typedef CVWArray<CString *, CString *> StringArray;

class ATL_NO_VTABLE CPuppet :
 	public IDispatchImpl<IPuppet, &IID_IPuppet, &LIBID_VWMMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPuppet, &CLSID_Puppet>,
	public IObjectSafetyImpl<CPuppet, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public ISupportErrorInfoImpl<&IID_IPuppet>
{

public:	

	DECLARE_AGGREGATABLE(CPuppet)

	CPuppet();
	~CPuppet();

	BEGIN_COM_MAP(CPuppet)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IPuppet)
		COM_INTERFACE_ENTRY(IBehavior)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IObjectSafety)
	END_COM_MAP()

	DECLARE_REGISTRY(CPuppet, _T("VWSYSTEM.Puppet.1"), _T("VWSYSTEM.Puppet"), IDS_PUPPET_DESC, THREADFLAGS_BOTH)

	STDMETHOD(GetTime)(float *t);
	STDMETHOD(EvaluateToTime)(float t);
	STDMETHOD(put_Character)(ICharacter *ch);
	STDMETHOD(get_Character)(ICharacter **ch);
	STDMETHOD(Update)(void *arg);
	STDMETHOD(SetFrame)(float frameNum);
	STDMETHOD(NextFrame)();
	STDMETHOD(Start)();
	STDMETHOD(Stop)();
	STDMETHOD(AddMotion)(BSTR filename);
	STDMETHOD(put_Animation)(int animNum);
	STDMETHOD(get_Animation)(int *animNum);

	STDMETHOD(SetFPS)(float newFPS);
	STDMETHOD(GetFPS)(float *oldFPS);

	STDMETHOD(GetGestureIndex)(BSTR gestureName, int *ind);
	STDMETHOD(GetGestureText)(int index, BSTR *text);

	STDMETHOD(Reset)();
	STDMETHOD(SetRestPose)();

	STDMETHOD(CalcWalkDist)();

private:

	bool UpdateCharacter(JointFrame jf);
	void BlendFrames(JointFrame j1, JointFrame j2, double q1, double q2, int numDOFs, JointFrame target);
	void AdaptRestPosition(JointFrame jf);

	void OutputMotionFile();

	ICharacter		*character;
	MotionArray		*motions;
	int				numMotions;
	float			curFrame;
	float			time;
	int				curAnim;
	JointFrame		*curMotion;

	IntArray		*numFrames;
	FloatArray		*maxTime;
	float			curMaxTime;

	int				curNumFrames;
	int				numDOFs;

	bool			running;
	bool			at_rest;
	bool			stopping;
	float			FPS;		//the frames per second
	float			prevFrame;

	JointFrame		restPos;	//contains the rest positions of all the DOFs.

	struct _timeb	prevTime;

	StringArray		*GestureNames;
	StringArray		*GestureTexts;

	int				walkCycle;	//the index of the animation to be use as the walk cycle.
	double			walkCycleDist;

	float lastX, lastY, lastZ;	//the previous position of the articulated figure.
	float lastSX, lastSY, lastSZ;	//the previous scalings of the geom_frame (for walk
									//cycle distance calculation.

	double blend;	
	double endX;		//the endpoint of the walk cycle
	double endY;
	double endZ;
};

#endif //__PUPPET_H_