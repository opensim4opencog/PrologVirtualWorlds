// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	GIFImage.cp
// Author:	MM    (MM)

#include "MMImage.h"

#define MAKENUM(nLo, nHi) (((nHi) << 8) | (nLo))

#define nHEADERSIZE 6
#define nSCREENDESCSIZE 7
#define nIMAGEDESCSIZE 9
#define nPLAINTEXTBLOCKSIZE 12
#define nAPPEXTBLOCKSIZE 11
#define nGRAPHICCONTROLEXTSIZE 6

static char gs_szGIFErrorPrefix[] = "invalid GIF";

CGIFImage::CGIFImage()
{
	MMINITSTRUCT(m_sdLogical);
	m_pFileData = NULL;
	m_pFileLimit = NULL;
	m_pmfibBuilder = NULL;
	m_pInterlacedPixels = m_pFinalPixels = NULL;
}


CGIFImage::~CGIFImage()
{
	m_pFileData = NULL;
	m_pFileLimit = NULL;
	m_pmfibBuilder = NULL;
	MMDELETE(m_pInterlacedPixels);
}


HRESULT
CGIFImage::LoadFrame(const BYTE *pFileData, DWORD nFileLength, CMFImageBuilder *pmfib, int nFrame)
{
	if ((pFileData == NULL) || (pmfib == NULL) || (nFileLength == 0))
		return E_INVALIDARG;

	// REVIEW: store these temporarily so we don't have to pass them as arguments
	//  during parsing -- this is a hacky but...
	m_pFileData = pFileData;
	m_pFileLimit = pFileData + nFileLength;
	m_pmfibBuilder = pmfib;

	HRESULT hr;
	if (FAILED(hr = ReadGIF(nFrame))) {
		MMTRACE("%s: unable to load image\n", gs_szGIFErrorPrefix);
		return hr;
	}	
	return S_OK;
}


