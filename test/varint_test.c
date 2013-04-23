#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "bmsg.h"

int main(int argc, char *argv[]) {
	size_t i;
	uint8_t buf[64];
	uint8_t zero[64];
	uint64_t input;
	(void)(argc);
	(void)(argv);

	srand(time(NULL));
	
	memset(zero, 0, ARRAY_SIZE(zero));
	plan_tests(2+2+2+2+3+3*2+3+5*2+3+9*2+3+7);

	diag("++++varint++++");

	input = 0xf10189a6be4d203f;
	bmsg_encode_uint64(buf, input);
	ok1(memcmp(buf, "\xf1\x01\x89\xa6\xbe\x4d\x20\x3f", 8) == 0);
	ok1(bmsg_decode_uint64(buf) == input);

	input = 0x2f5c3890;
	bmsg_encode_uint32(buf, (uint32_t) input);
	ok1(memcmp(buf, "\x2f\x5c\x38\x90", 4) == 0);
	ok1(bmsg_decode_uint32(buf) == input);

	input = 0x15ea;
	bmsg_encode_uint16(buf, (uint16_t) input);
	ok1(memcmp(buf, "\x15\xea", 2) == 0);
	ok1(bmsg_decode_uint16(buf) == input);

	memset(buf, 0, ARRAY_SIZE(buf) );

	ok1(bmsg_encode_varint(buf, 0, 1) == 1);
	ok1(memcmp(buf, zero, 64) == 0);

	ok1(bmsg_encode_varint(buf, 1, 0xe4) == 1);
	ok1(memcmp(buf, "\xe4", 1) == 0);
	ok1(memcmp(buf+1, zero, 63) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	for(i = 0; i < 3; i++) {
		ok1(bmsg_encode_varint(buf, i, 0xe438) == 3);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(bmsg_encode_varint(buf, 3, 0xe438) == 3);
	ok1(memcmp(buf, "\xfd\xe4\x38", 3) == 0);
	ok1(memcmp(buf+3, zero, 64-3) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	for(i = 0; i < 5; i++) {
		ok1(bmsg_encode_varint(buf, i, 0x69c93bbb) == 5);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(bmsg_encode_varint(buf, 5, 0x69c93bbb) == 5);
	ok1(memcmp(buf, "\xfe\x69\xc9\x3b\xbb", 5) == 0);
	ok1(memcmp(buf+5, zero, 64-5) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	for(i = 0; i < 9; i++) {
		ok1(bmsg_encode_varint(buf, i, 0x08b7016faed84e46) == 9);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(bmsg_encode_varint(buf, 9, 0x08b7016faed84e46) == 9);
	ok1(memcmp(buf, "\xff\x08\xb7\x01\x6f\xae\xd8\x4e\x46", 9) == 0);
	ok1(memcmp(buf+9, zero, 64-9) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	ok1(bmsg_encode_varint(NULL, 0, 0xfc) == 1);
	ok1(bmsg_encode_varint(NULL, 0, 0xfc+1) == 3);
	ok1(bmsg_encode_varint(NULL, 0, 0xffff) == 3);
	ok1(bmsg_encode_varint(NULL, 0, 0xffff+1) == 5);
	ok1(bmsg_encode_varint(NULL, 0, 0xffffffff) == 5);
	ok1(bmsg_encode_varint(NULL, 0, 0xffffffffLL + 1) == 9);
	ok1(bmsg_encode_varint(NULL, 0, 0xffffffffffffffffLL) == 9);
	memset(buf, 0, ARRAY_SIZE(buf) );

	diag("----varint----\n#");

	return exit_status();
}
