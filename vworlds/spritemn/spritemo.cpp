// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SpriteMO.cpp : Implementation of CSpriteManagerObject
#include "stdafx.h"
#include <vwobject.h>
#include <SpriteMn.h>
#include "SpriteMO.h"

#define	SpriteFileDelimChar	' '

/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerObject

CSpriteManagerObject::CSpriteManagerObject()
{
	m_bSpriteFileLoaded = VARIANT_FALSE;
}

CSpriteManagerObject::~CSpriteManagerObject()
{
	if (m_bSpriteFileLoaded)
		ReleaseSpriteFileData();
}

HRESULT CSpriteManagerObject::ReadSpriteFile(BSTR bstrSpriteFile, BSTR *pbstrError)
{
	CString			str, oldPath, newPath;
	char			*thePath;
	int				iGestureFrameTableNo = 0, iImageFrameTableNo = 0;
	int				iDelimPos;
	CStdioFile		SpriteFile;
	CPtrList		*plistGestureFrameTable = NULL, *plistImageFrameTable = NULL;
	GESTUREFRAME	*pGestureFrame;
	IMAGEFRAME		*pImageFrame;
	VARIANT_BOOL	bCreateNewTable, bEOF;
	CComBSTR		bstrImageFileURL;
	char			drive[_MAX_DRIVE];
	HRESULT	hr = S_OK;

	// Release old data (if any).
	if (m_bSpriteFileLoaded)
		ReleaseSpriteFileData();

	// Open the sprite file, read the version info and image file name.
	if (!SpriteFile.Open(CString(bstrSpriteFile), CFile::modeRead | CFile::typeText) ||
		!SpriteFile.ReadString(str) ||
		!SpriteFile.ReadString(str))
	{
		*pbstrError = CString(CString("Invalid sprite file format in file: ") + CString(bstrSpriteFile)).AllocSysString();
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// stow
	m_bstrSprFilePath = bstrSpriteFile;
	oldPath = bstrSpriteFile;
	thePath = newPath.GetBuffer(_MAX_DIR);
	_splitpath(oldPath, drive, thePath, NULL, NULL);
	newPath.ReleaseBuffer();
	newPath = drive + newPath;

	iDelimPos = str.Find('|');
	if (iDelimPos == -1)
	{
		m_bstrImageFile = str;
		newPath = newPath + str;
	}
	else
	{
		m_bstrImageFile = str.Left(iDelimPos);
		newPath = newPath + str.Left(iDelimPos);
		m_bstrThumbNailImage = str.Right(str.GetLength() - (iDelimPos+1));
	}

	// create the imagefile path
	m_bstrImageFilePath = newPath;

	// Read the gesture frame tables.
	bCreateNewTable = VARIANT_TRUE;
	while (SpriteFile.ReadString(str) && str != "")
	{
		if (bCreateNewTable)
		{
			plistGestureFrameTable = new CPtrList;
			bCreateNewTable = VARIANT_FALSE;
		}
		pGestureFrame = new GESTUREFRAME;
		if ((iDelimPos = str.Find(SpriteFileDelimChar)) == -1)
		{
			*pbstrError = CString(CString("VWGeometrySprite: Invalid gesture frame table entry in sprite file: ") + CString(bstrSpriteFile)).AllocSysString();
			delete pGestureFrame;
			m_aGestureFrameTables.Add(plistGestureFrameTable);
			ReleaseSpriteFileData();
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		plistGestureFrameTable->AddTail(pGestureFrame);
		pGestureFrame->dwTime = atoi(str.Left(iDelimPos));
		str = str.Mid(iDelimPos+1);
		if (pGestureFrame->dwTime != 0)
		{
			if ((iDelimPos = str.Find(SpriteFileDelimChar)) == -1)
			{
				pGestureFrame->iGestureFrame = atoi(str.Mid(iDelimPos+1));
				pGestureFrame->iProbability = 100;
			}
			else
			{
				pGestureFrame->iGestureFrame = atoi(str.Left(iDelimPos));
				str = str.Mid(iDelimPos+1);
				if (str.IsEmpty())
					pGestureFrame->iProbability = 100;
				else
					pGestureFrame->iProbability = atoi(str);
			}
		}
		else
		{
			// This is the end of a gesture frame table.
			m_aGestureFrameTables.Add(plistGestureFrameTable);

			// 100% probability of moving to next gesture.
			pGestureFrame->iProbability = 100;

			// Read in optional 2 text fields for gesture.
			if ((iDelimPos = str.Find(SpriteFileDelimChar)) == -1)
				pGestureFrame->iGestureFrame = atoi(str.Mid(iDelimPos+1));
			else
			{
				pGestureFrame->iGestureFrame = atoi(str.Left(iDelimPos));
				str = str.Mid(iDelimPos+1);
			}

			// Just check if the gesture frame table is a two liner which returns to itself
			// and if so make it a one liner in order to optimize the update callback.
			if (plistGestureFrameTable->GetCount() == 2 &&
				pGestureFrame->iGestureFrame == m_aGestureFrameTables.GetSize()-1)
			{

				delete (GESTUREFRAME *)plistGestureFrameTable->RemoveTail();
			}

			plistGestureFrameTable = NULL;
			bCreateNewTable = VARIANT_TRUE;
		}
	}
	if (plistGestureFrameTable != NULL)
	{
		*pbstrError = CString(CString("VWGeometrySprite: Unexpected end of gesture frame table entry in sprite file: ") + CString(bstrSpriteFile)).AllocSysString();
		m_aGestureFrameTables.Add(plistGestureFrameTable);
		ReleaseSpriteFileData();
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// Read the image frame tables.
	bCreateNewTable = VARIANT_TRUE;
	while (TRUE)
	{
		bEOF = !SpriteFile.ReadString(str) ? VARIANT_TRUE : VARIANT_FALSE;

		if (!bEOF && str != "" && bCreateNewTable)
		{
			// This is the beginning of an image frame table.
			plistImageFrameTable = new CPtrList;
			bCreateNewTable = VARIANT_FALSE;
		}
		else if (str == "" || bEOF)
		{
			// This is (still) the end of an image frame table.
			if (plistImageFrameTable)
			{
				m_aImageFrameTables.Add(plistImageFrameTable);
				plistImageFrameTable = NULL;
			}
			bCreateNewTable = VARIANT_TRUE;
			if (!bEOF)
				continue;
			else
			{
				if (m_aImageFrameTables.GetSize() == 0)
				{
					*pbstrError = CString(CString("VWGeometrySprite: No image frame tables found in sprite file: ") + CString(bstrSpriteFile)).AllocSysString();
					ReleaseSpriteFileData();
					hr = E_FAIL;
					goto ERROR_ENCOUNTERED;
				}
				break;
			}
		}
		pImageFrame = new IMAGEFRAME;
		if ((iDelimPos = str.Find(SpriteFileDelimChar)) == -1)
		{
			*pbstrError = CString(CString("VWGeometrySprite: Invalid image frame table entry in sprite file: ") + CString(bstrSpriteFile)).AllocSysString();
			delete pImageFrame;
			m_aImageFrameTables.Add(plistImageFrameTable);
			ReleaseSpriteFileData();
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		pImageFrame->iStartAngle = atoi(str.Left(iDelimPos));
		str = str.Mid(iDelimPos+1);
		if ((iDelimPos = str.Find(SpriteFileDelimChar)) == -1)
		{
			*pbstrError = CString(CString("VWGeometrySprite: Invalid image frame table entry in sprite file: ") + CString(bstrSpriteFile)).AllocSysString();
			delete pImageFrame;
			m_aImageFrameTables.Add(plistImageFrameTable);
			ReleaseSpriteFileData();
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		pImageFrame->iEndAngle = atoi(str.Left(iDelimPos));
		str = str.Mid(iDelimPos+1);
		if ((iDelimPos = str.Find(SpriteFileDelimChar)) == -1)
		{
			pImageFrame->iImageFrame = atoi(str.Mid(iDelimPos+1));
			pImageFrame->bShowAccessories = VARIANT_FALSE;
		}
		else
		{
			pImageFrame->iImageFrame = atoi(str.Left(iDelimPos));
			str = str.Mid(iDelimPos+1);
			if (str.IsEmpty())
				pImageFrame->bShowAccessories = VARIANT_FALSE;
			else
				pImageFrame->bShowAccessories = atoi(str) ? VARIANT_TRUE : VARIANT_FALSE;
		}
		plistImageFrameTable->AddTail(pImageFrame);
	}
	
	// Initialize at gesture 0.
	put_CurrentGesture(0);

	m_bSpriteFileLoaded = VARIANT_TRUE;

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CSpriteManagerObject::CreateSpriteFileData(IDirect2DRMImage *pSpriteImage, DWORD dwFrames)
{
	CPtrList		*plistGestureFrameTable = NULL, *plistImageFrameTable = NULL;
	GESTUREFRAME	*pGestureFrame;
	IMAGEFRAME		*pImageFrame;
	DWORD			dwFrame, dwDelay, dwTotalTime = 0, 
					dwCurrentGestureFrame = 0, dwCurrentImageFrame = 0;

	// Release old data (if any).
	if (m_bSpriteFileLoaded)
		ReleaseSpriteFileData();

	if (dwFrames == -1)
		pSpriteImage->get_NFrames(&dwFrames);

	plistGestureFrameTable = new CPtrList;
	m_aGestureFrameTables.Add(plistGestureFrameTable);
	for (dwFrame = 0; dwFrame < dwFrames; dwFrame++)
	{
		if (dwFrames > 1)
			pSpriteImage->GetFrameDelay(dwFrame, &dwDelay);
		else
			dwDelay = 0;
		pGestureFrame = new GESTUREFRAME;
		pGestureFrame->dwTime = dwTotalTime+dwDelay;
		dwTotalTime += dwDelay;
		pGestureFrame->iGestureFrame = dwCurrentGestureFrame;
		dwCurrentGestureFrame++;
		pGestureFrame->iProbability = 100;
		plistGestureFrameTable->AddTail(pGestureFrame);
		plistImageFrameTable = new CPtrList;
		pImageFrame = new IMAGEFRAME;
		pImageFrame->iStartAngle = 0;
		pImageFrame->iEndAngle = 360;
		pImageFrame->iImageFrame = dwCurrentImageFrame;
		dwCurrentImageFrame++;
		pImageFrame->bShowAccessories = VARIANT_TRUE;
		plistImageFrameTable->AddTail(pImageFrame);
		m_aImageFrameTables.Add(plistImageFrameTable);
	}
	if (dwFrames > 1)
	{
		pGestureFrame = new GESTUREFRAME;
		pGestureFrame->dwTime = 0;
		pGestureFrame->iGestureFrame = 0;
		pGestureFrame->iProbability = 100;
		plistGestureFrameTable->AddTail(pGestureFrame);
	}

	// Initialize at gesture 0.
	put_CurrentGesture(0);

	m_bSpriteFileLoaded = VARIANT_TRUE;

	return S_OK;
}

void	_DeleteGestureTable(CPtrList *pList);
void	_DeleteGestureTable(CPtrList *pList)
{	
	if (!pList->IsEmpty())
	{
		POSITION	pos;
		for( pos = pList->GetHeadPosition(); pos!=NULL; )
			delete (GESTUREFRAME *)pList->GetNext(pos);
		pList->RemoveAll();
	}
	delete pList;
}


void	_DeleteImageTable(CPtrList *pList);
void	_DeleteImageTable(CPtrList *pList)
{
	if (!pList->IsEmpty())
	{
		POSITION	pos;
		for( pos = pList->GetHeadPosition(); pos!=NULL; )
			delete (IMAGEFRAME*)pList->GetNext(pos);
		pList->RemoveAll();
	}
	delete pList;
}


void CSpriteManagerObject::ReleaseSpriteFileData()
{
	int				i, iNoTables;
	CPtrList		*plistFrameTable;

	iNoTables = m_aGestureFrameTables.GetSize();
	for(i = 0; i < iNoTables; i++)
	{
		plistFrameTable = (CPtrList*)m_aGestureFrameTables.GetAt(i);
		_DeleteGestureTable(plistFrameTable);
	}
	m_aGestureFrameTables.RemoveAll();

	iNoTables = m_aImageFrameTables.GetSize();
	for(i = 0; i < iNoTables; i++)
	{
		plistFrameTable = (CPtrList*)m_aImageFrameTables.GetAt(i);
		_DeleteImageTable(plistFrameTable);
	}
	m_aImageFrameTables.RemoveAll();

	m_bstrImageFile.Empty();
	m_bstrThumbNailImage.Empty();
	
	m_bSpriteFileLoaded = FALSE;
}

STDMETHODIMP CSpriteManagerObject::CalcCurrentGestureFrame()
{
	DWORD			dwTimeSinceGestureStarted;
	GESTUREFRAME	*pCurrentGestureFrame;

	// Find out which gesture frame should be displayed based on the gesture and time.

	if (!m_bSpriteFileLoaded)
		return E_FAIL;

	// How far through this gesture are we?
	dwTimeSinceGestureStarted = GetTickCount() - m_dwTimeCurrentGestureStarted;

	if (m_plistCurrentGestureFrameTable->GetCount() > 1 && dwTimeSinceGestureStarted >= m_pCurrentGestureFrame->dwTime)
	{
		// OK we're on to the next gesture frame.
		pCurrentGestureFrame = (GESTUREFRAME*)m_plistCurrentGestureFrameTable->GetNext(m_posCurrentGestureFrameTable);
		if (!m_posCurrentGestureFrameTable)
		{
			// This is the last element in this gesture frame table.
			// Look in the next gesture frame table specified and just use the first element
			// there.
			put_CurrentGesture(pCurrentGestureFrame->iGestureFrame);
		}
		else
		{
			// Calculate the probability now as we're entering this new gesture frame.
			// NOTE: We are ignoring probabilities for the first gesture frame in any table.
			m_pCurrentGestureFrame = pCurrentGestureFrame;
			if (rand() % 100 >= m_pCurrentGestureFrame->iProbability)
				m_bSkipCurrentGestureFrame = VARIANT_TRUE;
			else
			{
				m_bSkipCurrentGestureFrame = VARIANT_FALSE;
				if (m_pCurrentGestureFrame->iGestureFrame < m_aImageFrameTables.GetSize())
				{
					m_plistCurrentImageFrameTable = (CPtrList*)m_aImageFrameTables.GetAt(m_pCurrentGestureFrame->iGestureFrame);
					m_posCurrentImageFrameTable = m_plistCurrentImageFrameTable->GetHeadPosition();
					m_pCurrentImageFrame = (IMAGEFRAME*)m_plistCurrentImageFrameTable->GetNext(m_posCurrentImageFrameTable);
				}
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CSpriteManagerObject::GetImageFrame(float fAngle, int *piImageFrame, VARIANT_BOOL *pbShowAccessories)
{
	CPtrList		*plistCurrentImageFrameTable;
	POSITION		posCurrentImageFrameTable, posStartImageFrameTable;
	IMAGEFRAME		*pCurrentImageFrame;
	VARIANT_BOOL	bLooped = VARIANT_FALSE;

	// Find out which image frame should be displayed based on the current gesture frame
	// and fAngle.

	if (!m_bSpriteFileLoaded)
		return E_FAIL;

	plistCurrentImageFrameTable = m_plistCurrentImageFrameTable;
	posCurrentImageFrameTable = m_posCurrentImageFrameTable;
	posStartImageFrameTable = posCurrentImageFrameTable;
	pCurrentImageFrame = m_pCurrentImageFrame;
	*piImageFrame = pCurrentImageFrame->iImageFrame;
	*pbShowAccessories = pCurrentImageFrame->bShowAccessories;
	for (;;)
	{
		if ((fAngle >= pCurrentImageFrame->iStartAngle && fAngle <= pCurrentImageFrame->iEndAngle) ||
			((pCurrentImageFrame->iStartAngle > pCurrentImageFrame->iEndAngle) &&
			 (fAngle >= pCurrentImageFrame->iStartAngle || fAngle <= pCurrentImageFrame->iEndAngle)))
		{
			// Use this image frame.
			*piImageFrame = pCurrentImageFrame->iImageFrame;
			*pbShowAccessories = pCurrentImageFrame->bShowAccessories;
			if (plistCurrentImageFrameTable == m_plistCurrentImageFrameTable)
			{
				m_posCurrentImageFrameTable = posCurrentImageFrameTable;
				m_pCurrentImageFrame = pCurrentImageFrame;
			}
			break;
		}
		if (bLooped && posCurrentImageFrameTable == posStartImageFrameTable)
		{
			if (plistCurrentImageFrameTable != (CPtrList*)m_aImageFrameTables.GetAt(0))
			{
				// Didn't find fAngle in the associated image frame table. Look in the
				// first image frame table which is used as the default.
				plistCurrentImageFrameTable = (CPtrList*)m_aImageFrameTables.GetAt(0);
				posCurrentImageFrameTable = plistCurrentImageFrameTable->GetHeadPosition();
				posStartImageFrameTable = posCurrentImageFrameTable;
				bLooped = VARIANT_FALSE;
			}
			else
				break;
		}
		if (posCurrentImageFrameTable == NULL)
		{
			posCurrentImageFrameTable = plistCurrentImageFrameTable->GetHeadPosition();
			bLooped = VARIANT_TRUE;
		}
		pCurrentImageFrame = (IMAGEFRAME*)plistCurrentImageFrameTable->GetNext(posCurrentImageFrameTable);
	}

	return S_OK;
}

STDMETHODIMP CSpriteManagerObject::put_CurrentGesture(int iGesture)
{
	// Set the current gesture and image frames appropriately.
	if ((iGesture < 0) || (iGesture >= m_aGestureFrameTables.GetSize()))
		return E_FAIL;
	m_dwTimeCurrentGestureStarted = GetTickCount();
	m_iCurrentGestureFrameTable = iGesture;
	m_plistCurrentGestureFrameTable = (CPtrList*)m_aGestureFrameTables.GetAt(m_iCurrentGestureFrameTable);
	m_posCurrentGestureFrameTable = m_plistCurrentGestureFrameTable->GetHeadPosition();
	m_pCurrentGestureFrame = (GESTUREFRAME*)m_plistCurrentGestureFrameTable->GetNext(m_posCurrentGestureFrameTable);
	if (m_pCurrentGestureFrame->iGestureFrame >= m_aImageFrameTables.GetSize())
		return E_FAIL;
	m_plistCurrentImageFrameTable = (CPtrList*)m_aImageFrameTables.GetAt(m_pCurrentGestureFrame->iGestureFrame);
	m_posCurrentImageFrameTable = m_plistCurrentImageFrameTable->GetHeadPosition();
	m_pCurrentImageFrame = (IMAGEFRAME*)m_plistCurrentImageFrameTable->GetNext(m_posCurrentImageFrameTable);
	m_bSkipCurrentGestureFrame = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CSpriteManagerObject::get_CurrentGesture(int* piGesture)
{
	*piGesture = m_iCurrentGestureFrameTable;
	return S_OK;
}

STDMETHODIMP CSpriteManagerObject::get_ImageFile(BSTR *pbstrImageFile)
{
	*pbstrImageFile = m_bstrImageFile.Copy();
	return S_OK;
}

//******************************************************
// get_ThumbNailImage
// gets the ThumbNail name, if any

STDMETHODIMP CSpriteManagerObject::get_ThumbNailImage(BSTR *bstrThumbNailImage)
{
	*bstrThumbNailImage = m_bstrThumbNailImage.Copy();
	return S_OK;
}

//******************************************************
// put_ThumbNailImage
// sets the thumbnail name

STDMETHODIMP CSpriteManagerObject::put_ThumbNailImage(BSTR bstrThumbNailImage)
{
	HRESULT	hr = S_OK;
	
	m_bstrThumbNailImage = bstrThumbNailImage;

	return hr;
}

//******************************************************
// put_ImageFile
// sets the image file

STDMETHODIMP CSpriteManagerObject::put_ImageFile(BSTR bstrImageFile)
{
	HRESULT	hr = S_OK;
	
	m_bstrImageFile = bstrImageFile;

	return hr;
}

//******************************************************
// get_ImageFilePath
// Returns a path to the image file

STDMETHODIMP CSpriteManagerObject::get_ImageFilePath(BSTR *pbstrImageFile)
{
	*pbstrImageFile = m_bstrImageFilePath.Copy();
	return S_OK;
}

//******************************************************
// get_SprFilePath
// Returns a path to the .spr file

STDMETHODIMP CSpriteManagerObject::get_SprFilePath(BSTR *pbstrImageFile)
{
	*pbstrImageFile = m_bstrSprFilePath.Copy();
	return S_OK;
}



//******************************************************
// WriteSpriteFile
// Writes a new spr file.  This updates the ImageFilePath,
// the sprFileName, and the sprFilePath

STDMETHODIMP CSpriteManagerObject::WriteSpriteFile(BSTR bstrSpriteFile, BSTR *pbstrError)
{
	HRESULT		hr = S_OK;
	CStdioFile	SpriteFile;
	int			i, maxSize;
	GESTUREFRAME	*pCurGesture;
	IMAGEFRAME		*pCurImage;
	CPtrList		*plistFrameTable;
	CString			curLine;
	POSITION		pos;

	if (!SpriteFile.Open(CString(bstrSpriteFile), CFile::modeWrite | CFile::modeCreate | CFile::typeText))
	{
		*pbstrError = CString(CString("Failed to Create file for writing: ") + CString(bstrSpriteFile)).AllocSysString();
		hr = E_FAIL;
		goto CLEAN_UP;
		
	}

	// 1. write out the header
	SpriteFile.WriteString("SPRITE 00.00.00.0001\n");
	SpriteFile.WriteString((CString)m_bstrImageFile);
	if (m_bstrThumbNailImage.Length() > 0)
	{
		SpriteFile.WriteString("|");
		SpriteFile.WriteString((CString)m_bstrThumbNailImage.m_str);
	}
	SpriteFile.WriteString("\n");

	// 2. write out the gesture frame tables
	maxSize = m_aGestureFrameTables.GetSize();

	for (i = 0; i < maxSize; i++)
	{
		plistFrameTable = (CPtrList*)m_aGestureFrameTables.GetAt(i);
		
		for (pos = plistFrameTable->GetHeadPosition(); pos != NULL; )
		{
			pCurGesture = (GESTUREFRAME *)plistFrameTable->GetNext(pos);
		
			if (pCurGesture->iProbability != 100)
				curLine.Format("%d %d %d\n", pCurGesture->dwTime, pCurGesture->iGestureFrame, pCurGesture->iProbability);
			else
				curLine.Format("%d %d\n", pCurGesture->dwTime, pCurGesture->iGestureFrame);

			SpriteFile.WriteString(curLine);
		}

		// handle single-line looping SPR
		if (plistFrameTable->GetCount() == 1)
		{
			// append the single-line loop command
			curLine.Format("0 %d\n", i);
			SpriteFile.WriteString(curLine);
		}
	}

	// 3. write out the image frame tables
	maxSize = m_aImageFrameTables.GetSize();
	for(i = 0; i < maxSize; i++)
	{
		SpriteFile.WriteString("\n");
		plistFrameTable = (CPtrList*)m_aImageFrameTables.GetAt(i);

		for( pos = plistFrameTable->GetHeadPosition(); pos!=NULL; )
		{
			pCurImage = (IMAGEFRAME*)plistFrameTable->GetNext(pos);
			
			if (pCurImage->bShowAccessories == VARIANT_TRUE)
				curLine.Format("%d %d %d 1\n", pCurImage->iStartAngle, pCurImage->iEndAngle, pCurImage->iImageFrame);
			else
				curLine.Format("%d %d %d\n", pCurImage->iStartAngle, pCurImage->iEndAngle, pCurImage->iImageFrame);

			SpriteFile.WriteString(curLine);
		}
	}

CLEAN_UP:
	return hr;
}

//******************************************************
//******************************************************
// working with gesture tables

//******************************************************
// IsValidGestureTable
// Returns TRUE if this is a valid table index

BOOL	CSpriteManagerObject::IsValidGestureTable(long lWhichGesture)
{
	if ((lWhichGesture >= 0) &&
		(lWhichGesture < m_aGestureFrameTables.GetSize()))
		return TRUE;
	else
		return FALSE;
}

//******************************************************
// IsValidGestureAndFrame
// Returns TRUE if this is a valid table and frame index

BOOL	CSpriteManagerObject::IsValidGestureAndFrame(long lWhichGesture, long lWhichFrame)
{
	if ((lWhichGesture >= 0) &&
		(lWhichGesture < m_aGestureFrameTables.GetSize()))
	{
		CPtrList	*pList;
		int			count;

		pList = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);
		
		if (!pList)
			return FALSE;

		if (lWhichFrame < 0)
			return FALSE;

		count = pList->GetCount();

		if (count == 0)
			return FALSE;	// can't access an empty list
		else if (count == 1)
		{
			if (lWhichFrame == 0)
				return TRUE;
			else
				return FALSE;
		}
		else if (lWhichFrame <= (count-2))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}


//******************************************************
// CountGestureTables
// Returns the number of gesture tables in the .spr

STDMETHODIMP CSpriteManagerObject::CountGestureTables(long *plNumGestures)
{
	*plNumGestures = m_aGestureFrameTables.GetSize();

	return S_OK;
}


//******************************************************
// AddGestureTable
// Adds a new gesture table to the list

STDMETHODIMP CSpriteManagerObject::AddGestureTable(long *plNumGestures)
{
	HRESULT	hr = S_OK;
	CPtrList	*pNewTable = new CPtrList;

	if (pNewTable)
		*plNumGestures = m_aGestureFrameTables.Add(pNewTable);
	else
		hr = E_FAIL;	// doubtful we will ever hit this..

	return hr;
}


//******************************************************
// DeleteGestureTable
// Deletes a gesture table from the array..

STDMETHODIMP CSpriteManagerObject::DeleteGestureTable(long lWhichGesture)
{
	HRESULT	hr = S_OK;
	CPtrList	*pDeadTable;
	
	if (!IsValidGestureTable(lWhichGesture))
		hr = E_FAIL;
	else
	{
		pDeadTable = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);
		_DeleteGestureTable(pDeadTable);
		m_aGestureFrameTables.RemoveAt(lWhichGesture);
	}
	
	return hr;
}


//******************************************************
// GestureTableDuration
// Totals all of the durations in the gesture table
// and returns the result

STDMETHODIMP CSpriteManagerObject::GestureTableDuration(long lWhichGesture, long *plDuration)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurGesture;

	if (!IsValidGestureTable(lWhichGesture))
	{
		hr = E_FAIL;
		goto CLEAN_UP;
	}

	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);
	*plDuration = 0;

	if (pCurGesture && (!pCurGesture->IsEmpty()))
	{
		POSITION		pos;
		GESTUREFRAME	*pCurFrame;
		long			dur = 0;

		for( pos = pCurGesture->GetHeadPosition(); pos!=NULL; )
		{
			pCurFrame = (GESTUREFRAME *)pCurGesture->GetNext(pos);
			if (pCurFrame)
				dur += pCurFrame->dwTime;
		}
		*plDuration = dur;
	}
	else
		hr = E_FAIL;

CLEAN_UP:
	return hr;
}


//******************************************************
// get_NextGestureTable
// Returns the gesture table that comes next.  This is
// somewhat tricky because there are three cases:
// 1)  The gesture table might currently be empty
// 2)  We are a single frame gesture that loops.
// 3)  (default)The last image is the loop frame

STDMETHODIMP CSpriteManagerObject::get_NextGestureTable(long lWhichGesture, long *plNextTable)
{
	HRESULT		hr = S_OK;
	CPtrList	*pCurGesture;

	if (!IsValidGestureTable(lWhichGesture))
		return E_FAIL;

	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);
	*plNextTable = 0;

	if (pCurGesture)
	{
		if (pCurGesture->IsEmpty())
			*plNextTable = 0;	// default is to return to state 0
		else if (pCurGesture->GetCount() == 1)
			*plNextTable = lWhichGesture;	// single frame looping gesture
		else
		{
			GESTUREFRAME	*pCurFrame;		

			pCurFrame = (GESTUREFRAME *)pCurGesture->GetTail();
			if (pCurFrame)
				*plNextTable = pCurFrame->iGestureFrame;
			else
				*plNextTable = 0;
		}
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// put_NextGestureTable
// Sets the next gesture table.  This must handle the
// same cases as get_NextGestureTable (above)

STDMETHODIMP CSpriteManagerObject::put_NextGestureTable(long lWhichGesture, long nNewValue)
{
	HRESULT		hr = S_OK;
	CPtrList	*pCurGesture;

	if (!IsValidGestureTable(lWhichGesture))
		return E_FAIL;

	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;

		if (pCurGesture->IsEmpty() && (nNewValue != 0))
		{
			// this is not support
			hr = E_FAIL;
		}
		else if ((pCurGesture->GetCount() == 1) && (nNewValue != lWhichGesture))
		{
			pCurFrame = new GESTUREFRAME;
			pCurFrame->dwTime = 0;
			pCurFrame->iGestureFrame = nNewValue;
			pCurFrame->iProbability = 100;
			pCurGesture->AddTail(pCurFrame);			
		}
		else
		{
			if ((nNewValue == lWhichGesture) && (pCurGesture->GetCount() == 2))
			{
				// optimize this as a single-frame loop
				pCurFrame = (GESTUREFRAME *)pCurGesture->RemoveTail();
				delete pCurFrame;
			}
			else
			{
				pCurFrame = (GESTUREFRAME *)pCurGesture->GetTail();
				pCurFrame->iGestureFrame = nNewValue;
			}
		}
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
//******************************************************
// working with gesture frames

//******************************************************
// CountGestureFrames
// Returns the number of frames in the gesture.  Remember
// that in all multi-frame gestures, the final frame
// is really the loop-back specifier.

STDMETHODIMP CSpriteManagerObject::CountGestureFrames(long lWhichGesture, long *plNumFrames)
{
	HRESULT		hr = S_OK;
	CPtrList	*pCurGesture;

	if (!IsValidGestureTable(lWhichGesture))
		return E_FAIL;

	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);
	*plNumFrames = 0;

	if (pCurGesture)
	{	
		long	fCount;

		fCount = pCurGesture->GetCount();
		if (fCount > 1)
			fCount--;	// don't count loop "frame"

		*plNumFrames = fCount;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// AddGestureFrame
// Adds a frame to the specified gesture.  We have to
// handle the case of empty lists and single-frame
// lists.

STDMETHODIMP CSpriteManagerObject::AddGestureFrame(long lWhichGesture, long *plNewFrame)
{
	HRESULT		hr = S_OK;
	CPtrList	*pCurGesture;

	if (!IsValidGestureTable(lWhichGesture))
		return E_FAIL;

	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);
	*plNewFrame = 0;

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;

		if (pCurGesture->IsEmpty())
		{
			// it was empty - add a single frame
			pCurFrame = new GESTUREFRAME;
			pCurFrame->dwTime = 1000;
			pCurFrame->iGestureFrame = 0;
			pCurFrame->iProbability = 100;
			pCurGesture->AddTail(pCurFrame);
			*plNewFrame = 0;

			// and - we have to add a terminating frame
			pCurFrame = new GESTUREFRAME;
			pCurFrame->dwTime = 0;
			pCurFrame->iGestureFrame = 0;
			pCurFrame->iProbability = 100;
			pCurGesture->AddTail(pCurFrame);		
		}
		else if (pCurGesture->GetCount() == 1)
		{
			// add the new frame...
			pCurFrame = new GESTUREFRAME;
			pCurFrame->dwTime = 1000;
			pCurFrame->iGestureFrame = 0;
			pCurFrame->iProbability = 100;
			pCurGesture->AddTail(pCurFrame);
			*plNewFrame = 1;

			// and - we have to add a terminating frame that loops
			pCurFrame = new GESTUREFRAME;
			pCurFrame->dwTime = 0;
			pCurFrame->iGestureFrame = lWhichGesture;
			pCurFrame->iProbability = 100;
			pCurGesture->AddTail(pCurFrame);		
		}
		else
		{
			// add this new frame directly before the tail...
			POSITION	pos;

			pos = pCurGesture->GetTailPosition();
			pCurFrame = new GESTUREFRAME;
			pCurFrame->dwTime = 1000;
			pCurFrame->iGestureFrame = 0;
			pCurFrame->iProbability = 100;
			pCurGesture->InsertBefore(pos, pCurFrame);
			*plNewFrame = pCurGesture->GetCount()-2;
		}
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// DeleteGestureFrame
// Deletes the specified gesture frame from the table

STDMETHODIMP CSpriteManagerObject::DeleteGestureFrame(long lWhichGesture, long lWhichFrame)
{
	HRESULT		hr = S_OK;
	CPtrList	*pCurGesture;

	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;

		if (pCurGesture->IsEmpty())
			hr = E_FAIL;
		else if (pCurGesture->GetCount() == 1)
		{
			if (lWhichFrame > 0)
				hr = E_FAIL;
			else
			{
				// delete the frame
				pCurFrame = (GESTUREFRAME *)pCurGesture->GetHead();
				delete pCurFrame;
				pCurGesture->RemoveHead();
			}
		}
		else
		{
			// add this new frame directly before the tail...
			POSITION	pos;

			pos = pCurGesture->FindIndex(lWhichFrame);
			if (pos == NULL)
				hr = E_FAIL;
			else
			{
				pCurFrame = (GESTUREFRAME *)pCurGesture->GetAt(pos);
				if (pCurFrame)
					delete pCurFrame;
				pCurGesture->RemoveAt(pos);
			}

			if (pCurGesture->GetCount() == 1)
			{
				// only ref frame remains - kill it as well
				pCurFrame = (GESTUREFRAME *)pCurGesture->GetHead();
				delete pCurFrame;
				pCurGesture->RemoveHead();
			}
			else if (pCurGesture->GetCount() == 2)
			{
				// single frame is left - delete it if it loops to itself
				pCurFrame = (GESTUREFRAME *)pCurGesture->GetTail();
				if (pCurFrame->iGestureFrame == lWhichGesture)
				{
					delete pCurFrame;
					pCurGesture->RemoveTail();
				}
			}
		}
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// get_GestureFrameDuration
// Returns the duration of a gesture frame

STDMETHODIMP CSpriteManagerObject::get_GestureFrameDuration(long lWhichGesture, long lWhichFrame, long *plDuration)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurGesture;
	
	if (!IsValidGestureAndFrame(lWhichGesture, lWhichFrame))
	{
		hr = E_FAIL;
		goto CLEAN_UP;
	}
	
	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;
		POSITION		pos;

		pos = pCurGesture->FindIndex(lWhichFrame);
		pCurFrame = (GESTUREFRAME *)pCurGesture->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			*plDuration = pCurFrame->dwTime;
	}
	else
		hr = E_FAIL;

CLEAN_UP:
	return hr;
}


//******************************************************
// put_GestureFrameDuration
// Sets the duration of a gesture frame

STDMETHODIMP CSpriteManagerObject::put_GestureFrameDuration(long lWhichGesture, long lWhichFrame, long nNewValue)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurGesture;
	
	if (!IsValidGestureAndFrame(lWhichGesture, lWhichFrame))
	{
		hr = E_FAIL;
		goto CLEAN_UP;
	}
	
	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;
		POSITION		pos;

		pos = pCurGesture->FindIndex(lWhichFrame);
		pCurFrame = (GESTUREFRAME *)pCurGesture->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			pCurFrame->dwTime = nNewValue;
	}
	else
		hr = E_FAIL;

CLEAN_UP:
	return hr;
}


//******************************************************
// get_GestureFrameImage
// Returns the gestureFrame's image

STDMETHODIMP CSpriteManagerObject::get_GestureFrameImage(long lWhichGesture, long lWhichFrame, long *plImage)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurGesture;
	
	if (!IsValidGestureAndFrame(lWhichGesture, lWhichFrame))
	{
		hr = E_FAIL;
		goto CLEAN_UP;
	}
	
	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;
		POSITION		pos;

		pos = pCurGesture->FindIndex(lWhichFrame);
		pCurFrame = (GESTUREFRAME *)pCurGesture->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			*plImage = pCurFrame->iGestureFrame;
	}
	else
		hr = E_FAIL;

CLEAN_UP:
	return hr;
}


//******************************************************
// put_GestureFrameImage
// Sets the gestureframe's image

STDMETHODIMP CSpriteManagerObject::put_GestureFrameImage(long lWhichGesture, long lWhichFrame, long nNewValue)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurGesture;
	
	if (!IsValidGestureAndFrame(lWhichGesture, lWhichFrame))
	{
		hr = E_FAIL;
		goto CLEAN_UP;
	}
	
	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;
		POSITION		pos;

		pos = pCurGesture->FindIndex(lWhichFrame);
		pCurFrame = (GESTUREFRAME *)pCurGesture->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			pCurFrame->iGestureFrame = nNewValue;
	}
	else
		hr = E_FAIL;

CLEAN_UP:
	return hr;
}


//******************************************************
// get_GestureFrameChance
// Returns the gestureframe's chance

STDMETHODIMP CSpriteManagerObject::get_GestureFrameChance(long lWhichGesture, long lWhichFrame, long *plChance)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurGesture;
	
	if (!IsValidGestureAndFrame(lWhichGesture, lWhichFrame))
	{
		hr = E_FAIL;
		goto CLEAN_UP;
	}
	
	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;
		POSITION		pos;

		pos = pCurGesture->FindIndex(lWhichFrame);
		pCurFrame = (GESTUREFRAME *)pCurGesture->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			*plChance = pCurFrame->iProbability;
	}
	else
		hr = E_FAIL;

CLEAN_UP:
	return hr;
}


//******************************************************
// put_GestureFrameChance
// Sets the gestureframe's chance

STDMETHODIMP CSpriteManagerObject::put_GestureFrameChance(long lWhichGesture, long lWhichFrame, long nNewValue)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurGesture;
	
	if (!IsValidGestureAndFrame(lWhichGesture, lWhichFrame))
	{
		hr = E_FAIL;
		goto CLEAN_UP;
	}
	
	pCurGesture = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichGesture);

	if (pCurGesture)
	{
		GESTUREFRAME	*pCurFrame;
		POSITION		pos;

		pos = pCurGesture->FindIndex(lWhichFrame);
		pCurFrame = (GESTUREFRAME *)pCurGesture->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			pCurFrame->iProbability = nNewValue;
	}
	else
		hr = E_FAIL;

CLEAN_UP:
	return hr;
}


