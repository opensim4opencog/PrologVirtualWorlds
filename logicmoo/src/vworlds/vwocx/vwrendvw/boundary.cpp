// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.



#include "stdafx.h"
#include <vwmenu.h>
#include "VWRnVwCt.h"
#include "vwsgfxut.h"
#include <math.h>
#include "pickdata.h"
////////////////////////////////////////////////////////////////////
const COLORREF	colorBoundary = RGB(90,190,90); 
const COLORREF	colorSelectedBoundary = RGB(0,255,0); 

const COLORREF	colorSelectedNoModifyBoundary = RGB(255,120,120); 
const COLORREF	colorNoModifyBoundary = RGB(255,33,33); 

CVWRenderViewCtrl* CBoundaryDisplayObject::m_pRenderControl;

////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CBoundaryListPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
////////////////////////////////////////////////////////////////////
extern CComBSTR	bstrGetFrame;
extern CComBSTR	bstrBoundaryList;
extern CComBSTR	bstrEditingMode;
extern CComBSTR	bstrPosition;
////////////////////////////////////////////////////////////////////


float CVWRenderViewCtrl::ComputeAngleFromVector(float flX, float flY, float flZ)
{
	D3DVECTOR rlvPreDef, rlvDir;
	D3DVALUE  valCosTheta;
	float flAngle;

	// The pre-defined angle.
	rlvPreDef.x = 0.0F;
	rlvPreDef.y	= 0.0F;
	rlvPreDef.z	= 1.0F;

	// Make a D3DVECTOR from this vector.
	rlvDir.x = flX;
	rlvDir.y = flY;
	rlvDir.z = flZ;

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	flAngle = (D3DVALUE)acos(valCosTheta);

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.x > 0.0F)
	{
		// Acute angle will not work, need obtuse angle.
		flAngle = (float) ((2 * 3.14159265359) - flAngle);
	}
	
	return flAngle;

}


//For use when drawing BoundaryLists
void CVWRenderViewCtrl::ConvertFromThingToCameraCoords(D3DVECTOR* pvecPosition)
{

	D3DVECTOR vecResult, vecAxis;
	D3DVALUE valModulus;

	vecAxis.x = 0.0; 
	vecAxis.y = 1.0; 
	vecAxis.z = 0.0; 

	pvecPosition->x -= m_flCameraX;
	pvecPosition->z -= m_flCameraZ;

	valModulus = D3DRMVectorModulus(pvecPosition);
	D3DRMVectorRotate(&vecResult, pvecPosition, &vecAxis, m_flCameraAngle);
	D3DRMVectorScale(&vecResult, &vecResult, valModulus);

	pvecPosition->x = vecResult.x + m_flCameraX;
	pvecPosition->y = vecResult.y;
	pvecPosition->z = vecResult.z + m_flCameraZ;
}

//For use converting mouse coords to world space
void CVWRenderViewCtrl::ConvertFromCameraToThingCoords(D3DVECTOR* pvecPosition)
{
	D3DVECTOR vecResult, vecAxis;
	D3DVALUE valModulus;

	vecAxis.x = 0.0; 
	vecAxis.y = 1.0; 
	vecAxis.z = 0.0; 

	pvecPosition->x -= m_flCameraX;
	pvecPosition->z -= m_flCameraZ;

	valModulus = D3DRMVectorModulus(pvecPosition);
	D3DRMVectorRotate(&vecResult, pvecPosition, &vecAxis, -m_flCameraAngle);
	D3DRMVectorScale(&vecResult, &vecResult, valModulus);

	pvecPosition->x = vecResult.x + m_flCameraX;
	pvecPosition->y = vecResult.y;
	pvecPosition->z = vecResult.z + m_flCameraZ;

}



HRESULT CVWRenderViewCtrl::HandleDeviceToWorld(float *pflX, float *pflY,  float *pflZ)
{
	IVWRender3D * pr3D = NULL;

	HRESULT hr = S_OK;
	float flFactor = (m_flPixelsToWorldUnit ? m_flPixelsToWorldUnit : 1); // * m_flZoom;

//	TRACE( "HandleDeviceToWorld: Device x= %f, y = %f. ",*pflX , *pflY );
	if( (NULL == pflX) || (NULL == pflY) || (NULL == pflZ) )
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

//	ASSERT(0.0 != (flFactor) );

	if( flFactor < 0.001 && flFactor > -0.001 )
		return E_FAIL;

	// Offset to zero in center of screen and to camera coords
	*pflX -= ((float) m_cxPaintInfo / 2 );
	*pflY =  ((float) m_cyPaintInfo / 2 ) - *pflY;

	*pflX /= (flFactor);
	*pflX += m_flCameraX;

	*pflZ = *pflY / (flFactor);
	*pflZ += m_flCameraZ;
	*pflY = 0.0;

//	TRACE( " World x= %f, z = %f.\n",*pflX ,  *pflZ);

	D3DVECTOR vecVector;

	vecVector.x = *pflX;
	vecVector.y = *pflY;
	vecVector.z = *pflZ;

	ConvertFromCameraToThingCoords(&vecVector);

	*pflX = vecVector.x;
	*pflY = vecVector.y;
	*pflZ = vecVector.z;

EXIT_FAIL:

	return hr;

}

POINT   CVWRenderViewCtrl::WorldToDP(float flX, float flY, float flZ)
{
	POINT pt;
	D3DVECTOR vecVector;

	vecVector.x = flX;
	vecVector.y = flY;
	vecVector.z = flZ;

	ConvertFromThingToCameraCoords(&vecVector);

	flX = vecVector.x;
	flY = vecVector.y;
	flZ = vecVector.z;

	float flFactor = m_flPixelsToWorldUnit;// * m_flZoom;
//	TRACE( "WorldToDP: CamrX = %f, CamrZ = %f, World x= %f, y = %f, z = %f  ",m_flCameraX, m_flCameraZ, flX , flY, flZ);

	pt.x = (long)(m_cxPaintInfo / 2) + (long)((flX - m_flCameraX) * flFactor  );
	pt.y = (long)(m_cyPaintInfo / 2) - (long)((flZ - m_flCameraZ) * flFactor   );
//	TRACE( " DP x= %d, y = %d.\n",pt.x , pt.y);

	return pt;
}




