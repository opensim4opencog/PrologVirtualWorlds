Attribute VB_Name = "mdlTypeInfo"

Option Explicit

'*********************************************************************************************
' Documentation
'
' Contains the member description and help file
'*********************************************************************************************
Type Documentation
   HelpString As String ' Description
   HelpFile As String   ' Help file
   HelpContext As Long  ' Help context
End Type

'*********************************************************************************************
' DimInfo
'
' Contains info about an array dimension
'*********************************************************************************************
Type DimInfo
   Elements As Long  ' Number of elements
   LBound As Long    ' Index of the first element
End Type

'*********************************************************************************************
' TypeInfo
'
' Contains info about the type of a parameter
' or UDT member
'*********************************************************************************************
Type TypeInfo
   Name As String         ' Type name (String, Long, ...)
   Type As TypeKind       ' Indicates if the parameter type is an object, UDT or Enum
   ByRef As Boolean       ' Parameter is passed ByRef
   Array As Boolean       ' Parameter is an array
   DimsCount As Integer   ' Number of array dimensions.
                          ' Valid only if VariantType = VT_CARRAY
   Dims() As DimInfo      ' Array of array dimension info.
                          ' Valid only if VariantType = VT_CARRAY
   ParamArray As Boolean  ' Parameter is ParamArray
   VariantType As VARENUM ' Type of the parameter
End Type

'*********************************************************************************************
' VarInfo
'
' Contains a constant name and value, including
' Enum members
'*********************************************************************************************
Type VarInfo
   Name As String    ' Const name
   Value As Variant  ' Const value
   Type As TypeInfo  ' Type of the constant
End Type

'*********************************************************************************************
' Parameter
'
' Contains info about a parameter
'*********************************************************************************************
Type Parameter
   Name As String             ' Parameter name. Can be an empty string
   Flags As PARAMFLAGS        ' Parameter flags
   IsOptional As Boolean      ' If = True the parameter is Optional
   Type As TypeInfo           ' Parameter type
   HasDefaultValue As Boolean ' If = True the parameter has a default value
   DefaultValue As Variant    ' Parameter's default value. Valid
                              ' only if HasDefaultValue = True
End Type

'*********************************************************************************************
' DllEntry
'
' Contains info about an exported function
'*********************************************************************************************
Type DllEntry
   DllName As String    ' DLL name
   Ordinal As Integer   ' Ordinal of exported function. If 0 the
                        ' function is exported by name
   Name As String       ' Exported name in the DLL
End Type

'*********************************************************************************************
' FunctionInfo
'
' Contains info about a function
'*********************************************************************************************
Type FunctionInfo
   Name As String             ' Function name
   DISPID As Long             ' Member ID
   InvokeKind As InvokeKind   ' Invoke kind
   FunctionKind As FUNCKIND   ' Function kind
   Flags As FUNCFLAGS         ' Function flags
   Doc As Documentation       ' Function documentation
   DllEntry As DllEntry       ' DLL info
   ParamCount As Long         ' Number of parameters
   OptParamCount As Long      ' Number of optional parameters
   Parameters() As Parameter  ' Parameters
   ReturnType As TypeInfo     ' Return type
End Type

Public Declare Sub MoveMemory Lib "kernel32" Alias "RtlMoveMemory" (Dest As Any, Src As Any, ByVal DestL As Long)
Public Declare Sub VariantChangeType Lib "oleaut32.dll" (pvargDest As Variant, pvarSrc As Variant, ByVal wFlags As Long, ByVal vt As VARENUM)

Public Declare Function StringFromGUID2 Lib "ole32" (rclsid As UUID, ByVal lplpsz As Long, ByVal cchmax As Long) As Long

