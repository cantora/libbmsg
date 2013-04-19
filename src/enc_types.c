#include "enc_types.h"

#include <string.h>

#include <ccan/endian/endian.h>
#include <ccan/build_assert/build_assert.h>

void enc_uint64(uint8_t *buf, uint64_t val) {
	val = CPU_TO_BE64_CONST(val);

	/* code that calls this function will assume that
	 * 8 bytes will be written here, so we want to fail
	 * compilation if this is untrue for some reason. */
	BUILD_ASSERT(sizeof(val) == 8);
	memcpy(buf, &val, 8);
}

void enc_uint32(uint8_t *buf, uint32_t val) {
	val = CPU_TO_BE32_CONST(val);

	/* code that calls this function will assume that
	 * 4 bytes will be written here, so we want to fail
	 * compilation if this is untrue for some reason. */
	BUILD_ASSERT(sizeof(val) == 4);
	memcpy(buf, &val, 4);
}

void enc_uint16(uint8_t *buf, uint16_t val) {
	val = CPU_TO_BE16_CONST(val);

	/* code that calls this function will assume that
	 * 2 bytes will be written here, so we want to fail
	 * compilation if this is untrue for some reason. */
	BUILD_ASSERT(sizeof(val) == 2);
	memcpy(buf, &val, 2);
}

size_t enc_varint(uint8_t *buf, size_t size, uint64_t val) {
	size_t vint_size;

	if(val < 0xfd) {
		vint_size = 1;

		if(size >= vint_size)
			buf[0] = (uint8_t) val;
	}
	else if(val <= 0xffff) {
		vint_size = 3;

		if(size >= vint_size) {
			buf[0] = 0xfd;
			enc_uint16(buf+1, (uint16_t) val);	
		}
	}
	else if(val <= 0xffffffff) {
		vint_size = 5;

		if(size >= vint_size) {
			buf[0] = 0xfe;
			enc_uint32(buf+1, (uint32_t) val);
		}
	}
	else {
		vint_size = 9;

		if(size >= vint_size) {
			buf[0] = 0xff;
			enc_uint64(buf+1, (uint64_t) val);
		}
	}

	return vint_size;
}