//******************************************************
//******************************************************
// working with image frame tables

//******************************************************
// IsValidImageTable
// Returns true if this is a valid image table index

BOOL	CSpriteManagerObject::IsValidImageTable(long lWhichImage)
{
	if ((lWhichImage >= 0) &&
		(lWhichImage < m_aImageFrameTables.GetSize()))
		return TRUE;
	else
		return FALSE;
}

//******************************************************
// IsValidImageAndFrame
// Returns true if this is a valid frame in an image frame table.

BOOL	CSpriteManagerObject::IsValidImageAndFrame(long lWhichImage, long lWhichFrame)
{
	if ((lWhichImage >= 0) &&
		(lWhichImage < m_aImageFrameTables.GetSize()))
	{
		CPtrList	*pList;

		pList = (CPtrList *)m_aImageFrameTables.GetAt(lWhichImage);
		
		if (!pList)
			return FALSE;

		if ((lWhichFrame < 0) || (lWhichFrame >= pList->GetCount()))
			return FALSE;
		else
			return TRUE;
	}
	else
		return FALSE;
}

//******************************************************
// CountImageTables
// Returns the number of image tables in the SPR

STDMETHODIMP CSpriteManagerObject::CountImageTables(long *plNumImages)
{
	*plNumImages = m_aImageFrameTables.GetSize();

	return S_OK;
}


