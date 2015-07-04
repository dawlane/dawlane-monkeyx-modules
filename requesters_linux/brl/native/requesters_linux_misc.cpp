/*
	Requester Linux Miscellaneous Data and Functions v0.5
	by J.Cook aka dawlane
*/

// Could add this into the main.h template in the Linux section
#include <gtk/gtk.h>
#include <errno.h>

// The posible options for a dialog
#define _Notify 1
#define _Confirm 2
#define _Proceed 3

// This is for retrieveing data about a file or directorys existance
struct FileResponseData
{
    unsigned char _type;
    int _err;
};

// Possible results for FileResponseData
#define _ISREG 	0x01
#define _ISDIR 	0x02
#define _ISCHR 	0x04
#define _ISBLK 	0x08
#define _ISFIFO 0x10
#define _ISLNK 	0x20
#define _ISSOCK 0x40

/*
	Do a check on the path given. As there is no guarantee the the OS module would be used. I added this
	1 for file, 2 for directory. If there is a problem, pass errno. A zero means all was OK
	It returns a structure with either the file or directory flag set (or both?) along with the error code
	An error code of 0 means it successfully detected something and a value of ENOENT will mean that a path
	component is missing i.e. it doesn't exist.
*/
static FileResponseData RequesterFileType(const char *filename)
{
    // Create and clear the structure to return
    struct FileResponseData rfd;
    rfd._type = 0;
    rfd._err = 0;

    int result = 0;
    //printf("looking for %s\n", filename);
    struct stat st;
    // First Check that it's not a directory!
    result = stat(filename, &st);
    if (result == 0)
    {
        if (S_ISREG(st.st_mode)) 	rfd._type = _ISREG;
        if (S_ISDIR(st.st_mode)) 	rfd._type = _ISDIR;
        if (S_ISCHR(st.st_mode)) 	rfd._type = _ISCHR;
        if (S_ISBLK(st.st_mode)) 	rfd._type = _ISBLK;
        if (S_ISFIFO(st.st_mode)) 	rfd._type = _ISFIFO;
        if (S_ISLNK(st.st_mode)) 	rfd._type = _ISLNK;
        if (S_ISSOCK(st.st_mode)) 	rfd._type = _ISSOCK;
        rfd._err = 0;
    }
    else
        rfd._err = errno;
    return rfd;
}

// Get the window response and return the appropriate
static int GTK_ResponseID(int value)
{
    switch(value)
    {
    case GTK_RESPONSE_YES:
    case GTK_RESPONSE_ACCEPT:
    case GTK_RESPONSE_APPLY:
    case GTK_RESPONSE_OK:
        return 1;
        break;
    case GTK_RESPONSE_CLOSE:
    case GTK_RESPONSE_NO:
        return 0;
        break;
    case GTK_RESPONSE_REJECT:
    case GTK_RESPONSE_CANCEL:
        return -1;
        break;
    default:
        return -2;
    }
}

// Initialise the GTK framework
static bool pannelBegin()
{
    if (!gtk_init_check(NULL,NULL))
    {
        printf("ERROR: Unable to initailise the GTK windowing system.\n");
        return false;
    }

#if GTK_MAJOR_VERSION < 3

#endif
    return true;
}

// Close the dialog
static void pannelEnd()
{
    while (g_main_context_iteration(NULL, false));
}

