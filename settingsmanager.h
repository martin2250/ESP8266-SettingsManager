#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include "setting.h"
#include <vector>

struct SettingsManager {
	ESP8266WebServer *	server;
	std::vector<Setting *>	settings;
	memoryptr		loadfunc = NULL;
	memoryptr		savefunc = NULL;

	SettingsManager(ESP8266WebServer *server);

	void load();

	void handleSettingsGet(uint8_t groups);
	void handleSettingsPost();
};

#endif