HRESULT CBoundaryDisplayObject::DrawBoundaryEdgeIndex(HDC hDC, int nPosX, int nPosY, int nLastPosX, int nLastPosY, int nIndex)
{
	POINT ptMidpoint;
	char tmpstr[32];
	int nNumChar;
	float fX, fY, fLen;
	SIZE sizeSize;

	ptMidpoint.x = (nPosX - nLastPosX) / 2; 
	ptMidpoint.y = (nPosY - nLastPosY) / 2;

	if (ptMidpoint.x < 0.01 && ptMidpoint.y < 0.01 &&
		ptMidpoint.x > -0.01 && ptMidpoint.y > -0.01)
		goto EXIT_FAIL;

	fX = (float) ptMidpoint.y;
	fY = (float) -ptMidpoint.x;

	ptMidpoint.x += nLastPosX;
	ptMidpoint.y += nLastPosY;

	fLen = (float) sqrt( (fX * fX) + (fY * fY) );
	fX /= fLen;
	fY /= fLen;

	nNumChar = sprintf(tmpstr, "%d", nIndex);
	GetTextExtentPoint32(hDC, tmpstr, nNumChar, &sizeSize);
	TextOut(hDC,(int) (ptMidpoint.x - sizeSize.cx / 2 + fX * sizeSize.cx),
				(int) (ptMidpoint.y - sizeSize.cy / 2 + fY * sizeSize.cy),
				tmpstr, 
				nNumChar );

EXIT_FAIL:
	return S_OK;
}

HRESULT CVWRenderViewCtrl::DrawBoundaries()
{
	HRESULT					hr = S_OK;
	LPDIRECTDRAWSURFACE		pddsBackBuffer = NULL;
	HDC						hDC;
	LPDIRECT3DRMVIEWPORT	pViewport = NULL;
	IVWRender3D				*pVWRender3D = NULL;
	BOOL					bRecomputingBoundaries = FALSE;

//	if (m_bHaltRendering)
//	{
//		m_bRenderingHasHalted = TRUE;
//		goto EXIT_FAIL;
//	}

	hr = m_pVWRenderRoot->get_VWRender3D(&pVWRender3D);
	if (FAILED(hr) || !pVWRender3D)	goto EXIT_FAIL;

	hr = pVWRender3D->get_Viewport(&pViewport);
	if (FAILED(hr) || !pViewport) goto EXIT_FAIL;

	hr = m_pVWRenderRoot->GetBackBuffer(&pddsBackBuffer);
	if (FAILED(hr) || !pddsBackBuffer) goto EXIT_FAIL;

	hr = pddsBackBuffer->GetDC(&hDC);
	if (FAILED(hr))
	{
		ASSERT(0);
		goto EXIT_FAIL;
	}

	int i, nSize;

	if (bRecomputingBoundaries = m_bNeedUpdateBoundaryScreenCoordinates)
		m_bNeedUpdateBoundaryScreenCoordinates = FALSE;

	nSize = m_boundaryArray.GetSize();

	// Check m_bHaltRendering 'cause don't want to waste time drawing boundaries
	// while we're waiting to rebuild them.
	for (i = nSize - 1; i >= 0 && !m_bHaltRendering; i--)
	{
		CBoundaryDisplayObject* boundaryDisplayObj = m_boundaryArray.GetAt(i);

		if (boundaryDisplayObj->m_nStatus == BOUNDARYOBJECT_DELETE_ME)
		{
			while(boundaryDisplayObj->m_bIsLocked);
			boundaryDisplayObj->m_bIsLocked = TRUE;
			m_boundaryArray.RemoveAt(i);
			delete boundaryDisplayObj;
		}
		else
			boundaryDisplayObj->DrawBoundary(hDC, m_pControlManager, pViewport, bRecomputingBoundaries);
	}

	//Get rid of this stuff as quick as possible
	pddsBackBuffer->ReleaseDC(hDC);
	hDC = NULL;


EXIT_FAIL:
	SAFERELEASE(pddsBackBuffer);
	SAFERELEASE(pViewport);
	SAFERELEASE(pVWRender3D);

	return hr;
}

void CVWRenderViewCtrl::OnToolsEditBoundaries()
{
	HRESULT hr = S_OK;
	IWorld*						pWorld = NULL;
	IThing*						pGlobal = NULL;

	if (FAILED(hr = m_pRenderingContext->get_World(&pWorld)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pWorld->get_Global(&pGlobal)))
		goto ERROR_ENCOUNTERED;

	if (m_nEditingMode != EDIT_BOUNDARIES)
	{
		//m_nOldEditMode = m_nEditingMode;
		//SetEditMode(EDIT_BOUNDARIES);

		if (FAILED(hr = pGlobal->put_Long(bstrEditingMode, EDIT_BOUNDARIES)))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		//SetEditMode(m_nOldEditMode);
		if (FAILED(hr = pGlobal->put_Long(bstrEditingMode, m_nOldEditMode)))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pWorld);
	SAFERELEASE(pGlobal);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsEditBoundaries(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_nEditingMode == EDIT_BOUNDARIES ? 1 : 0);
}



