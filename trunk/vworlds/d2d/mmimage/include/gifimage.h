// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _GIFImage_h
#define _GIFImage_h

// File:	GIFImage.h
// Author:	MM    (MM)
//
// Description:
//    Parse the 89a GIF standard.
// 
// History:
// -@- 10/11/96 (MM) created

class CMFImageBuilder;

struct ImageDesc {
	RECT	rDim;
	BOOL	bLocalColorMap;
	BOOL	bInterlaced;
	BOOL	bSorted;
	int		cBPP;
};

struct GraphicControlDesc {
	BOOL	bUserInput;
	BOOL	bTransparentIndex;
	WORD	wDelay;
	BYTE 	uchTransparentIndex;
	BYTE	uchDisposalMethod;
};

class CGIFImage {
public:
					CGIFImage();
					~CGIFImage();

	HRESULT			LoadFrame(const BYTE *pFileData, DWORD nFileLength, CMFImageBuilder *pmfib, int nFrame);
	HRESULT			LoadAll(const BYTE *pFileData, DWORD nFileLength, CMFImageBuilder *pmfib);
	
private:
	struct ScreenDesc {
		int		cBPPColorMap;			// bit depth of global color table
		int		cBPPColorResolution;	// bit depth of the original image color palette
		BOOL	bSorted;				// is the global color table priority sorted?
		WORD	nWidth;
		WORD	nHeight;
		BOOL	bGlobalColorMap;
		BYTE	iBackgroundColor;
		float	fAspectRatio;			// width / height  [4:1..1:4]
	} m_sdLogical;
	const BYTE *			m_pFileData;
	const BYTE *			m_pFileLimit;
	CMFImageBuilder *		m_pmfibBuilder;
	LPBYTE					m_pFinalPixels;
	LPBYTE					m_pInterlacedPixels;

private:
	HRESULT ReadGIF(int nFrame);
	HRESULT ReadGlobalHeader();
	HRESULT ReadSignature();
	HRESULT ReadScreenDesc();
	HRESULT ReadColorMap(int nBPP, LPRGB *pprgbPalette);
	HRESULT ReadGraphicControlExt(GraphicControlDesc *pgcd);
	HRESULT ReadPlainTextExt(GraphicControlDesc &gcd, BOOL bSkipBlock);
	HRESULT ReadCommentExt();
	HRESULT ReadAppExt();
	HRESULT SkipToZeroSizedBlock();
	HRESULT ReadImage(GraphicControlDesc &gcd, BOOL bSkipBlock);
	HRESULT	UndoInterlace(LPBYTE pDstPixels, LPBYTE pSrcPixels, DWORD nWidth, DWORD nHeight);
	HRESULT ReadImageDesc(ImageDesc *pimgd);
	HRESULT ReadLZWData(LPBYTE pPixels, const ImageDesc &imgd, BOOL bSkipBlock);
	HRESULT UndoInterlace(LPBYTE pDstSurface, LPBYTE pSrcSurface);
};

inline HRESULT
CGIFImage::LoadAll(const BYTE *pFileData, DWORD nFileLength, CMFImageBuilder *pmfib)
{ 
	return LoadFrame(pFileData, nFileLength, pmfib, -1);
}


#define nDWORDSIZE (NUMBITS(DWORD))

// class: CCompressedBlockStreamReader
// ***Hungarian: cbsr
class CCompressedBlockStreamReader {
public:
					CCompressedBlockStreamReader();
					~CCompressedBlockStreamReader() {}

	HRESULT			Init(const BYTE *pFileData, const BYTE *pFileLimit, DWORD nEndOfDataCode);

	DWORD			GetCode(DWORD nBits);
 	HRESULT			GetCodes(DWORD nBitsPerCode, DWORD nCodes, LPDWORD rgCodeBuf);

	const BYTE *	GetFilePos() { return m_pFileData; }
	
private:
	HRESULT			FillBuffer();
	
	const BYTE *	m_pFileData;
	const BYTE *	m_pFileLimit;

	LPDWORD			m_pdwCurData;
	LPDWORD			m_pdwLimitData;
	DWORD			m_rgdwDataBuf[256];

	// stuff need for buffer fill
	DWORD			m_nBytesLeftOver;
	DWORD			m_nBitsLeft;		// number of bits left in current byte
	DWORD			m_codeEndOfDataCode;
};

#endif
