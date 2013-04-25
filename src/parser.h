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
	BMSG_PARSER_ERR_NONE = 0,
	BMSG_PARSER_ERR_FATAL
} bmsg_parser_err_codes;

struct bmsg_parser_err {
	int code;
	const char *msg;	
};

struct bmsg_parser {
	struct bmsg_parser_err err;

	/* buffer for seeking for the magic number */
	uint8_t mn_buf[4]; 

	/* stuff to parse the header */
	struct bmsg_parser_bytes cmd_prsr;
	struct bmsg_parser_uint32 payload_prsr;
	struct bmsg_parser_uint32 checksum_prsr;
	
	/* holds the header and payload result for a packet */
	struct {
		struct {
			char cmd[12];
			uint32_t payload_len;
			uint32_t checksum;
		} header;
	} packet;

	/* the current state of the parser */
	int state;	
};

#endif /* BMSG_PARSER_H */
