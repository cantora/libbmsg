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
	uint64_t input, output;
	(void)(argc);
	(void)(argv);

	srand(time(NULL));
	
	memset(zero, 0, ARRAY_SIZE(zero));
	plan_tests(2+2+2+2+1+5+3*2+3+2*2+2+5*2+3+4*2+2+9*2+3+8*2+2+2+3+3+3);

	diag("++++varint++++");

	output = 0;
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
	
	ok1(bmsg_decode_varint(NULL, 0, NULL) == 0);
	
	ok1(bmsg_encode_varint(buf, 1, 0xe4) == 1);
	ok1(memcmp(buf, "\xe4", 1) == 0);
	ok1(memcmp(buf+1, zero, 63) == 0);
	ok1(bmsg_decode_varint(buf, 64, &output) == 1);
	ok1(output == 0xe4);
	memset(buf, 0, ARRAY_SIZE(buf) );

	input = 0xe438;
	for(i = 0; i < 3; i++) {
		ok1(bmsg_encode_varint(buf, i, input) == 3);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(bmsg_encode_varint(buf, 3, input) == 3);
	ok1(memcmp(buf, "\xfd\xe4\x38", 3) == 0);
	ok1(memcmp(buf+3, zero, 64-3) == 0);

	output = 0;
	for(i = 1; i < 3; i++) {
		ok1(bmsg_decode_varint(buf, i, &output) == 3);
		ok1(output == 0);
	}
	ok1(bmsg_decode_varint(buf, 3, &output) == 3);
	ok1(output == input);
	memset(buf, 0, ARRAY_SIZE(buf) );

	input = 0x69c93bbb;
	for(i = 0; i < 5; i++) {
		ok1(bmsg_encode_varint(buf, i, input) == 5);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(bmsg_encode_varint(buf, 5, input) == 5);
	ok1(memcmp(buf, "\xfe\x69\xc9\x3b\xbb", 5) == 0);
	ok1(memcmp(buf+5, zero, 64-5) == 0);

	output = 0;
	for(i = 1; i < 5; i++) {
		ok1(bmsg_decode_varint(buf, i, &output) == 5);
		ok1(output == 0);
	}
	ok1(bmsg_decode_varint(buf, 5, &output) == 5);
	ok1(output == input);
	
	memset(buf, 0, ARRAY_SIZE(buf) );

	input = 0x08b7016faed84e46;
	for(i = 0; i < 9; i++) {
		ok1(bmsg_encode_varint(buf, i, input) == 9);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(bmsg_encode_varint(buf, 9, input) == 9);
	ok1(memcmp(buf, "\xff\x08\xb7\x01\x6f\xae\xd8\x4e\x46", 9) == 0);
	ok1(memcmp(buf+9, zero, 64-9) == 0);

	output = 0;
	for(i = 1; i < 9; i++) {
		ok1(bmsg_decode_varint(buf, i, &output) == 9);
		ok1(output == 0);
	}
	ok1(bmsg_decode_varint(buf, 9, &output) == 9);
	ok1(output == input);

	memset(buf, 0, ARRAY_SIZE(buf) );

	ok1(bmsg_encode_varint(NULL, 0, 0xfc) == 1);
	buf[0] = 0xfc;
	ok1(bmsg_decode_varint(buf, 1, &output) == 1);

	ok1(bmsg_encode_varint(NULL, 0, 0xfc+1) == 3);
	ok1(bmsg_encode_varint(NULL, 0, 0xffff) == 3);
	buf[0] = 0xfd;
	ok1(bmsg_decode_varint(buf, 1, NULL) == 3);

	ok1(bmsg_encode_varint(NULL, 0, 0xffff+1) == 5);
	ok1(bmsg_encode_varint(NULL, 0, 0xffffffff) == 5);
	buf[0] = 0xfe;
	ok1(bmsg_decode_varint(buf, 1, NULL) == 5);

	ok1(bmsg_encode_varint(NULL, 0, 0xffffffffLL + 1) == 9);
	ok1(bmsg_encode_varint(NULL, 0, 0xffffffffffffffffLL) == 9);
	buf[0] = 0xff;
	ok1(bmsg_decode_varint(buf, 1, NULL) == 9);

	memset(buf, 0, ARRAY_SIZE(buf) );

	diag("----varint----\n#");

	return exit_status();
}
