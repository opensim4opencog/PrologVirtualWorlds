// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	BMPImage.cpp
// Author:	MM    (MM)

#include "MMImage.h"

#define nBITMAPFILEHEADERSIZE	sizeof(BITMAPFILEHEADER)
#define nBITMAPINFOHADERSIZE	sizeof(BITMAPINFOHEADER)

char g_szBMPErrorPrefix[] = "invalid BMP";

PALETTEENTRY CBMPImage::mg_rgpePalette[nMAXPALETTEENTRIES];

CBMPImage::CBMPImage()
{
	m_nWidth = m_nHeight = 0;
	m_nBPP = 0;
	m_bTopDown = FALSE;
	m_pFileData = m_pFileLimit = NULL;
	m_pmfibBuilder = NULL;
	m_pPalette = NULL;
	m_pPixels = NULL;
	m_pMasks = NULL;
	m_pPixels = NULL;
}


CBMPImage::~CBMPImage()
{
}


HRESULT
CBMPImage::LoadAll(const BYTE *pFileData, DWORD nFileLength, CMFImageBuilder *pmfib)
{
	if ((pFileData == NULL) || (pmfib == NULL) || (nFileLength == 0))
		return E_INVALIDARG;

	// REVIEW: store these temporarily so we don't have to pass them as arguments
	//  during parsing -- this is a hacky but...
	m_pFileData = pFileData;
	m_pFileLimit = pFileData + nFileLength;
	m_pmfibBuilder = pmfib;

	HRESULT hr;
	if (FAILED(hr = ReadBMP())) {
		MMTRACE("%s: unable to load image\n", g_szBMPErrorPrefix);
		return hr;
	}
	return S_OK;
}


HRESULT
CBMPImage::ReadBMP()
{
	HRESULT hr;

	if (FAILED(hr = ReadFileHeader()) ||
		FAILED(hr = ReadInfoHeader()))
		return hr;

	RECT rFrame = {0, 0, m_nWidth, m_nHeight};

	if (FAILED(hr = m_pmfibBuilder->StartImage(m_nWidth, m_nHeight, m_pixiPalFmt, m_pixiPixFmt)) ||
		FAILED(hr = m_pmfibBuilder->StartGlobal(m_nBPP, m_pPalette, 0, 0, 0)) ||
		FAILED(hr = m_pmfibBuilder->StartFrame(rFrame, 0, m_nBPP, m_pPalette, m_pPixels, 0, flagPixAlloc)))
		return hr;

	if (FAILED(hr = ReadPixels()))
		return hr;

	return m_pmfibBuilder->EndFrame();
}


HRESULT
CBMPImage::ReadFileHeader()
{
	BITMAPFILEHEADER *pbmfh = (BITMAPFILEHEADER *) m_pFileData;

	// if this is a file, it will have file header info 
	// -- o.w. it is a resource and does not have a file header
	if (pbmfh->bfType == 0x4d42) {
		// skip the file header info
		m_pFileData += nBITMAPFILEHEADERSIZE;
		CHECKEOF(m_pFileData, m_pFileLimit);
	}
	return S_OK;
}