'*********************************************************************************************
' EnumFunctions
'
' Enumerates functions from a given ITypeInfo for
' a module, interface or dispinterface
'
' Parameters:
'
' oTInfo       Source ITypeInfo interface
' aFunctions() Array of FunctionInfo UDTs. The funtion will fill this array.
'
' Returns: The number of functions
'
'*********************************************************************************************
Public Function EnumFunctions(ByVal oTInfo As ITypeInfo, aFunctions() As FunctionInfo) As Long
Dim TA As TYPEATTR, FDPtr As Long
Dim Idx As Long, FD As FUNCDESC
   
   ' Get TYPEATTR struct
   TA = GetTypeAttr(oTInfo)
   
   On Error Resume Next
    
   ' Reset the array
   Erase aFunctions
   
   ' Check if there are function to enumerate
   If TA.cFuncs > 0 Then
   
      ' Redim the array
      ReDim aFunctions(0 To TA.cFuncs - 1)
      
      ' Set the return value
      EnumFunctions = TA.cFuncs
      
      ' Enumerate functions
      For Idx = 0 To TA.cFuncs - 1
         
         ' Get the a pointer to the
         ' FUNCDESC struct
         FDPtr = oTInfo.GetFuncDesc(Idx)
         
         ' Copy from the pointer to FD
         MoveMemory FD, ByVal FDPtr, LenB(FD)
         
         With aFunctions(Idx)
            
            ' Get the object documentation
            oTInfo.GetDocumentation FD.memid, .Name, .Doc.HelpString, .Doc.HelpContext, .Doc.HelpFile
            
            .DISPID = FD.memid
            .Flags = FD.wFuncFlags
            .FunctionKind = FD.FUNCKIND
            .InvokeKind = FD.invkind
            .Name = pvTrimNulls(.Name)
            .Doc.HelpString = pvTrimNulls(.Doc.HelpString)
            .Doc.HelpFile = pvTrimNulls(.Doc.HelpFile)
            .ParamCount = FD.cParams
            .OptParamCount = FD.cParamsOpt
                           
            ' Get the return type
            .ReturnType = pvGetType(oTInfo, FD.elemdescFunc.tdesc)
                           
            ' Get dll entry info only
            ' if oTInfo is a module
            If TA.TypeKind = TKIND_MODULE Then oTInfo.GetDllEntry FD.memid, FD.invkind, .DllEntry.DllName, .DllEntry.Name, .DllEntry.Ordinal
            
            ' Check if there're parameters
            If FD.cParams > 0 Then
               
               ' Enumerate the function parameters
               If pvEnumFuncParameters(oTInfo, FD, .Parameters) Then
                  
                  ' If pvEnumFuncParameters returns True
                  ' the last parameter has the
                  ' [retval] attribute and represents
                  ' the return value of the function
   
                  ' Move the last parameter to return type
                  .ReturnType = .Parameters(FD.cParams - 1).Type
                  
                  ' Remove the last parameter
                  If FD.cParams = 1 Then
                     Erase .Parameters
                  Else
                     ReDim Preserve .Parameters(0 To FD.cParams - 2)
                  End If
                  
                  ' Decrement the parameter count
                  .ParamCount = .ParamCount - 1
                  
               End If
               
            End If
            
         End With
         
         ' Release the pointer to
         ' FUNCDESC struct
         oTInfo.ReleaseFuncDesc FDPtr
         
      Next
   
   End If
   
End Function

'*********************************************************************************************
' EnumVariables
'
' Enumerates contants in a module and members of an enumeration given
' a ITypeInfo interface.
'
' Parameters:
'
' oTInfo:    source ITypeInfo interface
' aVars()    array of VarInfo UDTs. The function will fill this array.
'
' Returns: The number of constants returned in aVars.
'
'*********************************************************************************************
Public Function EnumVariables(ByVal oTInfo As ITypeInfo, aVars() As VarInfo) As Long
Dim TA As TYPEATTR, VDPtr As Long
Dim Idx As Long, VD As VARDESC
   
   ' Get TYPEATTR struct from ITypeInfo
   TA = GetTypeAttr(oTInfo)
        
   On Error Resume Next
   
   ' Reset the array
   Erase aVars
   
   ' Check if the module/enum
   ' contains constants
   If TA.cVars > 0 Then
      
      ' Redim the array
      ReDim aVars(0 To TA.cVars - 1)
      
      ' Set the return value
      EnumVariables = TA.cVars
         
      For Idx = 0 To TA.cVars - 1
            
         ' Get a pointer to the VARDESC
         ' struct for this constant
         VDPtr = oTInfo.GetVarDesc(Idx)
            
         ' Copy from the pointer
         MoveMemory VD, ByVal VDPtr, LenB(VD)
            
         With aVars(Idx)
            
            ' Get the constant name
            oTInfo.GetNames VD.memid, .Name, 1
            .Name = pvTrimNulls(.Name)
            
            ' If VD.VARKIND = VAR_CONST
            ' the constant has a value.
            If VD.VARKIND = VAR_CONST Then
               Dim TVar As Variant
               
               ' Copy from the pointer to
               ' the variant
               MoveMemory TVar, ByVal VD.oInst_varValue, 16
               
               ' Convert the Variant to
               ' a type supported by VB
               .Value = pvConvertVariant(TVar)
               
               ' "Destroy" the variant
               MoveMemory TVar, VT_EMPTY, 2
               
            End If
            
            ' Get constant type
            .Type = pvGetType(oTInfo, VD.elemdescVar.tdesc)
            
         End With
            
         ' Release the pointer
         oTInfo.ReleaseFuncDesc VDPtr
            
      Next
      
   End If

