// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// EdgeAttr
#ifndef EDGEATTR_H
#define EDGEATTR_H
#define PASSIBLE 	0x01	/* edge does NOT block movement */
#define OCCLUDER	0x02	/* edge is a completely opaque wall */
#define PORTAL		0x04	/* edge is a portal */
#define NONGEOM		0x08	/* edge connects two geometrically inconsistent cells */

class  EdgeAttr {
public:
						EdgeAttr();
						EdgeAttr(const EdgeAttr &eattr);
	BOOL				Sacrifice() { return TRUE; }
						~EdgeAttr() { Sacrifice(); }

	// I/O Interface
	BOOL				PrintASCII(FILE *pf) const;
	BOOL				ParseASCII(FILE *pf);

	void				Set(const EdgeAttr &eattr);
	void				Set(BitVector bvFlags);

	// QUERY FUNCTIONS
//	BOOL				ExistsOppCycle() const { return (nEdge != NULL_EINDEX); }

public:
	BitVector			bvFlags;	// flags indicating edge properties
};

inline void 
EdgeAttr::Set(const EdgeAttr &eattr) {
	*this = eattr;
}

inline void 
EdgeAttr::Set(BitVector tbvFlags) {
	bvFlags = tbvFlags;
}

inline
EdgeAttr::EdgeAttr() 
{
	bvFlags = 0;
}

inline
EdgeAttr::EdgeAttr(const EdgeAttr &eattr) 
{
	Set(eattr);
}

inline
BOOL
EdgeAttr::PrintASCII(FILE *pf) const
{
	fprintf(pf, "%u", bvFlags);
	
	return TRUE;
}

inline
BOOL
EdgeAttr::ParseASCII(FILE *pf)
{
	fscanf(pf, "%u", &bvFlags);

	return TRUE;
}
#endif