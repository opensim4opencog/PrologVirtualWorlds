/////////////////////////////////////////////////////////////////////////////
//
// PuppetObj.cpp -- implementation of the CPuppet class

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
#include "charobj.h"
#include "afx.h"
#include "afxtempl.h"
#include "puppetob.h"
#include <sys/types.h>
#include <sys/timeb.h>
#include <fstream.h>

#define D2R(X) (3.14159*(X)/180.)

CPuppet::CPuppet() {

	motions = new MotionArray;
	numMotions=0;
	character = NULL;
	curFrame = 0;
	time = 0.0;
	curAnim = -1;
	curMotion = NULL;

	numFrames = new IntArray;
	maxTime = new FloatArray;
	curMaxTime = 0.0;

	curNumFrames = 0;
	numDOFs = 0;

	running = false;
	at_rest = true;
	stopping = false;
	FPS = 15.0;

	GestureNames = new StringArray;
	GestureTexts = new StringArray;

	walkCycle = -1;
	walkCycleDist = -1.0;
	lastX = lastY = lastZ = 0.0f;

	lastSX = lastSY = lastSZ = 0.0f;

	restPos = NULL;
	blend = 1.0;		//how much of the rest position do we have?

	endX=1.0;
	endY=1.0;
	endZ=1.0;
}

CPuppet::~CPuppet() {
	delete motions;
	if (character!=NULL) character->Release();
	delete GestureNames;
}

STDMETHODIMP CPuppet::put_Character(ICharacter *ch) {
	IJoint *ij;
	if (ch) ch->AddRef();
	if (character) character->Release();
	character = ch;
	if (character) {
		int temp, nj;
		numDOFs = 0;
		character->JointCount(&nj);
		for (int c=0 ; c < nj; c++) {
			if (!SUCCEEDED(character->GetJoint(c, &ij))) break;
			ij->GetNumDOFs(&temp);
			numDOFs += temp;
			ij->Release();
		}
	}
	SetRestPose();
	return S_OK;
}

STDMETHODIMP CPuppet::get_Character(ICharacter **ch) {
	*ch = character;
	character->AddRef();
	return S_OK;
}