End Function

'*********************************************************************************************
' pvConvertVariant
'
' Converts a variant from a type not supported by VB to
' a variant with a type supported by VB.
'
' Parameters:
'
' Var:   Variant to convert. If the variant contains a value type supported
'        by VB the variant is not converted and is just returned as the function
'        result.
'
'*********************************************************************************************
Private Function pvConvertVariant(Var As Variant) As Variant
Dim vt As Integer

   On Error Resume Next
   
   ' Get variant type
   MoveMemory vt, Var, 2
   
   ' Convert the variant
   Select Case vt
                   
      Case VT_I1   ' From signed char to Byte
         VariantChangeType pvConvertVariant, Var, 0, VT_UI1
      
      Case VT_UI2  ' From unsigned short to Integer
         VariantChangeType pvConvertVariant, Var, 0, VT_I2
      
      Case VT_UI4, VT_UINT, VT_INT ' From unsigned int to Long
         VariantChangeType pvConvertVariant, Var, 0, VT_I4
      
      Case VT_I8, VT_UI8   ' From LONGLONG (64 bits) to Currency
         VariantChangeType pvConvertVariant, Var, 0, VT_CY
         
      Case VT_LPSTR, VT_LPWSTR ' From LPSTR and LPWSTR to String (BSTR)
         VariantChangeType pvConvertVariant, Var, 0, VT_BSTR
      
      Case Else
         If IsObject(Var) Then
            Set pvConvertVariant = Var
         Else
            pvConvertVariant = Var
         End If
         
   End Select

End Function

'*********************************************************************************************
' pvEnumFuncParameters
'
' Enumerates parameters of a function.
'
' Parameters:
'
' oTInfo    ITypeInfo that contains the function
' FD        FUNCDESC struct for the function
' Params    Array of parameters.
'
' Returns: True if the last parameter has the [retval] attribute,
'          otherwise returns False.
'
'*********************************************************************************************
Private Function pvEnumFuncParameters(ByVal oTInfo As ITypeInfo, FD As FUNCDESC, Params() As Parameter) As Boolean
Dim Idx As Long, ED As ELEMDESC, PDEx As PARAMDESCEX
            
   ' Redim the parameters array
   ReDim Params(0 To FD.cParams - 1)
   
   ' Create an array of string to
   ' store the parameter names.
   ' The array starts at -1 because
   ' the first string in the array
   ' is the function name.
   ReDim ParamNames(-1 To FD.cParams - 1) As String
                        
   ' Get the parameter names
   oTInfo.GetNames FD.memid, ParamNames(-1), FD.cParams + 1
   
   For Idx = 0 To FD.cParams - 1
                  
      ' Get the next ELEMDESC item
      ' from the pointed array
      MoveMemory ED, ByVal FD.lprgELEMDESCParam + (LenB(ED) * Idx), LenB(ED)
            
      With Params(Idx)
         
         ' Set the parameter name.
         ' Parameters can have no name!
         If ParamNames(Idx) <> "" Then
            .Name = pvTrimNulls(ParamNames(Idx))
         Else
            .Name = "NoNamedArg" & Idx
         End If
         
         ' If OptParam = -1 the last
         ' parameter is ParamArray
         If Idx = FD.cParams - 1 And FD.cParamsOpt = -1 Then
            
            .Flags = ED.PARAMDESC.wParamFlags
            .Type.ParamArray = True
            
            ' ParamArray parameters
            ' actually are arrays of
            ' variants
            .Type.Name = "Variant"
            .Type.Array = True
         
         Else
         
            ' Copy the flags
            .Flags = ED.PARAMDESC.wParamFlags
            
            ' Check if the parameter is Optional
            If (ED.PARAMDESC.wParamFlags And PARAMFLAG_FOPT) = PARAMFLAG_FOPT Then .IsOptional = True
            
            ' Get the parameter type
            .Type = pvGetType(oTInfo, ED.tdesc)
                        
            ' Check if the parameter has a default value
            If (ED.PARAMDESC.wParamFlags And PARAMFLAG_FHASDEFAULT) = PARAMFLAG_FHASDEFAULT Then
               
               ' Get the PARAMDESCEX struct
               ' where the default value
               ' is stored
               MoveMemory PDEx, ByVal ED.PARAMDESC.pPARAMDESCEX, LenB(PDEx)
               
               ' Copy the default value
               .DefaultValue = pvConvertVariant(PDEx.varDefaultValue)
               .HasDefaultValue = True
            
            End If
            
            ' Check if this parameter is the return value
            If (ED.PARAMDESC.wParamFlags And PARAMFLAG_FRETVAL) = PARAMFLAG_FRETVAL Then
            
               ' If the last parameter is has
               ' the [retval] attribute return
               ' True as the result of this
               ' function.
               pvEnumFuncParameters = True
               
            End If
                        
         End If
         
      End With
      
   Next

