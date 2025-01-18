#include <gtk/gtk.h>
#include <vte/vte.h>
#include <stdlib.h>
#include <string.h>

// Config: Hardcoded values
const char *font = "Comic Code 12"; // Default font
double transparency = 0.8;          // Default transparency (80%)
char *shell = "/bin/fish";          // Default shell

// Define the on_child_exited callback function
static void on_child_exited(VteTerminal *terminal, gint status, gpointer user_data) {
  // No action needed here
}

// Clipboard signal handler (Paste)
static void on_paste_clipboard(GtkWidget *widget, gpointer data) {
  VteTerminal *terminal = VTE_TERMINAL(widget);
  GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gchar *text = gtk_clipboard_wait_for_text(clipboard);
  if (text) {
	vte_terminal_feed(VTE_TERMINAL(terminal), text, strlen(text)); // Paste the text
	g_free(text); // Free the clipboard text
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *terminal;
  char *command = shell; // Default shell

  // Check if an argument was provided for the -e option (command to execute)
  int argc = user_data ? 1 : 0; // User data will be passed as an argument
  if (argc > 0) {
	command = (char *)user_data; // Use the provided command
  }

  // Create a new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "MTerm");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

  // Enable transparency for the window using Gdk
  GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
  if (visual != NULL && gdk_screen_is_composited(screen)) {
	gtk_widget_set_visual(window, visual); // Enable transparency
	gtk_widget_set_app_paintable(window, TRUE);
  }

  // Create a new VTE terminal widget
  terminal = vte_terminal_new();

  // Get the home directory (no arguments required here)
  const char *home_dir = g_get_home_dir();

  // Fork a new shell inside the terminal with the specified working directory
  const char *argv[] = {command, NULL};
  vte_terminal_spawn_async(
	VTE_TERMINAL(terminal),    // terminal widget
						   VTE_PTY_DEFAULT,           // default pseudo terminal
						   home_dir,                  // set working directory to home directory
						   (char **)argv,             // command (e.g., "/bin/fish")
  NULL,                      // environment
  0,                         // spawn flags
  NULL, NULL,                // child setup
  NULL,                      // child setup data
  -1,                        // timeout
  NULL,                      // cancellable
  NULL, NULL                 // user data
  );

  // Set the terminal font
  PangoFontDescription *font_desc;
  font_desc = pango_font_description_from_string(font);
  vte_terminal_set_font(VTE_TERMINAL(terminal), font_desc);
  pango_font_description_free(font_desc);

  // Set the terminal background color with transparency
  GdkRGBA color;
  char *color_str = g_strdup_printf("rgba(0, 0, 0, %.1f)", transparency); // Create dynamic color string
  gdk_rgba_parse(&color, color_str); // Parse the color string
  vte_terminal_set_color_background(VTE_TERMINAL(terminal), &color);
  g_free(color_str); // Free the dynamically allocated color string

  // Add the terminal to the window
  gtk_container_add(GTK_CONTAINER(window), terminal);

  g_signal_connect(terminal, "paste-clipboard", G_CALLBACK(on_paste_clipboard), NULL);

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
  g_signal_connect(app, "activate", G_CALLBACK(activate), argc > 2 ? argv[2] : NULL);

  // Run the GTK application
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