/*
A generic update method. This updates the position of the model based on
system clock time, start time etc.

If we're in the middle of an animation (i.e. non-walk-cycle) we finish the anim.
Else, we examine our transform node, note its offset from our last observed position
and play forward the appropriate number of frames. Cool.

  We should make some kind of top-level decision about whether we're in an animation or
  something. E.g. check the "running" variable.

*/
STDMETHODIMP CPuppet::Update(void *arg) {
	CVWGeometryArticulated *vwga;
	IVWFrame *frame;
	struct _timeb time1;

	if (running) {
		//Continue to run the animation -- disregard the transform frame
		float xt, yt, zt, xr, yr, zr, ux, uy, uz;
		IJoint *ij;
		IVWFrame *baseframe=NULL;
		
		_ftime(&time1);

		int elapsedMS = ((time1.time - prevTime.time)*1000) + time1.millitm - prevTime.millitm;
		float timeElapsed = (float)elapsedMS/1000;

		float newFrame = curFrame + timeElapsed*FPS;

		if (newFrame > curNumFrames) {
			//Transfer from root's frame to transform frame
			vwga = (CVWGeometryArticulated *)arg;
			vwga->get_Frame(&frame);
			character->GetRoot(&ij);
			ij->GetAppData((void**)&baseframe);

			baseframe->GetOrientationEx(NULL, &xr, &yr, &zr, &ux, &uy, &uz);
			baseframe->GetPositionEx(NULL, &xt, &yt, &zt);
			ij->Release();
			frame->SetOrientationEx(NULL, -xr, 0.0f, -zr, 0.0f, 1.0f, 0.0f);
			frame->SetPositionEx(NULL, xt, yt, zt);
			Reset();
			lastX = xt;
			lastY = yt;
			lastZ = zt;
		}
		else {
			SetFrame(newFrame);
			prevTime.time = time1.time;
			prevTime.millitm = time1.millitm;
		}
		return S_OK;
	}
	else {
		//check whether the transform frame has moved
		//Here we need to extract the position of the transform frame.
		float x, y, z, xo, yo, zo, xu, yu, zu;
		IVWFrame *geomframe;

		if (walkCycle == -1) return S_OK;		//no walk cycle!
		if (curAnim != walkCycle) put_Animation(walkCycle);

		vwga = (CVWGeometryArticulated *)arg;
		if (arg==NULL) {
			return E_FAIL;
		}

		IJoint *ij;
		IVWFrame *baseframe;
		character->GetRoot(&ij);
		ij->GetAppData((void**)&baseframe);	//frame contains an IVWFRame
		baseframe->GetParent(&geomframe);
		
		if (geomframe==NULL) {
			lastSX = 0;
			lastSY = 0;
			lastSZ = 0;
			return S_OK;
		}

		geomframe->GetScaleEx(NULL, &x, &y, &z);
		if (x!=lastSX || y!=lastSY || z!=lastSZ) {
			CalcWalkDist();
			if (walkCycleDist<0.001) walkCycleDist = 1.0;
			lastSX=x;
			lastSY=y;
			lastSZ=z;
		}

		//Advance the appropriate number of frames.
		vwga->get_Frame(&frame);
		if (frame==NULL) return E_FAIL;
		frame->GetPositionEx(NULL, &x, &y, &z);
		double dx = (double)(x-lastX);
		double dy = (double)(y-lastY);
		double dz = (double)(z-lastZ);
		double dist = sqrt(dx*dx + dy*dy + dz*dz);

		if (dist < 0.0001) {
			int numDOFs;
			character->GetNumDOFs(&numDOFs);
			JointFrame jf = new double[numDOFs];
			if (blend == 1.0) return S_OK;	//he's stopped.
			blend += 0.1;
			if (blend > 1.0) {
				blend = 1.0;
				Reset();
			}
			else {		//create blending function?
				if (blend == 0.1) {		//adjust the restposition.
					AdaptRestPosition(curMotion[(int)curFrame]);
				}
				BlendFrames(curMotion[(int)curFrame], restPos, 1.0f-blend, blend, numDOFs, jf);
				UpdateCharacter(jf);
			}
			delete jf;
			return S_OK;
		}
		blend = 0.0;

		/*
		if (dist < 0.0001) {
			if (!at_rest) {
				_ftime(&time1);
				if (stopping) {
					int elapsedMS = ((time1.time - prevTime.time)*1000) + time1.millitm - prevTime.millitm;
					if (elapsedMS > 1000) {
						Reset();
						at_rest = true;
						stopping = false;
					}
				}
				else {
					prevTime.time = time1.time;
					prevTime.millitm = time1.millitm;
					stopping = true;
				}
			}
			return S_OK;
		}
		at_rest = false;
*/
		frame->GetOrientationEx(NULL, &xo, &yo, &zo, &xu, &yu, &zu);
		float dot = xo*(float)dx + yo*(float)dy + zo*(float)dz;
		float factor;
		if (dot < 0) factor = -1.0;
		else factor = 1.0;
		float newFrame = factor*float(dist/walkCycleDist)*(float)curNumFrames + curFrame;
		SetFrame(newFrame);

		//Zero the position of the root joint.
/*
		ij->SetPosition(1, 0.0);
		ij->SetPosition(2, 0.0);
		ij->SetPosition(4, 0.0);
		ij->Release();
*/
		lastX = x;
		lastY = y;
		lastZ = z;

		return S_OK;
	}
}

/*
Set the current animation frame.
*/
STDMETHODIMP CPuppet::SetFrame(float frameNum) {
	while(frameNum >= (float)curNumFrames) frameNum -= (float)curNumFrames;
	while(frameNum < 0.0f) frameNum += (float)curNumFrames;
	prevFrame = curFrame;
	curFrame = frameNum;
	if ((int)curFrame!=(int)prevFrame) UpdateCharacter(curMotion[(int)curFrame]);
	return S_OK;
}

STDMETHODIMP CPuppet::EvaluateToTime(float t) {
	if (curMaxTime==0) return E_FAIL;
	time = t;
	float f = (time/curMaxTime)*(float)curNumFrames;
	SetFrame(f);
	return S_OK;
}

STDMETHODIMP CPuppet::GetTime(float *t) {
	//return the time based on the current frame
	if (curNumFrames==0) return E_FAIL;
	*t = (curFrame/(float)curNumFrames)*curMaxTime;
	return S_OK;
}

STDMETHODIMP CPuppet::NextFrame() {
	return SetFrame(curFrame+1.0f);
}

/*
This begins an animation.
  */
