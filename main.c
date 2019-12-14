#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>
#include <string.h>

typedef struct {

    GObject *window;

    GObject *menu_quit;
    GObject *menu_new;
    GObject *menu_open;

    GObject *entry_my_callsign;
    GObject *entry_date;
    GObject *entry_start_time;
    GObject *entry_end_time;
    GObject *entry_frequency;
    GObject *entry_mode;
    GObject *entry_remote_callsign;
    GObject *entry_power;
    GObject *entry_rst_sent;
    GObject *entry_rst_recv;
    GObject *entry_notes;

    GObject *button_today;
    GObject *button_start_time;
    GObject *button_end_time;
    GObject *button_qrz;
    GObject *button_59_sent;
    GObject *button_59_recv;
    GObject *button_save;
    GObject *button_reset;

    sqlite3 *db;

} ObjectList;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void reset(gpointer data)
{
    ObjectList *objects = (ObjectList *)data;

    gtk_entry_set_text(GTK_ENTRY(objects->entry_start_time), "");
    gtk_entry_set_text(GTK_ENTRY(objects->entry_end_time), "");
    gtk_entry_set_text(GTK_ENTRY(objects->entry_remote_callsign), "");
    gtk_entry_set_text(GTK_ENTRY(objects->entry_rst_sent), "");
    gtk_entry_set_text(GTK_ENTRY(objects->entry_rst_recv), "");

    // gtk_entry_set_text(GTK_ENTRY(objects->entry_notes), "");
}

void on_button_reset_clicked(GtkWidget *widget, gpointer data)
{
    reset(data);
}

void on_menu_open_click(gpointer data)
{

    ObjectList *objects = (ObjectList *)data;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          NULL,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          "Open",
                                          GTK_RESPONSE_ACCEPT,
                                          "Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        printf("%s\n", filename);

        rc = sqlite3_open(filename, &db);

        if( rc )
        {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            fprintf(stdout, "Opened database successfully\n");
        }
    }

    gtk_widget_destroy (dialog);
    objects->db = db;
}

void on_menu_new_click(gpointer data)
{
    ObjectList *objects = (ObjectList *)data;
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    dialog = gtk_file_chooser_dialog_new ("New File",
                                          NULL,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "Save",
                                          GTK_RESPONSE_ACCEPT,
                                          "Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        printf("%s\n", filename);

        rc = sqlite3_open(filename, &db);

        if( rc )
        {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        }
        else
        {
            fprintf(stdout, "Opened database successfully\n");
        }
    }

    gtk_widget_destroy (dialog);

    sql = "create table logs ( logid int auto_increment primary key," \
                             " local_callsign varchar(20)," \
                             " entry_date varchar(20)," \
                             " start_time varchar(20)," \
                             " end_time varchar(20), " \
                             " frequency varchar(20), " \
                             " mode varchar(20), " \
                             " remote_callsign varchar(20), " \
                             " power varchar(20), " \
                             " rst_sent varchar(20), " \
                             " rst_recv varchar(20))";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }


    objects->db = db;
}

void initialize_databse(char* filename, gpointer data)
{
    ObjectList *objects = (ObjectList *)data;
    printf("%s\n", filename);
}

void load_config(gpointer data)
{
    ObjectList *objects = (ObjectList *)data;
    FILE *fd;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *ptr;
    char *delim = " ";

    fd = fopen("default.cfg", "r");
    while ((nread = getline(&line, &len, fd)) != -1)
    {
        // ptr = strtok(line, delim);
        // while (ptr != NULL)
        // {
            // ptr = strtok(NULL, delim);
        // }
        // printf(line);
    }
    free(line);
    fclose(fd);

}

void on_button_qrz_clicked(GtkWidget *widget, gpointer data)
{
    ObjectList *objects = (ObjectList *)data;
    char cmd[80];
    char callsign[20];

    strncpy(callsign, gtk_entry_get_text(GTK_ENTRY(objects->entry_remote_callsign)), 20);
    sprintf(cmd, "firefox https://www.qrz.com/db/%s", callsign);
    system(cmd);
}

void on_button_59_sent_clicked(GtkWidget *widget, gpointer data)
{
    ObjectList *objects = (ObjectList *)data;
    gtk_entry_set_text(GTK_ENTRY(objects->entry_rst_sent), (const gchar *)"59");
}

void on_button_59_recv_clicked(GtkWidget *widget, gpointer data)
{
    ObjectList *objects = (ObjectList *)data;
    gtk_entry_set_text(GTK_ENTRY(objects->entry_rst_recv), (const gchar *)"59");
}

void on_button_start_time_clicked(GtkWidget *widget, gpointer data)
{

    ObjectList *objects = (ObjectList *)data;
    char buff[80];
    time_t rawtime;
    struct tm *lt;

    time(&rawtime);
    lt = localtime(&rawtime);
    strftime(buff, 80, "%H:%M:%S", lt);

    gtk_entry_set_text(GTK_ENTRY(objects->entry_start_time), buff);

}

void on_button_end_time_clicked(GtkWidget *widget, gpointer data)
{

    ObjectList *objects = (ObjectList *)data;
    char buff[80];
    time_t rawtime;
    struct tm *lt;

    time(&rawtime);
    lt = localtime(&rawtime);
    strftime(buff, 80, "%H:%M:%S", lt);

    gtk_entry_set_text(GTK_ENTRY(objects->entry_end_time), buff);

}

