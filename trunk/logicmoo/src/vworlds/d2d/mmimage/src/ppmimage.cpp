// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	PPMImage.cpp
// Author:	MM    (MM)

#include "MMImage.h"

static char gs_szPPMErrorPrefix[] = "invalid PPM";


CPPMImage::CPPMImage()
{
}


CPPMImage::~CPPMImage()
{
}


HRESULT
CPPMImage::LoadAll(const BYTE *pFileData, DWORD nFileLength, CMFImageBuilder *pmfib)
{
	HRESULT hr;

	if ((pFileData == NULL) || (pmfib == NULL))
		return E_INVALIDARG;

	// REVIEW: store these temporarily so we don't have to pass them as arguments
	//  during parsing -- this is a hacky but...
	m_pFileData = pFileData;
	m_pFileLimit = pFileData + nFileLength;
	m_pmfibBuilder = pmfib;

	if (FAILED(hr = ReadPPM())) {
		MMTRACE("%s: unable to load image\n", gs_szPPMErrorPrefix);
		return hr;
	}
	return S_OK;
}


HRESULT
CPPMImage::ReadPPM()
{
	HRESULT hr;

	if (FAILED(hr = ReadHeader()))
		return hr;

	LPBYTE pPalette = NULL;
	RECT rDim;

	if (FAILED(hr = m_pmfibBuilder->StartImage(m_nWidth, m_nHeight, g_pixiNULL, g_pixiRGB)) ||
		FAILED(hr = m_pmfibBuilder->StartGlobal(24, pPalette, 0, 0, 0)))
		return hr;

	// read the pixels
	LPBYTE pPixels = (LPBYTE) m_pFileData;
	m_pFileData += m_nWidth * m_nHeight * 3;

	if (m_pFileData != m_pFileLimit)
		MMTRACE("PPM warning: expecting EOF\n");

	SetRect(&rDim, 0, 0, m_nWidth, m_nHeight);

	if (FAILED(hr = m_pmfibBuilder->StartFrame(rDim, 0, 24, pPalette, pPixels, 0, 0)) ||
		FAILED(hr = m_pmfibBuilder->EndFrame()))
		return hr;

	return S_OK;
}


HRESULT
CPPMImage::ReadHeader()
{
	HRESULT hr;
	const BYTE *pMark = m_pFileData;

	// check magic number
	m_pFileData += 2;
	CHECKEOF(m_pFileData, m_pFileLimit);
	if ((pMark[0] != 'P') || (pMark[1] != '6')) {
		MMTRACE("%s: can only decode binary PPM\n", gs_szPPMErrorPrefix);
		return E_NOTIMPL;
	}

	if (FAILED(hr = SkipCommentsAndSpaces()))
		return hr;

	// read dimensions
	m_nWidth = atoi((const char *) m_pFileData);
	if (FAILED(hr = SkipToWhiteSpace()) || FAILED(hr = SkipCommentsAndSpaces()))
		return hr;
	m_nHeight = atoi((const char *) m_pFileData);
	if (FAILED(hr = SkipToWhiteSpace()) || FAILED(hr = SkipCommentsAndSpaces()))
		return hr;
	m_nMaxColor = atoi((const char *) m_pFileData);
	if (FAILED(hr = SkipToWhiteSpace()))
		return hr;
	// skip single white space character following the maxval
	m_pFileData++;
	CHECKEOF(m_pFileData, m_pFileLimit);

	if ((m_nWidth <= 0) || (m_nHeight <= 0) || (m_nMaxColor > 255)) {
		MMTRACE("%s: invalid image dimensions\n", gs_szPPMErrorPrefix);
		return E_UNEXPECTED;
	}

	return S_OK;
}


HRESULT
CPPMImage::SkipCommentsAndSpaces()
{
	while ((*m_pFileData == '#') || isspace(*m_pFileData)) {
		if (*m_pFileData == '#') {
			// skip comment
			LPBYTE pNewLine = (LPBYTE) memchr(m_pFileData, '\n', m_pFileLimit - m_pFileData);
			if (pNewLine == NULL) {
				MMTRACE("%s: invalid comment\n", gs_szPPMErrorPrefix);
				return E_UNEXPECTED;
			}
			m_pFileData = pNewLine + 1;
			CHECKEOF(m_pFileData, m_pFileLimit);
		} else {
			// skip whitespace
			m_pFileData++;
			CHECKEOF(m_pFileData, m_pFileLimit);
		}
	}
	return S_OK;
}


HRESULT
CPPMImage::SkipToWhiteSpace()
{
	while (!isspace(*m_pFileData)) {
		m_pFileData++;
		CHECKEOF(m_pFileData, m_pFileLimit);
	}
	return S_OK;
}
