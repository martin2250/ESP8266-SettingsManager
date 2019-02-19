#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include "Arduino.h"
#include <ESP8266WebServer.h>
#include "setting.h"
#include <vector>

struct SettingsManager
{
	ESP8266WebServer * server;
	std::vector<Setting *> settings;
	memoryptr readfunc = NULL;
	memoryptr writefunc = NULL;

	SettingsManager(ESP8266WebServer *server);

	void load();
	void save();

	void handleSettingsGet(uint8_t groups);
	void handleSettingsGet(uint8_t groups, PGM_P title);
	void handleSettingsGet(uint8_t groups, PGM_P title, PGM_P info);
	void handleSettingsPost();
};

#endif