//******************************************************
// AddImageTable
// Adds an image table

STDMETHODIMP CSpriteManagerObject::AddImageTable(long *plNewTable)
{
	HRESULT	hr = S_OK;
	CPtrList	*pNewTable = new CPtrList;

	if (pNewTable)
		*plNewTable = m_aImageFrameTables.Add(pNewTable);
	else
		hr = E_FAIL;	// doubtful we will ever hit this..

	return hr;
}


//******************************************************
// DeleteImageTable
// Deletes the imagetable from the file

STDMETHODIMP CSpriteManagerObject::DeleteImageTable(long lWhichTable)
{
	HRESULT	hr = S_OK;
	CPtrList	*pDeadTable;
	
	if (!IsValidImageTable(lWhichTable))
		hr = E_FAIL;
	else
	{
		pDeadTable = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);
		_DeleteGestureTable(pDeadTable);
		m_aImageFrameTables.RemoveAt(lWhichTable);
	}
	
	return hr;
}


//******************************************************
//******************************************************
// working with image frames

//******************************************************
// AddImageEntry
// Adds an entry to an image table

STDMETHODIMP CSpriteManagerObject::AddImageEntry(long lWhichTable, long *plNewEntry)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;

	if (!IsValidImageTable(lWhichTable))
		return E_FAIL;

	pCurImage = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichTable);
	*plNewEntry = 0;

	if (pCurImage)
	{
		IMAGEFRAME	*pCurEntry;
		
		pCurEntry = new IMAGEFRAME;
		pCurEntry->iStartAngle	= 0;
		pCurEntry->iEndAngle	= 0;
		pCurEntry->iImageFrame	= 0;
		pCurEntry->bShowAccessories = VARIANT_FALSE;
		pCurImage->AddTail(pCurEntry);
		*plNewEntry = pCurImage->GetCount() - 1;
	}

	return hr;
}


