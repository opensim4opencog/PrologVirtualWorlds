// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	RLESurf.cpp
// Author:	MM    (MM)
//
// Notes:
//    Here is my RLE format in pseudo-BNF: 
//		- symbols in [] are optional
//		- symbols in {} are required
//		- "0" && "1" indicate bit patterns and symbols immediately after these
//		  bit patterns are the low order bits of a byte
//		- symbols without either bracket are terminal symbols in Hungarian
//
//  RLEImage	= /* dwRLELength */ {ScanLines}
//  ScanLines	= ScanLine [ScanLines]
//  ScanLine	= /* wScanLineDataLength */ {PixelData}
//  PixelData	= {RLEBlock} [PixelData]
//  RLEBlock	= 00_nFillLength {FillPixel}	| 01_nTransparencyLength	|
//				  10_nPrevLineLength			| 11_nNonRunLength {Pixels}

#include "MMImage.h"

// class: CRLEDataBuilder
//    This helper class is used to construct the RLE data stream.
//
class CRLEDataBuilder {
public:
						CRLEDataBuilder();
						~CRLEDataBuilder();

	void				Reset();

	void				Start(DWORD nWidth, DWORD nHeight);
	void				StartLine();
	void				AddRun(DWORD nLength, BYTE iPixel);
	void				AddTransparent(DWORD nLength);
	void				AddCopyPrev(DWORD nLength);
	void				AddNonRun(DWORD nLength, const BYTE *pSrcPixels);
	void				EndLine();
	void				End(LPBYTE *ppData, LPDWORD pnDataLength, LPWORD *prgnLineLengths);

private:
	void				Grow();

	LPBYTE				m_pBuf;
	DWORD				m_nBufLength;
	LPWORD				m_rgnLineLengths;
	LPBYTE				m_pCurPos, m_pLineMark, m_pLimitPos;
	DWORD				m_iCurLine, m_nWidth, m_nHeight;
};

static CRLEDataBuilder gs_Builder;

#define nDEFBUFSIZE			1024
#define nRUNTOLERANCE		3
#define maskLength			0x3f
#define nMaskLength			6
#define codeRun				(0x0 << nMaskLength)
#define codeTransparent		(0x1 << nMaskLength)
#define codeCopyPrev		(0x2 << nMaskLength)
#define codeNonRun			(0x3 << nMaskLength)
#define codeMaxRun			(codeRun | maskLength)
#define codeMaxTransparent	(codeTransparent | maskLength)
#define codeMaxCopyPrev		(codeCopyPrev | maskLength)
#define codeMaxNonRun		(codeNonRun | maskLength)


CRLEDataBuilder::CRLEDataBuilder()
{
	// allocate an array of bytes that are aligned on DWORD boundary
	m_nBufLength = nDEFBUFSIZE;
	m_pBuf = (LPBYTE) new DWORD[m_nBufLength >> 2];
	m_rgnLineLengths = NULL;
	MMASSERT(m_pBuf);
	Reset();
}

CRLEDataBuilder::~CRLEDataBuilder()
{
//	MMTRACE("CRLEDataBuilder::~CRLEDataBuilder\n");
	MMDELETE(m_pBuf);
	MMDELETE(m_rgnLineLengths);
}

void
CRLEDataBuilder::Reset()
{
	m_nWidth = m_nHeight = 0;
	m_pCurPos = m_pBuf;
	m_pLineMark = NULL;
	MMASSERT(m_pCurPos);
	MMDELETE(m_rgnLineLengths);
}

void
CRLEDataBuilder::Start(DWORD nWidth, DWORD nHeight)
{
	MMASSERT(m_pCurPos == m_pBuf);
	MMASSERT((DWORD(m_pCurPos) & 0x3) == 0);	// check DWORD aligned

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_rgnLineLengths = new WORD[nHeight];
	MMASSERT(m_rgnLineLengths);
	m_iCurLine = 0;
	m_pLineMark = NULL;
	// set the limit position so that we only need to check the limits once per line
	m_pLimitPos = m_pBuf + m_nBufLength - (m_nWidth + (m_nWidth >> 5));
	// make sure that the buffer is big enough for at least one scanline
	while (m_pCurPos > m_pLimitPos)
		Grow();
//	m_pCurPos += sizeof(DWORD);
}

