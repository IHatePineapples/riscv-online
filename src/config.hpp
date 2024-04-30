#pragma once

/**
 * \file config.hpp Configuration for things such as Wi-Fi name and password
 * \brief Edit to match your configuration and system.
 *
 * \warning You must edit this. The default values are bogus.
 */

/**
 * \def wifi_country Where is your access point ? Find correct enum value.
 * \note Valid countries start with name `CYW43_COUNTRY_`.
 * \warning Edit this to match your location.
 */
#define wifi_country CYW43_COUNTRY_UK

/**
 * \def wifi_ssid The Wi-Fi name, you need to edit these as the defaults are bogus.
 * \warning Edit this to match a real Wi-Fi.
 */
#define wifi_ssid "QA"

/**
 * \def wifi_passwd The Wi-Fi password, you need to edit these as the defaults are bogus.
 * \warning Edit this to match a real Wi-Fi.
 */
#define wifi_passwd "RISCV-Online"
