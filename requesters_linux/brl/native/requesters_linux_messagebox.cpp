/*
	Requester Linux General Common Message Box v0.5
	by J.Cook aka dawlane
*/

int GTK_MessageDialog(String title, String text, int serious, int type)
{

    GtkWidget *message, *content_area, *box, *icon , *dialog;	// Set data objects etc
    if (!pannelBegin()) type = -1;
    // Select which dialog to display
    switch(type)
    {
    case 1:		// Notify
        dialog = gtk_dialog_new_with_buttons(title.ToCString<char>(),NULL,GTK_DIALOG_MODAL,("_OK"),GTK_RESPONSE_OK,NULL);
        break;
    case 2:		// Confirm
        dialog = gtk_dialog_new_with_buttons(title.ToCString<char>(),NULL,GTK_DIALOG_MODAL,("_YES"),GTK_RESPONSE_YES,("_NO"),GTK_RESPONSE_NO,NULL);
        break;
    case 3:		// Proceed
        dialog = gtk_dialog_new_with_buttons(title.ToCString<char>(),NULL,GTK_DIALOG_MODAL,("_YES"),GTK_RESPONSE_YES,("_NO"),GTK_RESPONSE_NO,("_Cancel"),GTK_RESPONSE_CANCEL,NULL);
        break;
    default:	// Return an error code if the message box type couldn't be deterimed
        printf("Error: No messagebox type specified. Options 1) Notify 2) Confirm 3) Proceed.\n");
        return -1000;
    }

    // Build the message dialog box
#if GTK_MAJOR_VERSION < 3
    box = gtk_hbox_new (FALSE,0);
#else
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,6);
#endif

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Tip you can add more icon types, but the safest is to use the known themes icons. See http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
    // To use more you will have to change the serious parameter to int in requesters.monkey
    switch(serious)
    {
    // Use the free desktop specifications for icons
    case 0:
        icon = gtk_image_new_from_icon_name("dialog-information",GTK_ICON_SIZE_DIALOG);
        break;
    case 1:
        icon = gtk_image_new_from_icon_name("dialog-warning",GTK_ICON_SIZE_DIALOG);
        break;
    case 2:
        icon = gtk_image_new_from_icon_name("dialog-password",GTK_ICON_SIZE_DIALOG);
        break;
    case 3:
        icon = gtk_image_new_from_icon_name("dialog-question",GTK_ICON_SIZE_DIALOG);
        break;
    default:
        icon = gtk_image_new_from_icon_name("dialog-error",GTK_ICON_SIZE_DIALOG);
    }

    // Build the dialog and show
    message = gtk_label_new(text.ToCString<char>());

    gtk_label_set_line_wrap(GTK_LABEL(message),TRUE);
    gtk_container_add(GTK_CONTAINER (content_area), box);
    gtk_box_pack_start(GTK_BOX(box), icon, TRUE, TRUE, 6);
    gtk_box_pack_start(GTK_BOX(box), message, TRUE, TRUE, 6);
    gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);

    // Show the dialog
    gtk_widget_show_all(dialog);

    // Run and wait for a result.
    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    // Clean up
    gtk_widget_destroy(icon);
    gtk_widget_destroy(message);
    gtk_widget_destroy(box);
    gtk_widget_destroy(content_area);
    gtk_widget_destroy(dialog);
    pannelEnd();
    
    return GTK_ResponseID(response);
}