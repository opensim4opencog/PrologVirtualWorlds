Attribute VB_Name = "mdlTestClassSharedData"
'*********************************************************************************************
'
' Implementing source interfaces
'
' Test class shared data.
'
'*********************************************************************************************
'
' Author: Eduardo A. Morcillo
' E-Mail: e_morcillo@yahoo.com
' Web Page: http://www.domaindlx.com/e_morcillo
'
' Distribution: You can freely use this code in your own applications but you
'               can't publish this code in a web site, online service, or any
'               other media, without my express permission.
'
' Usage: at your own risk.
'
' Tested on: Windows 98 + VB5
'
' History:
'           06/03/2000 - This code was released
'
'*********************************************************************************************
Option Explicit

' Data shared by all cTestClass objects
Public cTestClass_lOriginalFind As Long
Public cTestClass_lOriginalEnum As Long
Public cTestClass_lInstanceCounter As Long
Public cTestClass_DefaultCP As IConnectionPoint