void CVWRenderViewCtrl::OnToolsDelete() 
{
	CString szTmp;
	long nCount;
	int i, nNumDeleted = 0;
	IObjectProperty * pProperty = NULL;
	IPropertyList* pSelectionList = NULL, *pList = NULL;
	HRESULT hr;
	IVWUndoItem		*pVWUndoItem = NULL;
	BOOL bFirst = FALSE, bLast = FALSE;
	IWorld* pWorld = NULL;
	CComVariant varClone;

	ASSERT(m_pControlManager);

	if(m_pControlManager)
	{
		hr = m_pControlManager->get_SelectionList(&pSelectionList);	
		if( FAILED(hr)) goto ERROR_DONE;
	}

	if (FAILED(hr = pSelectionList->get_Count(&nCount)))
	{
		OutputDebugString("CVWRenderViewCtrl: Error getting count.\n");
		goto ERROR_DONE;
	}

	if (nCount <= 0)
	{
		OutputDebugString("CVWRenderViewCtrl: No boundaries selected.\n");
		goto ERROR_DONE;
	}

	for (i = nCount - 1; i >= 0 ; i--)
	{
		CVWPickDataPtr pickDataPtr;
		CBoundaryListPtr BoundaryListPtr;
		CBoundaryPtr boundaryPtr;
		CThingPtr thingPtr;
		int nIndex;

		pProperty = NULL;
		hr = pSelectionList->get_ObjectProperty(i, &pProperty);
		if (FAILED(hr )) 
		{		
			OutputDebugString("CVWRenderViewCtrl: Failed to retrieve property from prop list.\n");
			goto ERROR_DONE;
		}

		pickDataPtr = pProperty;

		if (pickDataPtr.p != NULL)
		{
			VARIANT_BOOL bAutoSolid, bCanWrite, bAnySelected;

			pickDataPtr->get_Boundary(&boundaryPtr.p);

			boundaryPtr->get_IsAutoSolidBoundary(&bAutoSolid);
			pickDataPtr->get_Thing(&thingPtr.p);

			hr = m_pControlManager->IsAnyVertexSelected(boundaryPtr.p, &bAnySelected);
			if (FAILED(hr)) goto ERROR_DONE;

			if (!bAnySelected && bAutoSolid == VARIANT_TRUE)
			{
				CWnd::MessageBox("This boundary was created by the object's IsSolid property.  To delete this boundary, change the object's IsSolid property to False.", "Microsoft Virtual Worlds");
			}
			else				
			{
				thingPtr->CheckPropertySecurity(bstrBoundaryList, PS_WRITE, &bCanWrite);

				if (!bCanWrite)
				{
					CWnd::MessageBox("You do not have permission to delete this boundary.", "Microsoft Virtual Worlds");
				}
				else
				{
					VARIANT_BOOL bSafe;
					int nNumPoints, bErrors, i;

					if (bAnySelected)
					{
						hr = boundaryPtr->get_VertexCount(&nNumPoints);
						if (FAILED(hr)) goto ERROR_DONE;

						bErrors = 0;

						for (i = nNumPoints; i >= 0; i--)
						{
							hr = pickDataPtr->get_VertexPicked(i, &bAnySelected);
							if (FAILED(hr)) goto ERROR_DONE;
			
							if (bAnySelected)
							{
								hr = pickDataPtr->put_VertexPicked(i, VARIANT_FALSE);
								if (FAILED(hr)) goto ERROR_DONE;

								hr = boundaryPtr->DeleteVertexSafe(i, &bSafe);
								if (FAILED(hr)) goto ERROR_DONE;

								if (!bSafe)
									bErrors++;
							}
						}

						pickDataPtr->get_BoundaryList(&BoundaryListPtr.p);
						pickDataPtr->get_BoundaryIndex(&nIndex);

						hr = BoundaryListPtr->get_World(&pWorld);
						if (FAILED(hr)) goto ERROR_DONE;

						pWorld->CloneProperty(CComVariant(boundaryPtr.p), &varClone);
						if (FAILED(hr)) goto ERROR_DONE;

						BoundaryListPtr->put_Dispatch(nIndex, varClone.pdispVal);
						if (FAILED(hr)) goto ERROR_DONE;

						if (bErrors)
							CWnd::MessageBox("One or more vertices could not be deleted.  Boundaries must be simple closed polygons.", "Microsoft Virtual Worlds");

					}
					else
					{
						hr = m_pControlManager->RemoveSelectionExt(m_pVWUIView, CComVariant(pProperty) ); //CComVariant(cBoundaryPtr.p) );
						if (FAILED(hr)) goto ERROR_DONE;

						pickDataPtr->get_BoundaryList(&BoundaryListPtr.p);

						if (BoundaryListPtr.p != NULL && boundaryPtr.p != NULL)
						{
							pickDataPtr->get_BoundaryIndex(&nIndex);

							hr = BoundaryListPtr->RemoveAt( nIndex );
							if (FAILED(hr)) goto ERROR_DONE;

							//RemoveBoundaryByIndex(thingPtr.p, nIndex);
						}
					}
				}
			}
		}
		else
		{
			thingPtr = pProperty;
			if (thingPtr.p != NULL)
			{
				hr = thingPtr->get_World(&pWorld);
				if (FAILED(hr)) goto ERROR_DONE;

				hr = pWorld->DestroyObject(thingPtr);
				if (FAILED(hr)) 
				{
					if (hr == VWOBJECT_E_PERMISSION_DENIED)
					{
						CWnd::MessageBox("You do not have permission to delete selected object.", "Microsoft Virtual Worlds");
						hr = S_OK;
					}
					else
					{
						CWnd::MessageBox("Failed to delete selected object.", "Microsoft Virtual Worlds");
						goto ERROR_DONE;
					}
					
				}
				SAFERELEASE(pWorld);
			}
		}
		SAFERELEASE(pProperty);
	}

ERROR_DONE:
	SAFERELEASE(pWorld);
	SAFERELEASE(pSelectionList);
	SAFERELEASE(pProperty);
	SAFERELEASE(pList  );

	return;
}

void CVWRenderViewCtrl::OnUpdateToolsDelete(CCmdUI* pCmdUI) 
{
	long nCount;
	int i;
	IObjectProperty * pProperty = NULL;
	IPropertyList* pSelectionList = NULL;
	HRESULT hr = S_OK;
	int nEdgeSelected = 0;
	VARIANT_BOOL vbWrite;

	ASSERT(m_pControlManager);

	hr = m_pControlManager->get_SelectionList(&pSelectionList);	
	if( FAILED(hr)) goto ERROR_DONE;

	if (FAILED(hr = pSelectionList->get_Count(&nCount)))
	{
		OutputDebugString("CVWRenderViewCtrl: Error getting count.\n");
		goto ERROR_DONE;
	}

	for (i = 0; i < nCount; i++)
	{
		CVWPickDataPtr pickDataPtr;
		CThingPtr pThing, pOwner;

		pProperty = NULL;
		hr = pSelectionList->get_ObjectProperty(i, &pProperty);

		pickDataPtr = pProperty;
		if (pickDataPtr.p != NULL)
		{
			pickDataPtr->get_Thing(&pThing.p);

			if (pThing.p)
			{
				hr = pThing->CheckPropertySecurity(bstrBoundaryList, PS_WRITE, &vbWrite);
				if (FAILED(hr)) goto ERROR_DONE;

				if (vbWrite == VARIANT_TRUE)
					nEdgeSelected++;
			}
		}
		else
		{
			pThing = pProperty;
			if (pThing.p != NULL)
			{
				hr = pThing->get_Owner(&pOwner.p);
				if (FAILED(hr)) goto ERROR_DONE;

				if (pOwner == m_pRenderingContext)
					nEdgeSelected++;
			}
		}
		SAFERELEASE(pProperty);

	}
	if (nEdgeSelected > 0)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetText("Delete");
	}
	else
		pCmdUI->Enable(FALSE);


