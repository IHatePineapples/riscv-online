#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

#include "config.hpp"

#include "emulator.hpp"
#include "server.hpp"

#include "utils/threaded/queue.hpp"
#include "utils/threaded/shared_queue.hpp"

job_queue jq_;

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
                                          CYW43_AUTH_WPA2_MIXED_PSK))
  {
    printf("Failed connecting, trying again.\n");
    sleep_ms(3000);
  };

  
  printf("Connected, starting Emulator on core 1.\n");
  multicore_launch_core1(emulator::run);

  printf("Starting server.\n");

  
  server::run();



}
