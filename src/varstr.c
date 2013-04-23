#include "bmsg.h"

#include <string.h>

/* here we impose a reasonable maximum string length size of 65535 */
size_t bmsg_encode_varstr(uint8_t *buf, size_t size, const char *str, uint16_t slen) {
	size_t vint_size;

	vint_size = bmsg_encode_varint(buf, size, slen);
	if(vint_size + slen > size)
		return vint_size;

	if(slen > 0)
		memcpy(buf+vint_size, str, slen);

	return vint_size;
}
