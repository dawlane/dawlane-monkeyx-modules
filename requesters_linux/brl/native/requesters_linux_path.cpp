/*
	Requester Linux Resolve File Paths v0.5
	by J.Cook aka dawlane
*/

// Process file path data structure
struct FilePathData
{
    gchar	*_dirPath;
    gchar	*_fileExt;
    gchar	*_fileName;
    bool  	_hiddenFile;
    bool  	_wildcard;
    int		_err;
};

// Process a path
// A rough method to get the path, filename and extension. No nice walking the path checking here!
static FilePathData *PathResolver(String path, int save)
{
    struct FilePathData *resolve = (FilePathData*)malloc(sizeof(FilePathData));

    int lastSlash = -1;                             // position of the trailing slash
    int extDot = -1;                                // position of the extension if any
    String fileName = "";                           // The file name extracted
    String dirPath = "";                            // The path name extracted
    String fileExt = "";                            // The extension extracted
    resolve->_hiddenFile = false;
    String homePath = String(getenv("HOME"))+"/";   // Store the users home directory

    // Split the path up into component parts.
    if (save > -1)
    {
        lastSlash = path.FindLast("/");
        fileName = path.Slice(lastSlash+1,path.Length());
        dirPath = path.Slice(0, lastSlash+1);
        extDot = fileName.FindLast(".");
        resolve->_hiddenFile = (fileName.StartsWith(".") == true)  ? TRUE : FALSE;
        resolve->_wildcard = (fileName.Find("*.") > -1) ? true : false;

        // This next bit should get the extension from a hidden file
        if (resolve->_hiddenFile)
        {
            String tmp_s = fileName.Slice(1, fileName.Length());
            extDot = tmp_s.FindLast(".");
            fileExt = fileName.Slice(extDot+2, fileName.Length());
            resolve->_hiddenFile = true;
        }
        else if (extDot > 0)	fileExt = fileName.Slice(extDot+1, fileName.Length());
    }
    else
    {
        dirPath = path;
    }

    switch(save)
    {
    case 1:
        if (fileName.Length() == 0) fileName = "Untitled";
        if (resolve->_wildcard) fileName = String("Untitled.")+fileExt;
        if ((resolve->_hiddenFile) && ((fileName.Length() == 0) || (fileName.Length() == 1))) fileName = ".Untitled";
        if ((resolve->_hiddenFile) && (resolve->_wildcard)) fileName = String(".Untitled.")+fileExt;
    }

    // Test the dirPath to see is it is valid, if not use the users home directory
    struct FileResponseData tmp = RequesterFileType(dirPath.ToCString<char>());
    if (tmp._err == ENOENT)
    {
        dirPath = homePath;
        tmp._err = 0;
    }

    // Load up the file path data
    resolve->_dirPath = g_strdup(dirPath.ToCString<gchar>());
    resolve->_fileName = g_strdup(fileName.ToCString<gchar>());
    resolve->_fileExt = g_strdup(fileExt.ToCString<gchar>());
    resolve->_err = tmp._err;
    return resolve;
}

