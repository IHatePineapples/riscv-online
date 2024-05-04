#include "server.hpp"

#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "RoL/threaded/shared.hpp"
#include "ssi.h"


namespace server
{
    void init_all()
    {
        httpd_init();
        ssi_init();
    }

    void run()
    {
        init_all();
        for (;;)
        {
            cyw43_arch_poll();
            sleep_ms(50);
        }
    }
}