ERROR_DONE:
	SAFERELEASE(pSelectionList);
	SAFERELEASE(pProperty);

	return;
}

void CVWRenderViewCtrl::OnUpdateToolsDumpToScript(CCmdUI* pCmdUI) 
{
	HRESULT hr = S_OK;
	long nContentCount = 0;

	hr = m_pControlManager->GetSelectedCount((GUID*) &IID_IThing, &nContentCount);
	if( FAILED(hr)) goto ERROR_ENCOUNTERED;

	if (nContentCount > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);

ERROR_ENCOUNTERED:
	return;
}

void CVWRenderViewCtrl::OnToolsDumpToScript() 
{
	IThing* pContainer = NULL, *pThing = NULL;
	IPropertyList* pContents = NULL, *pSelectList = NULL, *pSelectionList = NULL;
	HRESULT hr = S_OK;
	long nContentCount;
	BSTR bstrOut = NULL;
	CString str, strFilename = "c:\\edges.txt";
	VARIANT_BOOL bAsVal = VARIANT_TRUE;
	IObjectProperty* pBoundaryList = NULL, *pObjProp = NULL;
	CComBSTR bstrName;
	CString szFirstLetter;
	static CComBSTR cbstrBoundaryList("BoundaryList");
	CStdioFile file;
	HWND hWnd;
	static char szFilter[] = "Script files (*.scr)|*.scr|All files (*.*)|*.*\0";

	CFileDialog saveFileDlg(FALSE, "*.scr", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, szFilter);

	hr = m_pVWUIView->get_HWND(&hWnd);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = m_pControlManager->get_SelectionList(&pSelectionList);	
	if( FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pSelectionList->get_Count(&nContentCount);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	if (!nContentCount)
	{
		CWnd::MessageBox("Nothing is selected, the operation was canceled.", "Microsoft Virtual Worlds");
		goto ERROR_ENCOUNTERED;
	}

	if (saveFileDlg.DoModal() != IDOK)
	{
		goto ERROR_ENCOUNTERED;
	}

	// open output file
	if (!file.Open(	saveFileDlg.m_ofn.lpstrFile, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive |CFile::typeText ))
		goto ERROR_ENCOUNTERED;

	// write out header
	// REVIEW: Should probably put all this stuff in resources

	hr = pSelectionList->get_ObjectProperty(0, (IObjectProperty**) &pObjProp);
	if (FAILED(hr) || !pObjProp) goto ERROR_ENCOUNTERED;

	hr = pObjProp->QueryInterface(IID_IThing, (LPVOID*)&pThing);
	if (FAILED(hr) || !pThing) goto ERROR_ENCOUNTERED;

	hr = pThing->get_Name(&bstrName.m_str);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	str = bstrName;

	if (nContentCount > 0 && str.GetLength() > 1)
	{
		int npos;
		CString szGoodName;

		szFirstLetter = str.Left(1);
		szFirstLetter.MakeUpper();
		str = str.Mid(1);

		szGoodName = str;
		npos = -1;

		while ( (npos = str.Mid(npos+1).FindOneOf(" ")) != -1)
		{
			if (npos > 0)
				szGoodName = str.Left(npos);

			if (npos < str.GetLength() - 1)
			{
				szGoodName += str.Mid(npos + 1);
			}
		}

		file.WriteString( CString("Sub Create") + szFirstLetter + szGoodName + CString("Boundaries(thething)\n\n") );
	}
	else
		file.WriteString("Sub CreateBoundaries(thething)\n\n");

	file.WriteString("\tDim Ecb, theBoundaryList, theBoundaryList_t\n");

	hr = pThing->get_ObjectProperty(cbstrBoundaryList, (IObjectProperty**) &pBoundaryList ); // BoundaryListPtr = var;
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	if (pBoundaryList)
	{
		hr = pBoundaryList->MarshallToScript(MARSHALL_BYVALUE, CComBSTR("theBoundaryList"), &bAsVal, &bstrOut);
		if (FAILED(hr)) goto ERROR_ENCOUNTERED;

		str = bstrOut;
		file.WriteString(str);

		file.WriteString("\tset thething.BoundaryList = theBoundaryList\n");

		SAFEFREESTRING(bstrOut);
	}
	else
		file.WriteString("\t'This object has no BoundaryList property!\n");

	file.WriteString("End Sub\n\n");
	SAFERELEASE(pThing);

	CWnd::MessageBox("Script creation successful.", "Microsoft Virtual Worlds");

ERROR_ENCOUNTERED:
	SAFEFREESTRING(bstrOut);
	SAFERELEASE(pBoundaryList);
	SAFERELEASE(pSelectionList);
	SAFERELEASE(pSelectList);
	SAFERELEASE(pObjProp);
	SAFERELEASE(pThing);
	SAFERELEASE(pContents);
	SAFERELEASE(pContainer);
	return;
}

HRESULT CBoundaryDisplayObject::DrawBoundary(HDC hDC, IVWControlManager* pControlManager, LPDIRECT3DRMVIEWPORT pViewport, BOOL bNeedUpdateBoundaryScreenCoordinates)
{
	HPEN hPen = NULL, hOldPen;
	HRESULT hr = S_OK;
	int j;
	POINT ptFirstPosition, ptLastPosition;

	ASSERT(hDC);
	ASSERT(pViewport);

	if (m_nStatus == BOUNDARYOBJECT_UPDATE_INTERNAL_INFO) //m_bNeedUpdateInternalInfo)
		UpdateInternalInfo(pControlManager, pViewport);

	//If this guy is being updated, dump out and draw the next one.
	if (m_bIsLocked)
		return S_OK;

	m_bIsLocked = TRUE;

	if (m_bNoModify)
	{
		if (m_bIsSelected)
		{
			hPen = CreatePen( (m_bPassable ? PS_DASH : PS_INSIDEFRAME), 0, colorSelectedNoModifyBoundary);
			SetTextColor(hDC, colorSelectedNoModifyBoundary);
		}
		else
		{
			hPen = CreatePen((m_bPassable ? PS_DASH : PS_INSIDEFRAME), 0, colorNoModifyBoundary);
			SetTextColor(hDC, colorNoModifyBoundary);
		}
	}
	else
	{
		if (m_bIsSelected)
		{
			hPen = CreatePen((m_bPassable ? PS_DASH : PS_INSIDEFRAME), 0, colorSelectedBoundary);
			SetTextColor(hDC, colorSelectedBoundary);
		}
		else
		{
			hPen = CreatePen((m_bPassable ? PS_DASH : PS_INSIDEFRAME), 0, colorBoundary);
			SetTextColor(hDC, colorBoundary);
		}
	}

	hOldPen = (HPEN) ::SelectObject(hDC, hPen);

	//***WARNING***  Don't goto EXIT_FAIL before pen's been init'ed.

	SetBkColor(hDC, RGB(0,0,0) );
	SetBkMode(hDC, TRANSPARENT );
	
	// Check m_bHaltRendering 'cause don't want to waste time drawing boundaries
	// while we're waiting to rebuild them.
	for (j = 0; j < m_nNumPoints && !m_pRenderControl->m_bHaltRendering; j++)
	{
		if (bNeedUpdateBoundaryScreenCoordinates)
		{
			D3DVECTOR vIn, vOut;
			D3DRMVECTOR4D vScreen;


			vIn.x = m_pLocalPoints[j].x;
			vIn.y = 0.0f;
			vIn.z = m_pLocalPoints[j].y;

			hr = m_pVWFrame->Transform(AXIS_Y, &vOut.x, &vOut.y, &vOut.z, vIn.x, vIn.y, vIn.z);
			if (FAILED(hr) ) goto EXIT_FAIL;

			hr = pViewport->Transform(&vScreen, &vOut);
			if(FAILED(hr)) goto EXIT_FAIL;

			m_pLocalPoints[j].screenPnt.x = (long) vScreen.x;
			m_pLocalPoints[j].screenPnt.y = (long) vScreen.y;
		}

		if( 0 == j ) // First Point
		{
			::MoveToEx(hDC, (long) m_pLocalPoints[j].screenPnt.x, (long) m_pLocalPoints[j].screenPnt.y, NULL);
			ptFirstPosition.x = (long) m_pLocalPoints[j].screenPnt.x;
			ptFirstPosition.y = (long) m_pLocalPoints[j].screenPnt.y;
		}
		else
		{
			LineTo(hDC, (int) m_pLocalPoints[j].screenPnt.x, (int) m_pLocalPoints[j].screenPnt.y);

			//if (m_bIsSelected)
			//	DrawBoundaryEdgeIndex(hDC, (int) m_pLocalPoints[j].screenPnt.x, (int) m_pLocalPoints[j].screenPnt.y, ptLastPosition.x, ptLastPosition.y, j);
		}

		//If it's selected we need to draw the vertices as well.
		if (m_bIsSelected)
		{
			Rectangle(hDC, (int) m_pLocalPoints[j].screenPnt.x - 2, (int) m_pLocalPoints[j].screenPnt.y - 2, (int) m_pLocalPoints[j].screenPnt.x + 2, (int) m_pLocalPoints[j].screenPnt.y + 2);		

			if(VARIANT_TRUE == m_pLocalPoints[j].bVertexSelected)
			{
				MoveToEx(hDC, (int) m_pLocalPoints[j].screenPnt.x + 4, (int) m_pLocalPoints[j].screenPnt.y + 4, NULL);
				LineTo(hDC, (int) m_pLocalPoints[j].screenPnt.x + 4, (int) m_pLocalPoints[j].screenPnt.y - 4);
				LineTo(hDC, (int) m_pLocalPoints[j].screenPnt.x - 4, (int) m_pLocalPoints[j].screenPnt.y - 4);
				LineTo(hDC, (int) m_pLocalPoints[j].screenPnt.x - 4, (int) m_pLocalPoints[j].screenPnt.y + 4);
				LineTo(hDC, (int) m_pLocalPoints[j].screenPnt.x + 4, (int) m_pLocalPoints[j].screenPnt.y + 4);
				MoveToEx(hDC, (int) m_pLocalPoints[j].screenPnt.x, (int) m_pLocalPoints[j].screenPnt.y, NULL);
			}
		}
		else
		{
			Rectangle(hDC, (int) m_pLocalPoints[j].screenPnt.x - 1, (int) m_pLocalPoints[j].screenPnt.y - 1, (int) m_pLocalPoints[j].screenPnt.x + 1, (int) m_pLocalPoints[j].screenPnt.y + 1);		
		}

		::MoveToEx(hDC, (int) m_pLocalPoints[j].screenPnt.x, (int) m_pLocalPoints[j].screenPnt.y, NULL);

		ptLastPosition.x = (long) m_pLocalPoints[j].screenPnt.x;
		ptLastPosition.y = (long) m_pLocalPoints[j].screenPnt.y;
	}

	if (m_bClosed)
	{
		LineTo(hDC, ptFirstPosition.x, ptFirstPosition.y);
		//if (m_bIsSelected)
		//	DrawBoundaryEdgeIndex(hDC, ptFirstPosition.x, ptFirstPosition.y, ptLastPosition.x, ptLastPosition.y, j);
	}

EXIT_FAIL:
	m_bIsLocked = FALSE;

	::SelectObject(hDC, hOldPen);
	if (hPen)
		DeleteObject(hPen);

	return hr;
}


HRESULT CVWRenderViewCtrl::AddBoundaryByIndex(IThing* pThing, int nBoundaryIndex)
{
	HRESULT hr = S_OK;
	int i, nSize;
	IPropertyList* pBoundaryList = NULL;
	IBoundary* pBoundary = NULL;
	CBoundaryDisplayObject *pDisplayObject;
	IVWRender3D				*pVWRender3D = NULL;
	LPDIRECT3DRMVIEWPORT	pViewport = NULL;
	BOOL bFound = FALSE;

	hr = m_pVWRenderRoot->get_VWRender3D(&pVWRender3D);
	if (FAILED(hr) || !pVWRender3D)	goto EXIT_FAIL;

	hr = pVWRender3D->get_Viewport(&pViewport);
	if (FAILED(hr) || !pViewport) goto EXIT_FAIL;

	hr = pThing->get_ObjectProperty(bstrBoundaryList, (IObjectProperty**) &pBoundaryList);
	if (FAILED(hr) || !pBoundaryList) goto EXIT_FAIL;

	nSize = m_boundaryArray.GetSize();

	for (i = 0; i < nSize; i++)
	{
		pDisplayObject = m_boundaryArray.GetAt(i);

		if (pDisplayObject->m_pThing == pThing && pDisplayObject->m_nBoundaryIndex == nBoundaryIndex)
		{
			bFound = TRUE;
			pDisplayObject->m_nStatus = BOUNDARYOBJECT_UPDATE_INTERNAL_INFO;
			break;
		}
	}

	if (!bFound)
	{
		hr = pBoundaryList->get_ObjectProperty(nBoundaryIndex, (IObjectProperty**) &pBoundary);
		if (FAILED(hr) || !pBoundary) goto EXIT_FAIL;

		pDisplayObject = new CBoundaryDisplayObject(pThing, pBoundary, nBoundaryIndex, m_pControlManager, pViewport);

		if (pDisplayObject)
			m_boundaryArray.Add(pDisplayObject);
		else
			OutputDebugString("RenderView: Failed to create boundary display list object.\n");
	}

EXIT_FAIL:
	SAFERELEASE(pViewport);
	SAFERELEASE(pVWRender3D);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pBoundaryList);
	return hr;
}