STDMETHODIMP CPuppet::Start() {
	if (character==NULL) return E_FAIL;
	if (curAnim >= numMotions || curAnim < 0) {
		if (numMotions > 0) put_Animation(0);
		else return E_FAIL;
	}
	running = true;
	prevFrame =0;
	_ftime(&prevTime);
	return S_OK;
}

/*
Stop the animation
  */
STDMETHODIMP CPuppet::Stop() {
	running = false;
	return S_OK;
}

/*
This method reads in a motion file (given by BSTR filename) and parses into the
required 2-dimensional array of doubles. This array is then added to the current
set of motions.
*/
STDMETHODIMP CPuppet::AddMotion(BSTR bstrFileName) {
	//Read file and parse it

	//Assume we have the desired array in newMotion[][]
	//the dimensions are stored in newFrames and newJoints

	HRESULT		hr = E_FAIL;
	CStdioFile	motionIn;
	CString		str;
	char		buf[2048],*pbuf;
	int			newFrames, i;
	int			newJoints;
	int			start, count;

	int			motionCount=0;

	float		val;

	double		*frame;
	JointFrame	*newMotion;
	bool		cycle;

	char		gestureName[50], motion[50];
	BSTR		bstrFinalURL = NULL, bstrFinalPath = NULL;
	IInternetFileManager	*pInternetFileManager = NULL;
	static CComBSTR bstrInetfile("Inetfile");

	if (character == NULL) return E_FAIL;

	pbuf = &buf[0];
	if (!motionIn.Open(CString(bstrFileName), CFile::modeRead | CFile::typeText)) return E_FAIL;

	// first find the keyword for motion
	while(true) {
		cycle = false;
		gestureName[0] = '\0';
		motion[0] = '\0';
		while (motionIn.ReadString(str)) {
			pbuf = str.GetBuffer(1024);
			gestureName[0] = '\0';
			motion[0] = '\0';
			count = sscanf(pbuf, "%s %s", motion, gestureName);
			if (count > 0) {
				if (strcmp(motion, "MOTION")==0 || strcmp(motion, "MOTIONCYCLE")==0) break;
			}
		}
	
		// no motion in the file
		if (strcmp(motion, "MOTION")!=0 && strcmp(motion, "MOTIONCYCLE")!=0) goto Cleanup_ParseMotion;
		//Set the walk cycle index if this is a motion cycle.
		if (strcmp(motion, "MOTIONCYCLE")==0) {
			walkCycle = numMotions;
			cycle = true;
			lastSX = 0;
			lastSY = 0;
			lastSZ = 0;
		}

		//add gesture information to the GestureNames
		GestureNames->Add(new CString(gestureName));


		//Put gesture text on the next line?

		motionCount++;

		motionIn.ReadString(str);
		pbuf = str.GetBuffer( 1024);
		sscanf(pbuf, "%s", gestureName);
		if (strcmp(gestureName, "Text:") ==0) {
			GestureTexts->Add(new CString(&pbuf[5]));
			motionIn.ReadString(str);
			pbuf = str.GetBuffer( 1024);
		}
		else GestureTexts->Add(new CString(""));

		if (sscanf(pbuf, "Frames: %d", &newFrames)!=1) {
			hr = E_FAIL;
			goto Cleanup_ParseMotion;
		}

		newJoints = numDOFs;
		newMotion = new JointFrame[newFrames];
	
		//Now do the rest of the frames.
		for (i=0; i < newFrames; i++) {
			motionIn.ReadString(str);
			pbuf = str.GetBuffer(2048);
			frame = new double[newJoints];
			start=0;
			for (int c = 0; c < newJoints; c++) {
				if (sscanf(&pbuf[start],"%f %n", &val, &count)==0) {
					hr = E_FAIL;
					goto Cleanup_ParseMotion;
				}
				frame[c] = (double)val;
				start+=count;
			}
			newMotion[i] = frame;
		}

	// now go through and normalize the translations!
		double xb, yb, zb;
		xb = newMotion[0][0];
		yb = newMotion[0][1];
		zb = newMotion[0][2];
		if (cycle) {
			endX = newMotion[newFrames-1][0] - newMotion[0][0];
			endY = newMotion[newFrames-1][1] - newMotion[0][1];
			endZ = newMotion[newFrames-1][2] - newMotion[0][2];
			for (i=0;i<newFrames;i++) {
				newMotion[i][0] =0.0;
				newMotion[i][1] =0.0;
				newMotion[i][2] =0.0;
			}
		}
		else {
			for (i=0;i<newFrames;i++) {
				newMotion[i][0] -= xb;
				newMotion[i][1] -= yb;
				newMotion[i][2] -= zb;
			}
		}

		motions->Add(newMotion);
		numFrames->Add(newFrames);
		maxTime->Add(0.1f*(float)newFrames);	//random choice.
		numMotions++;

		hr = S_OK;
	}
	
Cleanup_ParseMotion:
	if (cycle) {
		walkCycle = -1;
	}
	SAFEFREESTRING(bstrFinalURL);
	if (bstrFinalPath) SysFreeString(bstrFinalPath);
	SAFERELEASE(pInternetFileManager);
		
	return hr;
}

