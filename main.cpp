#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <syslog.h>
#include "mongoose.h"

#define DAEMON_NAME "jetservd"
#define DAEMON_PORT 15243

struct mg_mgr mgr;
struct mg_connection *nc;
static int s_sig_num = 0;

using namespace std;


static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    string s_method_post = "POST";
    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->method, s_method_post.c_str())) {
                mg_printf(nc, "HTTP/1.0 200 OK\r\n"
                        "Content-Length: 2\r\n"
                        "Content-Type: text/html\r\n\r\nNo");
            }

            break;
        default:
            break;
    }
}


void jetservdaemon() {
    syslog(LOG_NOTICE, "jetservd successfully started");

    /* Open listening socket */
    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, (const char *) DAEMON_PORT, ev_handler);
    mg_set_protocol_http_websocket(nc);

    /* Run event loop until signal is received */
    while (s_sig_num == 0) {
        mg_mgr_poll(&mgr, 1000);
    }

    /* Cleanup */
    mg_mgr_free(&mgr);
}

int main(int argc, char *argv[]) {

    //Set our Logging Mask and open the Log
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Starting jetservd");

    pid_t pid, sid;

    //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    jetservdaemon();

    //Close the log
    closelog();
}