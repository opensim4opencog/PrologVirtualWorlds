VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "SWIProlog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'set this to 0 to disable debug code in this class

#Const DebugMode = 1

#If DebugMode Then
    'local variable to hold the serialized class ID that was created in Class_Initialize
    Private mlClassDebugID As Long
#End If

'Implements IObjectSafety

Public thePrologQuery As PROQUERYLib.PrologQuery
Attribute thePrologQuery.VB_VarHelpID = -1

Private tempStringIn As String
Private tempStringOut As String

Public Sub Reset()
    Set thePrologQuery = Nothing
    Set thePrologQuery = New PROQUERYLib.PrologQuery
End Sub

Public Sub CloseQuery()
    thePrologQuery.CloseQuery
End Sub


Public Sub CutQuery()
    thePrologQuery.CutQuery
End Sub


Function NextSolution(ByRef rgArgs() As Variant) As Boolean
    NextSolution = thePrologQuery.NextSolution(rgArgs())
End Function


Public Function program_pl(Term As String) As String
    Dim rgArgs() As Variant, pre_rgArgs() As String, rgOut() As Variant
    Dim Out As String, Predicate As String, Arity As Long
    Dim bit  As Boolean

    If proMod = "" Then proMod = "User"
    
    i = InStr(Term, "(")
    If i = 0 Then
        Predicate = Term
        Arity = 0
        rgArgs = Null
    Else
        Predicate = Left$(Term, i - 1)
        Term = pl_trim(Mid$(Term, i))
        pl_split_pred pre_rgArgs, Term, ","
        Arity = UBound(pre_rgArgs)
        ReDim rgArgs(Arity)
        For i = 0 To Arity
            rgArgs(i) = pre_rgArgs(i)
        Next i
    End If

    If Len(Predicate) = 0 Then Predicate = ","
    thePrologQuery.SetPredicate Predicate, Arity + 1, proMod
    thePrologQuery.OpenQuery 0, rgArgs

    
    While thePrologQuery.NextSolution(rgOut)
    
    #If DebugMode Then
        Debug.Print rgOut(0)
    #End If
    
    Wend
    Debug.Print "no."
    
End Function
    

Public Function doProlog_3(proMod As String, Predicate As String, Arity As Integer, Optional Arg0 As String = "", Optional Arg1 As String = "", Optional Arg2 As String = "") As String
    Dim rgArgs() As Variant
    Dim Out As String
    Dim bit  As Boolean
    
    If proMod = "" Then proMod = "User"
    thePrologQuery.SetPredicate Predicate, Arity, proMod
    
        ReDim rgArgs(0)
        rgArgs(0) = Empty
        
   Select Case Arity
        
        Case 1
            If Arg0 <> "" Then rgArgs(0) = Arg0 Else Arg0 = Empty
        
        Case 2
            ReDim rgArgs(1)
            If Arg0 <> "" Then rgArgs(0) = Arg0 Else rgArgs(0) = Empty
            If Arg1 <> "" Then rgArgs(1) = Arg1 Else rgArgs(1) = Empty
        
        Case 3
            ReDim rgArgs(2)
            If Arg0 <> "" Then rgArgs(0) = Arg0 Else rgArgs(0) = Empty
            If Arg1 <> "" Then rgArgs(1) = Arg1 Else rgArgs(1) = Empty
            If Arg2 <> "" Then rgArgs(2) = Arg2 Else rgArgs(2) = Empty
        
    End Select
       
    thePrologQuery.OpenQuery 0, rgArgs
    

 While thePrologQuery.NextSolution(rgArgs)
        Out = Out + vbLf + rgArgs(0)
    Wend

    thePrologQuery.CloseQuery
    
    doProlog = Out
  
End Function


