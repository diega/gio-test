#include <glib.h>
#include <gio/gio.h>

int
main (int argc, char *argv[])
{
  GError* error = NULL;

  // create a new connection
  GSocketClient* client = g_socket_client_new();
  g_socket_client_set_tls(client, TRUE);
  g_socket_client_set_tls_validation_flags(client, G_TLS_CERTIFICATE_GENERIC_ERROR);

  // connect to the host
  GSocketConnection* connection = g_socket_client_connect_to_host (client,
                                                                   (gchar*)"192.168.0.27",
                                                                   8009,
                                                                   NULL,
                                                                   &error);

  if (error != NULL)
  {
      g_error ("%s", error->message);
  }
  else
  {
      g_print ("Connection successful!\n");
  }

  /* use the connection */
  GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
  GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
  g_output_stream_write  (ostream,
                          "Hello server!", // your message
                          13, // length of your message
                          NULL,
                          &error);

  if (error != NULL)
  {
      g_error ("%s", error->message);
  }
  return 0;
}


