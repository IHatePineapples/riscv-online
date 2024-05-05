#include "ssi.h"

#include "lwip/apps/httpd.h"
#include "lwip/def.h"

#include "RoL/emulation/emulator.hpp"
#include "RoL/threaded/shared.hpp"

const char *ssi_tags[] = {
    /*  0 */ "stat",
    /*  1 */ "ram",
    /*  2 */ "ra",
    /*  3 */ "sp",
    /*  4 */ "gp",
    /*  5 */ "tp",
    /*  6 */ "t0",
    /*  7 */ "t1",
    /*  8 */ "t2",
    /*  9 */ "fp",
    /* 10 */ "s1",
    /* 11 */ "a0",
    /* 12 */ "a1",
    /* 13 */ "a2",
    /* 14 */ "a3",
    /* 15 */ "a4",
    /* 16 */ "a5",
    /* 17 */ "a6",
    /* 18 */ "a7",
    /* 19 */ "s2",
    /* 20 */ "s3",
    /* 21 */ "s4",
    /* 22 */ "s5",
    /* 23 */ "s6",
    /* 24 */ "s7",
    /* 25 */ "s8",
    /* 26 */ "s9",
    /* 27 */ "s10",
    /* 28 */ "s11",
    /* 29 */ "t3",
    /* 30 */ "t4",
    /* 31 */ "t5",
    /* 32 */ "t6",
    /* 33 */ "pc",
    /* 34 */ "prog",
};

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
    u16_t printed;
    const auto &[id, state] = current_job_;

    // If it's not ready, give up.
    if (!id) return 0;

    switch (iIndex)
    {
    case 0:
        printed = snprintf(pcInsert, iInsertLen, "%d", id);
        break;
    case 1:
        printed = snprintf(pcInsert, iInsertLen, state.serialize_ram().c_str());
        break;
    case 2:
        printed = snprintf(pcInsert, iInsertLen, state.ra.to_string().c_str());
        break;
    case 3:
        printed = snprintf(pcInsert, iInsertLen, state.sp.to_string().c_str());
        break;
    case 4:
        printed = snprintf(pcInsert, iInsertLen, state.gp.to_string().c_str());
        break;
    case 5:
        printed = snprintf(pcInsert, iInsertLen, state.tp.to_string().c_str());
        break;
    case 6:
        printed = snprintf(pcInsert, iInsertLen, state.t0.to_string().c_str());
        break;
    case 7:
        printed = snprintf(pcInsert, iInsertLen, state.t1.to_string().c_str());
        break;
    case 8:
        printed = snprintf(pcInsert, iInsertLen, state.t2.to_string().c_str());
        break;
    case 9:
        printed = snprintf(pcInsert, iInsertLen, state.fp.to_string().c_str());
        break;
    case 10:
        printed = snprintf(pcInsert, iInsertLen, state.s1.to_string().c_str());
        break;
    case 11:
        printed = snprintf(pcInsert, iInsertLen, state.a0.to_string().c_str());
        break;
    case 12:
        printed = snprintf(pcInsert, iInsertLen, state.a1.to_string().c_str());
        break;
    case 13:
        printed = snprintf(pcInsert, iInsertLen, state.a2.to_string().c_str());
        break;
    case 14:
        printed = snprintf(pcInsert, iInsertLen, state.a3.to_string().c_str());
        break;
    case 15:
        printed = snprintf(pcInsert, iInsertLen, state.a4.to_string().c_str());
        break;
    case 16:
        printed = snprintf(pcInsert, iInsertLen, state.a5.to_string().c_str());
        break;
    case 17:
        printed = snprintf(pcInsert, iInsertLen, state.a6.to_string().c_str());
        break;
    case 18:
        printed = snprintf(pcInsert, iInsertLen, state.a7.to_string().c_str());
        break;
    case 19:
        printed = snprintf(pcInsert, iInsertLen, state.s2.to_string().c_str());
        break;
    case 20:
        printed = snprintf(pcInsert, iInsertLen, state.s3.to_string().c_str());
        break;
    case 21:
        printed = snprintf(pcInsert, iInsertLen, state.s4.to_string().c_str());
        break;
    case 22:
        printed = snprintf(pcInsert, iInsertLen, state.s5.to_string().c_str());
        break;
    case 23:
        printed = snprintf(pcInsert, iInsertLen, state.s6.to_string().c_str());
        break;
    case 24:
        printed = snprintf(pcInsert, iInsertLen, state.s7.to_string().c_str());
        break;
    case 25:
        printed = snprintf(pcInsert, iInsertLen, state.s8.to_string().c_str());
        break;
    case 26:
        printed = snprintf(pcInsert, iInsertLen, state.s9.to_string().c_str());
        break;
    case 27:
        printed = snprintf(pcInsert, iInsertLen, state.s10.to_string().c_str());
        break;
    case 28:
        printed = snprintf(pcInsert, iInsertLen, state.s11.to_string().c_str());
        break;
    case 29:
        printed = snprintf(pcInsert, iInsertLen, state.t3.to_string().c_str());
        break;
    case 30:
        printed = snprintf(pcInsert, iInsertLen, state.t4.to_string().c_str());
        break;
    case 31:
        printed = snprintf(pcInsert, iInsertLen, state.t5.to_string().c_str());
        break;
    case 32:
        printed = snprintf(pcInsert, iInsertLen, state.t6.to_string().c_str());
        break;
    case 33:
        printed = snprintf(pcInsert, iInsertLen, state.pc.to_string().c_str());
        break;
    case 34:
        printed = snprintf(pcInsert, iInsertLen, state.serialize_ram().c_str());
        break;
    default:
        break;
    }
    return printed;
}

void ssi_init()
{
    http_set_ssi_handler(ssi_handler,
                         ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
