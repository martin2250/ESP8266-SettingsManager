#include "setting.h"
#include "util.h"

void Setting_IPAddr::save(memoryptr writefunc)
{
	union {
		uint32_t	uval;
		uint8_t		buffer[4];
	} conv = { (uint32_t)value };

	if (writefunc)
		writefunc(address, 4, conv.buffer);

	options.changed = 0;
}

void Setting_IPAddr::load(memoryptr readfunc)
{
	uint8_t buffer[4];

	if ((!readfunc) || (!readfunc(address, 4, buffer))) {
		value = val_default;
		return;
	}

	value = IPAddress(buffer);

	if (on_change)
		(*on_change)();
}

uint8_t Setting_IPAddr::parse(const char *input)
{
	IPAddress newval;

	if (!newval.fromString(input))
		return SETTING_PARSE_ERROR_FORMAT;

	value = newval;
	options.changed = 1;

	if (on_change)
		(*on_change)();

	return 0;
}

String Setting_IPAddr::toString(uint8_t which)
{
	switch (which) {
	case SETTING_TO_STRING_VALUE:
		if (options.hide_value)
			return "******";
		else
			return value.toString();
	case SETTING_TO_STRING_DEFAULT:
		return val_default.toString();
	case SETTING_TO_STRING_MIN:
	case SETTING_TO_STRING_MAX:
		return "";
	default:
		return "not good";
	}
}
