// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

class CPropListData : public CObject
{
public:
	// VARIANTS
	CPropListData(CString strName, CString strType, CString strValue, short type, VARTYPE vt, BOOL bEditable = TRUE) { m_strName = strName; m_strType = strType; m_strValue = strValue; m_bEditable = bEditable; m_iIndex = -1; m_type = type; m_vtType = vt; m_bScriptMethod = FALSE; m_bInLineMethod = FALSE;}
	CPropListData(CString strName, CString strType, CString strValue, VARTYPE vt, BOOL bEditable = TRUE) { m_strName = strName; m_strType = strType; m_strValue = strValue; m_bEditable = bEditable; m_iIndex = -1; m_type = -1; m_vtType = vt;}
	CPropListData(int index, CString strType, CString strValue, VARTYPE vt, BOOL bEditable = TRUE) { m_iIndex = index; m_strType = strType; m_strValue = strValue; m_bEditable = bEditable; m_type = -1; m_vtType = vt;}
	CPropListData(int index, long lSortOrder, BOOL bEditable = TRUE) { m_iIndex = index; m_bEditable = bEditable; m_vtType = VT_DISPATCH;}
	
	CPropListData() {};
	~CPropListData() {};

	long	m_iIndex;	// the index of the item if it is in a list
	CString m_strName;	// the name of the item if it is in a map
	CString m_strType;	// the string value of the type (saved for sorting speed)
	CString m_strValue;	// the string value of the value (saved for sorting speed)
	short	m_type;		// where does this prop come from
	VARTYPE	m_vtType;	// used so that we know if we can edit it in-line
	BOOL	m_bScriptMethod;	// is this a method from a script module
	BOOL	m_bInLineMethod;	// is this method an in-line
	BOOL	m_bEditable;		// do we have permission to edit this prop?
};