HRESULT CVWRenderViewCtrl::AddThingToBoundaryDisplayArray(IThing* pThing)
{
	HRESULT hr = S_OK;
	int i;
	long nNumObjs;
	IPropertyList* pBoundaryList = NULL;
	IBoundary* pBoundary = NULL;
	//VARIANT_BOOL vbSelected;
	CBoundaryDisplayObject *pDisplayObject;
	IVWRender3D				*pVWRender3D = NULL;
	LPDIRECT3DRMVIEWPORT	pViewport = NULL;

	hr = m_pVWRenderRoot->get_VWRender3D(&pVWRender3D);
	if (FAILED(hr) || !pVWRender3D)	goto EXIT_FAIL;

	hr = pVWRender3D->get_Viewport(&pViewport);
	if (FAILED(hr) || !pViewport) goto EXIT_FAIL;

	hr = pThing->get_ObjectProperty(bstrBoundaryList, (IObjectProperty**) &pBoundaryList);
	if (FAILED(hr) || !pBoundaryList) goto EXIT_FAIL;

	hr = pBoundaryList->get_Count(&nNumObjs);
	if (FAILED(hr)) goto EXIT_FAIL;

	for (i = 0; i < nNumObjs; i++)
	{
		hr = pBoundaryList->get_ObjectProperty(i, (IObjectProperty**) &pBoundary);
		if (FAILED(hr) || !pBoundary) goto EXIT_FAIL;

		pDisplayObject = new CBoundaryDisplayObject(pThing, pBoundary, i, m_pControlManager, pViewport);

		if (pDisplayObject)
			m_boundaryArray.Add(pDisplayObject);
		else
			OutputDebugString("RenderView: Failed to create boundary display list object.\n");

		SAFERELEASE(pBoundary);
	}

EXIT_FAIL:
	SAFERELEASE(pViewport);
	SAFERELEASE(pVWRender3D);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pBoundaryList);
	return hr;
}