inline void
CRLEDataBuilder::StartLine()
{
	// check the line limits
	if (m_pCurPos > m_pLimitPos)
		Grow();

//	MMASSERT((DWORD(m_pCurPos) & 0x1) == 0);	// check WORD aligned
	m_pLineMark = m_pCurPos;
//	m_pCurPos += sizeof(WORD);
}

inline void
CRLEDataBuilder::AddRun(DWORD nLength, BYTE iPixel)
{
	while (nLength > maskLength) {
		*m_pCurPos++ = (BYTE) codeMaxRun;
		*m_pCurPos++ = iPixel;
		nLength -= maskLength;
	}
	MMASSERT((nLength & ~maskLength) == 0);
	*m_pCurPos++ = (BYTE) (codeRun | nLength);
	*m_pCurPos++ = iPixel;
}

inline void
CRLEDataBuilder::AddTransparent(DWORD nLength)
{
	while (nLength > maskLength) {
		*m_pCurPos++ = (BYTE) codeMaxTransparent;
		nLength -= maskLength;
	}
	MMASSERT((nLength & ~maskLength) == 0);
	*m_pCurPos++ = (BYTE) (codeTransparent | nLength);
}

inline void
CRLEDataBuilder::AddCopyPrev(DWORD nLength)
{
	while (nLength > maskLength) {
		*m_pCurPos++ = (BYTE) codeMaxCopyPrev;
		nLength -= maskLength;
	}
	MMASSERT((nLength & ~maskLength) == 0);
	*m_pCurPos++ = (BYTE) (codeCopyPrev | nLength);
}

inline void
CRLEDataBuilder::AddNonRun(DWORD nLength, const BYTE *pSrcPixels)
{
	while (nLength > maskLength) {
		*m_pCurPos++ = (BYTE) codeMaxNonRun;
		memcpy(m_pCurPos, pSrcPixels, maskLength);
		nLength -= maskLength;
		pSrcPixels += maskLength;
		m_pCurPos += maskLength;
	}
	MMASSERT((nLength & ~maskLength) == 0);
	*m_pCurPos++ = (BYTE) (codeNonRun | nLength);
	memcpy(m_pCurPos, pSrcPixels, nLength);
	m_pCurPos += nLength;
}

inline void
CRLEDataBuilder::EndLine()
{
	MMASSERT(m_iCurLine < m_nHeight);

	// set the line length
	DWORD nLineLength = m_pCurPos - m_pLineMark;
	MMASSERT(nLineLength <= 0xffff);
	m_rgnLineLengths[m_iCurLine++] = (WORD) nLineLength;
//	*((LPWORD) m_pLineMark) = (WORD) nLineLength;

	// WORD align the stream
//	m_pCurPos += DWORD(m_pCurPos) & 0x1;
}

void
CRLEDataBuilder::End(LPBYTE *ppData, LPDWORD pnDataLength, LPWORD *prgnLineLengths)
{
	DWORD nLength = m_pCurPos - m_pBuf;
	LPBYTE pTmp = (LPBYTE) memcpy(new BYTE[nLength], m_pBuf, nLength);
	*pnDataLength = nLength;
	*ppData = pTmp;
	*prgnLineLengths = m_rgnLineLengths;
	m_rgnLineLengths = NULL;
	Reset();
}

void
CRLEDataBuilder::Grow()
{
	MMASSERT(m_pBuf);

	// save the old pointer positions
	LPBYTE pTmpData = m_pBuf;
	DWORD nCurPos = m_pCurPos - m_pBuf;
	DWORD nLineMark = m_pLineMark - m_pBuf;
	MMASSERT(nCurPos <= m_nBufLength);
	MMASSERT(nLineMark <= m_nBufLength);

	// allocate a larger buffer
	m_nBufLength <<= 1;
	m_pBuf = (LPBYTE) new DWORD[m_nBufLength >> 2];
	MMASSERT(m_pBuf);

	// reset the old pointers w.r.t. the new buffer
	m_pLimitPos = m_pBuf + m_nBufLength - (m_nWidth + (m_nWidth >> 5));
	m_pCurPos = m_pBuf + nCurPos;
	m_pLineMark = m_pBuf + nLineMark;

	// copy the old data into the new buffer
	memcpy(m_pBuf, pTmpData, nCurPos);

	// delete the old buffer
	delete pTmpData;
}


