// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _PPMImage_h
#define _PPMImage_h

// File:	PPMImage.h
// Author:	MM    (MM)
//
// Description:
//    Parse a PPM image into an MFImageBuilder.
// 
// History:
// -@- 7/2/97 (MM) created

class CPPMImage {
public:
						CPPMImage();
						~CPPMImage();

	HRESULT				LoadAll(const BYTE *pFileData, DWORD nFileLength, 
							CMFImageBuilder *pmfib);

private:
	HRESULT				ReadPPM();
	HRESULT				ReadHeader();
	HRESULT				SkipCommentsAndSpaces();
	HRESULT				SkipToWhiteSpace();

private:
	const BYTE *		m_pFileData;
	const BYTE *		m_pFileLimit;
	CMFImageBuilder *	m_pmfibBuilder;

	int					m_nWidth;
	int					m_nHeight;
	int					m_nMaxColor;
};

#endif
