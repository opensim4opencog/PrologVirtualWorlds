// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Tools15.cpp : Implementation of CTools1App and DLL registration.

#include "stdafx.h"
#include <stdtools.h>
#include "undostak.h"
#include "syshelp.h"

IMPLEMENT_OBJECTPROPERTY(CVWUndoStacksObject)

/////////////////////////////////////////////////////////////////////////////
//
// Keep track of the Redo and undo stack size with
// m_cStacksSize and m_iSizeUndo.
 
#define REDO_STACK_SIZE ( m_cStacksSize - m_iSizeUndo )
#define REDO_STACK_EXISTS ( REDO_STACK_SIZE > 0 )
#define REDOSTACK_TOP_INDEX	 (m_iSizeUndo ) 
#define UNDO_STACK_EXISTS ( m_iSizeUndo > 0 )
#define UNDOSTACK_TOP_INDEX ( m_iSizeUndo - 1 )

CVWUndoStacksObject::CVWUndoStacksObject()
{
	m_pproplistStacks = NULL;
	m_cStacksSize =
		m_iSizeUndo = 0;
	m_cUndoMax = 100; // Default max
	m_fReentrancyCheck = FALSE;
}

CVWUndoStacksObject::~CVWUndoStacksObject()
{
	Terminate();
}

STDMETHODIMP CVWUndoStacksObject::Terminate()
{
	CVWUndoStacksObjectBase::Terminate();

	SAFERELEASE( m_pproplistStacks );

	return S_OK;
}

// Test the validity of Undo/Redo stacks.
BOOL CVWUndoStacksObject::StacksAreValid()
{
	HRESULT hr = S_OK;
	long lcStacksSize;
	BOOL fResult = TRUE;

	ASSERT( m_pproplistStacks );
	if( !m_pproplistStacks )
		goto EXIT_FALSE;
	hr = m_pproplistStacks->get_Count( &lcStacksSize );
	ASSERT( (unsigned int)lcStacksSize == m_cStacksSize );
	if( (unsigned int)lcStacksSize != m_cStacksSize )
		goto EXIT_FALSE;
	ASSERT( m_iSizeUndo <= m_cStacksSize );
	if( !(m_iSizeUndo <= m_cStacksSize) )
		goto EXIT_FALSE;

	goto EXIT_TRUE;
EXIT_FALSE:
	fResult = ResetStacks( );

EXIT_TRUE:
	return fResult;
}

// Flush and recover valid stack state.
BOOL CVWUndoStacksObject::ResetStacks()
{
	HRESULT hr = S_OK;
	BOOL fResult = TRUE;

	// Review: Give error message.
	ASSERT(m_pproplistStacks );
	hr = m_pproplistStacks->RemoveAll();
	if( FAILED(hr) ) goto EXIT_FALSE;
	m_cStacksSize =
		m_iSizeUndo = 0;

	goto EXIT_TRUE;
EXIT_FALSE:
	fResult = FALSE;

EXIT_TRUE:
	return fResult;
}

// Object Methods
STDMETHODIMP CVWUndoStacksObject::Dump()
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fIsEmpty = VARIANT_FALSE;
//	IObjectProperty * pOP = NULL;

	VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "\nUndoStack: begin\n");
  	if( StacksAreValid() )
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "StacksAreValid() == TRUE\n");
	else
 		VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "StacksAreValid() == FALSE\n");
 
	hr = m_pproplistStacks->get_IsEmpty( &fIsEmpty ); 
	if(FAILED(hr))
	{
 		VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "Aborting CVWUndoStacksObject::Dump()\n");
		goto EXIT_FAIL;
	}
	if(fIsEmpty)
	{
 		VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "Undo and Redo stacks are empty.\n");
	}
	else
	{
		if( UNDO_STACK_EXISTS )
		{
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "Dumping Undo stack:\n");
			// Dump the undo stack.
			unsigned int uiUndo = m_iSizeUndo;
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "Undo stack size is: %u\n",uiUndo);
// UndoItems are no longer ObjectProperties
//			while( uiUndo-- > 0 )
//			{
//				hr = m_pproplistStacks->get_Dispatch( uiUndo ,(IDispatch ** ) &pOP); 	
//				if( FAILED(hr) ) goto EXIT_FAIL;
//				hr = pOP->Dump( ); 	
//				if( FAILED(hr) ) goto EXIT_FAIL;
//				SAFERELEASE(pOP);
//			}
		} // if( UNDO_STACK_EXISTS )
		else
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "No items in Undo stack.\n");

		if( REDO_STACK_EXISTS )
		{
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "Dumping Redo stack:\n");
			// Dump the redo stack.
			unsigned int uiRedo = REDOSTACK_TOP_INDEX;
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "Redo stack size is: %d\n",REDO_STACK_SIZE);
// UndoItems are no longer ObjectProperties
//			while( uiRedo++ < m_cStacksSize )
//			{
//				hr = m_pproplistStacks->get_Dispatch( uiRedo - 1 , (IDispatch ** )&pOP); 	
//				if( FAILED(hr) ) goto EXIT_FAIL;
//				hr = pOP->Dump( ); 	
//				if( FAILED(hr) ) goto EXIT_FAIL;
//				SAFERELEASE(pOP);
//			}
		} // if( REDO_STACK_EXISTS )
		else
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "No items in Redo stack.\n");
	}

	VWTRACE(m_pWorld, "VWSTUDIO", VWT_DUMP, "UndoStack: end\n");