void on_button_today_clicked(GtkWidget *widget, gpointer data)
{

    ObjectList *objects = (ObjectList *)data;
    char buff[80];
    time_t rawtime;
    struct tm *lt;

    time(&rawtime);
    lt = localtime(&rawtime);
    strftime(buff, 80, "%Y-%m-%d", lt);

    gtk_entry_set_text(GTK_ENTRY(objects->entry_date), buff);

}

void on_button_save_clicked(GtkWidget *widget, gpointer data)
{

    ObjectList *objects = (ObjectList *)data;

    // printf("entry_date %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_date)));
    // printf("entry_start_time %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_start_time)));
    // printf("entry_end_time %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_end_time)));
    // printf("entry_frequency %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_frequency)));
    // printf("entry_mode %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_mode)));
    // printf("entry_remote_callsign %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_remote_callsign)));
    // printf("entry_power %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_power)));
    // printf("entry_rst_sent %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_rst_sent)));
    // printf("entry_rst_recv %s\n", (char *)gtk_entry_get_text(GTK_ENTRY(objects->entry_rst_recv)));

    //sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    sqlite3 * db;

    db = objects->db;

    if( rc )
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(objects->db));
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */

    sprintf(sql, "insert into logs ( local_callsign, entry_date, start_time, " \
                            " end_time, frequency, mode, remote_callsign, " \
                            " power, rst_sent, rst_recv)" \
                            " values " \
                            "( '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )",
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_my_callsign)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_date)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_start_time)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_end_time)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_frequency)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_mode)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_remote_callsign)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_power)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_rst_sent)),
                             gtk_entry_get_text(GTK_ENTRY(objects->entry_rst_recv)));

    printf("%s\n", sql);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    printf("%d\n", rc);
    if( rc != SQLITE_OK )
    {
        printf("some kinda error...");
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);
        //sqlite3_free(zErrMsg);
    }

    printf("no kinda error...");

    reset(data);

}

void on_menu_item_quit_activate(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    ObjectList *object_list;
    object_list = malloc(sizeof(ObjectList));

    GtkBuilder *builder;
    GError *error = NULL;

    builder = gtk_builder_new();
    if (gtk_builder_add_from_file (builder, "logbook.glade", &error) == 0)
    {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }

    object_list->window = gtk_builder_get_object(builder, "window");
    g_signal_connect (object_list->window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    /* gtk_builder_connect_signals(builder, NULL); */

    object_list->menu_quit = gtk_builder_get_object(builder, "menu_item_quit");
    object_list->menu_new = gtk_builder_get_object(builder, "menu_item_new");
    object_list->menu_open = gtk_builder_get_object(builder, "menu_item_open");

    object_list->entry_my_callsign = gtk_builder_get_object(builder, "entry_my_callsign");
    object_list->entry_date = gtk_builder_get_object(builder, "entry_date");
    object_list->entry_start_time = gtk_builder_get_object(builder, "entry_start_time");
    object_list->entry_end_time = gtk_builder_get_object(builder, "entry_end_time");
    object_list->entry_frequency = gtk_builder_get_object(builder, "entry_frequency");
    object_list->entry_mode = gtk_builder_get_object(builder, "entry_mode");
    object_list->entry_remote_callsign = gtk_builder_get_object(builder, "entry_remote_callsign");
    object_list->entry_power = gtk_builder_get_object(builder, "entry_power");
    object_list->entry_rst_sent = gtk_builder_get_object(builder, "entry_rst_sent");
    object_list->entry_rst_recv = gtk_builder_get_object(builder, "entry_rst_recv");
    object_list->entry_notes = gtk_builder_get_object(builder, "entry_notes");

    object_list->button_today = gtk_builder_get_object(builder, "button_today");
    object_list->button_start_time = gtk_builder_get_object(builder, "button_start_time");
    object_list->button_end_time = gtk_builder_get_object(builder, "button_end_time");
    object_list->button_qrz = gtk_builder_get_object(builder, "button_qrz");
    object_list->button_59_sent = gtk_builder_get_object(builder, "button_59_sent");
    object_list->button_59_recv = gtk_builder_get_object(builder, "button_59_recv");
    object_list->button_save = gtk_builder_get_object(builder, "button_save");
    object_list->button_reset = gtk_builder_get_object(builder, "button_reset");

    g_signal_connect (object_list->menu_new, "activate", G_CALLBACK (on_menu_new_click), object_list);
    g_signal_connect (object_list->menu_open, "activate", G_CALLBACK (on_menu_open_click), object_list);
    g_signal_connect (object_list->menu_quit, "activate", G_CALLBACK (on_menu_item_quit_activate), object_list);

    g_signal_connect (object_list->button_today, "clicked", G_CALLBACK (on_button_today_clicked), object_list);
    g_signal_connect (object_list->button_start_time, "clicked", G_CALLBACK (on_button_start_time_clicked), object_list);
    g_signal_connect (object_list->button_end_time, "clicked", G_CALLBACK (on_button_end_time_clicked), object_list);
    g_signal_connect (object_list->button_59_sent, "clicked", G_CALLBACK (on_button_59_sent_clicked), object_list);
    g_signal_connect (object_list->button_59_recv, "clicked", G_CALLBACK (on_button_59_recv_clicked), object_list);
    g_signal_connect (object_list->button_qrz, "clicked", G_CALLBACK (on_button_qrz_clicked), object_list);
    g_signal_connect (object_list->button_save, "clicked", G_CALLBACK (on_button_save_clicked), object_list);
    g_signal_connect (object_list->button_reset, "clicked", G_CALLBACK (on_button_reset_clicked), object_list);

    load_config(object_list);

    gtk_widget_show(GTK_WIDGET(object_list->window));

    gtk_main();

    return 0;
}
