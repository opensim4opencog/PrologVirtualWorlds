// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SpriteMO.h : Declaration of the CSpriteManagerObject

#ifndef __SPRITEMANAGEROBJECT_H_
#define __SPRITEMANAGEROBJECT_H_

#include "resource.h"       // main symbols
#include <spritemn.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_SpriteManagerObject;

struct GESTUREFRAME
{
	DWORD	dwTime;
	int		iGestureFrame;
	int		iProbability;
};

struct IMAGEFRAME
{
	int		iStartAngle;
	int		iEndAngle;
	int		iImageFrame;
	VARIANT_BOOL	bShowAccessories;
};

/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerObject
class ATL_NO_VTABLE CSpriteManagerObject : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSpriteManagerObject, &CLSID_SpriteManagerObject>,
	public IDispatchImpl<ISpriteManager, &IID_ISpriteManager, &LIBID_VWMMLib>
{
public:
	CSpriteManagerObject();
	~CSpriteManagerObject();

BEGIN_COM_MAP(CSpriteManagerObject)
	COM_INTERFACE_ENTRY(ISpriteManager)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

DECLARE_REGISTRY(CSpriteManagerObject, _T("VWSYSTEM.SpriteManager.1"), _T("VWSYSTEM.SpriteManager"), IDS_SPRITEMANAGER_DESC, THREADFLAGS_BOTH)
 
// ISpriteManager
public:

	STDMETHOD(ReadSpriteFile)(BSTR bstrSpriteFile, BSTR *pbstrError);
	STDMETHOD(CreateSpriteFileData)(IDirect2DRMImage *pSpriteImage, DWORD dwFrames);
	STDMETHOD(CalcCurrentGestureFrame)();
	STDMETHOD(GetImageFrame)(float fAngle, int *piImageFrame, VARIANT_BOOL *pbShowAccessories);
	STDMETHOD(put_CurrentGesture)(int iGesture);
	STDMETHOD(get_CurrentGesture)(int* piGesture);
	STDMETHOD(get_ImageFile)(BSTR *pbstrImageFile);
	
	STDMETHOD(get_GestureText)(BSTR bstrGesture, BSTR *pbstrText);
	STDMETHOD(get_NamedGesture)(BSTR bstrGesture, int *pGestureNum);
	STDMETHOD(put_ThumbNailImage)(BSTR bstrThumbNailImage);
	STDMETHOD(get_ThumbNailImage)(BSTR *bstrThumbNailImage);
	STDMETHOD(put_ImageFile)(BSTR bstrImageFile);
	STDMETHOD(get_ImageFilePath)(BSTR *pbstrImageFile);
	STDMETHOD(get_SprFilePath)(BSTR *pbstrImageFile);
	STDMETHOD(WriteSpriteFile)(BSTR bstrSpriteFile, BSTR *pbstrError);
	// working with gesture table
	STDMETHOD(CountGestureTables)(long *plNumGestures);
	STDMETHOD(GetNamedGestures)(IPropertyList *pGestureNameList);
	STDMETHOD(AddGestureTable)(long *plNumGestures);
	STDMETHOD(DeleteGestureTable)(long lWhichGesture);
	STDMETHOD(GestureTableDuration)(long lWhichGesture, long *plDuration);
	STDMETHOD(get_NextGestureTable)(long lWhichGesture, long *plDuration);
	STDMETHOD(put_NextGestureTable)(long lWhichGesture, long nNewValue);

	// working with gesture frames
	STDMETHOD(CountGestureFrames)(long lWhichGesture, long *plNumFrames);
	STDMETHOD(AddGestureFrame)(long lWhichGesture, long *plNewFrame);
	STDMETHOD(DeleteGestureFrame)(long lWhichState, long lWhichFrame);
	STDMETHOD(get_GestureFrameDuration)(long lWhichGesture, long lWhichFrame, long *plDuration);
	STDMETHOD(put_GestureFrameDuration)(long lWhichGesture, long lWhichFrame, long nNewValue);
	STDMETHOD(get_GestureFrameImage)(long lWhichGesture, long lWhichFrame, long *plImage);
	STDMETHOD(put_GestureFrameImage)(long lWhichGesture, long lWhichFrame, long nNewValue);
	STDMETHOD(get_GestureFrameChance)(long lWhichGesture, long lWhichFrame, long *plChance);
	STDMETHOD(put_GestureFrameChance)(long lWhichGesture, long lWhichFrame, long nNewValue);
	// working with image frame tables
	STDMETHOD(CountImageTables)(long *plNumImages);
	STDMETHOD(AddImageTable)(long *plNewTable);
	STDMETHOD(DeleteImageTable)(long lWhichTable);
	// working with image frames
	STDMETHOD(AddImageEntry)(long lWhichTable, long *plNewEntry);
	STDMETHOD(DeleteImageEntry)(long lWhichTable, long lWhichImage);
	STDMETHOD(CountImageEntries)(long lWhichTable, long *plNumEntries);
	STDMETHOD(get_ImageEntryStartAngle)(long lWhichTable, long lWhichEntry, long *plStartAngle);
	STDMETHOD(put_ImageEntryStartAngle)(long lWhichTable, long lWhichEntry, long nNewValue);
	STDMETHOD(get_ImageEntryEndAngle)(long lWhichTable, long lWhichEntry, long *plEndAngle);
	STDMETHOD(put_ImageEntryEndAngle)(long lWhichTable, long lWhichEntry, long nNewValue);
	STDMETHOD(get_ImageEntryBitmapIndex)(long lWhichTable, long lWhichEntry, long *plIndex);
	STDMETHOD(put_ImageEntryBitmapIndex)(long lWhichTable, long lWhichEntry, long nNewValue);
	STDMETHOD(get_ImageEntryShowAcc)(long lWhichTable, long lWhichEntry, VARIANT_BOOL *pbShow);
	STDMETHOD(put_ImageEntryShowAcc)(long lWhichTable, long lWhichEntry, VARIANT_BOOL bNewValue);
// Helper funcs and data
private:

	void ReleaseSpriteFileData();
	BOOL	IsValidGestureTable(long lWhichGesture);
	BOOL	IsValidGestureAndFrame(long lWhichGesture, long lWhichFrame);
	BOOL	IsValidImageTable(long lWhichImage);
	BOOL	IsValidImageAndFrame(long lWhichImage, long lWhichFrame);
	CComBSTR		m_bstrImageFile;
	CComBSTR		m_bstrThumbNailImage;
	CComBSTR		m_bstrImageFilePath;
	CComBSTR		m_bstrSprFilePath;
	VARIANT_BOOL	m_bSpriteFileLoaded;
	int				m_iCurrentGestureFrameTable;
	CPtrList		*m_plistCurrentGestureFrameTable;
	POSITION		m_posCurrentGestureFrameTable;
	GESTUREFRAME	*m_pCurrentGestureFrame;
	CPtrList		*m_plistCurrentImageFrameTable;
	POSITION		m_posCurrentImageFrameTable;
	IMAGEFRAME		*m_pCurrentImageFrame;
	VARIANT_BOOL	m_bSkipCurrentGestureFrame;
	DWORD			m_dwTimeCurrentGestureStarted;
	CPtrArray		m_aGestureFrameTables;
	CPtrArray		m_aImageFrameTables;
};

#endif //__SPRITEMANAGEROBJECT_H_
