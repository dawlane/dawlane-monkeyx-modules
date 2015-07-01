/*
	Requesert Linux Mojo function bindings v0.4
	by J.Cook aka dawlane
*/

String bbRequestFile(String title,String filter,int save,String path)
{
    String str;
    struct FilePathData *resolvePath = PathResolver(path, save);
    // If there was no error, the execute the dialog
    if (resolvePath->_err == 0)
        str = GTK_FileChooserDialog(title, filter, save, resolvePath);
    // Note that any GTK/GLIB functions that return a value allocated on the heap have to be freed with g_free and NOT free
    // or weird things could happen.
    g_free(resolvePath->_dirPath);
    g_free(resolvePath->_fileName);
    g_free(resolvePath->_fileExt);
    free(resolvePath);
    return str;
}

String bbRequestDir(String title,String dir)
{
    String str = "";
    struct FilePathData *resolvePath = PathResolver(dir, -1);
    // If there was no error, the execute the dialog
    if (resolvePath->_err == 0)
        str = GTK_FileChooserDialog(title,"",-1, resolvePath);
    // Note that any GTK/GLIB functions that return a value allocated on the heap have to be freed with g_free and NOT free
    // or weird things could happen.
    g_free(resolvePath->_dirPath);
    g_free(resolvePath->_fileName);
    g_free(resolvePath->_fileExt);
    free(resolvePath);
    return str;
}

void bbNotify(String title,String text,int serious )
{
    GTK_MessageDialog(title, text, serious, _Notify);
}

int bbConfirm( String title,String text,int serious )
{
    return GTK_MessageDialog(title, text, serious, _Confirm);
}

int bbProceed( String title,String text,int serious )
{
    return GTK_MessageDialog(title, text, serious, _Proceed);
}