HRESULT
CGIFImage::ReadGIF(int nFrame)
{
	// REVIEW: frame skipping was removed
	MMASSERT(nFrame == -1);

	HRESULT hr = S_OK;
	GraphicControlDesc gcd;
	BOOL bClearGCD = TRUE, bSkipBlock = FALSE;
	BYTE uchBlockSep;

	if (FAILED(hr = ReadGlobalHeader()))
		return hr;
	
	//
	// read frame data blocks
	//
	while ((uchBlockSep = *m_pFileData++) != 0x3b) {	// 0x3b == ';'
		CHECKEOF(m_pFileData, m_pFileLimit);

		if (bClearGCD) {
			memset(&gcd, 0, sizeof(gcd));
			bClearGCD = FALSE;
		}
		
		// set up the decoder to skip to the desired frame
//		if (nFrame >= 0) {
//			bSkipBlock = (m_iNext < nFrame);
//			// terminate early if we downloaded the image frame
//			if (m_iNext > nFrame)
//				break;
//		}

		switch (uchBlockSep) {
		case 0x21: 	// 0x21 == '!'
			switch (*m_pFileData++) {
			case 0xF9:	hr = ReadGraphicControlExt(&gcd); break;
			case 0x01:	hr = ReadPlainTextExt(gcd, bSkipBlock); bClearGCD = TRUE; break;
			case 0xFE:	hr = ReadCommentExt(); break;
			case 0xFF:	hr = ReadAppExt(); break;
			default:
				hr = E_NOTIMPL;
				MMTRACE("%s: unknown block label\n", gs_szGIFErrorPrefix);
				break;
			}
			break;
		case 0x2C:	// 0x2C == ','
			hr = ReadImage(gcd, bSkipBlock);
			bClearGCD = TRUE;
			break;
		default:
			hr = E_NOTIMPL;
			MMTRACE("%s: unknown block separator\n", gs_szGIFErrorPrefix);
			break;
		}
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}


HRESULT
CGIFImage::ReadGlobalHeader()
{
	HRESULT hr;

		// header
	if (FAILED(hr = ReadSignature()) ||
		// logical screen
		FAILED(hr = ReadScreenDesc()))
		return hr;

	// get a pointer to the colormap
	LPBYTE prgbPalette = NULL;
	if (m_sdLogical.bGlobalColorMap &&
		FAILED(hr = ReadColorMap(m_sdLogical.cBPPColorMap, (LPRGB *) &prgbPalette)))
	{
		return hr;
	}

	// set up the image builder
	if (FAILED(hr = m_pmfibBuilder->StartImage(m_sdLogical.nWidth, m_sdLogical.nHeight,
						g_pixiRGB, g_pixiPalette8)) ||
		FAILED(hr = m_pmfibBuilder->StartGlobal(m_sdLogical.cBPPColorMap, prgbPalette, 
						0, m_sdLogical.iBackgroundColor, 0)))
	{
		return hr;
	}
	
	return S_OK;
}


HRESULT
CGIFImage::ReadSignature()
{
//	MMTRACE("ReadSignature\n");
	CHECKEOF(m_pFileData + nHEADERSIZE, m_pFileLimit);

	if ((m_pFileData[0] == 'G') && (m_pFileData[1] == 'I') && (m_pFileData[2] == 'F') &&
		(m_pFileData[3] == '8') && ((m_pFileData[4] == '7') || (m_pFileData[4] == '9')) && 
		(m_pFileData[5] == 'a')) {
		m_pFileData += nHEADERSIZE;
		return S_OK;
	}
	MMTRACE("%s: unknown or invalid GIF signature\n", gs_szGIFErrorPrefix);
	return E_NOTIMPL;
}


HRESULT
CGIFImage::ReadScreenDesc()
{
//	MMTRACE("ReadScreenDesc\n");
//	struct RawScreenDesc {
//		BYTE uchLoWidth, uchHiWidth, uchLoHeight, uchHiHeight;
//		BYTE uchPacked, uchBackgroundColor, uchAspectRatio;
//	} rsd;
	CHECKEOF(m_pFileData + nSCREENDESCSIZE, m_pFileLimit);

	m_sdLogical.nWidth = MAKENUM(m_pFileData[0], m_pFileData[1]);
	m_sdLogical.nHeight = MAKENUM(m_pFileData[2], m_pFileData[3]);

	BYTE uchPacked = m_pFileData[4];
	m_sdLogical.bGlobalColorMap = BOOL((uchPacked & 0x80) >> 7);

	if (m_sdLogical.bGlobalColorMap) {
		m_sdLogical.cBPPColorResolution = (int((uchPacked & 0x70) >> 4) + 1);
		m_sdLogical.bSorted = BOOL((uchPacked & 0x08) >> 3);
		m_sdLogical.cBPPColorMap = (int(uchPacked & 0x07) + 1);
		m_sdLogical.iBackgroundColor = m_pFileData[5];
		if (m_sdLogical.iBackgroundColor >= (1 << m_sdLogical.cBPPColorMap)) {
			MMTRACE("%s: invalid background color specified -- ignoring\n", gs_szGIFErrorPrefix);
			m_sdLogical.iBackgroundColor = 0;
		}
	} else {
		// some GIF encoders screw up these numbers, so zero them out
		m_sdLogical.cBPPColorResolution = m_sdLogical.cBPPColorMap = 0;
		m_sdLogical.bSorted = 0;
		m_sdLogical.iBackgroundColor = 0;
	}

	if (m_pFileData[6] == 0)
		m_sdLogical.fAspectRatio = 1.f;
	else
		m_sdLogical.fAspectRatio = float(int(m_pFileData[6]) + 15) / 64.f;
	
	m_pFileData += nSCREENDESCSIZE;

	return S_OK;
}


HRESULT
CGIFImage::ReadColorMap(int nBPP, LPRGB *pprgbPalette)
{
//	MMTRACE("ReadColorMap\n");
	MMASSERT(pprgbPalette && (nBPP <= 8));

	LPRGB prgb = NULL;

	if (nBPP) {
		// store addr where colormap starts -- valid until memmapped file is closed
		prgb = (LPRGB) m_pFileData;
		m_pFileData += (3 * (1 << nBPP));
		CHECKEOF(m_pFileData, m_pFileLimit);
	}

	*pprgbPalette = prgb;

	return S_OK;
}


HRESULT
CGIFImage::ReadGraphicControlExt(GraphicControlDesc *pgcd)
{
//	MMTRACE("ReadGraphicControlExt\n");
//	struct RawGraphicControlBlock {
//		BYTE nBlockSize;
//		BYTE uchPacked, uchDelayTimeLo, uchDelayTimeHi, uchTransparentColor;
//		BYTE nTerminator;
//	} rgcb;
	CHECKEOF(m_pFileData + nGRAPHICCONTROLEXTSIZE, m_pFileLimit);

	// the graphic control extension should be 4 bytes
	if ((m_pFileData[0] != 4) ||
		// the graphic control extension is followed by a zero-length data block
		(m_pFileData[5] != 0)) {
		MMTRACE("%s: invalid graphics control extension\n", gs_szGIFErrorPrefix);
		return E_FAIL;
	}

	// decode and store the graphic control information
	BYTE uchPacked = m_pFileData[1];
	pgcd->uchDisposalMethod = ((uchPacked & 0x1C) >> 2);
	pgcd->bUserInput = ((uchPacked & 0x02) != 0);
	pgcd->bTransparentIndex = ((uchPacked & 0x01) != 0);

	pgcd->wDelay = MAKENUM(m_pFileData[2], m_pFileData[3]);
	pgcd->uchTransparentIndex = m_pFileData[4];

	m_pFileData += nGRAPHICCONTROLEXTSIZE;

	return S_OK;
}


HRESULT 
CGIFImage::ReadPlainTextExt(GraphicControlDesc &gcd, BOOL bSkipBlock)
{
//	MMTRACE("ReadPlainTextExt\n");
	// requires global color map
	if (m_sdLogical.bGlobalColorMap == FALSE) {
		MMTRACE("%s: plain text extensions require a global colormap\n", gs_szGIFErrorPrefix);
		return E_FAIL;
	}
	return SkipToZeroSizedBlock();
}


HRESULT
CGIFImage::ReadAppExt()
{
//	MMTRACE("ReadAppExt\n");
	return SkipToZeroSizedBlock();
}


HRESULT 
CGIFImage::ReadCommentExt()
{
//	MMTRACE("ReadCommentExt\n");
	return SkipToZeroSizedBlock();
}


HRESULT
CGIFImage::SkipToZeroSizedBlock()
{
	DWORD nBytes;
	while (nBytes = *m_pFileData++) {
		m_pFileData += nBytes;
		CHECKEOF(m_pFileData, m_pFileLimit);
	}
	return S_OK;
}


HRESULT
CGIFImage::ReadImage(GraphicControlDesc &gcd, BOOL bSkipBlock)
{
//	MMTRACE("ReadImage\n");
	HRESULT hr;

	// read the image descriptor
	ImageDesc imgd;
	if (FAILED(hr = ReadImageDesc(&imgd)))
		return hr;

	// read the local palette, if it exists
	LPBYTE pRGB = NULL;
	if (imgd.bLocalColorMap && FAILED(hr = ReadColorMap(imgd.cBPP, (LPRGB *) &pRGB)))
		return hr;

	// set up the builder
	LPBYTE pPixels = NULL;
	DWORD nWidth = DWORD(imgd.rDim.right - imgd.rDim.left);
	DWORD nHeight = DWORD(imgd.rDim.bottom - imgd.rDim.top);
	if (bSkipBlock == FALSE) {
		DWORD dwFlags = flagPixAlloc;

		// set up the flags
		if (gcd.bTransparentIndex)
			dwFlags |= flagTRANSPARENT;

		// REVIEW: these fill rules need to be fixed here and in the MFIB
		if (gcd.uchDisposalMethod == 3)
			dwFlags |= flagFrameFillWithFirst;
		else if (gcd.uchDisposalMethod == 1)
			dwFlags |= flagFrameFillWithPrevious;
		else
			dwFlags |= flagFrameFillWithBackground;

		if (FAILED(hr = m_pmfibBuilder->StartFrame(imgd.rDim, DWORD(gcd.wDelay) * 10, 
							imgd.cBPP, pRGB, m_pFinalPixels, gcd.uchTransparentIndex, dwFlags)))
			return hr;

		if (imgd.bInterlaced) {
			if (FAILED(hr = AllocatePixels(nWidth * nHeight, 8, m_pInterlacedPixels)))
				return hr;
			pPixels = m_pInterlacedPixels;
		} else {
			pPixels = m_pFinalPixels;
		}
	}

	// read the image data
	if (FAILED(hr = ReadLZWData(pPixels, imgd, bSkipBlock)))
		return hr;

	// notify the builder that we are done with the frame
	if (bSkipBlock == FALSE) {
		// undo interlacing, if necessary
		if (m_pInterlacedPixels) {
			UndoInterlace(m_pFinalPixels, m_pInterlacedPixels, nWidth, nHeight);
			MMDELETE(m_pInterlacedPixels);
		}
		hr = m_pmfibBuilder->EndFrame();
		m_pFinalPixels = NULL;
	}

	return hr;
}


HRESULT
CGIFImage::UndoInterlace(LPBYTE pDst, LPBYTE pSrc, DWORD nWidth, DWORD nHeight)
{
//	MMTRACE("UndoGIFInterlace\n");
	MMASSERT(pDst && pSrc);

	// assume since we have GIF interlacing that the image is 8 bit
	DWORD nSrcRow, nDstRow;
	for (nSrcRow = 0, nDstRow = 0; nDstRow < nHeight; nDstRow += 8, nSrcRow++)
		memcpy(pDst + (nWidth * nDstRow), pSrc + (nWidth * nSrcRow), nWidth);
	for (nDstRow = 4; nDstRow < nHeight; nDstRow += 8, nSrcRow++)
		memcpy(pDst + (nWidth * nDstRow), pSrc + (nWidth * nSrcRow), nWidth);
	for (nDstRow = 2; nDstRow < nHeight; nDstRow += 4, nSrcRow++)
		memcpy(pDst + (nWidth * nDstRow), pSrc + (nWidth * nSrcRow), nWidth);
	for (nDstRow = 1; nDstRow < nHeight; nDstRow += 2, nSrcRow++)
		memcpy(pDst + (nWidth * nDstRow), pSrc + (nWidth * nSrcRow), nWidth);

	return S_OK;
}


HRESULT
CGIFImage::ReadImageDesc(ImageDesc *pimgd)
{
//	MMTRACE("ReadImageDesc\n");
	//	struct RawImageDesc {
	//		BYTE uchLoLeft, uchHiLeft, uchLoTop, uchHiTop;
	//		BYTE uchLoWidth, uchHiWidth, uchLoHeight, uchHiHeight;
	//		BYTE uchPacked;
	//	} rimgd;
	CHECKEOF(m_pFileData + nIMAGEDESCSIZE, m_pFileLimit);
	
	pimgd->rDim.left = MAKENUM(m_pFileData[0], m_pFileData[1]);
	pimgd->rDim.top = MAKENUM(m_pFileData[2], m_pFileData[3]);
	pimgd->rDim.right = pimgd->rDim.left + MAKENUM(m_pFileData[4], m_pFileData[5]);
	pimgd->rDim.bottom = pimgd->rDim.top + MAKENUM(m_pFileData[6], m_pFileData[7]);

	// decode packed byte
	BYTE uchPacked = m_pFileData[8];
	if (pimgd->bLocalColorMap = BOOL((uchPacked & 0x80) >> 7)) {
		pimgd->cBPP = int(uchPacked & 0x07) + 1;
	} else {
		// use global color map
		pimgd->cBPP = m_sdLogical.cBPPColorMap;
	}
	pimgd->bInterlaced = BOOL((uchPacked & 0x40) >> 6);
	pimgd->bSorted = BOOL((uchPacked & 0x20) >> 5);

	// verify the image position is valid w.r.t. the logical image size
	if ((pimgd->rDim.right > m_sdLogical.nWidth) ||
		(pimgd->rDim.bottom > m_sdLogical.nHeight)) {
		MMTRACE("%s: invalid frame dimensions\n", gs_szGIFErrorPrefix);
		return E_FAIL;
	}
	m_pFileData += nIMAGEDESCSIZE;

	return S_OK;
}


#define nLZWMAXCODESIZE 12
#define nLZWMAXENTRIES (1 << nLZWMAXCODESIZE)

class CStringTable {
public:
					CStringTable(int nLZWMinCodeSize, LPBYTE pPixelLimit);
					~CStringTable() {}

	DWORD			AddEntry(DWORD iBaseCode, BYTE uchNewByte);
	BYTE			WriteString(DWORD iCode, LPBYTE &pPixels) const;
	BOOL			IsEntry(DWORD iCode) const		{ return (iCode < m_iTableNext); }
	void			Reset();

	DWORD			m_cInitEntries;
	DWORD			m_iTableNext;
	struct StringEntry {
		struct StringEntry *pBasePos;
		BYTE				uchNewByte;
	} m_rgString[nLZWMAXENTRIES];
	DWORD			m_rgcLength[nLZWMAXENTRIES];
	LPBYTE			m_pPixelLimit;
};


inline void
CStringTable::Reset()
{ 
	m_iTableNext = m_cInitEntries;
}


CStringTable::CStringTable(int nLZWMinCodeSize, LPBYTE pPixelLimit)
{
	m_cInitEntries = (1 << nLZWMinCodeSize) + 2;
	Reset();
	
	for (WORD i = 0; i < m_cInitEntries; i++) {
		m_rgString[i].pBasePos = m_rgString;
		m_rgString[i].uchNewByte = (BYTE) i;
		m_rgcLength[i] = 1;
	}
	m_pPixelLimit = pPixelLimit;
}


inline DWORD
CStringTable::AddEntry(DWORD iBaseCode, BYTE uchNewByte)
{
	MMASSERT((m_iTableNext < nLZWMAXENTRIES) && (iBaseCode < m_iTableNext));
	StringEntry *pse = m_rgString + m_iTableNext;
	pse->pBasePos = m_rgString + iBaseCode;
	pse->uchNewByte = uchNewByte;
	m_rgcLength[m_iTableNext] = m_rgcLength[iBaseCode] + 1;
	return ++m_iTableNext;
}


inline BYTE
CStringTable::WriteString(DWORD iCode, LPBYTE &pPixels) const
{
	MMASSERT(INARRAY(iCode, nLZWMAXENTRIES));

	LPBYTE pStringLimit = pPixels;
	DWORD cCount = m_rgcLength[iCode];
	LPBYTE pCurPixel = pPixels + cCount;

	if (pCurPixel > m_pPixelLimit) {
		// this is some kind of corrupt write
		MMTRACE("%s: corrupt LZW stream -- recovering\n", gs_szGIFErrorPrefix);
		pPixels = m_pPixelLimit;
		return 0;
	} else {
		pPixels = pCurPixel;
	}

	// use a modified Duff machine to copy pixels
	MMASSERT(cCount != 0);		// the count is at least 1
	DWORD nRemainder = cCount & 0x7;
	const StringEntry *pse = m_rgString + iCode;
	pCurPixel -= nRemainder;
	switch (nRemainder) {
	do {
			pse = pse->pBasePos;
	case 0:	pCurPixel -= 8;
			pCurPixel[7] = pse->uchNewByte; pse = pse->pBasePos;
	case 7: pCurPixel[6] = pse->uchNewByte; pse = pse->pBasePos;
	case 6:	pCurPixel[5] = pse->uchNewByte; pse = pse->pBasePos;
	case 5: pCurPixel[4] = pse->uchNewByte; pse = pse->pBasePos;
	case 4:	pCurPixel[3] = pse->uchNewByte; pse = pse->pBasePos;
	case 3: pCurPixel[2] = pse->uchNewByte; pse = pse->pBasePos;
	case 2:	pCurPixel[1] = pse->uchNewByte; pse = pse->pBasePos;
	case 1: pCurPixel[0] = pse->uchNewByte;
	} while (pCurPixel != pStringLimit);
	} 

	return *pCurPixel;
}


HRESULT
CGIFImage::ReadLZWData(LPBYTE pPixels, const ImageDesc &imgd, BOOL bSkipBlock)
{
//	MMTRACE("ReadLZWData\n");
	HRESULT hr = S_OK;

	// read the first byte of LZW, which describes the initial # of code bits
	BYTE nLZWSetMinCodeSize = *m_pFileData++;

	if (bSkipBlock)
		return SkipToZeroSizedBlock();

	DWORD cPixels = ((imgd.rDim.right - imgd.rDim.left) * (imgd.rDim.bottom - imgd.rDim.top));
	LPBYTE pPixelLimit = pPixels + cPixels;

	// initialize the LZW machinery
	CStringTable stCodes(nLZWSetMinCodeSize, pPixelLimit);
	DWORD nCodeSize = nLZWSetMinCodeSize + 1;
	DWORD codeClear = 1 << nLZWSetMinCodeSize;
	DWORD codeEnd = codeClear + 1, codeBump, codeLast, codeCurrent;

	// prepare to read the compressed bit stream
	CCompressedBlockStreamReader cbsr;

	// - have the cbsr return the "codeEnd" if we hit the end of the data block
	// - we need to do this because some GIF encoders neglect to add the required
	//   "codeEnd" to the end of the LZW data stream
	cbsr.Init(m_pFileData, m_pFileLimit, codeEnd);

	// read the first code - it should be a clear code
	codeCurrent = cbsr.GetCode(nCodeSize);
	if (codeCurrent != codeClear) {
		hr = E_FAIL;
		goto e_ReadLZW;
	}

	// process the rest of the stream of coded pixels -- GetCode returns codeEnd on failure
	for (; codeCurrent != codeEnd; codeCurrent = cbsr.GetCode(nCodeSize)) {
		if (codeCurrent == codeClear) {
			nCodeSize = nLZWSetMinCodeSize + 1;
			codeBump = 1 << nCodeSize;
			stCodes.Reset();

			// get the 2nd code, which should be the first actual pixel after reset
			codeCurrent = cbsr.GetCode(nCodeSize);
			MMASSERT(codeCurrent <= codeClear);
			
			// output the first pixel to the surface
			if (codeCurrent < codeClear)
				*pPixels++ = (BYTE) codeCurrent;

		} else {
			BYTE uchFirstPixel;
			if (stCodes.IsEntry(codeCurrent)) {
				//
				// the pixel string is already in the table
				//
				// write the pixel string to the pixel surface
				uchFirstPixel = stCodes.WriteString(codeCurrent, pPixels);
			} else {
				//
				// the current code is a new entry in the string table
				//
				if (codeCurrent != stCodes.m_iTableNext)
				{
					// We HAVE to exit here or we end up writing in unallocated
					// memory.
					MMTRACE("%s: corrupt LZW stream\n", gs_szGIFErrorPrefix);
					hr = E_FAIL;
					break;
				}
				// write the pixel string to the pixel surface
				uchFirstPixel = stCodes.WriteString(codeLast, pPixels);
				*pPixels++ = uchFirstPixel;
			}

			// add entry to string table
			if (stCodes.AddEntry(codeLast, uchFirstPixel) == codeBump) {
				// some GIF's don't do a codeClear right away once the table is full
				if (nCodeSize != nLZWMAXCODESIZE) {
					nCodeSize++;
					codeBump = 1 << nCodeSize;
				}
			}
		}
		codeLast = codeCurrent;
	}

	m_pFileData = cbsr.GetFilePos();
	MMASSERT(pPixels <= pPixelLimit);

	CHECKEOF(m_pFileData, m_pFileLimit);

	// the last data block should be a zero-sized block
	if (*m_pFileData++ != 0) {
		MMTRACE("%s: expecting block terminator", gs_szGIFErrorPrefix);
		hr = E_FAIL;
	}

e_ReadLZW:
	return hr;
}


//
// CCompressedBlockStreamReader Functions
//

CCompressedBlockStreamReader::CCompressedBlockStreamReader()
{
	m_pFileLimit = NULL;
	m_pFileData = NULL;
	m_pdwCurData = m_pdwLimitData = NULL;
	m_nBitsLeft = 0;
	m_nBytesLeftOver = 0;
}

HRESULT
CCompressedBlockStreamReader::Init(const BYTE *pFileData, const BYTE *pFileLimit, 
								   DWORD codeEndOfDataCode)
{
	m_pFileLimit = pFileLimit;
	m_pFileData = pFileData;
	m_pdwCurData = m_pdwLimitData = m_rgdwDataBuf;
	m_nBitsLeft = 0;
	m_nBytesLeftOver = 0;
	m_codeEndOfDataCode = codeEndOfDataCode;

	return S_OK;
}

DWORD
CCompressedBlockStreamReader::GetCode(DWORD nBits)
{
	MMASSERT((nBits <= nDWORDSIZE) && m_pdwCurData);

	DWORD nCompositionReg, nShiftCompositionReg;

	if (nBits >= m_nBitsLeft) {
		if (m_pdwCurData == m_pdwLimitData) {
			// fill the raw data buffer
			if (FAILED(FillBuffer()))
				return m_codeEndOfDataCode;
			nCompositionReg = 0, nShiftCompositionReg = 0;
		} else {
			nCompositionReg = (*m_pdwCurData++ & (maskDWORD >> (nDWORDSIZE - m_nBitsLeft)));
			nShiftCompositionReg = m_nBitsLeft;
			nBits -= m_nBitsLeft;
			m_nBitsLeft = nDWORDSIZE;
		}
	} else {
		nCompositionReg = 0, nShiftCompositionReg = 0;
	}

	if (nBits != 0) {
		if (m_pdwCurData == m_pdwLimitData) {
			// fill the raw data buffer
			if (FAILED(FillBuffer()))
				return m_codeEndOfDataCode;
		}
		nCompositionReg |= ((*m_pdwCurData & (maskDWORD >> (nDWORDSIZE - nBits))) << nShiftCompositionReg);
		m_nBitsLeft -= nBits;
		*m_pdwCurData >>= nBits;
	}
	return nCompositionReg;
}


HRESULT
CCompressedBlockStreamReader::GetCodes(DWORD nBitsPerCode, DWORD nCodes, LPDWORD rgCodeBuf)
{
	MMASSERT((nBitsPerCode <= nDWORDSIZE) && m_pdwCurData);

	return E_NOTIMPL;
}


HRESULT
CCompressedBlockStreamReader::FillBuffer()
{
	// REVIEW: requires little endian machine

	// we should never get a fill while pointing at a terminator block
	if (*m_pFileData == 0)
		return E_FAIL;

	// copy leftovers from last DWORD into zeroth DWORD
	m_rgdwDataBuf[0] = *m_pdwLimitData;

	// copy new bytes to the data buffer after the leftovers
	LPBYTE pTmp = (LPBYTE) m_rgdwDataBuf;
	MMASSERT((((DWORD) pTmp) & 0x3) == 0);
	pTmp += m_nBytesLeftOver;

	// copy 4 blocks worth of data into the long data buffer
	for (DWORD i = 0, cSize = *m_pFileData++; (i < 4) && cSize; i++, cSize = *m_pFileData++) {
		// make sure we do not pass the end of the file if the data is corrupted
		CHECKEOF(m_pFileData + cSize, m_pFileLimit);
		memcpy(pTmp, m_pFileData, cSize);	// do a "little endian" copy
		pTmp += cSize;
		m_pFileData += cSize;
	}

	// we actually checked one too many block sizes -- this is good because it lets
	// us check if the next block is the terminator block, but we have to put the
	// byte back either way
	m_pFileData--;

	// compute number of bytes stored in the last DWORD
	m_nBytesLeftOver = (((DWORD) pTmp) & 0x3);
	pTmp -= m_nBytesLeftOver;

	// if the next block is the terminator, we want to just use up the leftovers
	if (cSize) {
		m_pdwLimitData = (LPDWORD) pTmp;
	} else {
		m_pdwLimitData = ((LPDWORD) pTmp) + 1;
		// unget the data block terminator
	}

	m_pdwCurData = m_rgdwDataBuf;
	m_nBitsLeft = nDWORDSIZE;

	return S_OK;
}
