// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.


#include "stdafx.h"

#include "d2d.h"
#include "d2d_i.c"
#include <vwobject.h>
#include <afxtempl.h>
#include <propbase.h>
#include "ddutil.h"
#include "cutils.h"
#include <inetfile.h>
#include <graphtls.h>
#include <vwgeomsp.h>
#include <math.h>
#include <spritemn.h>

// define the sprite geometry IID
#define IID_DEFINED
#include "vwgeomsp_i.c"
#include "spritemn_i.c"

LPBITMAPINFOHEADER DibCreate(int bits, int dx, int dy);
BOOL DibSetUsage(LPBITMAPINFOHEADER pdib, IDirectDrawPalette *pDDPalette,UINT wUsage);

COLORREF TranslateColor( COLORREF clr, IDirectDrawPalette *pDDPalette );


HRESULT GetThumbNailName(IWorld *pWorld, IThing* pthing, BSTR *pbstr );
HRESULT
CreatePlainSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, DWORD nBPP,
				   const DDPIXELFORMAT *pddpf, LPDIRECTDRAWPALETTE pPalette,void*data, 
				   LPDIRECTDRAWSURFACE *ppdds);
HRESULT
CopyPixelsDDSTo24( LPBITMAPINFOHEADER lpbi, LPDIRECTDRAWSURFACE pddsSrc );

#ifndef MMRELEASE
#define MMRELEASE(_p) do { if ((_p) != NULL) {(_p)->Release(); (_p) = NULL; }} while (0)
#endif

HRESULT GetBitmapFromThing(IWorld *pWorld, IThing *pThing, COLORREF rgbBackColor, LPBITMAPINFOHEADER *ppDIB)
{
HRESULT hr = S_OK;
IDirectDraw *pDD = NULL;
IDirect2DRM *pD2DRM = NULL;
LPDIRECT2DRMIMAGE pd2di = NULL;
IDirectDrawSurface *pdds = NULL;
LPBITMAPINFOHEADER pDIB = NULL;
IDirectDrawPalette *pDDPalette = NULL;
IInternetFileManager* pINetFileManager = NULL;
CComBSTR bstrName;
BSTR bstrFullURL = NULL, bstrFinalPath = NULL;
VARIANT_BOOL bFound;
DWORD width, height, depth;
DDBLTFX ddbfx;
VARIANT_BOOL bValid = VARIANT_FALSE;

// 1) get D2D tool....

	// Create the graphics tools and put them into the world if they haven't been already
	// by another OCX, otherwise just get them from the world.
	if (FAILED(hr = CreateGraphicsTools(pWorld, &pDD, &pD2DRM, NULL, NULL)))
		goto ERROR_ENCOUNTERED;

// 2) get imagefile name
	hr = GetThumbNailName(pWorld, pThing, &(bstrName.m_str) );
	if (FAILED(hr))	
		goto ERROR_ENCOUNTERED;

// Make sure it has a length...
	if ( bstrName.Length( )  == 0 )
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

// get the downloaded name
	hr = pWorld->get_Tool(CComBSTR("Inetfile"),(IUnknown**) &pINetFileManager);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = pINetFileManager->FindCachedFile(bstrName, &bstrFullURL, &bstrFinalPath, &bFound)) || bFound == VARIANT_FALSE)
	{
		hr = pINetFileManager->GetFileEx(bstrName, VARIANT_FALSE, &bstrFullURL, &bstrFinalPath);
		if ( FAILED(hr))
		{
			//Maybe it is already a fully qualified path?
			bstrFinalPath = bstrName.Copy();
		//	goto ERROR_ENCOUNTERED;
		}
	}
	

// 3) get D2Dimage
	hr = pD2DRM->CreateImageFromImageFile(bstrFinalPath, flagCREATE_IGNOREANIMATION, &pd2di);
	if (FAILED(hr ))
		goto ERROR_ENCOUNTERED;