EXIT_FAIL:
//	SAFERELEASE( pOP );
	return hr;
}

STDMETHODIMP CVWUndoStacksObject::put_World(IWorld* pworld)
{
	CVWUndoStacksObjectBase::put_World(pworld);

	HRESULT hr = S_OK;

	ASSERT(m_pWorld);

	// Create the 2 stacks.
	// This is one PropertyList with a pointer (m_iSizeUndo) to the top of the undo stack.
	// As items are put on the Redo stack m_iSizeUndo is incremented.
	if( m_pWorld )
	{
		hr = CreatePropertyList( m_pWorld, &m_pproplistStacks );
		if(FAILED(hr)) goto EXIT_FAIL;
	}

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWUndoStacksObject::Flush( )
{
	HRESULT hr = S_OK;
	ASSERT( m_pproplistStacks );

	if( !ResetStacks( ) )
	{
		hr = VWSTUDIO_E_UNDOSTACKSINVALID;
		goto EXIT_FAIL;
	}
EXIT_FAIL:
	// Don't leave screwed-up UndoStack.
	// Reset it on error.
	if(FAILED(hr))
		ResetStacks();
	
	return hr;
}

STDMETHODIMP CVWUndoStacksObject::get_SizeUndoStack(unsigned int * puSize)
{
	return m_cUndoMax;
}

// Don't bother adjusting real stack size until next time AddVWUndoItem() is called. 
STDMETHODIMP CVWUndoStacksObject::put_SizeUndoStack(unsigned int uSize)
{
	m_cUndoMax = uSize;
	return S_OK;
}

// Add an item to the Undo stack. If there is a redo stack, flush it.
// If Undo stack is larger than max size, shrink it.
STDMETHODIMP CVWUndoStacksObject::AddVWUndoItem(IVWUndoItem * pVWUndoItem)
{
	HRESULT hr = S_OK;
	IVWUndoItem * pUI = NULL;
	IUnknown *punkTemp = NULL;
	HWND hwndTemp = NULL;

	unsigned int uiUndo = m_iSizeUndo;
	VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "Before CVWUndoStacksObject::AddVWUndoItem() Undo stack size is: %u\n",uiUndo);	// Check to see if someone is adding an undoItem during Undo or Redo.

	if( TRUE == m_fReentrancyCheck )
	{
		ASSERT(FALSE);
		hr = VWSTUDIO_E_CANTADDUNDOITEM;
		goto EXIT_FAIL;
	}

	if( NULL == pVWUndoItem)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}


	if( StacksAreValid() )
	{

		if( REDO_STACK_EXISTS )
		{
			// Shrink the redo stack to nothing.
			while( REDO_STACK_SIZE > 0 )
			{
				// Remove the bottom redo item.
				hr = m_pproplistStacks->get_Dispatch( --m_cStacksSize , (IDispatch**)&pUI); 	
				if( FAILED(hr) ) goto EXIT_FAIL;
				hr = m_pproplistStacks->RemoveDispatch( (IDispatch*)pUI ); 	
				if( FAILED(hr) ) goto EXIT_FAIL;
			}
			if( !StacksAreValid( ) )
			{
				VWTRACE(m_pWorld, "VWSTUDIO", VWT_ERROR, "CVWUndoStacksObject::AddVWUndoItem. Stacks are invalid\n");
				hr = VWSTUDIO_E_UNDOSTACKSINVALID;	
				goto EXIT_FAIL;
			}
		}
		// If Size of undo stack has hit max then remove the bottom items.
		while( m_cUndoMax <= m_cStacksSize )
		{
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_IMPORTANT, "CVWUndoStacksObject::AddVWUndoItem - Hit Max size. Removing bottom item.\n");
			SAFERELEASE( pUI );
			hr = m_pproplistStacks->get_Dispatch( 0 ,(IDispatch**) &pUI); 	
			if( FAILED(hr) ) goto EXIT_FAIL;
			hr = m_pproplistStacks->RemoveDispatch( (IDispatch*)pUI ); 	
			if( FAILED(hr) ) goto EXIT_FAIL;
			m_iSizeUndo--;
			m_cStacksSize--;
		}
		// Add new item to UndoStack
		hr = m_pproplistStacks->AddDispatch( (IDispatch*) pVWUndoItem );
		if( FAILED(hr) ) goto EXIT_FAIL;
		m_cStacksSize++;
		m_iSizeUndo++;
		// Let's be paranoid.
		ASSERT( StacksAreValid() );
	}
	else
	{
		hr = VWSTUDIO_E_UNDOSTACKSINVALID;
		goto EXIT_FAIL;
	}

	goto EXIT_SUCCEED;

