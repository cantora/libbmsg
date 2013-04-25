#ifndef BMSG_PARSER_BYTES_H
#define BMSG_PARSER_BYTES_H

struct bmsg_parser_bytes {
	size_t size, idx;
	uint8_t *buf;
};

void bmsg_parser_bytes_init(struct bmsg_parser_bytes *prsr, uint8_t *buf,
		size_t size) {
	prsr->size = size;
	prsr->buf = buf;
	prsr->idx = 0;
}

static inline void bmsg_parser_bytes_consume(struct bmsg_parser_bytes *prsr, uint8_t byte) {
	if(prsr->idx < prsr->size)
		prsr->buf[prsr->idx++] = byte;
}

static inline int bmsg_parser_bytes_done(const struct bmsg_parser_bytes *prsr) {
	return !(prsr->idx < prsr->size);
}

#endif /* BMSG_PARSER_BYTES_H */
