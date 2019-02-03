#include "settingsmanager.h"

SettingsManager::SettingsManager(ESP8266WebServer *server) : server(server)
{
}

void SettingsManager::load()
{
	for (auto it = settings.begin(); it != settings.end(); ++it)
		(*it)->load(readfunc);
}

void SettingsManager::handleSettingsPost()
{
	String message_buffer;

	if ((!server->hasArg("id")) || (!server->hasArg("value"))) {
		message_buffer += "bad request (id and value args are missing)";
		server->send(400, "text/plain", message_buffer);
	}

	String arg_id = server->arg("id");
	Setting *setting = NULL;

	for (auto it = settings.begin(); it != settings.end(); ++it) {
		String abbrev = FPSTR((*it)->abbrev);

		if (arg_id.equals(abbrev)) {
			setting = *it;
			break;
		}
	}

	if (!setting) {
		message_buffer += "unknown id " + arg_id;
		server->send(400, "text/plain", message_buffer);
		return;
	}

	String arg_val = server->arg("value");

	if (uint8_t err = setting->parse(arg_val.c_str())) {
		switch (err) {
		case SETTING_PARSE_ERROR_FORMAT:
			message_buffer += "invalid format";
			break;
		case SETTING_PARSE_ERROR_RANGE:
			message_buffer += "value is outside of allowed range";
			break;
		case SETTING_PARSE_ERROR_LENGTH:
			message_buffer += "value is too long";
			break;
		}

		server->send(400, "text/plain", message_buffer);
		return;
	}

	if (setting->options.save_on_change)
		setting->save(writefunc);

	// TODO: SAVE SETTING

	message_buffer += "ok";
	server->sendHeader("Location", server->arg("backurl"));
	server->send(303, "text/plain", message_buffer);
}

#define SCRIPT_SET_BACKURL "<script>document.getElementById('backurl_element').value = window.location.href.split('?')[0];</script>"

// todo: add string header
void SettingsManager::handleSettingsGet(uint8_t groups)
{
	String message_buffer =
		"<html>"
		"<body>"
		"<h1>Settings</h1>"
		"<table>"
		"<tr>"
		"<th>Name</th>"
		"<th>Value</th>"
		"<th>Default</th>"
		"<th>Min</th>"
		"<th>Max</th>"
		"</tr>";

	for (auto it = settings.begin(); it != settings.end(); ++it) {
		Setting *setting = *it;

		if (!(setting->groups & groups))
			continue;

		message_buffer +=
			String("<tr>") +
			"<td>" + FPSTR(setting->name) + "</td>"
			"<td>" + setting->toString(SETTING_TO_STRING_VALUE) + "</td>"
			"<td>" + setting->toString(SETTING_TO_STRING_DEFAULT) + "</td>"
			"<td>" + setting->toString(SETTING_TO_STRING_MIN) + "</td>"
			"<td>" + setting->toString(SETTING_TO_STRING_MAX) + "</td>"
			"</tr>";
	}

	message_buffer +=
		"</table>"
		"<form method=\"post\">"
		"<select name=\"id\">"
		"<option value=\"--\">---</option>";

	for (auto it = settings.begin(); it != settings.end(); ++it) {
		Setting *setting = *it;

		if (!(setting->groups & groups))
			continue;

		message_buffer += "<option value=\"";
		message_buffer += FPSTR(setting->abbrev);
		message_buffer += "\">";
		message_buffer += FPSTR(setting->name);
		message_buffer += "</option>";
	}

	message_buffer +=
		"</select>"
		"<input name=\"value\">"
		"<input type=\"submit\" value=\"Save\">"
		"<input type=\"hidden\" name=\"backurl\" id=\"backurl_element\"/>"
		"</form>"
		"</body>"
		SCRIPT_SET_BACKURL
		"</html>";

	server->send(200, "text/html", message_buffer);
}