End Function

'*********************************************************************************************
' pvGetType
'
' Returns a TypeInfo struct with the type described by a TYPEDESC struct. The
' function resolves the type if there no direct conversion.
'
' Parameters:
'
' oTInfo:   ITypeInfo that contains the TD struct
' TD:       source TYPEDESC struct.
'
'*********************************************************************************************
Private Function pvGetType(oTInfo As ITypeInfo, TD As TYPEDESC) As TypeInfo

   On Error Resume Next
      
   ' Get flags
   pvGetType.ByRef = TD.vt And VT_BYREF   ' It's a pointer (passed ByRef)
   pvGetType.Array = TD.vt And VT_ARRAY   ' It's an array
   
   ' Remove flags
   TD.vt = TD.vt And VT_TYPEMASK
   
   ' Set the default values
   pvGetType.VariantType = TD.vt
   pvGetType.Type = -1
      
   Select Case TD.vt
   
      Case VT_PTR, VT_SAFEARRAY
         Dim TmpTD As TYPEDESC
                  
         ' VB array or pointer (ByRef).
         ' TD.pTypeDesc points to
         ' a TYPEDESC struct that
         ' specfies the pointed type
         ' or the array elements type .
         
         ' Get the TYPEDESC
         MoveMemory TmpTD, ByVal TD.pTypeDesc, Len(TmpTD)
               
         ' Call pvGetType on the new TYPEDESC
         pvGetType = pvGetType(oTInfo, TmpTD)
         
         ' Set the ByRef flag
         pvGetType.ByRef = True
         
         ' Set the array flag
         If TD.vt = VT_SAFEARRAY Then pvGetType.Array = True
         
      Case VT_USERDEFINED
         Dim TmpITInfo As ITypeInfo, TmpATTR As TYPEATTR, TmpPtr As Long
      
         ' User defined type. Can be an
         ' interface, structure, an enumeration
         ' or an alias (typedef).
         ' TD.pTypeDesc is a hRef to
         ' a ITypeInfo object that
         ' describes the user type.
         
         ' Get the referenced ITypeInfo
         Set TmpITInfo = oTInfo.GetRefTypeInfo(TD.pTypeDesc)
         
         ' Get the TYPEATTR struct
         TmpATTR = GetTypeAttr(TmpITInfo)
         
         ' Get the type name
         TmpITInfo.GetDocumentation DISPID_UNKNOWN, pvGetType.Name, "", 0, ""
         
         If TmpATTR.TypeKind = TKIND_ALIAS Then
            
            ' If the type is an alias
            ' get the aliased type
            
            pvGetType = pvGetType(oTInfo, TmpATTR.tdescAlias)
            
         ElseIf TmpATTR.TypeKind = TKIND_INTERFACE Then
         
            ' Return VT_UNKWNOWN
            pvGetType.VariantType = VT_UNKNOWN
            
         End If
         
         pvGetType.Type = TmpATTR.TypeKind

      Case VT_CARRAY
          
         Dim AD As ARRAYDESC, SB As SAFEARRAYBOUND, Idx As Long
         
         ' The type is a C array.
         ' TD.pTypeDesc points to
         ' an ARRAYDESC struct that
         ' contains the number of
         ' dimensions and the
         ' elements type.
      
         ' Copy the ARRAYDESC struct
         MoveMemory AD, ByVal TD.pTypeDesc, LenB(AD)
         
         ' Resolve the element type
         pvGetType = pvGetType(oTInfo, AD.tdescElem)
         
         ' Set flags and DimsCount
         pvGetType.DimsCount = AD.cDims
         pvGetType.Array = True
         
         ' Redim the Dims array
         ReDim pvGetType.Dims(0 To AD.cDims - 1)
         
         ' The ARRAYDESC struct is
         ' followed by an array of
         ' SAFEARRAYBOUND struct that
         ' contains the info for each dimension
         For Idx = 0 To AD.cDims - 1
            
            ' Get the next SAFEARRAYBOUND
            MoveMemory SB, ByVal TD.pTypeDesc + LenB(AD) + (Idx * LenB(SB)), LenB(SB)
            
            ' Copy the info
            pvGetType.Dims(Idx).Elements = SB.cElements
            pvGetType.Dims(Idx).LBound = SB.lLbound
            
         Next
         
      Case VT_VOID
      
         pvGetType.Name = "Any"
         
      Case VT_HRESULT
   
      Case VT_NULL
         pvGetType.Name = "Null"
                  
      Case VT_DISPATCH
      
         pvGetType.Name = "Object"
      
      Case VT_UNKNOWN
      
         pvGetType.Name = "IUnknown"
               
      Case VT_VARIANT
         
         pvGetType.Name = "Variant"
      
      Case VT_I1, VT_UI1
         pvGetType.Name = "Byte"
      
      Case VT_I2, VT_UI2
         pvGetType.Name = "Integer"
      
      Case VT_I4, VT_UI4, VT_UINT, VT_INT
         pvGetType.Name = "Long"
      
      Case VT_I8, VT_UI8
         pvGetType.Name = "Currency"
         
      Case VT_BSTR
         pvGetType.Name = "String"
         
      Case VT_LPSTR, VT_LPWSTR
      
         pvGetType.Name = "String"
         
      Case VT_DATE
         pvGetType.Name = "Date"
         
      Case VT_SAFEARRAY
         pvGetType.Name = "Array"
      
      Case VT_BOOL
         
         pvGetType.Name = "Boolean"
         
      Case VT_CLSID
      
         pvGetType.Name = "CLSID"
         
      Case VT_DECIMAL
      
         pvGetType.Name = "Decimal"
      
      Case VT_CLSID
      
         pvGetType.Name = "CLSID"
         
      Case Else
         
         Dim TmpVar As Variant
         
         With pvGetType
         
            .VariantType = TD.vt
               
            MoveMemory TmpVar, TD.vt, 2
            .Name = typename(TmpVar)
            MoveMemory TmpVar, 0, 2
            
            If .Name = "" Then .Name = CVErr(TD.vt)
         
         End With
            
   End Select

