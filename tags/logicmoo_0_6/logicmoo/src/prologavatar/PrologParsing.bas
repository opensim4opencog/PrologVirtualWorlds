Attribute VB_Name = "PrologParsing"



Public Function pl_trim(ByVal pl_string, Optional ByVal depth As Integer = 999, Optional ByVal atomic As Boolean = False)
Dim i, tempar()
If IsObject(pl_string) Then Set pl_trim = pl_string
If atomic Then
        ''If InStr(pl_string, ",") = 0 Then GoTo crunch:
        pl_split_pred tempar, pl_string, ","
        If typeName(tempar) = "Empty" Then pl_trim = pl_string: Exit Function
        If UBound(tempar) = 0 Then pl_trim = pl_trim(tempar(0), depth, False): Exit Function
        pl_trim = pl_string
    Else
crunch:
        depth = depth - 1
        pl_string = Trim(pl_string)
        i = InStr(vbQt & "[('", Left(pl_string, 1))
        If i > 0 And Len(pl_string) > 2 And i = InStr(vbQt & "])'", Right(pl_string, 1)) Then
            pl_string = Mid(pl_string, 2, Len(pl_string) - 2)
            If depth = 0 Then pl_trim = pl_string: Exit Function
            GoTo crunch:
        End If
        pl_trim = pl_string
       '
End If
End Function

Public Sub pl_split_pred(ByRef pl_split, ByVal theString, Optional ByVal substr = ",")
Dim escapelevel As Long, inastring As String, splitablestring As String, thechar As String, i As Long
Dim temparray() As String
If IsArray(theString) Then pl_split = theString: Exit Sub
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
'Stop
End Sub

Public Function parse_pl_string_vbstr(ByVal pl_string) As String
 Dim i As Long, arg() As String
'
If Val(pl_string) > 0 Then parse_pl_string_vbstr = "(" & pl_string & ")": Exit Function
    
    pl_string = pl_trim(pl_string, 1)
    pl_split_pred arg, pl_string, ","
    For i = 0 To UBound(arg)
        arg(i) = pl_trim(arg(i), 1)
    Next
    parse_pl_string_vbstr = Join(arg(), " ")
'
End Function

Public Function parse_vbstr_pl_string(ByVal theString) As String
Dim Poo As Object
'TODO
If IsObject(theString) Or typeName(theString) = "Unknown" Then
    theString = theMorphism.object_pl(theString, 2, True)
End If
parse_vbstr_pl_string = theString
'Exit Function

Dim temparray() As String, tinystring As String, i%, ii%, escapethis As String
':
    theString = Trim(Replace(theString, vbTab, " "))
    theString = Trim(Replace(theString, "'", " "))
    theString = Trim(Replace(theString, "\", "/"))
    While Right(theString, 1) = "."
        theString = Trim(Left(theString, Len(theString) - 1))
       Wend
    theString = Replace(theString, "?", " ?")
    theString = Replace(theString, "  ", " ")
    pl_split_pred temparray, theString, " "
    For i% = 0 To UBound(temparray)
        tinystring = Replace(temparray(i%), "'", "\'")
        For ii% = 0 To escapable_chars.Count - 1
            escapethis = escapable_chars.Property(ii%)
            If InStr(tinystring, escapethis) Then temparray(i%) = "'" & tinystring & "'": GoTo escapedthis
        Next ii%
        temparray(i%) = tinystring
escapedthis:
    Next i%
    parse_vbstr_pl_string = stringlist_opener & Join(temparray(), ",") & stringlist_terminator
End Function

Public Function error_string(ByVal theerror As ErrObject)
    error_string = "true"
    If theerror Then error_string = LCase("vwError(" & parse_vbstr_pl_string(Err.Source & ":" & Err.Description & ":" & Err.LastDllError) & ")")
End Function

Public Function pl_regexp(ByVal pl_stringin)
   ' pl_string = pl_stringin
   ' pl_string = pl_trim(pl_string, , True)
    pl_regexp = Replace(pl_stringin, ")", "\)")
    pl_regexp = Replace(pl_regexp, "]", "\]")
    pl_regexp = Replace(pl_regexp, "(", "\(")
    pl_regexp = Replace(pl_regexp, "[", "\[")
    'pl_regexp = Replace(pl_regexp, ",", ".+")
    pl_regexp = Replace(pl_regexp, "_", ".+")
    pl_regexp = Replace(pl_regexp, " ", "")

End Function


Public Function pl_compare(ByVal pl_string, ByVal vbstr)
    pl_string = pl_trim(pl_string)
    pl_string = Replace(pl_string, ")", "\)")
    pl_string = Replace(pl_string, "(", "\(")
    pl_string = Replace(pl_string, "_", ".+")
    pl_string = Replace(pl_string, " ", "")
    java_vbscript_engine.ExecuteStatement ("regex.pattern = """ & LCase(pl_string) & """")
    pl_compare = java_vbscript_engine.Eval("regex.test(""" & vbstr & """)")
End Function


