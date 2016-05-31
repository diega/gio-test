#include <glib.h>
#include <gio/gio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "cast_channel.pb-c.h"

void send_cast_message(char *namespace, char *message, GOutputStream *ostream);

gboolean send_ping (gpointer ostream);

int
main (int argc, char *argv[])
{
  GError* error = NULL;

  GSocketClient* client = g_socket_client_new();
  //g_socket_client_set_tls(client, TRUE);
  //g_socket_client_set_tls_validation_flags(client, G_TLS_CERTIFICATE_GENERIC_ERROR);

  GSocketConnection* connection = g_socket_client_connect_to_host (client, "localhost", 8009, NULL, &error);

  if (error != NULL)
  {
      g_error ("%s", error->message);
  }
  else
  {
      g_print ("Connection successful!\n");
  }

  GOutputStream *ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
//  GInputStream *istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));

  send_cast_message ("urn:x-cast:com.google.cast.tp.connection", "{\"type\":\"CONNECT\"}", ostream);
  g_timeout_add (500, send_ping, ostream);
  send_cast_message ("urn:x-cast:com.google.cast.receiver", "{\"type\":\"LAUNCH\",\"appId\":\"YouTube\",\"requestId\":1}", ostream);

  GMainLoop *loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);

  return 0;
}

void
send_cast_message(char *namespace, char *message, GOutputStream *ostream)
{
  Extensions__Api__CastChannel__CastMessage castMessage = EXTENSIONS__API__CAST_CHANNEL__CAST_MESSAGE__INIT;
  castMessage.protocol_version = 0;
  castMessage.source_id = "sender-0";
  castMessage.destination_id = "receiver-0";
  castMessage.namespace_ = namespace;
  castMessage.payload_type = 0;
  castMessage.payload_utf8 = message;

  GError* error = NULL;
  unsigned len = extensions__api__cast_channel__cast_message__get_packed_size(&castMessage);
  void *buf = malloc(len);
  extensions__api__cast_channel__cast_message__pack (&castMessage, buf);
  g_output_stream_write  (ostream, buf, len, NULL, &error);

  if (error != NULL)
  {
      g_error ("%s", error->message);
  }
}

gboolean
send_ping (gpointer ostream)
{
  send_cast_message("urn:x-cast:com.google.cast.tp.heartbeat", "{\"type\":\"PING\"}", ostream);
  return TRUE;
}

