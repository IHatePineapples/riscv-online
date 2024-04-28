#include "lwip/opt.h"

#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"

#include <cstdio>
#include <cstring>

#include "RoL/threaded/shared_queue.hpp"
#include "RoL/emulation/emulator.hpp"

#define USER_PASS_BUFSIZE 16

static void *current_connection;
static void *valid_connection;

static bool submit_job = false;
static bool return_job = false;

err_t httpd_post_begin(void *connection, const char *uri, const char *http_request,
                       u16_t http_request_len, int content_len, char *response_uri,
                       u16_t response_uri_len, u8_t *post_auto_wnd)
{
    if (!memcmp(uri, "/submit", 8))
    {
        if (current_connection == connection)
            return ERR_VAL;
        current_connection = connection;
        /* default page is "login failed" */
        snprintf(response_uri, response_uri_len, "/submit.json");
        /* e.g. for large uploads to slow flash over a fast connection, you should
           manually update the rx window. That way, a sender can only send a full
           tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
           We do not need to throttle upload speed here, so: */
        *post_auto_wnd = 1;
        submit_job = true;
        return ERR_OK;
    }
    else if (!memcmp(uri, "/return", 8))
    {

        if (current_connection == connection)
            return ERR_VAL;
        current_connection = connection;
        /* default page is "login failed" */
        snprintf(response_uri, response_uri_len, "/return.json");
        /* e.g. for large uploads to slow flash over a fast connection, you should
           manually update the rx window. That way, a sender can only send a full
           tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
           We do not need to throttle upload speed here, so: */
        *post_auto_wnd = 1;
        return ERR_OK;
        return_job = true;
    }

    return ERR_VAL;
}

