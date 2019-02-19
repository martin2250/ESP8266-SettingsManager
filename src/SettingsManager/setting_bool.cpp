#include "setting.h"
#include "util.h"
#include <string.h>

void Setting_Bool::save(memoryptr writefunc)
{
	if (writefunc)
		writefunc(address, 1, (uint8_t *)&value);
	options.changed = 0;
}

void Setting_Bool::load(memoryptr readfunc)
{
	if ((!readfunc) || (!readfunc(address, 1, (uint8_t *)&value)))
		value = val_default;

	if (on_change)
		(*on_change)();
}

uint8_t Setting_Bool::parse(const char *input)
{
	if (strlen(input) > 5)
		return SETTING_PARSE_ERROR_LENGTH;

	if (!strcmp(input, "true") || !strcmp(input, "t") || !strcmp(input, "1") || !strcmp(input, "y") || !strcmp(input, "yes")) {
		value = true;
		options.changed = 1;

		if (on_change)
			(*on_change)();

		return 0;
	} else if (!strcmp(input, "false") || !strcmp(input, "f") || !strcmp(input, "0") || !strcmp(input, "n") || !strcmp(input, "no")) {
		value = false;
		options.changed = 1;

		if (on_change)
			(*on_change)();

		return 0;
	}

	return SETTING_PARSE_ERROR_FORMAT;
}

String bool_to_string(bool input)
{
	if (input)
		return "true";
	return "false";
}

String Setting_Bool::toString(uint8_t which)
{
	switch (which) {
	case SETTING_TO_STRING_VALUE:
		if (options.hide_value)
			return "******";
		else
			return bool_to_string(value);
	case SETTING_TO_STRING_DEFAULT:
		return bool_to_string(val_default);
	case SETTING_TO_STRING_MIN:
	case SETTING_TO_STRING_MAX:
		return "";
	default:
		return "not good";
	}
}
