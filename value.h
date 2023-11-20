#include <stdint.h>
#include <stddef.h>

struct list {
	struct value *value;
	size_t len;
};

struct value {
	enum type {
		BOOL,
		REAL,
		TEXT,
		BOOL_N,
		REAL_N,
		TEXT_N,
		BOOL_L,
		REAL_L,
		TEXT_L,
	} type;
	union {
		uint8_t bool;
		double real;
		char *text;
		struct list list;
	} data;
};

struct value value_bool_null();
struct value value_real_null();
struct value value_text_null();
struct value value_bool_with(const int);
struct value value_real_with(const double);
struct value value_text_with(const char *);

void value_free(struct value *);
void value_copy(struct value *, const struct value);

/* TODO */
char *
value_into_text(const struct value *);
struct value
value_from_text(char *);