HRESULT CVWRenderViewCtrl::UpdateBoundary(IBoundary* pBoundary, unsigned long nHint)
{
	int i, nSize;
	HRESULT hr = S_OK;
	BOOL					bUpdated = FALSE;
	IVWRender3D				*pVWRender3D = NULL;
	LPDIRECT3DRMVIEWPORT	pViewport = NULL;

	hr = m_pVWRenderRoot->get_VWRender3D(&pVWRender3D);
	if (FAILED(hr) || !pVWRender3D)	goto EXIT_FAIL;

	hr = pVWRender3D->get_Viewport(&pViewport);
	if (FAILED(hr) || !pViewport) goto EXIT_FAIL;

	nSize = m_boundaryArray.GetSize();

	for (i = 0; i < nSize; i++)
	{
		CBoundaryDisplayObject* boundaryDisplayObj;

		boundaryDisplayObj = m_boundaryArray.GetAt(i);

		//If we're clearing selections just reset the clear selection bit.
		if (nHint == BOUNDARYUPDATE_CLEARSELECTION)
		{
			boundaryDisplayObj->m_bIsSelected = FALSE;
		}
		else if ( boundaryDisplayObj->m_pBoundary == pBoundary)
		{
			if (nHint == BOUNDARYUPDATE_ADDSELECTION)
			{
				boundaryDisplayObj->m_bIsSelected = TRUE;
				bUpdated = TRUE;
			}
			else if (nHint == BOUNDARYUPDATE_REMOVESELECTION)
			{
				boundaryDisplayObj->m_bIsSelected = FALSE;
				bUpdated = TRUE;
			}
			else if (nHint == BOUNDARYUPDATE_UPDATEINFO)
			{
				//hr = boundaryDisplayObj->UpdateInternalInfo(m_pControlManager, pViewport);
				//boundaryDisplayObj->m_bNeedUpdateInternalInfo = TRUE;
				boundaryDisplayObj->m_nStatus = BOUNDARYOBJECT_UPDATE_INTERNAL_INFO;
				bUpdated = TRUE;
			}
			else
			{
				OutputDebugString("Unhandled hint flag passed to UpdateBoundary\n");
				ASSERT(FALSE);
			}
			break;
		}
	}

	if (!bUpdated)
		hr = S_FALSE;

EXIT_FAIL:
//	m_bHaltRendering = FALSE;

	SAFERELEASE(pViewport);
	SAFERELEASE(pVWRender3D);

	return hr;
}

