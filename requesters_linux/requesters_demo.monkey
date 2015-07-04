#REM
	Requester Linux v0.5

	The requesters module is strictly for GLFW PC Desktops, but with a bit of file editing the Linux GTK version will work with the C++Tool.
	The offical release from Blitz Research only support Window and Apple OS X.
	You will need to install the GTK+2/3 development libraries for Linux.
	To make it simple without having to hack the Linux Makefiles. Modify the files below and rebuild transcc

	In src/trans/builders directory
	glfw.monkey modifications in GCC section:
		Delete
			Local ccpots:=""
	Replace with:
		Local OPTS:="",LIBS:=""
		Local cc_opts:=GetConfigVar( "CC_OPTS" )
		If cc_opts OPTS+=" "+cc_opts.Replace( ";"," " )			
		Local cc_libs:=GetConfigVar( "CC_LIBS" )
		If cc_libs LIBS+=" "+cc_libs.Replace( ";"," " )
	Change:
		ccopts+=" -O0"
		ccopts+=" -O3 -DNDEBUG"
		Execute cmd+" CCOPTS=~q"+ccopts+"~q OUT=~q"+casedConfig+"/MonkeyGame~q"
	To:
		OPTS+=" -O0"
		OPTS+=" -O3 -DNDEBUG"
		Execute cmd+" CCOPTS=~q"+OPTS+"~q CCLIBS=~q"+LIBS+"~q OUT=~q"+casedConfig+"/MonkeyGame~q"
		
	In targets/glfw/gcc_linux/Makefile and targets/glfw3/gcc_linux/Makefile
	Append $(CCLIBS) to LDLIBS. Note that this must be placed last. Libaries need to be linked in the correct order

	KNOWN ISSUES:
		Mouse cursor input in full screen mode will only work with GLFW 3

		As there are basically two windowing systems at work. Clicking the background on a GLFW fullscreen window will select it and hide the requester.
		You can cycle through applications with Atl+Tab key combanation to bring them back into focus.

		Passing an hidden file will activate the hidden file switch, but you may have to manually disable it the next time the file browser opens.
		This has more to do with the file manager settings that the module. You should find the settings files that forces this in the users home ./config/gtk directory.

		If the distribution uses GTK+2 (e.g Linux Mint) and the compiled binary is GTK+3. Then expect issues with dialogs not showing in the centre of the display etc
		It's recommened that you use Gtk+2 as those distributions that are based on Gtk+3 should be backward compatible. There is still a hell of a lot of Gtk+2 software out there.

		To make it consistant if a malformed or non existant directory is passes. Then the default is to use the users home directory.
		The orignal idea was to use the recent, but GTK+3 doesn't work correctly with Linux Mint. So it went out the window.

	RULES:
		A malformed or non existent path opens the users home directory. If there is a file there with the file name passed, it will be selected.
		Just passing a wildcard/file with a known filter extension will open the users home directory with the filter selector, else all files are displayed
		For the RequestFile function, the last field in a path will be assumed to be a file name, so to open a direcory end it with a back-slash /
		For the RequestFile function set to saving. Using a wildcard with extension will set the file name to Untitled plus the extension specified.
		Passing a hidden file will activate the Show Hidden files attribute.
		If the filter is in the filter list, then the filter selector is shown
		Just setting the directory will open that directory and set the file name to Untitled.
		An empty filter list will show all files.

	TO DO
		Check for memory leaks. Kind of tricky with all the noise from Gtk and glib and the suppersion files available do not work well

		Figure out why a GTK+3 application on a GTK+2 distribution has problems not detecting recent files and the miss placement of the
		file chooser dialog when GTK_FILE_CHOOSER_ACTION_OPEN is used. Both issues so far are with Linux Mint.

#END

#CC_OPTS="`pkg-config --cflags gtk+-2.0`"
#CC_LIBS="`pkg-config --libs gtk+-2.0`"
'#GLFW_WINDOW_FULLSCREEN=True

Import mojo
Import os

Import brl.requesters

Class MyApp Extends App

	Field dsktp:DisplayMode
	Field homePath:String = GetEnv("HOME")
	Field file:String = ""
	Field dir:String = ""
	Field confirm:Int = 0
	Field proceed:Int = 0
	Field start:Bool
	Field filter:="Image Files:png,jpg,bmp;Text Files:txt,doc,rtf;All Files:*"

	Method OnCreate()
		dsktp = DesktopMode() ' Get the current desktop resolution so it can be restored. Do not use ExitApp or EndApp before you have restored with SetDeviceWindow
		'SetDeviceWindow(1024,768,1)	'Uncomment for full screen, but remember to uncomment the SetDeviceWindow in the ShutDown method
		SetUpdateRate(30)
    	HideMouse()
	End
    
	Method OnUpdate()
		If KeyDown(KEY_SPACE) And start = False
			start = True
			ShowMouse()
			' No path, but hidden file with a wildcard and extension, so open the user home directory with hidden attributes set.
			' As the filter list is being passed and the extension used is in there; set the filter to show directory and txt files only.
			file = RequestFile( "Select file to save",filter, True, ".*.txt")
			If file Notify "Notice: RequestFile Save",file

			' Full path being passed with wildcard and filter. As jpg is in the filter list, it will be shown.  
			file = RequestFile( "Select file to open",filter, False, homePath+"/Pictures/*.jpg")
			If file Notify "Notice: RequestFile Open",file
			dir = RequestDir("Select Directory to open", "/opt")
			If dir Notify "Notice: RequestDir",dir
			If file<>"" Or dir <> "" Then confirm = Confirm("Confirm?","Confirm: did the notice display the correct paths?", False)       
			If confirm 
				Notify "Notify","You said that the file paths were correct"
				proceed = Proceed("Proceed?","Do you wish to display the file selected?", True)
				If proceed = 1
					Notify "Notify","Only joking. I cannot be bothered to write the code for it."
				Else
					If proceed = 0 Notify "Notify","So you don't like my code then?" Else Notify "Notify","Guess I will never know"
				Endif
			Else
					Notify "Notify","Don't just stand there. Call support!"
			Endif
		Endif
		If KeyDown(KEY_ESCAPE) Then ShutDown()
	End
    
	Method OnRender()
		DrawText("Press Space key to start",  (DeviceWidth()/2)-80,(DeviceHeight()/2)-20)
		DrawText("Press Escape key to exit", (DeviceWidth()/2)-80,DeviceHeight()/2)
	End

	Method ShutDown()
		'SetDeviceWindow(dsktp.Width, dsktp.Height,1)
		EndApp()
	End       
End

Function Main()

    New MyApp

End
