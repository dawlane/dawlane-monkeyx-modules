' This is a modified version of the official BRL requester.monkey file to allow the use with Linux operating systems
#If TARGET<>"glfw" Or (HOST<>"winnt" And HOST<>"macos" And HOST<>"linux")	' dawlane mod
#Error "Requesters module unavailable on this target"
#Endif

' dawlane additional: Linux file are broken up to make them more easy to edit
#If HOST<>"linux"
Import "native/requesters.cpp"
#Else
Import "native/requesters_linux_misc.cpp"			' Miscellaneous functions and data
Import "native/requesters_linux_path.cpp"			' Path resolver
Import "native/requesters_linux_messagebox.cpp"		' Message box function
Import "native/requesters_linux_file.cpp"			' Directory and File Chooser
Import "native/requesters_linux_glue.cpp"			' Binding functions wrappers
#End

Extern

Function Notify:Void( title:String,text:String,serious:Bool=False )="bbNotify"

Function Confirm:Bool( title:String,text:String,serious:Bool=False )="bbConfirm"

Function Proceed:Int( title:String,text:String,serious:Bool=false )="bbProceed"

Function RequestFile:String( title:String,extensions:String="",save:Bool=False,file:String="" )="bbRequestFile"

Function RequestDir:String( title:String,dir:String="" )="bbRequestDir"