HRESULT CVWRenderViewCtrl::RebuildBoundaryArray()
{
	HRESULT hr = S_OK;
	IThing* pRoom = NULL, *pChildThing = NULL;
	IVWFrame* pScene = NULL;
	IPropertyList* pList = NULL;
	long nNumObjs;
	int i, nSize;
	nSize = m_boundaryArray.GetSize();
	CBoundaryDisplayObject* pBDObject;

	OutputDebugString("RenderView: Rebuilding boundary display list.\n");

	if (!m_bRenderingHasHalted)
	{
		m_bHaltRendering = TRUE;

		// Wait for rendering to halt.
		while (!m_bRenderingHasHalted)
			Sleep(50);
	}

	//Set static back pointer.
	CBoundaryDisplayObject::m_pRenderControl = this;

	for (i = nSize - 1; i >= 0 ; i--)
	{
		pBDObject = m_boundaryArray.GetAt(i);
		m_boundaryArray.RemoveAt(i);
		delete pBDObject;
	}

	ASSERT(m_boundaryArray.GetSize() == 0);

	hr = m_pVWRenderRoot->get_Scene(&pScene);
	if (FAILED(hr) || !pScene) goto EXIT_FAIL;

	hr = m_pRenderingContext->get_Container(&pRoom);
	if (FAILED(hr) || !pRoom) goto EXIT_FAIL;

	AddThingToBoundaryDisplayArray(pRoom);

	hr = pRoom->get_Contents(&pList);
	if (FAILED(hr) || !pList) goto EXIT_FAIL;

	hr = pList->get_Count(&nNumObjs);
	if (FAILED(hr)) goto EXIT_FAIL;

	for (i = 0; i < nNumObjs; i++)
	{
		hr = pList->get_ObjectProperty(i, (IObjectProperty**) &pChildThing);
		if (FAILED(hr) || !pChildThing) goto EXIT_FAIL;

		AddThingToBoundaryDisplayArray(pChildThing);

		SAFERELEASE(pChildThing);
	}


EXIT_FAIL:

	m_bHaltRendering = FALSE;

	SAFERELEASE(pScene);
	SAFERELEASE(pRoom);
	SAFERELEASE(pList);
	SAFERELEASE(pChildThing);
	return hr;
}

CBoundaryDisplayObject::CBoundaryDisplayObject(IThing* pThing, 
											   IBoundary* pBoundary, 
											   int nBoundaryIndex, 
											   IVWControlManager* m_pControlManager,
											   LPDIRECT3DRMVIEWPORT pViewport) 
{ 
	HRESULT hr = S_OK;
	VARIANT var;
	IVWFrame* pVWFrame = NULL;

	ASSERT(pThing);
	ASSERT(pBoundary);
	
	//First and foremost, initialize all pointers.
	m_pThing = NULL;
	m_pBoundary = NULL;
	m_pVWFrame = NULL;
	m_pLocalPoints = NULL;
	m_bIsLocked = FALSE;
	//m_bNeedUpdateInternalInfo = FALSE;
	m_nStatus = 0;
	m_nNumPoints = 0;

	m_pThing = pThing;
	m_pThing->AddRef();

	m_pBoundary = pBoundary;
	m_pBoundary->AddRef();

	m_nBoundaryIndex = nBoundaryIndex;

	VariantInit(&var);

	hr = m_pThing->InvokeMethodExt(bstrGetFrame, NULL, &var);
	if (var.vt != VT_DISPATCH || var.pdispVal == NULL) goto EXIT_FAIL;

	m_pVWFrame = (IVWFrame*)var.pdispVal;
	SAFEADDREF(m_pVWFrame);
	if (!m_pVWFrame) goto EXIT_FAIL;

	if (FAILED(hr = m_pThing->CheckPropertySecurityExt(bstrBoundaryList, PS_WRITE)))
	{
		m_bNoModify = TRUE;
		hr = S_OK;
	}
	else
		m_bNoModify = FALSE;

	UpdateInternalInfo(m_pControlManager, pViewport);

	goto EXIT_SUCCESSES;

EXIT_FAIL:
	SAFERELEASE(m_pBoundary);
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pVWFrame);

	nBoundaryIndex = -1;

	VariantClear(&var);
	if (m_pLocalPoints) free(m_pLocalPoints);

	delete this;
EXIT_SUCCESSES:
	VariantClear(&var);

};

CBoundaryDisplayObject::~CBoundaryDisplayObject()
{
	if (m_pLocalPoints) 
		free(m_pLocalPoints); 

	SAFERELEASE(m_pBoundary);
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pVWFrame);

}

