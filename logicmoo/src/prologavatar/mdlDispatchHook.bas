Attribute VB_Name = "mdlDispatchHook"
Option Explicit

' ==== API declarations ====

Private Declare Function VirtualProtect Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long, ByVal flNewProtect As Long, lpflOldProtect As Long) As Long
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Dest As Any, Src As Any, ByVal l As Long)

Private Const DISP_INVOKE_INDEX = 7
Private Const DISP_GION_INDEX = 6
Private Const PAGE_EXECUTE_READWRITE& = &H40&

Private Declare Function lstrlenW Lib "kernel32" (ByVal Str As Long) As Long
Private Declare Function lstrcpyA Lib "kernel32" (ByVal Dest As Any, Src As Any) As Long

Private Type SAFEARRAY_1D
    cDims As Integer       ' Number of dimensions
    fFeatures As Integer   ' Flags
    cbElements As Long     ' Length of each element
    cLocks As Long         ' Lock count
    pvData As Long         ' Pointer to the data
    Bounds(0 To 0) As SAFEARRAYBOUND   ' Array of dimensions
End Type

Private Declare Function VarPtrArray Lib "kernel32" Alias "lstrcpyA" (PtrDest() As Any, PtrSrc() As Any) As Long
Private Declare Function VariantCopyIndPtrVar Lib "oleaut32" Alias "VariantCopyInd" (ByVal pvargDest As Long, pvargSrc As Variant) As Long

'*********************************************************************************************
'
' HookDispatch
'
' Hooks the IDispatch interface
'
'*********************************************************************************************
Public Sub HookDispatch(ByVal Obj As IVBDispatch)
Dim Callback As IDispatchCallback
   
   Set Callback = Obj

   Callback.GIONAddr = pvReplaceGION(Obj)
   Callback.InvokeAddr = pvReplaceInvoke(Obj)

   Set Callback = Nothing
   
End Sub

'*********************************************************************************************
'
' UnHookDispatch
'
' Unhooks the IDispatch interface
'
'*********************************************************************************************
Public Sub UnHookDispatch(ByVal Obj As IVBDispatch)
Dim Callback As IDispatchCallback
   
   Set Callback = Obj

   pvReplaceGION Obj, Callback.GIONAddr
   pvReplaceInvoke Obj, Callback.InvokeAddr

   Set Callback = Nothing
   
End Sub


'*********************************************************************************************
'
' pvPtr2Str
'
' Returns a String given a pointer to a Unicode string.
'
'*********************************************************************************************
Private Function pvPtr2Str(ByVal Ptr As Long) As String

   pvPtr2Str = Space$(lstrlenW(Ptr))
   CopyMemory ByVal StrPtr(pvPtr2Str), ByVal Ptr, LenB(pvPtr2Str)
   
End Function


'*********************************************************************************************
'
' pvReplaceInvoke
'
' Replaces the entry of Invoke in a IDispatch interface v-table
'
'*********************************************************************************************
Private Function pvReplaceInvoke(ByVal oExtObject As IVBDispatch, Optional ByVal FuncPtr As Long) As Long

   If FuncPtr = 0 Then
      pvReplaceInvoke = pvReplaceVTableEntry(ObjPtr(oExtObject), DISP_INVOKE_INDEX, AddressOf pvInvoke)
   Else
      pvReplaceInvoke = pvReplaceVTableEntry(ObjPtr(oExtObject), DISP_INVOKE_INDEX, FuncPtr)
   End If
   
End Function

'*********************************************************************************************
'
' pvReplaceGION
'
' Replaces the entry of GetIDsOfNames in a IDispatch interface v-table
'
'*********************************************************************************************
Private Function pvReplaceGION(ByVal oExtObject As IVBDispatch, Optional ByVal FuncPtr As Long) As Long

   If FuncPtr = 0 Then
      pvReplaceGION = pvReplaceVTableEntry(ObjPtr(oExtObject), DISP_GION_INDEX, AddressOf pvGetIDsOfNames)
   Else
      pvReplaceGION = pvReplaceVTableEntry(ObjPtr(oExtObject), DISP_GION_INDEX, FuncPtr)
   End If
   
End Function


