#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "enc_types.h"

int main(int argc, char *argv[]) {
	size_t i;
	int j;
	uint8_t buf[64];
	uint8_t zero[1 << 12];
	uint8_t varstr[1 << 12];
	char *test_strs[256];
	(void)(argc);
	(void)(argv);

	srand(time(NULL));
	
	memset(zero, 0, ARRAY_SIZE(zero));
	plan_tests(1+1+1+2+3+3*2+3+5*2+3+9*2+3+7+1+2*256+1);

	diag("++++basic++++");

	enc_uint64(buf, 0xf10189a6be4d203f);
	ok1(memcmp(buf, "\xf1\x01\x89\xa6\xbe\x4d\x20\x3f", 8) == 0);

	enc_uint32(buf, 0x2f5c3890);
	ok1(memcmp(buf, "\x2f\x5c\x38\x90", 4) == 0);

	enc_uint16(buf, 0x15ea);
	ok1(memcmp(buf, "\x15\xea", 2) == 0);

	memset(buf, 0, ARRAY_SIZE(buf) );

	ok1(enc_varint(buf, 0, 1) == 1);
	ok1(memcmp(buf, zero, 64) == 0);

	ok1(enc_varint(buf, 1, 0xe4) == 1);
	ok1(memcmp(buf, "\xe4", 1) == 0);
	ok1(memcmp(buf+1, zero, 63) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	for(i = 0; i < 3; i++) {
		ok1(enc_varint(buf, i, 0xe438) == 3);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(enc_varint(buf, 3, 0xe438) == 3);
	ok1(memcmp(buf, "\xfd\xe4\x38", 3) == 0);
	ok1(memcmp(buf+3, zero, 64-3) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	for(i = 0; i < 5; i++) {
		ok1(enc_varint(buf, i, 0x69c93bbb) == 5);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(enc_varint(buf, 5, 0x69c93bbb) == 5);
	ok1(memcmp(buf, "\xfe\x69\xc9\x3b\xbb", 5) == 0);
	ok1(memcmp(buf+5, zero, 64-5) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	for(i = 0; i < 9; i++) {
		ok1(enc_varint(buf, i, 0x08b7016faed84e46) == 9);
		ok1(memcmp(buf, zero, 64) == 0);
	}
	ok1(enc_varint(buf, 9, 0x08b7016faed84e46) == 9);
	ok1(memcmp(buf, "\xff\x08\xb7\x01\x6f\xae\xd8\x4e\x46", 9) == 0);
	ok1(memcmp(buf+9, zero, 64-9) == 0);
	memset(buf, 0, ARRAY_SIZE(buf) );

	ok1(enc_varint(NULL, 0, 0xfc) == 1);
	ok1(enc_varint(NULL, 0, 0xfc+1) == 3);
	ok1(enc_varint(NULL, 0, 0xffff) == 3);
	ok1(enc_varint(NULL, 0, 0xffff+1) == 5);
	ok1(enc_varint(NULL, 0, 0xffffffff) == 5);
	ok1(enc_varint(NULL, 0, 0xffffffffLL + 1) == 9);
	ok1(enc_varint(NULL, 0, 0xffffffffffffffffLL) == 9);
	memset(buf, 0, ARRAY_SIZE(buf) );

	ok1(enc_varstr(NULL, 0, "asdf", 4) == 1);

	for(i = 0; i < ARRAY_SIZE(test_strs); i++) {
		int len = i*32+1;
		test_strs[i] = malloc(len*sizeof(char));
		if(test_strs[i] == NULL) {
			diag("memory error");
			return -1;
		}
			
		for(j = 0; j < len-1; j++)
			test_strs[i][j] = (rand() % 255) + 1; /* no null chars */
		
		test_strs[i][len-1] = '\0';
	}

	int amt_bigger = 0;
	for(i = 0; i < ARRAY_SIZE(test_strs); i++) {
		size_t vint_size, slen;
		slen = strlen(test_strs[i]);
		vint_size = enc_varstr(varstr, ARRAY_SIZE(varstr), test_strs[i], slen);
		ok1(vint_size <= 3);
		diag("vint_size = %d, slen = %d", vint_size, slen);

		if(vint_size + slen > ARRAY_SIZE(varstr)) {
			amt_bigger++;
			ok1(memcmp(varstr+vint_size, zero, ARRAY_SIZE(varstr)-vint_size) == 0);
		}
		else {
			ok1(memcmp(varstr+vint_size, test_strs[i], slen) == 0);
		}
		memset(varstr, 0, ARRAY_SIZE(varstr) );
	}

	ok1(amt_bigger = 128);
	diag("----basic----\n#");

	return exit_status();
}
