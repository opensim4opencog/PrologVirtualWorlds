// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// FileAcTl.cpp : Implementation of CVWFileAccessTool
#include "stdafx.h"
#include "vwstudio.h"
#include "FileAcTl.h"
#include "commdlg.h"
#include <afxdlgs.h>
#include "reghelp.h"
#include <cderr.h>
/////////////////////////////////////////////////////////////////////////////
// CVWFileAccessTool


STDMETHODIMP CVWFileAccessTool::ReadFile(BSTR bstrFilename, BSTR* bstrFileText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	char szFile[_MAX_PATH] = "";
	CString szText;
	DWORD dwBytesToRead;
	DWORD dwBytesRead;
	CString szString(bstrFilename);
	CComBSTR bszString;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	static char szFilter[] = "Text files (*.txt)|*.txt|Script files (*.scr;*.vbs;*.js)|*.scr;*.vbs;*.js|All files (*.*)|*.*||";

	(*bstrFileText) = NULL;

	if (szString.GetLength() == 0)
	{
		CFileDialog openFileDlg(TRUE, "*.txt", NULL, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, szFilter);

		if (openFileDlg.DoModal() != IDOK)
		{
			goto EXIT_OK;
		}

		strcpy(szFile, openFileDlg.m_ofn.lpstrFile);
	}
	else
	{
		strcpy(szFile, (LPCTSTR) szString);
	}

	hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		goto EXIT_FAIL;
	}

	dwBytesToRead = GetFileSize(hFile, NULL);

	if (! ::ReadFile(hFile, szText.GetBuffer(dwBytesToRead), dwBytesToRead, &dwBytesRead, NULL))
	{
		goto EXIT_FAIL;
	}

	szText.ReleaseBuffer(dwBytesToRead);

	*bstrFileText = szText.AllocSysString();

	goto EXIT_OK;

EXIT_FAIL:
	ASSERT(0);

EXIT_OK:
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);

	return S_OK;
}

STDMETHODIMP CVWFileAccessTool::WriteFile(BSTR bstrFilename, BSTR bstrFileText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	char szFile[_MAX_PATH] = "";
	char* szText = NULL;
	DWORD dwBytesToWrite;
	DWORD dwBytesWritten;
	CString szString(bstrFilename);
	CString szOutput(bstrFileText);
	HANDLE hFile = INVALID_HANDLE_VALUE;
	static char szFilter[] = "Text files (*.txt)|*.txt|Script files (*.scr;*.vbs;*.js)|*.scr;*.vbs;*.js|All files (*.*)|*.*||";

	if (szString.GetLength() == 0)
	{
		CFileDialog saveFileDlg(FALSE, "*.txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, szFilter);

		if (saveFileDlg.DoModal() != IDOK)
		{
			goto EXIT_OK;
		}

		strcpy(szFile, saveFileDlg.m_ofn.lpstrFile);
	}
	else
	{
		strcpy(szFile, (LPCTSTR) szString);
	}

	hFile = CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		goto EXIT_FAIL;
	}

	dwBytesToWrite = strlen(szOutput);

	if (! ::WriteFile(hFile, szOutput, dwBytesToWrite, &dwBytesWritten, NULL))
	{
		goto EXIT_FAIL;
	}

	goto EXIT_OK;

EXIT_FAIL:
	ASSERT(0);

EXIT_OK:
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);

	return S_OK;
}

STDMETHODIMP CVWFileAccessTool::SelectColor(long dwInitialColor, long* pdwPickedColor)
{
	HRESULT hr = S_OK;
	CHOOSECOLOR chooseColor;
	COLORREF crCustom[16];
	PALETTEENTRY peSystem[16];
	HDC hDC;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(pdwPickedColor);

	hDC = GetDC(0);

	GetSystemPaletteEntries(hDC, 0, 16, (LPPALETTEENTRY) &peSystem);

	ReleaseDC(0, hDC);

	for (int i = 0; i < 16; i++)
	{
		crCustom[i] = RGB(peSystem[i].peRed,peSystem[i].peGreen, peSystem[i].peBlue);
	}

	ZeroMemory((LPVOID) &chooseColor, sizeof CHOOSECOLOR);
	chooseColor.lStructSize = sizeof CHOOSECOLOR;
	chooseColor.Flags = CC_RGBINIT;
	chooseColor.lpCustColors = crCustom;
	chooseColor.rgbResult = (COLORREF) dwInitialColor;

	if (ChooseColor(&chooseColor))
	{
		//user clicked ok
		*pdwPickedColor = (DWORD) chooseColor.rgbResult;
	}
	else
	{
		//User clicked cancel
		*pdwPickedColor = dwInitialColor;
		hr = S_FALSE;
	}

	return hr;
}


