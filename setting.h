#ifndef SETTING_H_
#define SETTING_H_

#include <Arduino.h>
#include <stdint.h>
#include <climits>

// address, length, buffer
typedef bool (*memoryptr)(uint16_t, uint8_t, uint8_t *);

#define SETTING_PARSE_ERROR_FORMAT 1
#define SETTING_PARSE_ERROR_RANGE 2
#define SETTING_PARSE_ERROR_LENGTH 3

#define SETTING_TO_STRING_VALUE 0
#define SETTING_TO_STRING_DEFAULT 1
#define SETTING_TO_STRING_MIN 2
#define SETTING_TO_STRING_MAX 3

struct Setting {
	uint16_t	address;
	PGM_P		abbrev;
	PGM_P		name;

	uint8_t		groups = 0xFF; // bit flags to specify different setting groups

	struct {
		unsigned int	hide_value : 1;         // hide the current value from user
		unsigned int	save_on_change : 1;     // save new value when changed
		unsigned int	changed : 1;            // set to 1 after change, set to 0 after save
	} options = { 0, 1, 0 };

	void (*on_change)() = NULL;

	virtual void save(memoryptr savefunc) = 0;
	virtual void load(memoryptr loadfunc) = 0;
	virtual uint8_t parse(const char *input) = 0;
	virtual String toString(uint8_t which) = 0;
};

struct Setting_Int32 : Setting {
	int32_t val_default;
	int32_t min = -2147483648;
	int32_t max = 2147483647;
	int32_t value;

	// bool (*checkptr)(Setting_Int32*);

	void save(memoryptr savefunc);
	void load(memoryptr loadfunc);
	uint8_t parse(const char *input);
	String toString(uint8_t which);
};

struct Setting_Int64 : Setting {
	int64_t value;
	int64_t val_default;
	int64_t min = -9223372036854775807;     // not 8 because string to int function don't allow that value
	int64_t max = 9223372036854775807;

	// bool (*checkptr)(Setting_Int64*);

	void save(memoryptr savefunc);
	void load(memoryptr loadfunc);
	uint8_t parse(const char *input);
	String toString(uint8_t which);
};

struct Setting_Str : Setting {
	uint8_t *	value;
	PGM_P		val_default;
	uint8_t		max_length; // must be one less than sizeof value

	// bool (*checkptr)(Setting_Str*);

	void save(memoryptr savefunc);
	void load(memoryptr loadfunc);
	uint8_t parse(const char *input);
	String toString(uint8_t which);
};

#endif