HRESULT CBoundaryDisplayObject::UpdateInternalInfo(IVWControlManager* m_pControlManager, LPDIRECT3DRMVIEWPORT pViewport)
{
	HRESULT hr = S_OK;
	int i;
	VARIANT_BOOL bAnySelected = VARIANT_FALSE;
	IPropertyList* pBoundaryList = NULL;

//	while (m_bIsLocked);
	m_bIsLocked = TRUE;

	//Because of the clone/put issue we need to re-get the boundary....
	SAFERELEASE(m_pBoundary);
	hr = m_pThing->get_ObjectProperty(bstrBoundaryList, (IObjectProperty**) &pBoundaryList);
	if (FAILED(hr) || !pBoundaryList) goto EXIT_FAIL;

	hr = pBoundaryList->get_ObjectProperty(m_nBoundaryIndex, (IObjectProperty**) &m_pBoundary);
	if (FAILED(hr) || !m_pBoundary) 
	{
		ASSERT(0);
		m_nStatus = BOUNDARYOBJECT_DELETE_ME;
		goto EXIT_FAIL;
	}

	hr = m_pControlManager->IsBoundarySelected(m_pThing, m_nBoundaryIndex, &m_bIsSelected);
	if (FAILED(hr)) goto EXIT_FAIL;

	if (m_bIsSelected)
	{
		hr = m_pControlManager->IsAnyVertexSelected(m_pBoundary, &bAnySelected);
		if (FAILED(hr)) goto EXIT_FAIL;
	}

	hr = m_pBoundary->get_IsPassable(&m_bPassable);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = m_pBoundary->get_IsClosed(&m_bClosed);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = m_pBoundary->get_VertexCount(&m_nNumPoints);
	if (FAILED(hr)) goto EXIT_FAIL;

	if (m_pLocalPoints)
	{
		free(m_pLocalPoints);
		m_pLocalPoints = NULL;
	}

	m_pLocalPoints = (BoundaryPoint*) malloc(sizeof(BoundaryPoint) * m_nNumPoints);

	for (i = 0; i < m_nNumPoints; i++)
	{
		hr = m_pBoundary->GetVertexXYExt(i, &m_pLocalPoints[i].x, &m_pLocalPoints[i].y);
		if (FAILED(hr)) goto EXIT_FAIL;

		if (bAnySelected)
		{
			hr = m_pControlManager->IsVertexSelected(m_pBoundary, i, &m_pLocalPoints[i].bVertexSelected);
			if (FAILED(hr)) goto EXIT_FAIL;
		}
	}

	UpdateScreenInfo(pViewport);

	m_nStatus = 0;
	//m_bNeedUpdateInternalInfo = FALSE;

EXIT_FAIL:
	m_bIsLocked = FALSE;
	SAFERELEASE(pBoundaryList);

//	m_pRenderControl->m_bHaltRendering = FALSE;
	return hr;
}

HRESULT CBoundaryDisplayObject::UpdateScreenInfo(LPDIRECT3DRMVIEWPORT pViewport)
{
	HRESULT hr = S_OK;
	int j;
	D3DVECTOR vIn, vOut;
	D3DRMVECTOR4D vScreen;

//	if (!m_pRenderControl->m_bRenderingHasHalted)
//	{
//		m_pRenderControl->m_bHaltRendering = TRUE;
//		while (m_pRenderControl->m_bRenderingHasHalted == FALSE);
//	}

//	while(m_bIsLocked);
	m_bIsLocked = TRUE;

	for (j = 0; j < m_nNumPoints; j++)
	{
		vIn.x = m_pLocalPoints[j].x;
		vIn.y = 0.0f;
		vIn.z = m_pLocalPoints[j].y;

		hr = m_pVWFrame->Transform(AXIS_Y, &vOut.x, &vOut.y, &vOut.z, vIn.x, vIn.y, vIn.z);
		if (FAILED(hr) ) goto EXIT_FAIL;

		hr = pViewport->Transform(&vScreen, &vOut);
		if(FAILED(hr)) goto EXIT_FAIL;

		m_pLocalPoints[j].screenPnt.x = (long) vScreen.x;
		m_pLocalPoints[j].screenPnt.y = (long) vScreen.y;
	}

EXIT_FAIL:
	m_bIsLocked = FALSE;
//	m_pRenderControl->m_bHaltRendering = FALSE;

	return hr;
}

HRESULT CVWRenderViewCtrl::RemoveBoundaryByIndex(IThing* pThing, long nBoundaryIndex)
{
	int i, nSize;
	HRESULT hr = S_OK;
	BOOL					bUpdated = FALSE;
	CBoundaryDisplayObject* boundaryDisplayObj;

	nSize = m_boundaryArray.GetSize();

	for (i = 0; i < nSize; i++)
	{
		boundaryDisplayObj = m_boundaryArray.GetAt(i);

		if (boundaryDisplayObj->m_pThing == pThing)
		{
			if (boundaryDisplayObj->m_nBoundaryIndex == nBoundaryIndex)
			{
				boundaryDisplayObj->m_nStatus = BOUNDARYOBJECT_DELETE_ME;
				bUpdated = TRUE;
			}

			if (boundaryDisplayObj->m_nBoundaryIndex > nBoundaryIndex)
				boundaryDisplayObj->m_nBoundaryIndex--;

//			while(boundaryDisplayObj->m_bIsLocked);
//			boundaryDisplayObj->m_bIsLocked = TRUE;
//
//			bUpdated = TRUE;
//			m_boundaryArray.RemoveAt(i);
//			delete boundaryDisplayObj;
//			goto EXIT_FAIL;
		}
	}

	if (!bUpdated)
		hr = S_FALSE;

//EXIT_FAIL:

//	m_bHaltRendering = FALSE;

	return hr;
}

HRESULT CVWRenderViewCtrl::UpdateBoundaryByIndex(IThing* pThing, long nBoundaryIndex)
{
	int i, nSize;
	HRESULT hr = S_OK;
	BOOL					bUpdated = FALSE;
	CBoundaryDisplayObject* boundaryDisplayObj;
//	IVWRender3D				*pVWRender3D = NULL;
//	LPDIRECT3DRMVIEWPORT	pViewport = NULL;
//
//	hr = m_pVWRenderRoot->get_VWRender3D(&pVWRender3D);
//	if (FAILED(hr) || !pVWRender3D)	goto EXIT_FAIL;
//
//	hr = pVWRender3D->get_Viewport(&pViewport);
//	if (FAILED(hr) || !pViewport) goto EXIT_FAIL;

	nSize = m_boundaryArray.GetSize();

	for (i = 0; i < nSize; i++)
	{
		boundaryDisplayObj = m_boundaryArray.GetAt(i);

		if (boundaryDisplayObj->m_pThing == pThing && boundaryDisplayObj->m_nBoundaryIndex == nBoundaryIndex)
		{
			bUpdated = TRUE;
			//boundaryDisplayObj->UpdateInternalInfo(m_pControlManager, pViewport);
			//boundaryDisplayObj->m_bNeedUpdateInternalInfo = TRUE;
			boundaryDisplayObj->m_nStatus = BOUNDARYOBJECT_UPDATE_INTERNAL_INFO;

			break;
		}
	}

	if (!bUpdated)
		hr = S_FALSE;

//EXIT_FAIL:
//	SAFERELEASE(pViewport);
//	SAFERELEASE(pVWRender3D);

	return hr;
}

