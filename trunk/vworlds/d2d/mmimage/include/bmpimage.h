// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _BMPImage_h
#define _BMPImage_h

// File:	BMPImage.h
// Author:	MM    (MM)
//
// Description:
//    Parse a BMP image into an MFImageBuilder.
// 
// History:
// -@- 2/5/97 (MM) created

class CBMPImage {
public:
						CBMPImage();
						~CBMPImage();

	HRESULT				LoadAll(const BYTE *pFileData, DWORD nFileLength, 
							CMFImageBuilder *pmfib);

private:
	HRESULT				ReadBMP();
	HRESULT				ReadFileHeader();
	HRESULT				ReadInfoHeader();
	HRESULT				ReadPixels();

private:
	DWORD				m_nWidth, m_nHeight;
	LPBYTE				m_pPalette;
	BYTE				m_nBPP, m_nBPPUsed;
	CPixelInfo			m_pixiPalFmt;

	LPBYTE				m_pPixels;
	CPixelInfo			m_pixiPixFmt;

	BOOL				m_bTopDown;
	struct RGBMasks {
		DWORD r, g, b;
	} *m_pMasks;

	static PALETTEENTRY	mg_rgpePalette[nMAXPALETTEENTRIES];

private:
	const BYTE *		m_pFileData;
	const BYTE *		m_pFileLimit;
	CMFImageBuilder *	m_pmfibBuilder;
};

#endif
