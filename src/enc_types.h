#ifndef LIBBMSG_ENC_TYPES_H
#define LIBBMSG_ENC_TYPES_H

#include <stdint.h>
#include <stddef.h>

/* assumes there are at least 8 bytes available in @buf. */
void enc_uint64(uint8_t *buf, uint64_t val);
/* assumes there are at least 4 bytes available in @buf. */
void enc_uint32(uint8_t *buf, uint32_t val);
/* assumes there are at least 2 bytes available in @buf. */
void enc_uint16(uint8_t *buf, uint16_t val);

/* encodes @val into @buf where @size is the number of bytes
 * available in @buf.
 * returns the size of the varint. if the return value is larger
 * than the value @size then the operation is a failure and
 * @buf was not modified.
 */
size_t enc_varint(uint8_t *buf, size_t size, uint64_t val);


#endif /* LIBBMSG_ENC_TYPES_H */