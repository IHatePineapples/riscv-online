#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "config.hpp"

int main() {
  stdio_init_all();
  cyw43_arch_init_with_country(CYW43_COUNTRY_UK);
  cyw43_arch_enable_sta_mode();

  printf("Connecting...\n");
  while (cyw43_arch_wifi_connect_blocking(wifi_ssid, wifi_passwd,
                                          CYW43_AUTH_WPA2_MIXED_PSK)) {
    printf("Failed connecting, trying again.\n");
    sleep_ms(3000);
  };
  printf("Connected, starting server.\n");

}
