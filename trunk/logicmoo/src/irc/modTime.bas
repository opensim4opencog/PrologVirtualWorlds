Attribute VB_Name = "modTime"
'* This module is composed of functions
'* off of planet-source-code given to me
'* by a person going under the name of 'Jason'
'* and I thank him for it.

Public Type SYSTEMTIME
        wYear As Integer
        wMonth As Integer
        wDayOfWeek As Integer
        wDay As Integer
        wHour As Integer
        wMinute As Integer
        wSecond As Integer
        wMilliseconds As Integer
End Type
Public Type TIME_ZONE_INFORMATION
        Bias As Long
        StandardName(32) As Integer
        StandardDate As SYSTEMTIME
        StandardBias As Long
        DaylightName(32) As Integer
        DaylightDate As SYSTEMTIME
        DaylightBias As Long
End Type
Public Declare Function GetTimeZoneInformation Lib "kernel32" (lpTimeZoneInformation As TIME_ZONE_INFORMATION) As Long

Public Function GetGMTBias() As Integer
    Dim lpTimeZoneInformation As TIME_ZONE_INFORMATION
    
    GetTimeZoneInformation lpTimeZoneInformation
    GetGMTBias = lpTimeZoneInformation.Bias
End Function


Public Function GetGMTBiasString() As String
    Dim x As Long, Y As Long
    x = -GetGMTBias
    Y = x Mod 60
   
    x = x \ 60
    If Y < 0 Then
        Y = -Y
        GetGMTBiasString = "GMT-" & Format$(x, "00") & ":" & _
        Format$(Y, "00")
    ElseIf x < 0 Then
        GetGMTBiasString = "GMT-" & _
        Format$(x, "00") & ":" & Format$(Y, "00")
    Else
        GetGMTBiasString = "GMT+" & _
        Format$(x, "00") & ":" & Format$(Y, "00")
    End If
End Function


Function CTime() As Long
    CTime = toCTime(Now)
End Function

Function toCTime(d As Date) As Long
    toCTime = DateDiff("s", CDate(#1/1/1970# - GetGMTBias / 60 / 24), d)
End Function

Function AscTime(CTime As Long) As Date
    AscTime = CDate(#1/1/1970# - GetGMTBias / 60 / _
    24) + (CTime / 3600& / 24)
End Function


