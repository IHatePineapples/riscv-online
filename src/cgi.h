#ifndef __CGI_H__
#define __CGI_H__

/* initialize the CGI handler */
void cgi_init();

const char *cgi_index_handler(int, int, char *[], char *[]);

#endif // __CGI_H__
