#ifndef BMSG_PARSER_INTEGER_H
#define BMSG_PARSER_INTEGER_H

#include "varint.h"

#define BMSG_PARSER_INTEGER_DEF_STRUCT(bits) \
	struct bmsg_parser_uint##bits { \
		uint8_t buf[bits/8]; \
		size_t idx; \
	};
BMSG_PARSER_INTEGER_DEF_STRUCT(16)
BMSG_PARSER_INTEGER_DEF_STRUCT(32)
BMSG_PARSER_INTEGER_DEF_STRUCT(64)
#undef BMSG_PARSER_INTEGER_DEF_STRUCT

#define BMSG_PARSER_INTEGER_DEF_INIT(bits) \
	static inline void bmsg_parser_uint##bits##_init( \
			struct bmsg_parser_uint##bits *prsr) { \
		prsr->idx = 0; \
	}
BMSG_PARSER_INTEGER_DEF_INIT(16)
BMSG_PARSER_INTEGER_DEF_INIT(32)
BMSG_PARSER_INTEGER_DEF_INIT(64)
#undef BMSG_PARSER_INTEGER_DEF_INIT

#define BMSG_PARSER_INTEGER_DEF_DONE(bits) \
	static inline int bmsg_parser_uint##bits##_done( \
			const struct bmsg_parser_uint##bits *prsr) { \
		return !(prsr->idx < (bits/8)); \
	}
BMSG_PARSER_INTEGER_DEF_DONE(16)
BMSG_PARSER_INTEGER_DEF_DONE(32)
BMSG_PARSER_INTEGER_DEF_DONE(64)
#undef BMSG_PARSER_INTEGER_DEF_DONE

#define BMSG_PARSER_INTEGER_DEF_CONSUME(bits) \
	static inline void bmsg_parser_uint##bits##_consume( \
			struct bmsg_parser_uint##bits *prsr, uint8_t byte) { \
		if(prsr->idx < (bits/8)) { \
			prsr->buf[prsr->idx++] = byte; \
		} \
	}
BMSG_PARSER_INTEGER_DEF_CONSUME(16)
BMSG_PARSER_INTEGER_DEF_CONSUME(32)
BMSG_PARSER_INTEGER_DEF_CONSUME(64)
#undef BMSG_PARSER_INTEGER_DEF_CONSUME

#define BMSG_PARSER_INTEGER_DEF_VALUE(bits) \
	static inline void bmsg_parser_uint##bits##_value( \
			const struct bmsg_parser_uint##bits *prsr, uint##bits##_t *value) { \
		*value = bmsg_decode_uint##bits (prsr->buf); \
	}
BMSG_PARSER_INTEGER_DEF_VALUE(16)
BMSG_PARSER_INTEGER_DEF_VALUE(32)
BMSG_PARSER_INTEGER_DEF_VALUE(64)
#undef BMSG_PARSER_INTEGER_DEF_VALUE

#endif /* BMSG_PARSER_INTEGER_H */
