#include <iostream>
#include "mongoose.h"

using namespace std;

bool daemon_running = false;
static const char *daemon_bindport = "1234";
struct mg_mgr mgr;
struct mg_connection *nc;

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

static bool daemon_manager(bool status) {
    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, daemon_bindport, daemon_handler);
    printf("Starting web server on port %s\n", daemon_bindport);
    if (status) {
        daemon_running = true;
        mg_mgr_poll(&mgr, 1000);
        return true;
    } else {
        daemon_running = false;
        return false;
    }
}

int main() {
    bool bLoop = true;
    string string_input;
    do {
        print_menu(daemon_running);
        printf("Selection: ");
        getline(cin, string_input);
        switch (atoi(string_input.c_str())) {
            case 1:
                daemon_manager(daemon_running);
                break;
            case 2:
                bLoop = false;
                break;
            default:
                printf("\nInvalid input\n");
        }
    } while (bLoop);
    printf("\nShutting down emulator");
    mg_mgr_free(&mgr);
    return 0;
}