//******************************************************
// DeleteImageEntry
// Deletes an entry from the image frame table

STDMETHODIMP CSpriteManagerObject::DeleteImageEntry(long lWhichTable, long lWhichImage)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;

	if (!IsValidImageAndFrame(lWhichTable, lWhichImage))
		return E_FAIL;

	pCurImage = (CPtrList *)m_aGestureFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME	*pCurFrame;
		POSITION	pos;

		pos = pCurImage->FindIndex(lWhichImage);
		if (pos == NULL)
			hr = E_FAIL;
		else
		{
			pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
			if (pCurFrame)
				delete pCurFrame;
			pCurImage->RemoveAt(pos);
		}
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// CountImageEntries
// Returns the number of entries an an image table

STDMETHODIMP CSpriteManagerObject::CountImageEntries(long lWhichTable, long *plNumEntries)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	*plNumEntries = 0;
	if (!IsValidImageTable(lWhichTable))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
		*plNumEntries = pCurImage->GetCount();
	
	return hr;
}


//******************************************************
// get_ImageEntryStartAngle
// Returns the imageentry's startAngle

STDMETHODIMP CSpriteManagerObject::get_ImageEntryStartAngle(long lWhichTable, long lWhichEntry, long *plStartAngle)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			*plStartAngle = pCurFrame->iStartAngle;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// put_ImageEntryStartAngle
// Sets the entry's startAngle

