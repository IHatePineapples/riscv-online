#include "lwip/apps/httpd.h"

#include "server.hpp"
#include "ssi.h"

namespace server
{
    void init_all()
    {
        httpd_init();
        ssi_init();
    }
    void run(){
        init_all();
    }
}
