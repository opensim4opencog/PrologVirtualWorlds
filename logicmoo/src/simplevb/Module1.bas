Attribute VB_Name = "Module"
Option Explicit
Public a As frmBrowser
Sub main()

Set a = New frmBrowser
a.Show

App.TaskVisible = True

End Sub