STDMETHODIMP CSpriteManagerObject::put_ImageEntryStartAngle(long lWhichTable, long lWhichEntry, long nNewValue)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			pCurFrame->iStartAngle = nNewValue;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// get_ImageEntryEndAngle
// Returns the entry's endangle

STDMETHODIMP CSpriteManagerObject::get_ImageEntryEndAngle(long lWhichTable, long lWhichEntry, long *plEndAngle)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			*plEndAngle = pCurFrame->iEndAngle;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// put_ImageEntryEndAngle
// Sets the entry's endangle

STDMETHODIMP CSpriteManagerObject::put_ImageEntryEndAngle(long lWhichTable, long lWhichEntry, long nNewValue)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			pCurFrame->iEndAngle = nNewValue;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// get_ImageEntryBitmapIndex
// Returns the entry's bitmap index

STDMETHODIMP CSpriteManagerObject::get_ImageEntryBitmapIndex(long lWhichTable, long lWhichEntry, long *plIndex)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			*plIndex = pCurFrame->iImageFrame;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// put_ImageEntryBitmapIndex
// Sets the entry's bitmapindex

STDMETHODIMP CSpriteManagerObject::put_ImageEntryBitmapIndex(long lWhichTable, long lWhichEntry, long nNewValue)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			pCurFrame->iImageFrame = nNewValue;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// get_ImageEntryShowAcc
// returns the entrie's showAcc

