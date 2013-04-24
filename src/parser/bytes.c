#include "parser/bytes.h"

int bmsg_parser_bytes_init(struct bmsg_parser_bytes *prsr, uint8_t *buf,
		size_t size) {
	prsr->size = size;
	if(prsr->size < 1)
		return -1;

	prsr->buf = buf;
	prsr->idx = 0;

	return 0;
}

int bmsg_parser_bytes_consume(struct bmsg_parser_bytes *prsr, uint8_t byte) {
	if(prsr->idx < prsr->size)
		buf[prsr->idx++] = byte;

	if(prsr->idx < prsr->size)
		return 0;

	return 1;
}