'*********************************************************************************************
'
' pvReplaceVTableEntry
'
'
' Replaces a v-table entry
'
'*********************************************************************************************
Private Function pvReplaceVTableEntry(ByVal oObject As Long, ByVal nEntry As Integer, ByVal pFunc As Long) As Long
Dim pFuncOld As Long, pVTableHead As Long
Dim pFuncTmp As Long, lOldProtect As Long
    
   ' Get the pointer to the v-table
   CopyMemory pVTableHead, ByVal oObject, 4
    
   ' Calculate the pointer to the entry
   pFuncTmp = pVTableHead + (nEntry - 1) * 4
    
   ' Get the current pointer to the function
   CopyMemory pFuncOld, ByVal pFuncTmp, 4
     
   ' Replace the pointer
   If pFuncOld <> pFunc Then
   
      VirtualProtect pFuncTmp, 4, PAGE_EXECUTE_READWRITE, lOldProtect
      
      CopyMemory ByVal pFuncTmp, pFunc, 4
      
      VirtualProtect pFuncTmp, 4, lOldProtect, lOldProtect
       
   End If
    
   ' Return the previous pointer
   pvReplaceVTableEntry = pFuncOld
    
End Function

'*********************************************************************************************
'
' pvInvoke
'
' IDispatch.Invoke method
'
'*********************************************************************************************
Private Function pvInvoke(ByVal This As IVBDispatch, _
            ByVal dispIdMember As Long, _
            riid As UUID, _
            ByVal lcid As Long, _
            ByVal wFlags As Integer, _
            pDispParams As ITLib.DISPPARAMS, _
            ByVal pVarResult As Long, _
            pExcepInfo As ITLib.EXCEPINFO, _
            puArgErr As Long) As Long
Dim Callback As IDispatchCallback
Dim SafeArray As SAFEARRAY_1D
Dim Params() As Variant
Dim lIdx As Long
Dim OldFunc As Long
Dim Result As Variant

   On Error Resume Next
   
   ' Get the callback interface
   Set Callback = This
   
   ' Replace the Invoke method
   ' with the original
   OldFunc = pvReplaceInvoke(This, Callback.InvokeAddr)
   
   ' Call the original method
   pvInvoke = This.Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr)
   
   ' The original Invoke has failed.
   If pvInvoke = DISP_E_MEMBERNOTFOUND Then
       
      ' Convert the C array of
      ' parameters to a VB array.
   
      ' Fill a SAFEARRAY struct
      ' that points to the parameters
      With SafeArray
         .Bounds(0).cElements = pDispParams.cArgs
         .Bounds(0).lLbound = 0
         .cDims = 1
         .cbElements = 16
         .pvData = pDispParams.rgPointerToVariantArray
      End With
      
      ' Set the array with the SAFEARRAY struct
      CopyMemory ByVal VarPtrArray(Params, Params), VarPtr(SafeArray), 4
      
      ' Call the callback InvokeMethod function
      pvInvoke = Callback.InvokeMethod(wFlags, dispIdMember, Params, Result)
      
      ' Copy the result if pVarResult <> 0
      If pVarResult <> 0 Then VariantCopyIndPtrVar pVarResult, Result
      
      ' Release the array
      CopyMemory ByVal VarPtrArray(Params, Params), 0&, 4
      
   End If
   
   ' Replace the Invoke method
   ' with this function
   pvReplaceInvoke This, OldFunc
   
End Function

'*********************************************************************************************
'
' pvGetIDsOfNames
'
' IDispatch.GetIDsOfNames method
'
'*********************************************************************************************
Private Function pvGetIDsOfNames(ByVal This As IVBDispatch, _
                                        riid As UUID, _
                                        rgszNames As Long, _
                                        ByVal cNames As Long, _
                                        ByVal lcid As Long, _
                                        rgDispID As Long) As Long
Dim Callback As IDispatchCallback
Dim OldFunc As Long

   On Error Resume Next
   
   If cNames = 1 Then
   
      ' Get the callback interface
      Set Callback = This
      
      ' Replace the GetIDsOfNames method
      ' with the original one
      OldFunc = pvReplaceGION(This, Callback.GIONAddr)
         
      ' Call the callback to get a DISPID
      rgDispID = Callback.GetDISPID(pvPtr2Str(rgszNames))
         
      If rgDispID = DISPID_UNKNOWN Then
      
         ' If the callback returns
         ' DISPID_UNKNOWN, use the
         ' original function to get
         ' the DISPID
         
         ' Call the original function
         rgDispID = This.GetIDsOfNames(riid, pvPtr2Str(rgszNames), cNames, lcid)
         
         If rgDispID = DISPID_UNKNOWN Then
         
            ' The original function
            ' doesn't recognize the name
            pvGetIDsOfNames = DISP_E_MEMBERNOTFOUND
            
         End If
         
      End If
      
      ' Replace the GetIDsOfNames method
      ' with this function
      pvReplaceGION This, OldFunc
      
   Else
      
      ' The hooked object will not
      ' support named arguments anymore
      pvGetIDsOfNames = DISP_E_NONAMEDARGS
   
   End If
   
End Function