STDMETHODIMP CSpriteManagerObject::get_ImageEntryShowAcc(long lWhichTable, long lWhichEntry, VARIANT_BOOL *pbShow)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			*pbShow = pCurFrame->bShowAccessories;
	}
	else
		hr = E_FAIL;

	return hr;
}


//******************************************************
// put_ImageEntryShowAcc
// Sets the entry's showacc

STDMETHODIMP CSpriteManagerObject::put_ImageEntryShowAcc(long lWhichTable, long lWhichEntry, VARIANT_BOOL bNewValue)
{
	HRESULT	hr = S_OK;
	CPtrList	*pCurImage;
	
	if (!IsValidImageAndFrame(lWhichTable, lWhichEntry))
		return E_FAIL;
	
	pCurImage = (CPtrList *)m_aImageFrameTables.GetAt(lWhichTable);

	if (pCurImage)
	{
		IMAGEFRAME		*pCurFrame;
		POSITION		pos;

		pos = pCurImage->FindIndex(lWhichEntry);
		pCurFrame = (IMAGEFRAME *)pCurImage->GetAt(pos);
		if (!pCurFrame)
			hr = E_FAIL;
		else
			pCurFrame->bShowAccessories = bNewValue;
	}
	else
		hr = E_FAIL;

	return hr;
}

