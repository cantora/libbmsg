#ifndef BMSG_PARSER_H
#define BMSG_PARSER_H

#include "parser/integer.h"
#include "parser/bytes.h"

typedef enum {
	BMSG_PARSER_STATE_SEEK = 0, /* seek for the magic number */
	BMSG_PARSER_STATE_HEADER, /* extract the header data */
	BMSG_PARSER_STATE_PAYLOAD /* parse the payload into a protocol object */
} parser_state;

typedef enum {
	BMSG_PARSER_ERR_FATAL = 0
} bmsg_parser_err_codes;

struct bmsg_parser_err {
	int code;
	const char *msg;	
}

struct bmsg_parser {
	struct bmsg_parser_err err;
	uint8_t mn_buf[4]; /* for magic number */
	int header_parser;
	struct bmsg_parser_bytes cmd_prsr;
	struct bmsg_parser_uint32 u32_prsr;
	struct {
		struct {
			char cmd[12];
			uint32_t payload_len;
			uint32_t checksum;
		} header;
	} packet;
	int state;	
}

#endif /* BMSG_PARSER_H */
