#include "bmsg.h"

#include <string.h>

#include <ccan/endian/endian.h>
#include <ccan/build_assert/build_assert.h>

void bmsg_encode_uint64(uint8_t *buf, uint64_t val) {
	val = CPU_TO_BE64_CONST(val);

	/* code that calls this function will assume that
	 * 8 bytes will be written here, so we want to fail
	 * compilation if this is untrue for some reason. */
	BUILD_ASSERT(sizeof(val) == 8);
	
	memcpy(buf, &val, 8);
}

uint64_t bmsg_decode_uint64(const uint8_t *buf) {
	uint64_t val = *((uint64_t *) buf);

	return BE64_TO_CPU_CONST(val);
}

void bmsg_encode_uint32(uint8_t *buf, uint32_t val) {
	val = CPU_TO_BE32_CONST(val);

	/* code that calls this function will assume that
	 * 4 bytes will be written here, so we want to fail
	 * compilation if this is untrue for some reason. */
	BUILD_ASSERT(sizeof(val) == 4);
	memcpy(buf, &val, 4);
}

uint32_t bmsg_decode_uint32(const uint8_t *buf) {
	uint32_t val = *((uint32_t *) buf);

	return BE32_TO_CPU_CONST(val);
}

void bmsg_encode_uint16(uint8_t *buf, uint16_t val) {
	val = CPU_TO_BE16_CONST(val);

	/* code that calls this function will assume that
	 * 2 bytes will be written here, so we want to fail
	 * compilation if this is untrue for some reason. */
	BUILD_ASSERT(sizeof(val) == 2);
	memcpy(buf, &val, 2);
}

uint16_t bmsg_decode_uint16(const uint8_t *buf) {
	uint16_t val = *((uint16_t *) buf);

	return BE16_TO_CPU_CONST(val);
}

size_t bmsg_encode_varint(uint8_t *buf, size_t size, uint64_t val) {
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
			bmsg_encode_uint16(buf+1, (uint16_t) val);	
		}
	}
	else if(val <= 0xffffffff) {
		vint_size = 5;

		if(size >= vint_size) {
			buf[0] = 0xfe;
			bmsg_encode_uint32(buf+1, (uint32_t) val);
		}
	}
	else {
		vint_size = 9;

		if(size >= vint_size) {
			buf[0] = 0xff;
			bmsg_encode_uint64(buf+1, (uint64_t) val);
		}
	}

	return vint_size;
}

size_t bmsg_decode_varint(const uint8_t *buf, size_t size, uint64_t *val) {
	size_t vint_size;

	if(size < 1)
		return 0;

	switch(buf[0]) {
	case 0xfd:
		vint_size = 3;
		if(size >= vint_size)
			*val = (uint64_t) bmsg_decode_uint16(buf+1);
		
		break;
	case 0xfe:
		vint_size = 5;
		if(size >= vint_size)
			*val = (uint64_t) bmsg_decode_uint32(buf+1);

		break;
	case 0xff:
		vint_size = 9;
		if(size >= vint_size)
			*val = (uint64_t) bmsg_decode_uint64(buf+1);

		break;
	default:
		vint_size = 1;
		*val = (uint64_t) buf[0];
	}

	return vint_size;
}
