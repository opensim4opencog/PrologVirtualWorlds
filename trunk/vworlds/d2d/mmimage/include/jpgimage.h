// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _JPGImage_h
#define _JPGImage_h

// File:	JPGImage.h
// Author:	MM    (MM)
//
// Description:
//    Parse a JPG image into an MFImageBuilder.
// 
// History:
// -@- 7/2/97 (MM) created

class CJPGImage {
public:
						CJPGImage();
						~CJPGImage();

	HRESULT				LoadAll(const BYTE *pFileData, DWORD nFileLength, 
							CMFImageBuilder *pmfib);

private:
	const BYTE *		m_pFileData;
	const BYTE *		m_pFileLimit;
	CMFImageBuilder *	m_pmfibBuilder;
};

#endif
