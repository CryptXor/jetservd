#include <iostream>
#include <stdio.h>
#include "mongoose.h"
//#include "crypto\rsa.h"
using namespace std;

static const char *s_http_port = "12341";
struct mg_mgr mgr;
struct mg_connection *nc;
static int s_sig_num = 0;
static string new_line = "\n";
static string RSA_KEY = "-----BEGIN RSA PRIVATE KEY-----\n"
        "MIGaAgEAAkEAt5yrcHAAjhglnCEn6yecMWPeUXcMyo0+itXrLlkpcKIIyqPw546b\n"
        "GThhlb1ppX1ySX/OUA4jSakHekNP5eWPawIBAAJAW6/aVD05qbsZHMvZuS2Aa5Fp\n"
        "NNj0BDlf38hOtkhDzz/hkYb+EBYLLvldhgsD0OvRNy8yhz7EjaUqLCB0juIN4QIB\n"
        "AAIBAAIBAAIBAAIBAA==\n"
        "-----END RSA PRIVATE KEY-----";

char *calucate_char(char c) {
    return (char *) (c < 10 ? (char) 48 + c : (char) 97 + c - (char) '\n');
}

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