// 4) create surface
	hr = pd2di->get_Width(&width);
	if (FAILED(hr ))
		goto ERROR_ENCOUNTERED;

	hr = pd2di->get_Height(&height);
	if (FAILED(hr ))
		goto ERROR_ENCOUNTERED;

	hr = pd2di->get_Depth(&depth);
	if (FAILED(hr ))
		goto ERROR_ENCOUNTERED;

	pDIB = DibCreate(depth, width, -((long)height));
	if (!pDIB )
		goto ERROR_ENCOUNTERED;


	hr = pd2di->CreateFittedSurface( &pdds );
	if (FAILED(hr ))
		goto ERROR_ENCOUNTERED;

	// we are in palette mode
	hr = pdds->GetPalette(&pDDPalette);
	if (SUCCEEDED(hr) && pDDPalette)
	{
		rgbBackColor = TranslateColor( rgbBackColor, pDDPalette );
		DibSetUsage(pDIB, pDDPalette,DIB_RGB_COLORS);
	}

// 5) Clear to background color
	memset(&ddbfx, 0, sizeof(DDBLTFX));
	ddbfx.dwSize = sizeof(DDBLTFX);
	ddbfx.dwFillColor = rgbBackColor;

	hr = pdds->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbfx);
	if (FAILED(hr ))
		goto ERROR_ENCOUNTERED;
	

	hr = pd2di->BltToSurface(NULL,pdds,NULL,flagBLT_USEDYNAMICMAPPING|DDBLT_KEYSRC);
	if (FAILED(hr ))
		goto ERROR_ENCOUNTERED;
// 6) Create bitmap of the right size

// 7) copy the pixels
	hr = CopyPixelsDDSTo24( pDIB, pdds);
	if (!pDIB )
		goto ERROR_ENCOUNTERED;

	*ppDIB = pDIB;
	pDIB = NULL;


ERROR_ENCOUNTERED:

	SAFERELEASE( pDD );
	SAFERELEASE( pD2DRM );
	SAFERELEASE( pd2di);
	SAFERELEASE( pdds );
	SAFERELEASE( pDDPalette );
	SAFERELEASE( pINetFileManager );
	SAFEFREESTRING(bstrFullURL );
	SAFEFREESTRING(bstrFinalPath);

	if ( pDIB ) free( pDIB );
	return hr;
}

