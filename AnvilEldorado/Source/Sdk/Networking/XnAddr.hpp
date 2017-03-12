#pragma once
#include <winsock.h>
#include <cstdint>

 struct XNADDR
 {
 	IN_ADDR     ina;                            // IP address (zero if not static/DHCP)
 	IN_ADDR     inaOnline;                      // Online IP address (zero if not online)
 	uint16_t        wPortOnline;                    // Online port
 	uint8_t        abEnet[6];                      // Ethernet MAC address
 	uint8_t        abOnline[20];                   // Online identification
 };
