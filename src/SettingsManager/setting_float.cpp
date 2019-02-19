#include "setting.h"
#include "util.h"
#include <math.h>

union Convert_Float {
	float	floatval;
	uint8_t buffer[4];
};

void Setting_Float::save(memoryptr writefunc)
{
	Convert_Float conv = { value };

	if (writefunc)
		writefunc(address, 4, conv.buffer);

	options.changed = 0;
}

void Setting_Float::load(memoryptr readfunc)
{
	Convert_Float conv;

	if ((!readfunc) || (!readfunc(address, 4, conv.buffer)) || (!isnan(min) && conv.floatval < min) || (!isnan(max) && conv.floatval > max)) {
		value = val_default;
		return;
	}

	value = conv.floatval;

	if (on_change)
		(*on_change)();
}

uint8_t Setting_Float::parse(const char *input)
{
	char *endptr;

	float newval = strtof(input, &endptr);

	if (endptr == input)
		return SETTING_PARSE_ERROR_FORMAT;

	if (isinf(newval))
		return SETTING_PARSE_ERROR_RANGE;

	if ((!isnan(min) && newval < min) || (!isnan(max) && newval > max))
		return SETTING_PARSE_ERROR_RANGE;

	if (on_change)
		(*on_change)();

	value = newval;
	options.changed = 1;
	return 0;
}

String Setting_Float::toString(uint8_t which)
{
	switch (which) {
	case SETTING_TO_STRING_VALUE:
		if (options.hide_value)
			return "******";
		else
			return String(value, decimals_show);
	case SETTING_TO_STRING_DEFAULT:
		return String(val_default, decimals_show);
	case SETTING_TO_STRING_MIN:
		if (isnan(min))
			return "";
		return String(min, decimals_show);
	case SETTING_TO_STRING_MAX:
		if (isnan(max))
			return "";
		return String(max, decimals_show);
	default:
		return "not good";
	}
}
