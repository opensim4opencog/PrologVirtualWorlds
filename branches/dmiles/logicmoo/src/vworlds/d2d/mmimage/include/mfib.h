// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _MFIB_h
#define _MFIB_h

// File:	MFIB.h
// Author:	MM    (MM)
//
// Description:
//    This defines the multiframe image builder class that constructs MFImages.

#define nMAXIMAGEDIM 0xFFFF

#define flagPalDealloc				0x100
#define flagPalAlloc				(0x200 | flagPalDealloc)
#define flagPixDealloc				0x400
#define flagPixAlloc				(0x800 | flagPixDealloc)

#define flagFrameFillWithBackground	0x1000
#define flagFrameFillWithPrevious	0x2000
#define flagFrameFillWithFirst		0x4000
#define flagFrameFillWithStretch	0x8000
#define flagFrameFillIgnoreTrans	0x10000

#define flagUsageD3DTextureEnabled	0x20000
#define flagUsageD3DTextureOnly		(flagUsageD3DTextureEnabled | 0x40000)
#define flagUsageResizeTrunc		0x100000
#define flagUsageResizeFillBG		0x200000
#define flagUsageResizeFillTrans	0x400000
#define flagUsageResizeStretch		0x800000
#define flagUsageResizeCenter		0x1000000
#define flagUsageRLE				0x10000000
#define flagUsageMapStatically		0x20000000
#define flagUsageVerifyTransparency	0x40000000
#define flagUsageShareOnePalette	0x80000000

// REVIEW: clean up where all of this stuff is stored!!!
class CGlobalBuilderInfo {
	friend class CMFImageBuilder;
public:
	WORD				nWidth, nHeight;
	CPixelInfo			pixiPalFmt;			// fmt for content palettes
	CPixelInfo			pixiPixFmt;			// fmt for content pixels
	BYTE				cBytesPerPixel;		// bytes per pixel for content pixels

	DWORD				dwFlags;

	// content palette info
	LPBYTE				pPalette;			// def palette for image (might be memmap file ptr)
	BYTE				nBPPPalette;		// bpp of the global palette


	DWORD				dwTransColor;		// def (packed || index) content transparency color
	DWORD				dwBackgroundColor;	// def (packed || index) content background color for fills

	BOOL				bGlobalColorModel;
	BOOL				bVideoModeDependent;

	// information for constructing the final image
	CPixelInfo			pixiFinalFmt;
	WORD				nFinalWidth, nFinalHeight;
	BOOL				bFinalFmtSame;
	LPCOLORINFO			pciFinal;
	DWORD				dwFinalTransColor;
	DWORD				dwFinalBackgroundColor;
public:
	void				Reset();

						CGlobalBuilderInfo();
						~CGlobalBuilderInfo();
};


class CFrameBuilderInfo {
	friend class CMFImageBuilder;
public:
	RECT				rDim;			// frame dimensions
	WORD				nWidth, nHeight;// store these for convenience
	DWORD				nDelay;
	DWORD				dwFlags;
	DWORD				dwTransColor;	// (packed || index) transparency color key

	// content palette info
	LPBYTE				pPalette;		// palette for frame (might be memmap file ptr)
	BYTE				nBPPPalette;	// bpp of the frame palette

	// content pixel info
	LPBYTE				pPixels;		// ptr to allocated pixels (might point to DDS or memmap file)
	DWORD				nPitch;

	BOOL				bUseRLE;
	BOOL				bFullSize;

	BOOL				bDDSLocked;
	LPBYTE				pSurfacePixels;
	LPDIRECTDRAWSURFACE	pdds;
	CRLESurface *		prles;
public:
	void				Reset();

						CFrameBuilderInfo();
						~CFrameBuilderInfo();
};
typedef CFrameBuilderInfo *LPFBI;

// Construction Methods:
typedef enum {conmNone = 0, conmInit, conmInteractive, conmExisting} ConstructionMethod;


// class: CMFImageBuilder
//    This class is used to construct multi-frame images from image files.  It is best
//  to use the same builder to construct many images, since buffers are cached and less
//  memory fragmentation will occur.  Just call Reset() between images.
class CMFImageBuilder {
public:
						CMFImageBuilder();
						~CMFImageBuilder();

	HRESULT				Init(LPDIRECTDRAW pDD, CColorManager *pColMgr);

	// preferences
	HRESULT				Setup(DWORD dwUsageFlags);

	//
	// image construction
	//
	// common construction calls
	HRESULT				Reset(const char *szName);

	HRESULT				StartImage(DWORD nWidth, DWORD nHeight, 
							const CPixelInfo &pixiPalFmt, const CPixelInfo &pixiPixFmt);
	
	// Method 1:
	// interactive frame construction for image loaders
	HRESULT				StartGlobal(BYTE nBPPPalette, LPBYTE &pPalette, 
							DWORD dwTransColor, DWORD dwBackgroundColor, DWORD dwFlags);
	HRESULT				StartFrame(const RECT &rDstRect, DWORD nFrameDelay, BYTE nBPPPalette, 
							LPBYTE &pPalette, LPBYTE &pPixels, DWORD dwTransColor, DWORD dwFlags);
	HRESULT				EndFrame();

	// Method 2:
	// frame construction from existing surfaces
	HRESULT				AddFrame(LPDIRECTDRAWSURFACE pdds, LPCOLORINFO pci, DWORD nFrameDelay);

	// image generation
	HRESULT				Compile(CMFImage **ppmfi, BOOL *pbVideoModeDependent);

private:
	HRESULT				ComputeFinalPixelInfo();
	HRESULT				ComputeFinalColorModel();
	HRESULT				CreateFinalPixels(LPFBI pfbi);
	HRESULT				ComputeRLE(LPFBI pfbi);

private:
	LPDIRECTDRAW		m_pDD;
	CColorManager *		m_pColMgr;

	char *				m_szName;
	CGlobalBuilderInfo	m_gbi;
	ConstructionMethod	m_conm;
	DSet<LPFBI>			m_dspfbiFrames;
	LPFBI				m_pfbiCur;			// current frame

	// REVIEW: this is a total HACK!!!
	CPaletteMap			m_pmFinal;


	// compile preferences
	DWORD				m_dwUsageFlags;
};


inline
CGlobalBuilderInfo::CGlobalBuilderInfo()
{
	memset(this, 0, sizeof(*this));
}

inline
CGlobalBuilderInfo::~CGlobalBuilderInfo()
{
	Reset();
}


inline
CFrameBuilderInfo::CFrameBuilderInfo()
{
	memset(this, 0, sizeof(*this));
}

inline
CFrameBuilderInfo::~CFrameBuilderInfo()
{
	Reset();
}


#endif