HBITMAP DIBToBitmap(int cx, int cy, int depth, LPBITMAPINFOHEADER pDIB, int nFlags )
{
	HBITMAP hbm = NULL;
	HANDLE hOldDIB = NULL;
	HDC hdcMem = NULL;
	HDC hDC = NULL;
	RGBQUAD FAR * pRgb= DibColors(pDIB);
	int nColors = DibNumColors(pDIB);
	BITMAPINFO *pbmi = NULL;
	int i;

	pbmi = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFOHEADER) + nColors * sizeof(RGBQUAD)];
	if (pbmi == NULL)
		goto ERROR_ENCOUNTERED;

	// specify bitmip info
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biSizeImage = 0;
	pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biClrImportant = 0;
	pbmi->bmiHeader.biBitCount = (WORD) depth;
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biWidth = (LONG) cx;
	pbmi->bmiHeader.biHeight = -(LONG) cy;

	// copy palette into bmi
	for(i = 0; i < nColors; i++) {
		pbmi->bmiColors[i].rgbRed = pRgb[i].rgbRed;
		pbmi->bmiColors[i].rgbGreen= pRgb[i].rgbGreen;
		pbmi->bmiColors[i].rgbBlue = pRgb[i].rgbBlue;
		pbmi->bmiColors[i].rgbReserved = 0;
	}

	// Get a DC
	hDC = GetDC(NULL);
	if (!hDC)
		goto ERROR_ENCOUNTERED;
	// create bitmap
	hbm = ::CreateDIBSection(hDC, pbmi, DIB_RGB_COLORS, NULL, NULL, 0);
	if (!hbm) 
		goto ERROR_ENCOUNTERED;

	hdcMem = ::CreateCompatibleDC(hDC);
	if (!hdcMem)
		goto ERROR_ENCOUNTERED;

	hOldDIB =::SelectObject(hdcMem, hbm);

	i = ::StretchDIBits(hdcMem,0, 0,cx,cy,
								0,0,DibWidth(pDIB),DibHeight(pDIB), 
								DibPtr(pDIB), DibInfo(pDIB),
								DIB_RGB_COLORS, SRCCOPY);

	switch (nFlags)
	{
		//friend
		case 1:
		{
			LOGBRUSH logbrush = { BS_SOLID,RGB(255,0,0),0};
			RECT rect = {0,0,cx,cy};
			HBRUSH hbrush = ::CreateBrushIndirect(&logbrush);
			::FrameRect( hdcMem, &rect, hbrush ); 
			::DeleteObject(hbrush);
		} 
		break;
		case 2:
		{
			HPEN hpen = ::CreatePen(PS_SOLID,3,RGB(0,0,0));
			HPEN holdpen = NULL;
			holdpen = (HPEN) ::SelectObject(hdcMem,hpen);
			::Arc( hdcMem,0,0,cx,cy,0,cy/2,cx, cy/2);
			::Arc( hdcMem,0,0,cx,cy,cx,cy/2,0, cy/2);
			::MoveToEx(hdcMem,0,cy,NULL);
			::LineTo(hdcMem,cx,0);
			::SelectObject(hdcMem,holdpen);
			::DeleteObject(hpen);
		}
		break;
		default:
		break;
	}
	::SelectObject(hdcMem, hOldDIB);
	::ReleaseDC(NULL, hdcMem);
	::DeleteDC( hdcMem );
	
	
ERROR_ENCOUNTERED:
	
	if ( hDC ) ::ReleaseDC( NULL, hDC );
	if (pbmi) delete[] pbmi;

return hbm;
}

const char	kstrDefaultThumbNailName[] = "tdefault.gif";