End Function

'*********************************************************************************************
' GetTypeAttr
'
' Returns a TYPEATTR struct given a ITypeInfo interface
'
'*********************************************************************************************
Public Function GetTypeAttr(ByVal oTInfo As ITypeInfo) As TYPEATTR
Dim TAPtr As Long

   ' Get TYPEATTR pointer
   TAPtr = oTInfo.GetTypeAttr
   
   ' Copy the struct from the pointer
   MoveMemory GetTypeAttr, ByVal TAPtr, Len(GetTypeAttr)
   
   ' Release the pointer
   oTInfo.ReleaseTypeAttr TAPtr
   
End Function

'*********************************************************************************************
' pvTrimNulls
'
' Removes everything at the right of the last null char in a string.
'*********************************************************************************************
Private Function pvTrimNulls(ByVal Str As String) As String
Dim lPos As Long

   lPos = InStr(Str, vbNullChar)
   
   If lPos Then
      pvTrimNulls = Left$(Str, lPos - 1)
   Else
      pvTrimNulls = Str
   End If
   
End Function

'*********************************************************************************************
' TypeInfoFromObject
'
' Returns a ITypeInfo interface for an object.
'*********************************************************************************************
Public Function TypeInfoFromObject(ByVal oObject As Object) As ITypeInfo
Dim oIDispatch As IVBDispatch

   ' Get IDispatch interface
   Set oIDispatch = oObject
   
   If oIDispatch.GetTypeInfoCount = 1 Then
            
      Set TypeInfoFromObject = oIDispatch.GetTypeInfo(0, 0)
         
   Else
         
      Err.Raise vbObjectError, "TypeInfoFromObject", "The object do not contain class info"
         
   End If
   
End Function




