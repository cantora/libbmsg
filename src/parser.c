#include "parser.h"

#include <string.h>
#include <ccan/array_size/array_size.h>

static void set_err_bad_state(struct bmsg_parser *prsr) {
	prsr->err.code = BMSG_PARSER_ERR_FATAL;
	prsr->err.msg = "entered unknown state";
}

static int transition(struct bmsg_parser *prsr, int state) {

	switch(state) {
	case BMSG_PARSER_STATE_SEEK:
		memset(prsr->mn_buf, 0, ARRAY_SIZE(prsr->mn_buf));

		break;
	case BMSG_PARSER_STATE_HEADER:
		bmsg_parser_bytes_init(&prsr->cmd_prsr, 
			(uint8_t *) prsr->packet.header.cmd,
			ARRAY_SIZE(prsr->packet.header.cmd));
		bmsg_parser_uint32_init(&prsr->payload_prsr);
		bmsg_parser_uint32_init(&prsr->checksum_prsr);
		
		break;
	case BMSG_PARSER_STATE_PAYLOAD:
		/*nothing*/
		
		break;
	default:
		set_err_bad_state(prsr);
		return -1;
	}
	
	prsr->state = state;
	return 0;
}

void bmsg_parser_init(struct bmsg_parser *prsr) {
	prsr->err.code = BMSG_PARSER_ERR_NONE;
	prsr->err.msg = "no error";
	transition(prsr, BMSG_PARSER_STATE_SEEK);
}

size_t bmsg_parser_consume(struct bmsg_parser *prsr,
		const uint8_t *bytes, size_t blen) {
	size_t i,j;
	int next_state, brk;

	brk = 0;
	/* default is to stay on current state */
	next_state = prsr->state;

	for(i = 0; i < blen; i++) {
		switch(prsr->state) {
		case BMSG_PARSER_STATE_SEEK:
			BUILD_ASSERT(ARRAY_SIZE(prsr->mn_buf) == 4);
			for(j = ARRAY_SIZE(prsr->mn_buf); j > 1; j--)
				prsr->mn_buf[j-1] = prsr->mn_buf[j-2];
			
			prsr->mn_buf[ARRAY_SIZE(prsr->mn_buf)-1] = bytes[i];
			if(bmsg_decode_uint32(prsr->mn_buf) == 0xE9BEB4D9)
				next_state = BMSG_PARSER_STATE_HEADER;

			break;
		case BMSG_PARSER_STATE_HEADER:
			if(bmsg_parser_bytes_done(&prsr->cmd_prsr) == 0) {
				bmsg_parser_bytes_consume(&prsr->cmd_prsr, bytes[i]);
				continue;
			}
			if(bmsg_parser_uint32_done(&prsr->payload_prsr) == 0) {
				bmsg_parser_uint32_consume(&prsr->payload_prsr, bytes[i]);
				continue;
			}
			if(bmsg_parser_uint32_done(&prsr->checksum_prsr) == 0) {
				bmsg_parser_uint32_consume(&prsr->checksum_prsr, bytes[i]);
				continue;
			}

			/* if we get here then the header is parsed :D */
			bmsg_parser_uint32_value(&prsr->payload_prsr, 
					&prsr->packet.header.payload_len);
			bmsg_parser_uint32_value(&prsr->checksum_prsr, 
					&prsr->packet.header.checksum);
			next_state = BMSG_PARSER_STATE_PAYLOAD;

			break;
		case BMSG_PARSER_STATE_PAYLOAD:
			brk = 1;
			break;
		default:
			set_err_bad_state(prsr);
			brk = 1;
		}

		if(brk != 0)
			break;

		if(next_state != prsr->state) 
			if(transition(prsr, next_state) != 0)
				break;
	}

	return i;
}