EXIT_FAIL:

EXIT_SUCCEED:
	SAFERELEASE( pUI );	 
	SAFERELEASE( punkTemp );	  
	return hr;
}

STDMETHODIMP CVWUndoStacksObject::get_UndoLabel(BSTR *pbstrLabel)
{
	HRESULT hr = S_OK;
	IVWUndoItem * pUI = NULL;

	hr = m_pproplistStacks->get_Dispatch( UNDOSTACK_TOP_INDEX, (IDispatch ** )&pUI); 	
	if( FAILED(hr) ) goto EXIT_FAIL;
	ASSERT( pUI );

	hr = pUI->get_Label( pbstrLabel );

EXIT_FAIL:
	SAFERELEASE(pUI);
	return hr;
}

STDMETHODIMP CVWUndoStacksObject::get_RedoLabel(BSTR *pbstrLabel)
{
	HRESULT hr = S_OK;
	IVWUndoItem * pUI = NULL;

	hr = m_pproplistStacks->get_Dispatch( REDOSTACK_TOP_INDEX, (IDispatch ** ) &pUI); 	
	if( FAILED(hr) ) goto EXIT_FAIL;
	ASSERT( pUI );

	hr = pUI->get_Label( pbstrLabel );

EXIT_FAIL:
	SAFERELEASE(pUI);
	return hr;
}


STDMETHODIMP CVWUndoStacksObject::Redo(void)
{
	HRESULT hr = S_OK;
	IVWUndoItem * pUI = NULL;

	unsigned int uiRedo = REDOSTACK_TOP_INDEX;
	unsigned int uiUndo = m_iSizeUndo;
	VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "Before CVWUndoStacksObject::Redo(): Redo stack size is: %d\n",REDO_STACK_SIZE);
	VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "Before CVWUndoStacksObject::Redo() Undo stack size is: %u\n",uiUndo);

	// Flag that it is illegal to AddUndoItem
	m_fReentrancyCheck = TRUE;	

	if( !StacksAreValid() )
	{
		hr = VWSTUDIO_E_UNDOSTACKSINVALID;
		goto EXIT;
	}

	ASSERT( m_pproplistStacks );
	if( REDO_STACK_EXISTS )
	{
		hr = m_pproplistStacks->get_Dispatch( REDOSTACK_TOP_INDEX, (IDispatch ** ) &pUI); 	
		if( FAILED(hr) ) goto EXIT;
		ASSERT( pUI );

		// get the hint out of the undo item to see if it is part of a group
		enum enumUndoHint iUndoHints;

		hr = pUI->get_UndoHints((int *) &iUndoHints );
		if (FAILED(hr) ) goto EXIT;
		if (iUndoHints != END_UNDO_GROUP)
		{
			// not in a group, or put a BEGIN by mistake, just redo the top item
			hr = pUI->Redo( );
			if( FAILED(hr) ) goto EXIT;
			m_iSizeUndo++;
		}
		else
		{
			hr = pUI->Redo( );
			if( FAILED(hr) ) goto EXIT;
			m_iSizeUndo++;
			// part of a group, undo each item until we reach the beginning
			// since the order is reversed because we are in a stack
			while (iUndoHints != BEGIN_UNDO_GROUP)
			{
				SAFERELEASE(pUI);
				
				// this is just to make sure that we don't crash if someone forgot to add the BEGIN
				if( !REDO_STACK_EXISTS )
					goto EXIT;

				hr = m_pproplistStacks->get_Dispatch( REDOSTACK_TOP_INDEX, (IDispatch ** ) &pUI); 	
				if( FAILED(hr) ) goto EXIT;
				ASSERT( pUI );
				
				hr = pUI->get_UndoHints((int *) &iUndoHints );
				if (FAILED(hr) ) goto EXIT;

				hr = pUI->Redo( );
				if( FAILED(hr) ) goto EXIT;
				m_iSizeUndo++;
			}
		}

		ASSERT( StacksAreValid() );
	}
	
