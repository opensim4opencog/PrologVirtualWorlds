// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _RLESurf_h
#define _RLESurf_h

// File:	RLESurf.h
// Author:	MM    (MM)
//
// Description:
//    This class defines a run length encoding for an 8 bit paletted image 
//  surface.
// 
// History:
// -@- 2/24/97 (MM) created

//#define RLEERR_SURFACEBUSY MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x3001)
//#define RLEERR_NOTLOCKED MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x3002)
//#define flagRLELOCKED 0x1

class CRLESurface {
public:
						CRLESurface();
						~CRLESurface();

	HRESULT				Init(const BYTE *pPixels, long nPitch, DWORD nWidth, DWORD nHeight, 
							BYTE nBPP, int nTransColor);

	HRESULT				BltToSurface(LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, 
							LPRECT prDst, DWORD bvFlags) const;
	HRESULT				BltFastToSurface(LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, 
							WORD nXPos, WORD nYPos, DWORD bvFlags) const;

	DWORD				GetSize() const;
	float				CompressionRatio() const;

public:
	DWORD				AddRef();
	DWORD				Release();

private:
	HRESULT				RLEDecode(const BYTE *puchRLEStream, const BYTE *puchDstSurface, 
							long nDstPitch) const;

	HRESULT				ComputeRLE8(const BYTE *pPixels, long nPitch, int nTransColor);
	HRESULT				ComputeRLE4(const BYTE *pPixels, long nPitch, int nTransColor);

private:
	DWORD				m_nRef;
	DWORD				m_bvFlags;

	LPBYTE				m_pData;
	DWORD				m_nDataLength;
	LPWORD				m_rgnLineLengths;

	DWORD				m_nWidth;
	DWORD				m_nHeight;
	BYTE				m_nBPP;
	BYTE				m_iTransColor;
};

typedef CRLESurface *LPRLESURFACE;

inline DWORD
CRLESurface::AddRef()
{
	return ++m_nRef;
}

inline DWORD
CRLESurface::Release()
{
	if (--m_nRef == 0) {
		delete this;
		return 0;
	}

	return m_nRef;
}

inline DWORD
CRLESurface::GetSize() const
{
	return m_nDataLength + (m_nHeight * sizeof(WORD));
}

inline float
CRLESurface::CompressionRatio() const
{
	return float(m_nWidth * m_nHeight) / float(GetSize());
}

#endif