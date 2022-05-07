#ifndef WIFI_STORAGE_H
#define WIFI_STORAGE_H

#include "wifi_device_config.h"

int WriteWifiDeviceConfigToFile(const WifiDeviceConfig *cfg);
int ReadWifiDeviceConfigFromFile(WifiDeviceConfig *cfg);
int DelWifiDeviceConfigFile();

#endif
