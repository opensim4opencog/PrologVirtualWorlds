#ifndef ACTOR_H
#define ACTOR_H
#include <afx.h>
#include "joint.h"

class Actor {
	Joint		*pRoot;
	CString		csName;
	void		(*pfUpdateJoint)(void *data, int type, double pos);

public:
					Actor (void);
					Actor (const CString &name);
					~Actor(void);
	Joint*			GetJoint(int jointid);

	void			SetName(const CString &csn);
	const CString&	GetName(void) const;

	void			SetRoot(Joint* pJ);
	Joint*			GetRoot(void) const;

	void			SetJointPositionByID(int ID, double pos);

	void			SetJointUpdateFunction(void (*fncptr)(void *, int, double));

};

#endif