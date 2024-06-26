#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

#include "config.hpp"

#include "server.hpp"

#include "RoL/emulation/emulator.hpp"
#include "RoL/threaded/queue.hpp"
#include "RoL/threaded/shared.hpp"

/**
 * \file Main entry point of the firmware.
*/

job_queue in_jq_{};
job_vector out_jq_{};

int main()
{
  if (!stdio_init_all())
  {
    printf("%s:%d: Failed to initialise any output facility from stdio, exiting.\n", __PRETTY_FUNCTION__, __LINE__);
    return 1;
  }

  if (cyw43_arch_init_with_country(wifi_country))
  {
    printf("%s:%d: Failed to init Wi-Fi with country , exiting.\n", __PRETTY_FUNCTION__, __LINE__);
  };

  cyw43_arch_enable_sta_mode();

  printf("Connecting...\n");
  while (cyw43_arch_wifi_connect_blocking(wifi_ssid, wifi_passwd,
                                          CYW43_AUTH_OPEN))
  {
    printf("Failed connecting, trying again.\n");
    sleep_ms(1000);
  };

  printf("Connected, starting Emulator on core 1.\n");
  multicore_launch_core1(emulation::run);

  printf("Starting server.\n");
  server::run();
}