EXIT:
	SAFERELEASE( pUI );
	m_fReentrancyCheck = FALSE;	
	return hr;
}


// If there are any items on the Undo stack,
// call Undo() on the top item and add it to the Redo stack. 
STDMETHODIMP CVWUndoStacksObject::Undo(void)
{
	HRESULT hr = S_OK;
	IVWUndoItem * pUI = NULL;
	BOOL fCanUndo = FALSE;
		
	unsigned int uiRedo = REDOSTACK_TOP_INDEX;
	unsigned int uiUndo = m_iSizeUndo;
	VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "Before CVWUndoStacksObject::Undo(): Redo stack size is: %d\n",REDO_STACK_SIZE);
	VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "Before CVWUndoStacksObject::Undo() Undo stack size is: %u\n",uiUndo);

	// Flag that it is illegal to AddUndoItem
	m_fReentrancyCheck = TRUE;	

	if( !StacksAreValid() )
	{
		hr = VWSTUDIO_E_UNDOSTACKSINVALID;
		goto EXIT;
	}

	ASSERT( m_pproplistStacks );
	// If there are UndoItems, call redo.
	if( UNDO_STACK_EXISTS )
	{
		hr = m_pproplistStacks->get_Dispatch( UNDOSTACK_TOP_INDEX, (IDispatch ** ) &pUI); 	
		if( FAILED(hr) ) goto EXIT;
		ASSERT( pUI );

		// get the hint out of the undo item to see if it is part of a group
		enum enumUndoHint iUndoHints;

		hr = pUI->get_UndoHints((int *) &iUndoHints );
		if (FAILED(hr) ) goto EXIT;
		if (iUndoHints != BEGIN_UNDO_GROUP)
		{
			// not in a group, or put the END in the wrong place
			hr = pUI->Undo( );
			if( FAILED(hr) ) goto EXIT;
			m_iSizeUndo--;
		}
		else if (iUndoHints == BEGIN_UNDO_GROUP)
		{
			hr = pUI->Undo( );
			if( FAILED(hr) ) goto EXIT;
			m_iSizeUndo--;
			// part of a group, undo each item until we reach the end.
			while (iUndoHints != END_UNDO_GROUP)
			{
				SAFERELEASE(pUI);
				
				// just in case they forgot the END tag, make sure we don't run off the end
				if( !UNDO_STACK_EXISTS )
					goto EXIT;

				hr = m_pproplistStacks->get_Dispatch( UNDOSTACK_TOP_INDEX, (IDispatch ** ) &pUI); 	
				if( FAILED(hr) ) goto EXIT;
				ASSERT( pUI );
				
				hr = pUI->get_UndoHints((int *) &iUndoHints );
				if (FAILED(hr) ) goto EXIT;

				hr = pUI->Undo( );
				if( FAILED(hr) ) goto EXIT;
				m_iSizeUndo--;
			}
		}
		ASSERT( StacksAreValid() );
	}
	
EXIT:
	SAFERELEASE( pUI );
	m_fReentrancyCheck = FALSE;	
	return hr;
}