//
// RLESurface Class
//

CRLESurface::CRLESurface()
{
	m_nRef = 1;
	m_bvFlags = 0;
	m_pData = NULL;
	m_nDataLength = 0;
	m_rgnLineLengths = NULL;
	m_nWidth = m_nHeight = 0;
	m_nBPP = 0;
	m_iTransColor = 0;
//	InitializeCriticalSection(&m_csLock);
}

CRLESurface::~CRLESurface()
{
	MMASSERT(m_nRef == 0);
//	DeleteCriticalSection(&m_csLock);
	MMDELETE(m_pData);
	MMDELETE(m_rgnLineLengths);
}

// Function: Init
//    Compute the run length encoding for the image.
HRESULT
CRLESurface::Init(const BYTE *pPixels, long nPitch, DWORD nWidth, DWORD nHeight, 
				  BYTE nBPP, int nTransColor)
{
	if (m_pData)
		return E_ALREADYINITIALIZED;

	if ((pPixels == NULL) || (nWidth == 0) || (nHeight == 0) || (nBPP > 8))
		return E_INVALIDARG;

	HRESULT hr;

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBPP = nBPP;

	switch (nBPP) {
	case 4:
		hr = ComputeRLE4(pPixels, nPitch, nTransColor);
		break;
	case 8:
		hr = ComputeRLE8(pPixels, nPitch, nTransColor);
		break;
	default:
		hr = E_NOTIMPL;
		break;
	}

	return hr;
}

HRESULT
CRLESurface::BltToSurface(LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, 
						  LPRECT prDst, DWORD bvFlags) const
{
	if (m_pData == NULL)
		return E_NOTINITIALIZED;

	if ((prSrc == NULL) || (pddsDst == NULL) || (prDst == NULL))
		return E_INVALIDARG;

	return E_NOTIMPL;
}


// Function: RLEDecode
// Notes: assume we are decoding the entire image, not a subregion
HRESULT
CRLESurface::RLEDecode(const BYTE *puchRLEStream, const BYTE *puchDstSurface, long nDstPitch) const
{
	MMASSERT(DWORD(nDstPitch) >= m_nWidth);

	DWORD nLine = 0, nExcessPitch = (nDstPitch - m_nWidth);
	
	//
	// decode the RLE image
	//
	const BYTE *puchImageLimit = puchRLEStream + m_nDataLength;

	// loop thru all the scanlines
	while (puchRLEStream != puchImageLimit) {
		// compute the scanline sentinel
		const BYTE *puchScanlineLimit = puchRLEStream + m_rgnLineLengths[nLine++];
		
		// decode the RLE data for the scanline
		while (puchRLEStream != puchScanlineLimit) {
			BYTE uchRLECode = *puchRLEStream++;
			int nRLELength = (uchRLECode & maskLength);
			if (uchRLECode > 0x80) {
				//
				// memory copy case
				//
				const BYTE *puchSrcPixels;
				if (uchRLECode > 0xC0) {
					// copy following "non-run" of pixels
					puchSrcPixels = puchRLEStream;
					puchRLEStream += nRLELength;	// advance to the next RLE block
				} else {
					// copy corresponding pixels from previous scanline
					puchSrcPixels = puchDstSurface - nDstPitch;
				}
				memcpy((LPVOID) puchDstSurface, puchSrcPixels, nRLELength);
			} else {
				//
				// pixel fill case
				//
				BYTE iFillPixel;
				if (uchRLECode > 0x40) {
					// fill with the transparency pixel
					iFillPixel = m_iTransColor;
				} else {
					// fill with the following pixel
					iFillPixel = *puchRLEStream++;
				}
				memset((LPVOID) puchDstSurface, (int) iFillPixel, nRLELength);
			}
			puchDstSurface += nRLELength;	// advance dst pointer
		}
		puchDstSurface += nExcessPitch;		// advance the dst surface to the next line
	}
	return S_OK;
}


