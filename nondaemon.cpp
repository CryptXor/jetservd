#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <syslog.h>
#include "mongoose.h"

static const char *s_http_port = "12341";
struct mg_mgr mgr;
struct mg_connection *nc;
static int s_sig_num = 0;


using namespace std;


static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    switch (ev) {
        case MG_EV_HTTP_REQUEST:

            break;
        default:
            mg_printf(nc, "HTTP/1.0 200 OK\r\n"
                    "Content-Length: 2\r\n"
                    "Content-Type: text/html\r\n\r\nNo");
            break;
    }
}

int main(int argc, char *argv[]) {
    printf("Please do not abuse the system...\n");
    printf("Starting Jetbrains Server on port %s\n", s_http_port);

    /* Open listening socket */
    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    mg_set_protocol_http_websocket(nc);

    /* Run event loop until signal is received */
    while (s_sig_num == 0) {
        mg_mgr_poll(&mgr, 1000);
    }

    /* Cleanup */
    mg_mgr_free(&mgr);
    return (EXIT_SUCCESS);
}