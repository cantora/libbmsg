#ifndef BMSG_VARINT_H
#define BMSG_VARINT_H

#include <stdint.h>
#include <stddef.h>

/* assumes there are at least 8 bytes available in @buf. */
void bmsg_encode_uint64(uint8_t *buf, uint64_t val);
/* assumes there are at least 8 readable bytes in @buf */
uint64_t bmsg_decode_uint64(const uint8_t *buf);

/* assumes there are at least 4 bytes available in @buf. */
void bmsg_encode_uint32(uint8_t *buf, uint32_t val);
/* assumes there are at least 4 readable bytes in @buf */
uint32_t bmsg_decode_uint32(const uint8_t *buf);

/* assumes there are at least 2 bytes available in @buf. */
void bmsg_encode_uint16(uint8_t *buf, uint16_t val);
/* assumes there are at least 2 readable bytes in @buf */
uint16_t bmsg_decode_uint16(const uint8_t *buf);

/* encodes @val into @buf where @size is the number of bytes
 * available in @buf.
 * returns the size of the varint. if the return value is larger
 * than the value @size then the operation is a failure and
 * @buf was not modified.
 */
size_t bmsg_encode_varint(uint8_t *buf, size_t size, uint64_t val);

/* decodes the bytes in @buf and stores the value of the int in @val.
 * this function will not read more than @size bytes from buf. 
 *
 * returns the size of the decoded varint. if the return value is greater
 * than @size, then the @val is unchanged. if the return value is 0 then
 * @size == 0 and decoding could not be attempted.
 */
size_t bmsg_decode_varint(const uint8_t *buf, size_t size, uint64_t *val);

#endif /* BMSG_VARINT_H */