HRESULT
CRLESurface::BltFastToSurface(LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, 
							  WORD nXPos, WORD nYPos, DWORD bvFlags) const
{
	if (m_pData == NULL)
		return E_NOTINITIALIZED;

	if ((prSrc == NULL) || (pddsDst == NULL))
		return E_INVALIDARG;

	HRESULT hr;
	LPBYTE puchDstSurface = NULL;

	// REVIEW: make sure that the src is the whole image
	int nSrcWidth = prSrc->right - prSrc->left;
	int nSrcHeight = prSrc->bottom - prSrc->top;
	if ((prSrc->left != 0) || (prSrc->top != 0) || 
		(DWORD(nSrcWidth) != m_nWidth) || (DWORD(nSrcHeight) != m_nHeight))
		return E_NOTIMPL;

	// lock the destination
	DDSURFACEDESC ddsdDst;
	INIT_DXSTRUCT(ddsdDst);

	// REVIEW: this lock could just lock the minimum rectangle...
	if (FAILED(hr = pddsDst->Lock(NULL, &ddsdDst, DDLOCK_WAIT, NULL)))
	{
		ASSERT(FALSE);
		return hr;
	}

	// REVIEW: verify that the destination surface is 8 bit
	if (ddsdDst.ddpfPixelFormat.dwRGBBitCount != 8) {
		hr = DDERR_INVALIDPIXELFORMAT;
		goto e_BltFastToSurface;
	}

	puchDstSurface = (LPBYTE) ddsdDst.lpSurface;

	// REVIEW: verify the destination is big enough
	if ((DWORD(nSrcHeight + nYPos) > ddsdDst.dwHeight) || 
		(DWORD(nSrcWidth + nXPos) > ddsdDst.dwWidth)) {
		hr = DDERR_INVALIDRECT;
		goto e_BltFastToSurface;
	}

	hr = RLEDecode(m_pData, puchDstSurface + nXPos + nYPos * ddsdDst.lPitch, 
			ddsdDst.lPitch);

e_BltFastToSurface:
	pddsDst->Unlock(ddsdDst.lpSurface);
	return hr;
}


// Function: _FindNextTransPixel
//    Return the pointer to the next transparency pixel or the limit pixel,
//  whichever comes first.
inline const BYTE *
_FindNextTransPixel(const BYTE *pPixels, const BYTE *pLimitPixel, BYTE iTrans)
{
	MMASSERT(pPixels <= pLimitPixel);
	while ((pPixels != pLimitPixel) && (*pPixels != iTrans))
		pPixels++;
	return pPixels;
}

inline DWORD 
_LengthRun(const BYTE *pPixels, const BYTE *pLimitPixel)
{
	MMASSERT(pPixels < pLimitPixel);
	const BYTE *pCurPos = pPixels;
	BYTE iRun = *pCurPos++;
	while ((pCurPos != pLimitPixel) && (iRun == *pCurPos))
		pCurPos++;
	return pCurPos - pPixels;
}

inline DWORD 
_LengthNonRun(const BYTE *pPixels, const BYTE *pLimitPixel)
{
	MMASSERT(pPixels < pLimitPixel);
	// - assume there is no transparency pixel between pPixels and pLimitPixel

	const BYTE *pCurPos = pPixels;
	BYTE iPrev = *pCurPos++;
	while ((pCurPos != pLimitPixel)) {
		if (iPrev != *pCurPos)
			iPrev = *pCurPos++;
		else {
			// - so far, we have a two-length run
			// - check if we reached the end of the scanline
			if ((pLimitPixel - pCurPos) < nRUNTOLERANCE) {
				pCurPos = pLimitPixel;
				break;
			}
			if (_LengthRun(pCurPos, pCurPos + nRUNTOLERANCE) == nRUNTOLERANCE) {
				// the run is greater than the run tolerance
				// -- back up to the beginning of the run
				pCurPos--;
				break;
			} else {
				pCurPos++;
			}
		}
	}
	return (pCurPos - pPixels);
}