Public Function Consult(ByVal Resource As String) As Boolean
    doProlog "system", "consult", 1, Replace(Resource, "\", "/")
    Consult = True
    
End Function

Public Function Execute(ByVal mvarINativeQuery As String) As String
    Dim rgArgsIn() As Variant
    Dim rgArgsOut() As Variant
    Dim Limit As Long
    Limit = 500
        thePrologQuery.SetPredicate "plquery", 1, "user"
    
        ReDim rgArgsIn(0)
        ReDim rgArgsOut(0)
        rgArgsIn(0) = mvarINativeQuery
            
        thePrologQuery.OpenQuery 0, rgArgsIn
    
        mvarIAnswerList = ""
        While thePrologQuery.NextSolution(rgArgsOut)
            mvarIAnswerList = mvarIAnswerList + vbLf + rgArgsOut(0)
         Wend
        
        thePrologQuery.CloseQuery
        
        thePrologQuery.SetPredicate "getresult", 1, "user"
        
        ReDim rgArgsOut(0)
        
        thePrologQuery.OpenQuery 0, rgArgsOut
        
        mvarIAnswerList = ""
        While (thePrologQuery.NextSolution(rgArgsOut) And Limit > 0)
        Limit = Limit - 1
            mvarIAnswerList = mvarIAnswerList + vbLf + rgArgsOut(0)
         Wend
        
       Execute = mvarIAnswerList
        
End Function

Private Sub Class_Initialize()
    Set thePrologQuery = New PROQUERYLib.PrologQuery
    On Error GoTo mp_end
    '  Display the path to the SWI binaries
    'sPath = thePrologQuery.ModulePath
    'MsgBox sPath & " (" & sCurDir & ")", vbOKOnly, "SWI Path"
mp_end:
    Consult App.Path & "plquery.pl"
    #If DebugMode Then
        'get the next available class ID, and print out
        'that the class was created successfully
        mlClassDebugID = GetNextClassDebugID()
        Debug.Print "'" & TypeName(Me) & "' instance " & mlClassDebugID & " created"
    #End If
End Sub


Private Sub Class_Terminate()
    'the class is being DestinationNameroyed
    #If DebugMode Then
        Debug.Print "'" & TypeName(Me) & "' instance " & CStr(mlClassDebugID) & " is terminating"
    #End If
    Set thePrologQuery = Nothing
End Sub


#If DebugMode Then
    Public Property Get ClassDebugID()
        'if we are in debug mode, surface this property that consumers can query
        ClassDebugID = mlClassDebugID
    End Property
#End If


Public Sub SetPredicate(ByVal bstrName As String, ByVal iArity As Long, ByVal bstrModule As String)
    thePrologQuery.SetPredicate bstrName, iArity, bstrModule
End Sub


Public Sub OpenQuery(iFlags As Long, rgArgs() As Variant)
    thePrologQuery.OpenQuery iFlags, rgArgs()
End Sub

Public Function ModulePath() As String
ModulePath = thePrologQuery.ModulePath
End Function

Public Function CountQueries() As Long
    On Error GoTo CQ_exit
    Dim rgArgs() As Variant
    Dim cQuery As Integer
    cQuery = 0
    While thePrologQuery.NextSolution(rgArgs)
        cQuery = cQuery + 1
    Wend
CQ_exit:
    CountQueries = cQuery
    Exit Function
End Function

Public Function pl_trim(ByVal plstr, Optional ByVal depth As Long = 999, Optional ByVal atomic As Boolean = False)
    Dim i, tempar()
    If IsObject(plstr) Then Set pl_trim = plstr
    If atomic Then
            ''If InStr(plstr, ",") = 0 Then GoTo crunch:
            pl_split_pred tempar, plstr, ","
            If TypeName(tempar) = "Empty" Then pl_trim = plstr: Exit Function
            If UBound(tempar) = 0 Then pl_trim = pl_trim(tempar(0), depth, False): Exit Function
            pl_trim = plstr
        Else
crunch:
            depth = depth - 1
            plstr = Trim(plstr)
            i = InStr(vbQt & "[('", Left(plstr, 1))
            If i > 0 And Len(plstr) > 2 And i = InStr(vbQt & "])'", Right(plstr, 1)) Then
                plstr = Mid(plstr, 2, Len(plstr) - 2)
                If depth = 0 Then pl_trim = plstr: Exit Function
                GoTo crunch:
            End If
            pl_trim = plstr
           '
    End If
End Function

Public Sub pl_split_pred(ByRef pl_split, ByVal theString, Optional ByVal substr = ",")
Dim escapelevel As Long, inastring As String, splitablestring As String, thechar As String, i As Long
Dim temparray() As String
    For i = 1 To Len(theString)
        thechar = Mid(theString, i, 1)
        Select Case thechar
            Case "[", "("
                If inastring = "" Then escapelevel = escapelevel + 1
            Case ")", "]"
                If inastring = "" Then escapelevel = escapelevel - 1
            Case """", "'"
                If inastring = "" Then
                        inastring = thechar
                    Else
                        If inastring = thechar Then inastring = ""
                End If
            Case substr
                If escapelevel = 0 And inastring = "" Then thechar = vbLf
        End Select
        splitablestring = splitablestring & thechar
    Next
    
    If splitablestring = "" Then ReDim pl_split(0): pl_split(0) = "": Exit Sub
    temparray = Split(splitablestring, vbLf)
    ReDim pl_split(UBound(temparray))
    If UBound(pl_split) <> UBound(temparray) Then:
    For i = 0 To UBound(temparray)
    pl_split(i) = temparray(i)
    Next i

End Sub