const char kMaxGestures = 13;

const char	kstrGestureNameTable[kMaxGestures][12] = 
{
	"IDLE",
	"WAVE",
	"SMILE",
	"FLIRT",
	"SHRUG",
	"TRICK 1",
	"TRICK 2",
	"TURN",
	"LAUGH",
	"BLUSH",
	"ANGRY",
	"TRICK 3",
	"SLEEP",
};

const char	kstrGestureTextTable[kMaxGestures][32] = 
{
	"does nothing.",
	"waves.",
	"smiles.",
	"flirts.",
	"shrugs.",
	"performs an act.",
	"does something silly.",
	"turns around.",
	"laughs.",
	"blushes.",
	"frowns.",
	"poses.",
	"sleeps.",
};

//******************************************************
// get_NamedGesture
// Given the name of a gesture, returns the index of that
// gesture, or 0 if there is none.

STDMETHODIMP CSpriteManagerObject::get_NamedGesture(BSTR bstrGesture, int *pGestureNum)
{
	CString	strGest = bstrGesture;

	strGest.MakeUpper();

	if (0)
	{
		// TO DO: check the gesture table
	}
	else
	{
		*pGestureNum = 0;
		for (int i = 0; i < kMaxGestures; i++)
		{
			if (!strGest.Compare(kstrGestureNameTable[i]))
			{
				*pGestureNum = i;
				break;
			}
		}
	}

	return S_OK;
}


//******************************************************
// GetNamedGestures
// Returns an IPropertyList of all the named gestures

STDMETHODIMP CSpriteManagerObject::GetNamedGestures(IPropertyList *pGestureNameList)
{
	if (!pGestureNameList)
		return E_POINTER;

	if (0)
	{
		// TO DO: check the gesture table
	}
	else
	{
		for (int i = 0; i < kMaxGestures; i++)
			pGestureNameList->AddString(CComBSTR(kstrGestureNameTable[i]));
	}

	return S_OK;
}

//******************************************************
// GestureText
// Returns the text associated with that gesture

STDMETHODIMP CSpriteManagerObject::get_GestureText(BSTR bstrGesture, BSTR *pbstrText)
{
	CString	strGest = bstrGesture;

	strGest.MakeUpper();

	if (0)
	{
		// TO DO: check the gesture table
	}
	else
	{
		for (int i = 0; i < kMaxGestures; i++)
		{
			if (!strGest.Compare(kstrGestureNameTable[i]))
			{
				*pbstrText = CComBSTR(kstrGestureTextTable[i]).Copy();
				break;
			}
		}
	}

	return S_OK;
}
