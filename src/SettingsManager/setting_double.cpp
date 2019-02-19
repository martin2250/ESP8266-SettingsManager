#include "setting.h"
#include "util.h"
#include <math.h>

union Convert_Double {
	double	doubleval;
	uint8_t buffer[8];
};

void Setting_Double::save(memoryptr writefunc)
{
	Convert_Double conv = { value };

	if (writefunc)
		writefunc(address, 8, conv.buffer);

	options.changed = 0;
}

void Setting_Double::load(memoryptr readfunc)
{
	Convert_Double conv;

	if ((!readfunc) || (!readfunc(address, 8, conv.buffer)) || (!isnan(min) && conv.doubleval < min) || (!isnan(max) && conv.doubleval > max)) {
		value = val_default;
		return;
	}

	value = conv.doubleval;

	if (on_change)
		(*on_change)();
}

uint8_t Setting_Double::parse(const char *input)
{
	char *endptr;

	float newval = strtod(input, &endptr);

	if (endptr == input)
		return SETTING_PARSE_ERROR_FORMAT;

	if (isinf(newval))
		return SETTING_PARSE_ERROR_RANGE;

	if ((!isnan(min) && newval < min) || (!isnan(max) && newval > max))
		return SETTING_PARSE_ERROR_RANGE;

	value = newval;
	options.changed = 1;

	if (on_change)
		(*on_change)();

	return 0;
}

String Setting_Double::toString(uint8_t which)
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