HRESULT	_FetchThumbnailFromSPRFile(IWorld *pWorld, BSTR bstrFile, BSTR *pbstrThumbnail)
{
	CComBSTR		bstrSpriteFileURL, bstrSpriteFilePath, bstrSpriteFile,
					bstrImageFile, bstrImageFileURL, bstrError, bstrThumbNail;
	VARIANT_BOOL	bFound;
	char			szExt[_MAX_EXT];
	HRESULT			hr = S_OK;
	ISpriteManager	*pSpriteManager = NULL;
	IInternetFileManager *pINetFileManager = NULL;

	if (FAILED(hr = pWorld->get_Tool(CComBSTR("Inetfile"),(IUnknown**) &pINetFileManager)))
		return E_FAIL;
	
	// Create the sprite manager if we haven't already.
	hr = CoCreateInstance(CLSID_SpriteManagerObject, NULL, CLSCTX_INPROC_SERVER,
		IID_ISpriteManager, (LPVOID*)&pSpriteManager);
	if (FAILED(hr) || (!pSpriteManager))
		goto ERROR_ENCOUNTERED;

	// Is this a sprite file or an image file?
	_splitpath(CString(bstrFile), NULL, NULL, NULL, szExt );
	if (stricmp(szExt, ".SPR") == 0)
		bstrSpriteFile = bstrFile;

	if (bstrSpriteFile)
	{
		// Download the necessary sprite file syncrhonously.
		if ((SUCCEEDED(hr = pINetFileManager->FindCachedFile(bstrSpriteFile, &bstrSpriteFileURL.m_str, &bstrSpriteFilePath.m_str, &bFound)) && bFound) ||
			SUCCEEDED(hr = pINetFileManager->GetFileEx(bstrSpriteFile, VARIANT_FALSE, &bstrSpriteFileURL.m_str, &bstrSpriteFilePath.m_str)))
		{
			// Read the sprite file.
			if (FAILED(hr = pSpriteManager->ReadSpriteFile(bstrSpriteFilePath, &bstrError.m_str)))
			{
				pWorld->Report(CComBSTR(CString("User List: Failed to read file: ") + CString(bstrSpriteFile)), -1);
				goto ERROR_ENCOUNTERED;
			}
			else
			{
				if (FAILED(hr = pSpriteManager->get_ThumbNailImage(&bstrThumbNail.m_str)))
					bstrThumbNail = kstrDefaultThumbNailName;

				// now, stick it together
				char	pathName[_MAX_DIR], driveName[_MAX_DRIVE];
				CString	fullName;

				_splitpath(CString(bstrSpriteFilePath), driveName, pathName, NULL, NULL );
				fullName = CString(driveName) + CString(pathName) + CString(bstrThumbNail);
				bstrThumbNail = fullName;
				hr = S_OK;
			}
		}
		else
		{
			pWorld->Report(CComBSTR(CString("User List: Failed to download file: ") + CString(bstrSpriteFile)), -1);
			goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	*pbstrThumbnail = bstrThumbNail.Copy();	
	SAFERELEASE(pSpriteManager);
	SAFERELEASE(pINetFileManager);

	return hr;
}



HRESULT GetThumbNailName(IWorld *pWorld, IThing* pthing, BSTR *pbstr )
{ 
static CComBSTR	bstrGeometry("Geometry");
	
	HRESULT hr = S_OK;
	typedef CVWComPtr<IVWGeometrySprite, NULL,&IID_IVWGeometrySprite> CIVWGeometrySpritePtr;
	CIVWGeometrySpritePtr GeometrySpritePtr;
	CComVariant var;

	hr = pthing->get_Property(bstrGeometry, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	GeometrySpritePtr = var;
	if ( GeometrySpritePtr == NULL )
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hr = GeometrySpritePtr->get_ThumbnailName(pbstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if ( ::SysStringLen( *pbstr ) == 0 )
	{
		// Now, we have to crack the SPR file....
		CComBSTR	bstrGeoName;
		SAFEFREESTRING( *pbstr );
		
		hr = GeometrySpritePtr->get_GeometryName(&bstrGeoName.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = _FetchThumbnailFromSPRFile(pWorld, bstrGeoName, pbstr);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

	}

ERROR_ENCOUNTERED:

	return hr;
}






HRESULT
CreatePlainSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, DWORD nBPP,
				   const DDPIXELFORMAT *pddpf, LPDIRECTDRAWPALETTE pPalette, void* data,
				   LPDIRECTDRAWSURFACE *ppdds)
{
	ASSERT(pDD && ppdds);

	HRESULT hr;
	DDSURFACEDESC ddsd;
	ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_PITCH;
	ddsd.dwWidth = nWidth;
	ddsd.dwHeight = nHeight;
	ddsd.lPitch = ((nHeight*nBPP/8+3)&~3);
	ddsd.lpSurface = data;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	if (pddpf) {
		// nBPP is ignored if a PixelFormat exists
		ddsd.ddpfPixelFormat = *pddpf;
	} else {
		if (nBPP <= 8) 
		{
			// palettized
			if (pPalette == NULL)
				return E_INVALIDARG;
			DWORD dwFlags = DDPF_RGB;
			switch (nBPP) {
			case 8:	dwFlags |= DDPF_PALETTEINDEXED8;
			case 4:	dwFlags |= DDPF_PALETTEINDEXED4;
			case 2:	dwFlags |= DDPF_PALETTEINDEXED2;
			case 1:	dwFlags |= DDPF_PALETTEINDEXED1;
			}
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = dwFlags;
			ddsd.ddpfPixelFormat.dwRGBBitCount = nBPP;
		}
		else 
		if( nBPP == 24)
		{
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount = nBPP;
			ddsd.ddpfPixelFormat.dwRBitMask = 0xFFL;
			ddsd.ddpfPixelFormat.dwGBitMask = 0xFF00L;
			ddsd.ddpfPixelFormat.dwBBitMask = 0xFF0000L;
		} 
		else {
			return E_INVALIDARG;
		}
	}

	LPDIRECTDRAWSURFACE pdds;
	if (FAILED(hr = pDD->CreateSurface(&ddsd, &pdds, NULL)))
		return hr;

	// attach palette if it exists
	if (pPalette)
		if (FAILED(hr = pdds->SetPalette(pPalette))) {
			pdds->Release();
			return hr;
		}

	*ppdds = pdds;

	return hr;
}


LPBITMAPINFOHEADER DibCreate(int bits, int dx, int dy)
{
    LPBITMAPINFOHEADER lpbi ;
    DWORD       dwSizeImage;
    int         i;
    DWORD FAR  *pdw;

    dwSizeImage = abs(dy)*(DWORD)((dx*bits/8+3)&~3);

    lpbi = (LPBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER)+dwSizeImage + 1024);

    if (lpbi == NULL)
        return NULL;

    lpbi->biSize            = sizeof(BITMAPINFOHEADER) ;
    lpbi->biWidth           = dx;
    lpbi->biHeight          = dy;
    lpbi->biPlanes          = 1;
    lpbi->biBitCount        = bits ;
    lpbi->biCompression     = BI_RGB ;
    lpbi->biSizeImage       = dwSizeImage;
    lpbi->biXPelsPerMeter   = 0 ;
    lpbi->biYPelsPerMeter   = 0 ;
    lpbi->biClrUsed         = 0 ;
    lpbi->biClrImportant    = 0 ;

    if (bits == 4)
        lpbi->biClrUsed = 16;

    else if (bits == 8)
        lpbi->biClrUsed = 256;

    pdw = (DWORD FAR *)((LPBYTE)lpbi+(int)lpbi->biSize);

    for (i=0; i<(int)lpbi->biClrUsed/16; i++)
    {
        *pdw++ = 0x00000000;    // 0000  black
        *pdw++ = 0x00800000;    // 0001  dark red
        *pdw++ = 0x00008000;    // 0010  dark green
        *pdw++ = 0x00808000;    // 0011  mustard
        *pdw++ = 0x00000080;    // 0100  dark blue
        *pdw++ = 0x00800080;    // 0101  purple
        *pdw++ = 0x00008080;    // 0110  dark turquoise
        *pdw++ = 0x00C0C0C0;    // 1000  gray
        *pdw++ = 0x00808080;    // 0111  dark gray
        *pdw++ = 0x00FF0000;    // 1001  red
        *pdw++ = 0x0000FF00;    // 1010  green
        *pdw++ = 0x00FFFF00;    // 1011  yellow
        *pdw++ = 0x000000FF;    // 1100  blue
        *pdw++ = 0x00FF00FF;    // 1101  pink (magenta)
        *pdw++ = 0x0000FFFF;    // 1110  cyan
        *pdw++ = 0x00FFFFFF;    // 1111  white
    }

    return lpbi;
}

BOOL DibSetUsage(LPBITMAPINFOHEADER pdib, IDirectDrawPalette *pDDPalette,UINT wUsage)
{
PALETTEENTRY ape[256];
RGBQUAD FAR * pRgb;
int n;

	if (!pdib)
		return FALSE;

	int nColors = DibNumColors(pdib);

	if (!pDDPalette ||
		FAILED(pDDPalette->GetEntries(0,0,nColors,ape)))
	{  // Not necessary to delete
		HPALETTE hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
		GetPaletteEntries(hpal,0,nColors,ape);
	}

	if (nColors > 0)
	{
		pRgb = DibColors(pdib);

		switch (wUsage)
		{
			//
			// Set the DIB color table to palette indexes
			//
			case DIB_RGB_COLORS:
				nColors = min(nColors,256);
				for (n=0; n<nColors; n++)
				{
					pRgb[n].rgbRed      = ape[n].peRed;
					pRgb[n].rgbGreen    = ape[n].peGreen;
					pRgb[n].rgbBlue     = ape[n].peBlue;
					pRgb[n].rgbReserved = 0;
				}
				break;
		}
	}

return TRUE;
}


HRESULT
CopyPixelsDDSTo24( LPBITMAPINFOHEADER lpbi, LPDIRECTDRAWSURFACE pddsSrc )
{
	HRESULT hr = S_OK;
	DWORD nDestWidth =  DibWidth(lpbi);
	DWORD nDestHeight = DibHeight(lpbi);
	DWORD bits = lpbi->biBitCount;
	DWORD pitch = ((nDestWidth*bits/8+3)&~3); 
	DDSURFACEDESC ddsd;
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	RECT rDst = {0, 0, nDestWidth, nDestHeight};

	if ( bits <= 8 )
	{
		IDirectDrawPalette *pddp = NULL;
		hr = pddsSrc->GetPalette( &pddp );
		if (SUCCEEDED(hr))
		{
			int nColors = DibNumColors(lpbi);
			int i;
			RGBQUAD *pRGB = DibColors(lpbi);
			hr = pddp->GetEntries(0, 0, nColors, (PALETTEENTRY*)DibColors(lpbi));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			//now swap the BGR to RGB
			for(i=0;i<nColors;i++)
			{
				BYTE temp = pRGB[i].rgbBlue;
				pRGB[i].rgbBlue = pRGB[i].rgbRed;
				pRGB[i].rgbRed = temp;
			}

		}
		SAFERELEASE(pddp);
		
		if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
	}

	// lock the surface for reading
	if (FAILED(hr = pddsSrc->Lock(&rDst, &ddsd, DDLOCK_WAIT, NULL)))
		return hr;

	// check that the surface is the right size for the copy
	if ((ddsd.dwWidth != nDestWidth) || 
		(ddsd.dwHeight != nDestHeight)) 
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	//
	// copy the pixels
	//
	if ( bits == ddsd.ddpfPixelFormat.dwRGBBitCount ) 
	{ 
		DWORD dwSizeImage = nDestHeight* pitch;
		DWORD dwSizeSurface = ddsd.dwHeight * ddsd.lPitch;
		if (  dwSizeImage == dwSizeSurface )
		{
			memcpy(DibPtr(lpbi),ddsd.lpSurface,dwSizeImage);
		}
		else
		{
			unsigned char *pDestinationScanline = (unsigned char *)DibPtr(lpbi);
			unsigned char *pSurface = ((unsigned char *)ddsd.lpSurface);		
			for(DWORD y=0;y<nDestHeight;y++)
			{
				memcpy( pDestinationScanline, pDestinationScanline,nDestWidth);
				pDestinationScanline += pitch;
				pSurface += ddsd.lPitch;
			}

		}
	}

	
ERROR_ENCOUNTERED:
	pddsSrc->Unlock(ddsd.lpSurface);

	return hr;
}

COLORREF TranslateColor( COLORREF clr, IDirectDrawPalette* pDDPalette )
{
	if ( !pDDPalette ) return clr;

	PALETTEENTRY ape[256];
	long index = 0;
	if( SUCCEEDED(pDDPalette->GetEntries(0,0,255,ape)))
	{
		// search for the closest rgb to background color
		
		long closest = 0x00FFFFFF;
		long distance, dr,dg,db;
		long r = GetRValue(clr);
		long g = GetGValue(clr);
		long b = GetBValue(clr);
		for(int i=0;i<255 && closest;i++)
		{
			dr = (ape[i].peRed -r); dr *= dr;
			dg = (ape[i].peGreen - g); dg *= dg;
			db = (ape[i].peBlue - b); db *= db;
			distance = dr + dg + db;
			if ( distance < closest )
			{
				closest = distance;
				index = i;
			}
		}
	}
	return index;
}