err_t httpd_post_receive_data(void *connection, struct pbuf *p)
{
    if (current_connection != connection)
        return ERR_VAL;

    // Locate tokens
    if (submit_job)
    {
        u16_t token_user = pbuf_memfind(p, "user=", 5, 0);
        u16_t token_pass = pbuf_memfind(p, "pass=", 5, 0);

        u16_t token_id = pbuf_memfind(p, "id=", 3, 0);
        if ( token_id   == 0xFFFF) return ERR_VAL;
        u16_t token_ram  = pbuf_memfind(p, "ram=", 4, 0);
        if ( token_ram  == 0xFFFF) return ERR_VAL;
        u16_t token_ra   = pbuf_memfind(p, "ra=", 3, 0);
        if ( token_ra   == 0xFFFF) return ERR_VAL;
        u16_t token_sp   = pbuf_memfind(p, "sp=", 3, 0);
        if ( token_sp   == 0xFFFF) return ERR_VAL;
        u16_t token_gp   = pbuf_memfind(p, "gp=", 3, 0);
        if ( token_gp   == 0xFFFF) return ERR_VAL;
        u16_t token_tp   = pbuf_memfind(p, "tp=", 3, 0);
        if ( token_tp   == 0xFFFF) return ERR_VAL;
        u16_t token_t0   = pbuf_memfind(p, "t0=", 3, 0);
        if ( token_t0   == 0xFFFF) return ERR_VAL;
        u16_t token_t1   = pbuf_memfind(p, "t1=", 3, 0);
        if ( token_t1   == 0xFFFF) return ERR_VAL;
        u16_t token_t2   = pbuf_memfind(p, "t2=", 3, 0);
        if ( token_t2   == 0xFFFF) return ERR_VAL;
        u16_t token_fp   = pbuf_memfind(p, "fp=", 3, 0);
        if ( token_fp   == 0xFFFF) return ERR_VAL;
        u16_t token_s1   = pbuf_memfind(p, "s1=", 3, 0);
        if ( token_s1   == 0xFFFF) return ERR_VAL;
        u16_t token_a0   = pbuf_memfind(p, "a0=", 3, 0);
        if ( token_a0   == 0xFFFF) return ERR_VAL;
        u16_t token_a1   = pbuf_memfind(p, "a1=", 3, 0);
        if ( token_a1   == 0xFFFF) return ERR_VAL;
        u16_t token_a2   = pbuf_memfind(p, "a2=", 3, 0);
        if ( token_a2   == 0xFFFF) return ERR_VAL;
        u16_t token_a3   = pbuf_memfind(p, "a3=", 3, 0);
        if ( token_a3   == 0xFFFF) return ERR_VAL;
        u16_t token_a4   = pbuf_memfind(p, "a4=", 3, 0);
        if ( token_a4   == 0xFFFF) return ERR_VAL;
        u16_t token_a5   = pbuf_memfind(p, "a5=", 3, 0);
        if ( token_a5   == 0xFFFF) return ERR_VAL;
        u16_t token_a6   = pbuf_memfind(p, "a6=", 3, 0);
        if ( token_a6   == 0xFFFF) return ERR_VAL;
        u16_t token_a7   = pbuf_memfind(p, "a7=", 3, 0);
        if ( token_a7   == 0xFFFF) return ERR_VAL;
        u16_t token_s2   = pbuf_memfind(p, "s2=", 3, 0);
        if ( token_s2   == 0xFFFF) return ERR_VAL;
        u16_t token_s3   = pbuf_memfind(p, "s3=", 3, 0);
        if ( token_s3   == 0xFFFF) return ERR_VAL;
        u16_t token_s4   = pbuf_memfind(p, "s4=", 3, 0);
        if ( token_s4   == 0xFFFF) return ERR_VAL;
        u16_t token_s5   = pbuf_memfind(p, "s5=", 3, 0);
        if ( token_s5   == 0xFFFF) return ERR_VAL;
        u16_t token_s6   = pbuf_memfind(p, "s6=", 3, 0);
        if ( token_s6   == 0xFFFF) return ERR_VAL;
        u16_t token_s7   = pbuf_memfind(p, "s7=", 3, 0);
        if ( token_s7   == 0xFFFF) return ERR_VAL;
        u16_t token_s8   = pbuf_memfind(p, "s8=", 3, 0);
        if ( token_s8   == 0xFFFF) return ERR_VAL;
        u16_t token_s9   = pbuf_memfind(p, "s9=", 3, 0);
        if ( token_s9   == 0xFFFF) return ERR_VAL;
        u16_t token_s10  = pbuf_memfind(p, "s10=", 4, 0);
        if ( token_s10  == 0xFFFF) return ERR_VAL;
        u16_t token_s11  = pbuf_memfind(p, "s11=", 4, 0);
        if ( token_s11  == 0xFFFF) return ERR_VAL;
        u16_t token_t3   = pbuf_memfind(p, "t3=", 3, 0);
        if ( token_t3   == 0xFFFF) return ERR_VAL;
        u16_t token_t4   = pbuf_memfind(p, "t4=", 3, 0);
        if ( token_t4   == 0xFFFF) return ERR_VAL;
        u16_t token_t5   = pbuf_memfind(p, "t5=", 3, 0);
        if ( token_t5   == 0xFFFF) return ERR_VAL;
        u16_t token_t6   = pbuf_memfind(p, "t6=", 3, 0);
        if ( token_t6   == 0xFFFF) return ERR_VAL;
        u16_t token_pc   = pbuf_memfind(p, "pc=", 3, 0);
        if ( token_pc   == 0xFFFF) return ERR_VAL;
        u16_t token_prog = pbuf_memfind(p, "prog=", 5, 0);
        if ( token_prog == 0xFFFF) return ERR_VAL;


        u16_t value_id   = token_id + 3;
        u16_t value_ram  = token_ram + 4;
        u16_t value_ra   = token_ra + 3;
        u16_t value_sp   = token_sp + 3;
        u16_t value_gp   = token_gp + 3;
        u16_t value_tp   = token_tp + 3;
        u16_t value_t0   = token_t0 + 3;
        u16_t value_t1   = token_t1 + 3;
        u16_t value_t2   = token_t2 + 3;
        u16_t value_fp   = token_fp + 3;
        u16_t value_s1   = token_s1 + 3;
        u16_t value_a0   = token_a0 + 3;
        u16_t value_a1   = token_a1 + 3;
        u16_t value_a2   = token_a2 + 3;
        u16_t value_a3   = token_a3 + 3;
        u16_t value_a4   = token_a4 + 3;
        u16_t value_a5   = token_a5 + 3;
        u16_t value_a6   = token_a6 + 3;
        u16_t value_a7   = token_a7 + 3;
        u16_t value_s2   = token_s2 + 3;
        u16_t value_s3   = token_s3 + 3;
        u16_t value_s4   = token_s4 + 3;
        u16_t value_s5   = token_s5 + 3;
        u16_t value_s6   = token_s6 + 3;
        u16_t value_s7   = token_s7 + 3;
        u16_t value_s8   = token_s8 + 3;
        u16_t value_s9   = token_s9 + 3;
        u16_t value_s10  = token_s10 + 4;
        u16_t value_s11  = token_s11 + 4;
        u16_t value_t3   = token_t3 + 3;
        u16_t value_t4   = token_t4 + 3;
        u16_t value_t5   = token_t5 + 3;
        u16_t value_t6   = token_t6 + 3;
        u16_t value_pc   = token_pc + 3;
        u16_t value_prog = token_prog + 5;

        u16_t len_id   = 0;
        u16_t len_ram  = 0;
        u16_t len_ra   = 0;
        u16_t len_sp   = 0;
        u16_t len_gp   = 0;
        u16_t len_tp   = 0;
        u16_t len_t0   = 0;
        u16_t len_t1   = 0;
        u16_t len_t2   = 0;
        u16_t len_fp   = 0;
        u16_t len_s1   = 0;
        u16_t len_a0   = 0;
        u16_t len_a1   = 0;
        u16_t len_a2   = 0;
        u16_t len_a3   = 0;
        u16_t len_a4   = 0;
        u16_t len_a5   = 0;
        u16_t len_a6   = 0;
        u16_t len_a7   = 0;
        u16_t len_s2   = 0;
        u16_t len_s3   = 0;
        u16_t len_s4   = 0;
        u16_t len_s5   = 0;
        u16_t len_s6   = 0;
        u16_t len_s7   = 0;
        u16_t len_s8   = 0;
        u16_t len_s9   = 0;
        u16_t len_s10  = 0;
        u16_t len_s11  = 0;
        u16_t len_t3   = 0;
        u16_t len_t4   = 0;
        u16_t len_t5   = 0;
        u16_t len_t6   = 0;
        u16_t len_pc   = 0;
        u16_t len_prog = 0;


        u16_t tmp;
        
        tmp = pbuf_memfind(p, "&", 1, value_id       );
        len_id       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_id        ; 
        tmp = pbuf_memfind(p, "&", 1, value_ram      );
        len_ram      = (tmp != 0xFFFF ? tmp : p->tot_len) - value_ram       ; 
        tmp = pbuf_memfind(p, "&", 1, value_ra       );
        len_ra       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_ra        ; 
        tmp = pbuf_memfind(p, "&", 1, value_sp       );
        len_sp       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_sp        ; 
        tmp = pbuf_memfind(p, "&", 1, value_gp       );
        len_gp       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_gp        ; 
        tmp = pbuf_memfind(p, "&", 1, value_tp       );
        len_tp       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_tp        ; 
        tmp = pbuf_memfind(p, "&", 1, value_t0       );
        len_t0       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_t0        ; 
        tmp = pbuf_memfind(p, "&", 1, value_t1       );
        len_t1       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_t1        ; 
        tmp = pbuf_memfind(p, "&", 1, value_t2       );
        len_t2       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_t2        ; 
        tmp = pbuf_memfind(p, "&", 1, value_fp       );
        len_fp       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_fp        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s1       );
        len_s1       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s1        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a0       );
        len_a0       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a0        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a1       );
        len_a1       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a1        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a2       );
        len_a2       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a2        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a3       );
        len_a3       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a3        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a4       );
        len_a4       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a4        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a5       );
        len_a5       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a5        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a6       );
        len_a6       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a6        ; 
        tmp = pbuf_memfind(p, "&", 1, value_a7       );
        len_a7       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_a7        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s2       );
        len_s2       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s2        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s3       );
        len_s3       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s3        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s4       );
        len_s4       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s4        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s5       );
        len_s5       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s5        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s6       );
        len_s6       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s6        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s7       );
        len_s7       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s7        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s8       );
        len_s8       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s8        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s9       );
        len_s9       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s9        ; 
        tmp = pbuf_memfind(p, "&", 1, value_s10      );
        len_s10      = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s10       ; 
        tmp = pbuf_memfind(p, "&", 1, value_s11      );
        len_s11      = (tmp != 0xFFFF ? tmp : p->tot_len) - value_s11       ; 
        tmp = pbuf_memfind(p, "&", 1, value_t3       );
        len_t3       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_t3        ; 
        tmp = pbuf_memfind(p, "&", 1, value_t4       );
        len_t4       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_t4        ; 
        tmp = pbuf_memfind(p, "&", 1, value_t5       );
        len_t5       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_t5        ; 
        tmp = pbuf_memfind(p, "&", 1, value_t6       );
        len_t6       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_t6        ; 
        tmp = pbuf_memfind(p, "&", 1, value_pc       );
        len_pc       = (tmp != 0xFFFF ? tmp : p->tot_len) - value_pc        ; 
        tmp = pbuf_memfind(p, "&", 1, value_prog     );
        len_prog     = (tmp != 0xFFFF ? tmp : p->tot_len) - value_prog      ; 

        char id_buf  [4] = {} ;
        char ram_buf [100 * (4 * 2 + 2)] = {}; 
        char ra_buf  [4 * 2 + 2 +1] = {}; 
        char sp_buf  [4 * 2 + 2 +1] = {}; 
        char gp_buf  [4 * 2 + 2 +1] = {}; 
        char tp_buf  [4 * 2 + 2 +1] = {}; 
        char t0_buf  [4 * 2 + 2 +1] = {}; 
        char t1_buf  [4 * 2 + 2 +1] = {}; 
        char t2_buf  [4 * 2 + 2 +1] = {}; 
        char fp_buf  [4 * 2 + 2 +1] = {}; 
        char s1_buf  [4 * 2 + 2 +1] = {}; 
        char a0_buf  [4 * 2 + 2 +1] = {}; 
        char a1_buf  [4 * 2 + 2 +1] = {}; 
        char a2_buf  [4 * 2 + 2 +1] = {}; 
        char a3_buf  [4 * 2 + 2 +1] = {}; 
        char a4_buf  [4 * 2 + 2 +1] = {}; 
        char a5_buf  [4 * 2 + 2 +1] = {}; 
        char a6_buf  [4 * 2 + 2 +1] = {}; 
        char a7_buf  [4 * 2 + 2 +1] = {}; 
        char s2_buf  [4 * 2 + 2 +1] = {}; 
        char s3_buf  [4 * 2 + 2 +1] = {}; 
        char s4_buf  [4 * 2 + 2 +1] = {}; 
        char s5_buf  [4 * 2 + 2 +1] = {}; 
        char s6_buf  [4 * 2 + 2 +1] = {}; 
        char s7_buf  [4 * 2 + 2 +1] = {}; 
        char s8_buf  [4 * 2 + 2 +1] = {}; 
        char s9_buf  [4 * 2 + 2 +1] = {}; 
        char s10_buf [4 * 2 + 2 +1] = {}; 
        char s11_buf [4 * 2 + 2 +1] = {}; 
        char t3_buf  [4 * 2 + 2 +1] = {}; 
        char t4_buf  [4 * 2 + 2 +1] = {}; 
        char t5_buf  [4 * 2 + 2 +1] = {}; 
        char t6_buf  [4 * 2 + 2 +1] = {}; 
        char pc_buf  [4 * 2 + 2 +1] = {}; 
        char prog_buf[100 * (4 * 2 + 2)]    = {};


        pbuf_get_contiguous(p, id_buf  , sizeof(id_buf    )  ,len_id  ,   value_id      );
        pbuf_get_contiguous(p, ram_buf , sizeof(ram_buf   )  ,len_ram ,   value_ram     );
        pbuf_get_contiguous(p, ra_buf  , sizeof(ra_buf    )  ,len_ra  ,   value_ra      );
        pbuf_get_contiguous(p, sp_buf  , sizeof(sp_buf    )  ,len_sp  ,   value_sp      );
        pbuf_get_contiguous(p, gp_buf  , sizeof(gp_buf    )  ,len_gp  ,   value_gp      );
        pbuf_get_contiguous(p, tp_buf  , sizeof(tp_buf    )  ,len_tp  ,   value_tp      );
        pbuf_get_contiguous(p, t0_buf  , sizeof(t0_buf    )  ,len_t0  ,   value_t0      );
        pbuf_get_contiguous(p, t1_buf  , sizeof(t1_buf    )  ,len_t1  ,   value_t1      );
        pbuf_get_contiguous(p, t2_buf  , sizeof(t2_buf    )  ,len_t2  ,   value_t2      );
        pbuf_get_contiguous(p, fp_buf  , sizeof(fp_buf    )  ,len_fp  ,   value_fp      );
        pbuf_get_contiguous(p, s1_buf  , sizeof(s1_buf    )  ,len_s1  ,   value_s1      );
        pbuf_get_contiguous(p, a0_buf  , sizeof(a0_buf    )  ,len_a0  ,   value_a0      );
        pbuf_get_contiguous(p, a1_buf  , sizeof(a1_buf    )  ,len_a1  ,   value_a1      );
        pbuf_get_contiguous(p, a2_buf  , sizeof(a2_buf    )  ,len_a2  ,   value_a2      );
        pbuf_get_contiguous(p, a3_buf  , sizeof(a3_buf    )  ,len_a3  ,   value_a3      );
        pbuf_get_contiguous(p, a4_buf  , sizeof(a4_buf    )  ,len_a4  ,   value_a4      );
        pbuf_get_contiguous(p, a5_buf  , sizeof(a5_buf    )  ,len_a5  ,   value_a5      );
        pbuf_get_contiguous(p, a6_buf  , sizeof(a6_buf    )  ,len_a6  ,   value_a6      );
        pbuf_get_contiguous(p, a7_buf  , sizeof(a7_buf    )  ,len_a7  ,   value_a7      );
        pbuf_get_contiguous(p, s2_buf  , sizeof(s2_buf    )  ,len_s2  ,   value_s2      );
        pbuf_get_contiguous(p, s3_buf  , sizeof(s3_buf    )  ,len_s3  ,   value_s3      );
        pbuf_get_contiguous(p, s4_buf  , sizeof(s4_buf    )  ,len_s4  ,   value_s4      );
        pbuf_get_contiguous(p, s5_buf  , sizeof(s5_buf    )  ,len_s5  ,   value_s5      );
        pbuf_get_contiguous(p, s6_buf  , sizeof(s6_buf    )  ,len_s6  ,   value_s6      );
        pbuf_get_contiguous(p, s7_buf  , sizeof(s7_buf    )  ,len_s7  ,   value_s7      );
        pbuf_get_contiguous(p, s8_buf  , sizeof(s8_buf    )  ,len_s8  ,   value_s8      );
        pbuf_get_contiguous(p, s9_buf  , sizeof(s9_buf    )  ,len_s9  ,   value_s9      );
        pbuf_get_contiguous(p, s10_buf , sizeof(s10_buf   )  ,len_s10 ,   value_s10     );
        pbuf_get_contiguous(p, s11_buf , sizeof(s11_buf   )  ,len_s11 ,   value_s11     );
        pbuf_get_contiguous(p, t3_buf  , sizeof(t3_buf    )  ,len_t3  ,   value_t3      );
        pbuf_get_contiguous(p, t4_buf  , sizeof(t4_buf    )  ,len_t4  ,   value_t4      );
        pbuf_get_contiguous(p, t5_buf  , sizeof(t5_buf    )  ,len_t5  ,   value_t5      );
        pbuf_get_contiguous(p, t6_buf  , sizeof(t6_buf    )  ,len_t6  ,   value_t6      );
        pbuf_get_contiguous(p, pc_buf  , sizeof(pc_buf    )  ,len_pc  ,   value_pc      );
        pbuf_get_contiguous(p, prog_buf, sizeof(prog_buf  )  ,len_prog,   value_prog    );

        char* endptr = "";

        u16_t value_user = token_user + 5;
        long  id   = strtol(   id_buf       , &endptr, 16);
        /** \todo Implement ram */
        // long  ram  = strtol(   ram_buf      , &endptr, 16); 
        long  ra   = strtol(   ra_buf       , &endptr, 16);
        long  sp   = strtol(   sp_buf       , &endptr, 16);
        long  gp   = strtol(   gp_buf       , &endptr, 16);
        long  tp   = strtol(   tp_buf       , &endptr, 16);
        long  t0   = strtol(   t0_buf       , &endptr, 16);
        long  t1   = strtol(   t1_buf       , &endptr, 16);
        long  t2   = strtol(   t2_buf       , &endptr, 16);
        long  fp   = strtol(   fp_buf       , &endptr, 16);
        long  s1   = strtol(   s1_buf       , &endptr, 16);
        long  a0   = strtol(   a0_buf       , &endptr, 16);
        long  a1   = strtol(   a1_buf       , &endptr, 16);
        long  a2   = strtol(   a2_buf       , &endptr, 16);
        long  a3   = strtol(   a3_buf       , &endptr, 16);
        long  a4   = strtol(   a4_buf       , &endptr, 16);
        long  a5   = strtol(   a5_buf       , &endptr, 16);
        long  a6   = strtol(   a6_buf       , &endptr, 16);
        long  a7   = strtol(   a7_buf       , &endptr, 16);
        long  s2   = strtol(   s2_buf       , &endptr, 16);
        long  s3   = strtol(   s3_buf       , &endptr, 16);
        long  s4   = strtol(   s4_buf       , &endptr, 16);
        long  s5   = strtol(   s5_buf       , &endptr, 16);
        long  s6   = strtol(   s6_buf       , &endptr, 16);
        long  s7   = strtol(   s7_buf       , &endptr, 16);
        long  s8   = strtol(   s8_buf       , &endptr, 16);
        long  s9   = strtol(   s9_buf       , &endptr, 16);
        long  s10  = strtol(   s10_buf      , &endptr, 16);
        long  s11  = strtol(   s11_buf      , &endptr, 16);
        long  t3   = strtol(   t3_buf       , &endptr, 16);
        long  t4   = strtol(   t4_buf       , &endptr, 16);
        long  t5   = strtol(   t5_buf       , &endptr, 16);
        long  t6   = strtol(   t6_buf       , &endptr, 16);
        long  pc   = strtol(   pc_buf       , &endptr, 16);


        /** \todo Parse program. */
        emulation::emulator state{};
        state.ra     =  ra   ;
        state.sp     =  sp   ;
        state.gp     =  gp   ;
        state.tp     =  tp   ;
        state.t0     =  t0   ;
        state.t1     =  t1   ;
        state.t2     =  t2   ;
        state.fp     =  fp   ;
        state.s1     =  s1   ;
        state.a0     =  a0   ;
        state.a1     =  a1   ;
        state.a2     =  a2   ;
        state.a3     =  a3   ;
        state.a4     =  a4   ;
        state.a5     =  a5   ;
        state.a6     =  a6   ;
        state.a7     =  a7   ;
        state.s2     =  s2   ;
        state.s3     =  s3   ;
        state.s4     =  s4   ;
        state.s5     =  s5   ;
        state.s6     =  s6   ;
        state.s7     =  s7   ;
        state.s8     =  s8   ;
        state.s9     =  s9   ;
        state.s10    =  s10  ;
        state.s11    =  s11  ;
        state.t3     =  t3   ;
        state.t4     =  t4   ;
        state.t5     =  t5   ;
        state.t6     =  t6   ; 
        state.pc     =  pc   ;

        in_jq_.push({id, state});
    }
    /** \todo Add retrieve functionality */
    /* not returning ERR_OK aborts the connection, so return ERR_OK unless the
       conenction is unknown */
    return ERR_OK;
}

void httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len)
{
    /* default page is "login failed" */
    snprintf(response_uri, response_uri_len, "/loginfail.html");
    if (current_connection == connection)
    {
        if (valid_connection == connection)
        {
            /* login succeeded */
            snprintf(response_uri, response_uri_len, "/session.html");
        }
        current_connection = NULL;
        valid_connection = NULL;
    }
}