inline DWORD 
_LengthCopyPrev(const BYTE *pPixels, const BYTE *pPrevPixels, const BYTE *pLimitPixel)
{
	MMASSERT(pPixels < pLimitPixel);
	// - assume there is no transparency pixel between pPixels and pLimitPixel
	const BYTE *pCurPos = pPixels;
	while ((*pCurPos == *pPrevPixels) && (pCurPos != pLimitPixel)) {
		pCurPos++; pPrevPixels++;
	}
	return pCurPos - pPixels;
}


// Function: ComputeRLE8
// Notes:
//    There are a few ideas that can simplify the way we think about the encoding
//  process.  Each scanline is independent of the others w.r.t. how we encode a
//  particular line.  This means we only really ever have to figure out how best
//  to encode the current line.  We must always code specially for transparency.
HRESULT
CRLESurface::ComputeRLE8(const BYTE *pPixels, long nPitch, int nTransColor)
{
	MMASSERT(pPixels);
	MMASSERT(DWORD(nPitch) >= m_nWidth);
	
	m_iTransColor = 0;
	BOOL bTransColor = FALSE;
	if (INARRAY(nTransColor, 0xff)) {
		bTransColor = TRUE;
		m_iTransColor = (BYTE) nTransColor;
	}

	const BYTE *pCurPos = pPixels, *pNextTrans = NULL;
	const BYTE *pLimitPos = pPixels + m_nHeight * nPitch, *pLineLimit = NULL;
	DWORD nExcessPitch = nPitch - m_nWidth;
	DWORD nRunLength = 0;

	// REVIEW: for now, ignore the copy prev code and just compute all the lines
	//    the same way

	gs_Builder.Start(m_nWidth, m_nHeight);

	while (pCurPos != pLimitPos) {
		gs_Builder.StartLine();

		pLineLimit = pCurPos + m_nWidth;
		if (bTransColor)
			pNextTrans = _FindNextTransPixel(pCurPos, pLineLimit, m_iTransColor);
		else
			pNextTrans = pLineLimit;

		// compute codes for the current scanline
		while (pCurPos != pLineLimit) {
			if (pCurPos == pNextTrans) {
				// add transparency run
				nRunLength = _LengthRun(pCurPos, pLineLimit);
				gs_Builder.AddTransparent(nRunLength);
				pCurPos += nRunLength;
				pNextTrans = _FindNextTransPixel(pCurPos, pLineLimit, m_iTransColor);
			} else {
				// - compute the length of the next run
				// - use the location of the next TransPixel as the limit position
				nRunLength = _LengthRun(pCurPos, pNextTrans);
				if (nRunLength > nRUNTOLERANCE) {
					gs_Builder.AddRun(nRunLength, *pCurPos);
				} else {
					// must be a non-run...
					nRunLength = _LengthNonRun(pCurPos, pNextTrans);
					gs_Builder.AddNonRun(nRunLength, pCurPos);
				}
				pCurPos += nRunLength;
			}
		}
		pCurPos += nExcessPitch;
		gs_Builder.EndLine();
	}
	gs_Builder.End(&m_pData, &m_nDataLength, &m_rgnLineLengths);

	return S_OK;
}

HRESULT
CRLESurface::ComputeRLE4(const BYTE *pPixels, long nPitch, int TransColor)
{
	MMASSERT(pPixels);

	return E_NOTIMPL;
}


/*
HRESULT
CRLESurface::Lock(DWORD dwLockFlags, BYTE *pData)
{
	EnterCriticalSection(&m_csLock);
	if (m_bvFlags & flagRLELOCKED) {
		LeaveCriticalSection(&m_csLock);
		return RLEERR_SURFACEBUSY;
	} else {
		m_bvFlags |= flagRLELOCKED;
	}

	pData = m_pData;

	LeaveCriticalSection(&m_csLock);
	return S_OK;
}

HRESULT
CRLESurface::Unlock()
{
	if ((m_bvFlags & flagRLELOCKED) == 0)
		return RLEERR_NOTLOCKED;
	else
		m_bvFlags ^= flagRLELOCKED;
		
	return E_NOTIMPL;
}
*/