HRESULT
CBMPImage::ReadInfoHeader()
{
	// find the size of the header
	DWORD nPaletteSize = 0;
	DWORD dwHeaderSize = *((DWORD *) m_pFileData);
	const BYTE *pHeaderMark = m_pFileData;

	// advance the file data pointer
	m_pFileData += dwHeaderSize;
	CHECKEOF(m_pFileData, m_pFileLimit);

	if (dwHeaderSize == sizeof(BITMAPCOREHEADER)) {
		BITMAPCOREHEADER *pbmch = (BITMAPCOREHEADER *) pHeaderMark;
		m_bTopDown = (pbmch->bcHeight < 0);
		m_nWidth = pbmch->bcWidth;
		m_nHeight = (m_bTopDown ? -pbmch->bcHeight : pbmch->bcHeight);
		m_nBPP = (BYTE) pbmch->bcBitCount;
		m_pixiPalFmt = g_pixiRGBTRIPLE;

		// if the BMP is palettized, the palette immediately follows the info header
		if (m_nBPP <= 8) {
			nPaletteSize = ((1 << m_nBPP) * sizeof(RGBTRIPLE));
			m_pPalette = (LPBYTE) m_pFileData;
			m_pixiPixFmt.Init(m_nBPP);
		} else {
			m_pixiPixFmt = g_pixiBGR;
		}

	} else if (dwHeaderSize == sizeof(BITMAPINFOHEADER)) {
		BITMAPINFOHEADER *pbmih = (BITMAPINFOHEADER *) pHeaderMark;
		m_bTopDown = (pbmih->biHeight < 0);
		m_nWidth = pbmih->biWidth;
		m_nHeight = (m_bTopDown ? -pbmih->biHeight : pbmih->biHeight);
		m_nBPP = (BYTE) pbmih->biBitCount;
		
		DWORD cEntries = 0;
		
		switch (pbmih->biCompression) {
		case BI_BITFIELDS:
			MMASSERT((m_nBPP == 16) || (m_nBPP == 32));
			m_pMasks = (struct RGBMasks *) m_pFileData;
			// skip the RGB masks
			m_pFileData += 3 * sizeof(DWORD);
			CHECKEOF(m_pFileData, m_pFileLimit);
			m_pixiPixFmt.Init(m_nBPP, m_pMasks->r, m_pMasks->g, m_pMasks->b, 0);
			break;
		case BI_RGB:
			cEntries = pbmih->biClrUsed;
			switch (m_nBPP) {
			case 16:
				// default color format for 16 bit is 5:5:5
				m_pixiPixFmt.Init(m_nBPP, 0x7C00, 0x03E0, 0x001F, 0);
				break;
			case 24:
			case 32:
				// default color format for 24 and 32 bit is BGR
				m_pixiPixFmt = g_pixiBGR;
				break;
			default:
				if (cEntries == 0) {
					cEntries = (1 << m_nBPP);
					m_pPalette = (LPBYTE) m_pFileData;
					m_pixiPalFmt = g_pixiRGBQUAD;
				} else {
					// copy the entries into the palette buffer
					BltFastRGBToRGB((LPBYTE) m_pFileData, 0, (LPBYTE) mg_rgpePalette, 0, cEntries, 1, 
						g_pixiRGBQUAD, g_pixiPALETTEENTRY);
					ZeroDWORDAligned((LPDWORD) mg_rgpePalette + cEntries, nMAXPALETTEENTRIES - cEntries);

					m_pPalette = (LPBYTE) mg_rgpePalette;
					m_pixiPalFmt = g_pixiPALETTEENTRY;
				}
				m_pixiPixFmt.Init(m_nBPP);
				break;
			}
			break;
		default:
			MMTRACE("%s: BMP RLE decompression not implemented\n", g_szBMPErrorPrefix);
			return E_NOTIMPL;
			break;
		}
		if (cEntries) {
			m_pFileData += (cEntries * sizeof(RGBQUAD));
			CHECKEOF(m_pFileData, m_pFileLimit);
		}
	} else {
		MMTRACE("%s: unknown info header\n", g_szBMPErrorPrefix);
		return E_FAIL;
	}

	return S_OK;
}


HRESULT
CBMPImage::ReadPixels()
{
	HRESULT hr;
	LPBYTE pFilePixels = (LPBYTE) m_pFileData;

	DWORD nBytesPerPixel = (m_nBPP + 7) >> 3;
	m_nWidth *= nBytesPerPixel;

	// compute pitch given that each scanline must be DWORD aligned
	DWORD nSrcPitch = (m_nWidth + 3) & ~3;
	m_pFileData += nSrcPitch * m_nHeight;

	// KM: changed to check for overflow, OK if <= file limit
	if (m_pFileData > m_pFileLimit) {
		MMTRACE("%s: expecting EOF\n", g_szBMPErrorPrefix);
		return E_UNEXPECTED;
	}

	if (m_bTopDown)
		hr = BltFast(pFilePixels, nSrcPitch, m_pPixels, m_nWidth, 
				m_nWidth, m_nHeight);
	else
		hr = BltFastMirrorY(pFilePixels, nSrcPitch, m_pPixels, m_nWidth, 
				m_nWidth, m_nHeight);

	return hr;
}
