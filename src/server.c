#define _BSD_SOURCE

#include <lacewing.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>

void on_get (lw_ws webserver, lw_ws_req req)
{
	lw_stream_writef (req, "Hello world from %s", lw_version ());
}

lw_ws webserver;
lw_pump pump;

void server_clean() {
	lw_ws_delete (webserver);
	lw_pump_delete (pump);
}

int main (int argc, char * argv[])
{
	pump = lw_eventpump_new ();
	webserver = lw_ws_new (pump);

	lw_ws_on_get (webserver, on_get);
	lw_ws_host (webserver, 8081);

	printf("starting server");
	lw_eventpump_start_eventloop (pump);

	printf("cleaning up...\n");
	server_clean();
	printf("server killed\n");
	fflush(stdout);

	return 0;
}
