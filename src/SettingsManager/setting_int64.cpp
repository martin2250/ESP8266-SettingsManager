#include "setting.h"
#include "util.h"

union Convert_Int64 {
	int64_t intval;
	uint8_t buffer[8];
};

void Setting_Int64::save(memoryptr writefunc)
{
	Convert_Int64 conv = { value };

	if (writefunc)
		writefunc(address, 8, conv.buffer);

	options.changed = 0;
}

void Setting_Int64::load(memoryptr readfunc)
{
	Convert_Int64 conv;

	if ((!readfunc) || (!readfunc(address, 8, conv.buffer)) || (conv.intval < min || conv.intval > max)) {
		value = val_default;
		return;
	}

	value = conv.intval;

	if (on_change)
		(*on_change)();
}

uint8_t Setting_Int64::parse(const char *input)
{
	int64_t newval;

	if (!parse_int64(newval, input))
		return SETTING_PARSE_ERROR_FORMAT;

	if (newval < min || newval > max)
		return SETTING_PARSE_ERROR_RANGE;

	value = newval;
	options.changed = 1;

	if (on_change)
		(*on_change)();

	return 0;
}

String Setting_Int64::toString(uint8_t which)
{
	switch (which) {
	case SETTING_TO_STRING_VALUE:
		if (options.hide_value)
			return "******";
		else
			return int64_to_string(value);
	case SETTING_TO_STRING_DEFAULT:
		return int64_to_string(val_default);
	case SETTING_TO_STRING_MIN:
		return int64_to_string(min);
	case SETTING_TO_STRING_MAX:
		return int64_to_string(max);
	default:
		return "not good";
	}
}
