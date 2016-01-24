#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <syslog.h>
#include "mongoose.h"

using namespace std;

bool daemon_running = false;
static const char *daemon_bindport = "1234";
struct mg_mgr mgr;
struct mg_connection *nc;
static pid_t process_id, session_id;

static void daemon_handler(struct mg_connection *nc, int ev, void *p) {
    if (ev == MG_EV_HTTP_REQUEST) {
        mg_printf(nc, "HTTP/1.0 200 OK\r\n"
                "Content-Length: 2\r\n"
                "Content-Type: text/html\r\n\r\nCheck the docs!");
    }

}

static void print_menu(bool status) {
    printf("Select an option from below:\n");
    if (status) {
        printf("\t[1] Start emulator\n");
        printf("\t[2] Install emulator (NOT IMPLEMENTED)\n");
    } else {
        printf("\t[1] Stop emulator\n");
        printf("\t[2] Uninstall emulator (NOT IMPLEMENTED)\n");
    }
    printf("\t[3] Exit emulator\n");
}

static void daemon_manager(bool status) {
    static bool daemon_loop = true;
    if (status) {
        daemon_running = true;
        process_id = fork();
        if (process_id < 0) {
            syslog(LOG_CRIT, "Failed to create child process");
            exit(EXIT_FAILURE);
        }
        if (process_id > 0) {
            exit(EXIT_SUCCESS);
        }
        session_id = setsid();
        if (session_id < 0) {
            syslog(LOG_CRIT, "Failed to create Session ID");
            exit(EXIT_FAILURE);
        }
        mg_mgr_init(&mgr, NULL);
        nc = mg_bind(&mgr, daemon_bindport, daemon_handler);
        syslog(LOG_INFO, "Daemon has been bound to port %s", daemon_bindport);
        while (daemon_loop) {
            mg_mgr_poll(&mgr, 1000);
        }
        exit(EXIT_SUCCESS);
    } else {
        daemon_running = false;
    }
}

int main() {
    bool bLoop = true;
    string string_input;
    openlog("jetservd", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    do {
        print_menu(daemon_running);
        printf("Selection: ");
        getline(cin, string_input);
        switch (atoi(string_input.c_str())) {
            case 1:
                daemon_manager(daemon_running);
                break;
            case 2:
            case 3:
                bLoop = false;
                break;
            default:
                printf("\nInvalid input\n");
        }
    } while (bLoop);
    printf("\nShutting down emulator");
    mg_mgr_free(&mgr);
    return (EXIT_SUCCESS);
}