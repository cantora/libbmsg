#ifndef BMSG_PARSER_BYTES_H
#define BMSG_PARSER_BYTES_H

struct bmsg_parser_bytes {
	size_t size, idx;
	uint8_t *buf;
}

int bmsg_parser_bytes_init(struct bmsg_parser_bytes *prsr, uint8_t *buf,
		size_t size);
int bmsg_parser_bytes_consume(struct bmsg_parser_bytes *prsr, uint8_t byte);

#endif /* BMSG_PARSER_BYTES_H */