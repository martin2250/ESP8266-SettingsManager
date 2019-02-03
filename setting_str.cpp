#include "setting.h"
#include "util.h"
#include <string.h>

void Setting_Str::save(memoryptr writefunc)
{
	if (writefunc)
		writefunc(address, strlen((const char *)value) + 1, value);
	options.changed = 0;
}

void Setting_Str::load(memoryptr readfunc)
{
	if ((!readfunc) || (!readfunc(address, max_length + 1, value)) || (strlen((const char *)value) > max_length))
		strcpy_P((char *)value, val_default);
}

uint8_t Setting_Str::parse(const char *input)
{
	if (strlen(input) > max_length)
		return SETTING_PARSE_ERROR_LENGTH;

	strcpy((char *)value, input);
	options.changed = 1;
	return 0;
}

String Setting_Str::toString(uint8_t which)
{
	switch (which) {
	case SETTING_TO_STRING_VALUE:
		if (options.hide_value)
			return String("******");
		else
			return String((char *)value);
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