//********************************************
// OpenFileDialog
// Brings up the open file dialog, and returns
// the selected filename

STDMETHODIMP CVWFileAccessTool::OpenFileDialog(BSTR bstrDefaultFilename, long dwFileFlags, BSTR bstrFileFilter, BSTR *pbstrFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr			= S_OK;
	CString		strFileName	= bstrDefaultFilename;
	CString		strFilter	= bstrFileFilter;
	
	(*pbstrFileName) = NULL;

	if (dwFileFlags == 0)
		dwFileFlags = OFN_OVERWRITEPROMPT;

	CFileDialog openFileDlg(TRUE, NULL, strFileName, dwFileFlags, strFilter);
	int			iResult;

	iResult = openFileDlg.DoModal();
	if (iResult == IDOK)
		*pbstrFileName = openFileDlg.GetPathName().AllocSysString();
	else
	{
		// check for bogus init string
		DWORD	commErr = CommDlgExtendedError();

		if (commErr == FNERR_INVALIDFILENAME)
		{
			// try again, this time with no trailing
			CComBSTR	bstrNull = "";
			return OpenFileDialog(bstrNull.m_str, dwFileFlags, bstrFileFilter, pbstrFileName);

		}

	}

	return hr;
}

//********************************************
// SaveFileDialog
// Brings up the save file dialog, and returns
// the new filename

STDMETHODIMP CVWFileAccessTool::SaveFileDialog(BSTR bstrDefaultFilename, long dwFileFlags,  BSTR bstrFileFilter , BSTR *pbstrFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT		hr			= S_OK;
	CString		strFileName	= bstrDefaultFilename;
	CString		strFilter	= bstrFileFilter;

	(*pbstrFileName) = NULL;

	if (dwFileFlags == 0)
		dwFileFlags = OFN_OVERWRITEPROMPT;

	CFileDialog saveFileDlg(FALSE, NULL, strFileName, dwFileFlags, strFilter);

	if (saveFileDlg.DoModal() == IDOK)
		*pbstrFileName = saveFileDlg.GetPathName().AllocSysString();

	return hr;
}


//********************************************
// FileExists
// Given a filename, returns true if the file exists

STDMETHODIMP CVWFileAccessTool::FileExists(BSTR bstrFileName, VARIANT_BOOL *pbExists)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	HRESULT				hr = S_OK;
	WIN32_FIND_DATA	fileFindData;
	CString				strFileName = bstrFileName;
	HANDLE				findHandle;

	*pbExists = VARIANT_FALSE;

	findHandle = ::FindFirstFile(strFileName, &fileFindData);

	if (findHandle != INVALID_HANDLE_VALUE)
	{
		BOOL	bFoundOne = TRUE;

		while (bFoundOne)
		{
			 if (fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			 {
				 // we got a dir - search again
				 bFoundOne = ::FindNextFile(findHandle, &fileFindData);
			 }
			 else
			 {
				::FindClose(findHandle);
				*pbExists = VARIANT_TRUE;
				break;
			 }
		}
	}
	
	return hr;
}

//********************************************
// GetWorldPath
// Returns the path that local worlds are stored in
// from the registry.

STDMETHODIMP CVWFileAccessTool::GetWorldPath(BSTR *pbstrWorldPath)
{
	HRESULT	hr = S_OK;
	ASSERT(pbstrWorldPath);
	if (!pbstrWorldPath)
		hr = E_FAIL;
	else
		hr = FindWorldPath(pbstrWorldPath);

	return hr;
}

//********************************************
// GetHelpPath
// Returns the path that help files are stored in
// from the registry.

STDMETHODIMP CVWFileAccessTool::GetHelpPath(BSTR *pbstrHelpPath)
{
	HRESULT	hr = S_OK;
	ASSERT(pbstrHelpPath);
	if (!pbstrHelpPath)
		hr = E_FAIL;
	else
		hr = FindHelpPath(pbstrHelpPath);

	return hr;
}