STDMETHODIMP CPuppet::put_Animation(int animNum) {
	if (animNum >= numMotions || animNum < 0) return E_FAIL;
	curAnim = animNum;
	curMotion = motions->GetAt(animNum);
	curNumFrames = numFrames->GetAt(animNum);
	curMaxTime = maxTime->GetAt(animNum);
	time = 0.0;
	curFrame = 0;
	if (curAnim != walkCycle) running = true;
	prevFrame =0;
	_ftime(&prevTime);
	return S_OK;
}

STDMETHODIMP CPuppet::get_Animation(int *animNum) {
	*animNum = curAnim;
	return S_OK;
}

/*
This is the method which actually takes the current frame of curMotion and pumps it into the
character model. It is not visible from outside of the class. It is called automatically by
the settime and setframe types of methods.
*/
bool CPuppet::UpdateCharacter(JointFrame jf) {
	//Use the ordering provided by the joints to assign the motion values.
	int jointNum;
	int DOFcount;
	int temp;
	int curVal=0;
	double factor=1.0;
	if (character ==NULL) return false;
	IJoint *ij;
	int *dof;

	character->JointCount(&jointNum);

	for (int c=0 ; c < jointNum; c++) {
		if (!SUCCEEDED(character->GetJoint(c, &ij))) return false;
		ij->GetOrderDOFs(&dof);
		for (DOFcount=0; DOFcount < 6 && dof[DOFcount]!=0; DOFcount++) {
			temp = dof[DOFcount];
			ij->SetDOF(temp, jf[curVal]);
			curVal++;
		}
		ij->Release();
	}
	character->GetJoint(0, &ij);
	ij->UpdateGeometry();
	ij->Release();
	
	return true;
}

/*
Set the speed of the animation in Frams per second
  */
STDMETHODIMP CPuppet::SetFPS(float newFPS) {
	FPS = newFPS;
	return S_OK;
}

/*
Return the speed of the animation in frames per second.
  */
STDMETHODIMP CPuppet::GetFPS(float *oldFPS) {
	*oldFPS = FPS;
	return S_OK;
}

/*
Return the gesture index of the gestyre of the given name
  */
STDMETHODIMP CPuppet::GetGestureIndex(BSTR gestureName, int *ind) {

	CString *gn = new CString(gestureName);
	CString *temp;

	if (CString(gestureName) == CString("output")) OutputMotionFile();
	
	for (int c=0; c < numMotions; c++) {
		temp = GestureNames->GetAt(c);
		if (*temp == *gn) {
			*ind = c;
			return S_OK;
		}
	}
	return E_FAIL;
}

/*

  */
STDMETHODIMP CPuppet::GetGestureText(int index, BSTR *text) {
	CString *cs = GestureTexts->GetAt(index);
	const char *cp = LPCTSTR(*cs);
	*text = CComBSTR(cp).Copy();
	return S_OK;
}

/*
	Reset the state of the behavior. In this case that means setting the time to 0 
	and setting the values of all DOF's to 0 ... assuming this is the rest position.
	We should have the rest position stored somewhere or other....
*/
STDMETHODIMP CPuppet::Reset() {
	Stop();
	if (curAnim!=walkCycle && walkCycle>-1) put_Animation(walkCycle);
	prevFrame = curFrame;
	curFrame = 0;

	if (restPos) UpdateCharacter(restPos);
	return S_OK;
}

/*
Set the current position of the puppet as the rest position
This will be called automatically in the constructor, but can be overridden.
  */
