#include <gtk/gtk.h>
#include <vte/vte.h>


// Config:
const char *font = "Comic Code 12"; // example "Monospace 12";
const char *shell = "/bin/fish"; // example "/bin/bash";



// Define the on_child_exited callback function
static void on_child_exited(VteTerminal *terminal, gint status, gpointer user_data) {
	GApplication *app = G_APPLICATION(user_data);
	g_application_quit(app); // Exit the GTK application when the terminal child process exits
}

static void activate(GtkApplication *app, gpointer user_data) {
	GtkWidget *window;
	GtkWidget *terminal;

	// Create a new window
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "MTerm");
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

	// Create a new VTE terminal widget
	terminal = vte_terminal_new();

	// Fork a new shell inside the terminal
	const char *argv[] = {shell, NULL};
	vte_terminal_spawn_async(
			VTE_TERMINAL(terminal),    // terminal widget
			VTE_PTY_DEFAULT,           // default pseudo terminal
			NULL,                      // working directory
			(char **)argv,             // command
			NULL,                      // environment
			0,                         // spawn flags
			NULL, NULL,                // child setup
			NULL,                      // child setup data
			-1,                        // timeout
			NULL,                      // cancellable
			NULL, NULL                 // user data
			);

	// Add the terminal to the window
	gtk_container_add(GTK_CONTAINER(window), terminal);

	// Set the terminal font
	PangoFontDescription *font_desc;
	font_desc = pango_font_description_from_string(font); // font
	vte_terminal_set_font(VTE_TERMINAL(terminal), font_desc);
	pango_font_description_free(font_desc);

	// Connect the child-exited signal to the callback
	g_signal_connect(terminal, "child-exited", G_CALLBACK(on_child_exited), app);

	// Show all widgets in the window
	gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;

	// Create a new GTK application
	app = gtk_application_new("com.mterm.terminal", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	// Run the GTK application
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}

