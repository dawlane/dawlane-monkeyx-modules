/*
	Requester Linux File/Directory Selector Dialog v0.4
	by dawlane
*/

static void GTK_callBackShow(GtkWidget *widget, gpointer *_userData)
{
    bool b = _userData;
    gtk_file_chooser_set_show_hidden(GTK_FILE_CHOOSER(widget),b);
}

String GTK_FileChooserDialog(String title,String filterStr,int save, FilePathData *filePathData)
{

    // Set GTK Struff
    GtkWidget *dialog;
    GtkFileChooserAction action;
	GArray *filterList = NULL;
	const gchar *confirmButton;

	int filterIndex = -1;				// Make a note of the the filter to use if a known extension is used
	bool extKnown = false;				// Flag to set if the extension isn't in the filters list
	String returnStr = "";			    // Create an empty string in case no vale is returned. Should really return a structure with a string and an error code

	// GTK initialise error check
	if (!pannelBegin())
	{
        printf("Error: Unable to initialise gtk\n");
        return returnStr;
    }

	// See what the chooser action should be bases on the value of save
	switch(save)
	{
		case -1:
			action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
			confirmButton = "_Open";
			break;
		case 0:
			action = GTK_FILE_CHOOSER_ACTION_OPEN;
			confirmButton = "_Open";
			break;
		case 1:
			action = GTK_FILE_CHOOSER_ACTION_SAVE;
			confirmButton = "_Save";
			break;
	}

	// Process filters
	if ((filterStr.Length()) && (save != -1) && (strlen(filePathData->_fileExt) != 0))
	{
		// Generate a filter array
        GtkFileFilter *filter;
        filterList = g_array_new(FALSE,TRUE,sizeof(filter));
		// Parse the filter string. Modified version of Marks apple filter loop
        int i0 = 0, i1, i2, i3, start = 0, id = 0;
		while( i0 < filterStr.Length())
		{
			i1 = filterStr.Find(":", i0)+1;
           	if (!i1) break;
            else
            {
                // Find the end of the group list
                i2 = filterStr.Find(";", i1);
                if (i2 == -1) i2 = filterStr.Length();
                filter = gtk_file_filter_new();
                String group = filterStr.Slice(start,i2);
                gtk_file_filter_set_name(filter,group.ToCString<char>());
                start = i2+1; // Record the end of the group so the next group name can be retrieved
            }
            // Now add the extensions for the group
            while(i1 < i2)
            {
            	i3= filterStr.Find( ",", i1 );
            	if( i3==-1 ) i3=i2;
				if (filterStr.Slice(i1,i3) == filePathData->_fileExt){
					filterIndex = id;
					extKnown = true;
				}
				String g_ext = String("*.")+filterStr.Slice(i1,i3);
				gtk_file_filter_add_pattern(filter,g_ext.ToCString<gchar>());
				i1=i3+1;
			}
			g_array_append_val(filterList,filter);
			id++;
			i0=i2+1;
		}
	}

    // Set up the file chooser dialog
	dialog = gtk_file_chooser_dialog_new(NULL,NULL,action,("_Cancel"),GTK_RESPONSE_CANCEL,(confirmButton),GTK_RESPONSE_ACCEPT,NULL);
	g_object_ref_sink(dialog);

	// Set the window
	gtk_window_set_title(GTK_WINDOW(dialog),title.ToCString<char>());
	gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
	gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER(dialog), TRUE);
	g_signal_connect(dialog, "show", G_CALLBACK(GTK_callBackShow), (gpointer)filePathData->_hiddenFile);

    // Sort out what the dialog paths should be set to
	gchar *path = NULL;
	struct FileResponseData isType;

	if (strlen(filePathData->_dirPath) != 0)
	{
		switch(save)
		{
			case -1: // Directory
				gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),filePathData->_dirPath);
				break;
			case 0: // Load
				if ((strlen(filePathData->_fileName) != 0) && (!filePathData->_wildcard))
				{
            	    path = g_strconcat(filePathData->_dirPath,filePathData->_fileName,NULL);
               	 	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),path);
            	}
            	else
            	{
                	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),filePathData->_dirPath);
            	}
            	break;
			case 1: // Save
				if ((strlen(filePathData->_fileName) != 0) && (!filePathData->_wildcard))
				{
                	path = g_strconcat(filePathData->_dirPath,filePathData->_fileName,NULL);
                	isType = RequesterFileType(path);
                	if (isType._err == 0)
                   		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),path);
                	else
                	{
                    	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),filePathData->_dirPath);
                    	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),filePathData->_fileName);
                	}
            	}
            	else
            	{
                	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),filePathData->_dirPath);
                	if (filePathData->_wildcard) gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),filePathData->_fileName);
            	}
				break;
		}
	}

	// Add the filters if any
	if ((filterList != NULL) && (extKnown))
	{
		for(int i = 0; i < (filterList->len); i++)
		{
        	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), g_array_index(filterList, GtkFileFilter*, i));
		}
		if (filterIndex > -1) gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), g_array_index(filterList, GtkFileFilter*, filterIndex));
	}
	// Should check that there are no memory leaks. If I read correctly the docs say the file chooser takes ownership of filters, so they should be deleted when it goes.
	if (filterList != NULL) g_array_free(filterList, TRUE);

	// Show and retrieve the data from the dialog
    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    int result = GTK_ResponseID(response);
    if (result > 0)
    {
    	gchar *fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    	returnStr = fname;
    	g_free(fname);
    }
    // Cleanup and return result
    if (path != NULL) g_free(path);
    pannelEnd(dialog);
    return returnStr;
}