STDMETHODIMP CPuppet::SetRestPose() {
	//Loop through all the joints, getting their DOFs in turn and contructing a JointFrame
	
	if (character==NULL) return E_FAIL;

	IJoint *ij;
	character->GetRoot(&ij);
	int *dof;
	int DOFcount;
	int temp=0;

	int nj;				//nj will contain the number of joints.
	character->JointCount(&nj);

	//We now have the total number of DOFs. Now let's get their states
	if (restPos) delete restPos;
	restPos = new double[numDOFs];
	VARIANT var;
	int curVal =0;

	for (int c=0 ; c < nj; c++) {
		if (!SUCCEEDED(character->GetJoint(c, &ij))) return E_FAIL;
		ij->GetOrderDOFs(&dof);
		for (DOFcount=0; DOFcount < 6 && dof[DOFcount]!=0; DOFcount++) {
			temp = dof[DOFcount];
			ij->GetPosition(temp, &var);
			restPos[curVal] = var.dblVal;
			curVal++;
		}
		ij->Release();
	}
	return S_OK;
}


/*
This routine calculates the walkcycle distance (given the current scaling of 
the VWGeomar
  */
STDMETHODIMP CPuppet::CalcWalkDist() {
	IJoint *ij;
	IVWFrame *frame;
	float x1, y1, z1;
	float xt, yt, zt;
	double xold, yold, zold;
	VARIANT var;

	character->GetJoint(0, &ij);
	ij->GetAppData((void**)&frame);	//frame contains an IVWFRame
	frame->GetPositionEx(NULL, &x1, &y1, &z1);

	//Save current state of joint
	ij->GetPosition(1, &var);
	xold = var.dblVal;
	ij->GetPosition(2, &var);
	yold = var.dblVal;
	ij->GetPosition(4, &var);
	zold = var.dblVal;

	ij->SetPosition(1, endX);
	ij->SetPosition(2, endY);
	ij->SetPosition(4, endZ);
	frame->GetPositionEx(NULL, &xt, &yt, &zt);
	ij->SetPosition(1, xold);
	ij->SetPosition(2, yold);
	ij->SetPosition(4, zold);
	ij->Release();

	//Now, finally find the distance travelled.
	x1 = (xt - x1);
	y1 = (yt - y1);
	z1 = (zt - z1);
	walkCycleDist = sqrt(double(x1*x1 + y1*y1 + z1*z1));
	return S_OK;
}

/*
Returns a new JointFrame which is a combination of the two JointFrames.
  */
void CPuppet::BlendFrames(JointFrame j1, JointFrame j2, double q1, double q2, int numDOFs, JointFrame target) {
	for (int c=0; c < numDOFs; c++) {
		target[c] = q1*j1[c] + q2*j2[c];
	}
}

/*
This method takes a jointframe and adapts the restposition to it ... i.e. it takes care of
the circular interpolation problem.
  */
void CPuppet::AdaptRestPosition(JointFrame jf) {
	int numj;
	int *dofs;
	int total = 0;
	int curVal;
	int tempDOF;
	character->JointCount(&numj);
	IJoint *ij;
	for (int c=0; c < numj; c++) {
		character->GetJoint(c, &ij);
		ij->GetOrderDOFs(&dofs);
		for (curVal=0; curVal < 6; curVal++) {
			tempDOF = dofs[curVal];
			if (tempDOF == 0) break;
			if (tempDOF > 4) {
				//Now we see which is closer
				if (jf[total]-restPos[total] > 180.0) restPos[total]+=360.0;
				if (restPos[total]-jf[total] > 180.0) restPos[total]-=360.0;
			}
			total++;
		}
	}
}


void CPuppet::OutputMotionFile() {

	//We need to cycle through the motions we have loaded and output them each.
	int c, d;

	IJoint *ij;
	int jointNum;

	int *dof;

	int DOFcount;
	int temp;
	int curVal=0;
	VARIANT var;

	//Let's open the file and dump.
	ofstream f("newmot.opt");

	character->JointCount(&jointNum);

	for (c = 0; c < numMotions; c++) {
		f << "MOTION \n";
		put_Animation(c);
		Stop();
		SetFrame(3.0f);
		for (d = 0; d < curNumFrames; d++) {
			SetFrame((float)d);
			for (int c=0 ; c < jointNum; c++) {
				character->GetJoint(c, &ij);
				ij->GetOrderDOFs(&dof);
				for (DOFcount=0; DOFcount < 6 && dof[DOFcount]!=0; DOFcount++) {
					temp = dof[DOFcount];
					ij->GetPosition(temp, &var);
					f << var.dblVal << "\t";
				}
				ij->Release();
			}
			f << "\n";
		}
	}
	
	f.close();
	return;
}

