#include "cgi.h"
#include "lwip/apps/httpd.h"
#include "lwipopts.h"
#include "pico/cyw43_arch.h"

/* cgi-handler triggered by a request for "/leds.cgi" */
static const tCGI cgi_handlers[] = {
    /* Html request for "/leds.cgi" will start cgi_index_handler */
    {"/", cgi_index_handler},
};

const char *cgi_index_handler(int, int, char *[], char *[]) {
  /* Our response to the "SUBMIT" is to simply send the same page again*/
  return "/index.html";
}

/* initialize the CGI handler */
void cgi_init() { http_set_cgi_handlers(cgi_handlers, 1); }
