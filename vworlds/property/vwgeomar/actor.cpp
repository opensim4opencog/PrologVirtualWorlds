#include "stdafx.h"
#include "actor.h"


Actor::Actor(void) : pRoot(NULL), pfUpdateJoint(NULL)
{
}

Actor::Actor(const CString &name) : pRoot(NULL), csName(name), pfUpdateJoint(NULL)
{
}

Actor::~Actor(void)
{
	if (pRoot) delete pRoot;
}

Joint* Actor::GetJoint(int jointid) 
{
	return pRoot->FindJointByID(jointid);
}

void	Actor::SetName(const CString &csn)
{
	csName = csn;
}

const CString&	Actor::GetName(void) const
{
	return csName;
}

void Actor::SetRoot(Joint* pJ)
{
	pRoot = pJ;
}
	
Joint*	Actor::GetRoot(void) const
{
	return pRoot;
}

void Actor::SetJointPositionByID(int ID, double pos)
{	
	Joint *pJ;

	pJ = GetJoint(ID);
	if (pJ != NULL) {
		pJ->SetPosition(pos);
		if (pfUpdateJoint != NULL) 
			pfUpdateJoint(pJ->GetAppData(), pJ->GetType(), pos);
	} 
}

void Actor::SetJointUpdateFunction(void (*fncptr)(void *, int, double))
{
	pfUpdateJoint = fncptr;
}
// ------------------------------------
// TEST MAIN
#if 0

void main()
{
	Actor foo("Spuck");
	Joint *pJ, *pK, *pFound;

	pJ = new Joint;
	pK = new Joint;
	foo.SetRoot(pJ);
	pJ->AddChild(pK);
	pJ->SetID(69);
	pK->SetID(42);

	pFound = foo.GetJoint(42);
}


#endif


