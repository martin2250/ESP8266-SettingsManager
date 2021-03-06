#include "setting.h"
#include "util.h"
#include <string.h>

void Setting_String::save(memoryptr writefunc)
{
	if (writefunc)
		writefunc(address, strlen(value) + 1, (uint8_t *)value);
	options.changed = 0;
}

void Setting_String::load(memoryptr readfunc)
{
	if ((!readfunc) || (!readfunc(address, max_length + 1, (uint8_t *)value)) || (strlen(value) > max_length))
		strcpy_P((char *)value, val_default);

	for (uint8_t i = 0; i < strlen(value); i++)
		if (!isprint(value[i])) {
			strcpy_P((char *)value, val_default);
			break;
		}

	if (on_change)
		(*on_change)();
}

uint8_t Setting_String::parse(const char *input)
{
	if (strlen(input) > max_length)
		return SETTING_PARSE_ERROR_LENGTH;

	for (uint8_t i = 0; i < strlen(input); i++)
		if (!isprint(input[i]))
			return SETTING_PARSE_ERROR_FORMAT;

	strcpy(value, input);
	options.changed = 1;

	if (on_change)
		(*on_change)();

	return 0;
}

String Setting_String::toString(uint8_t which)
{
	switch (which) {
	case SETTING_TO_STRING_VALUE:
		if (options.hide_value)
			return "******";
		else
			return String(value);
	case SETTING_TO_STRING_DEFAULT:
		return FPSTR(val_default);
	case SETTING_TO_STRING_MIN:
		return "";
	case SETTING_TO_STRING_MAX:
		return String(max_length);
	default:
		return "not good";
	}
}
