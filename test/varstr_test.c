#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "bmsg.h"

int main(int argc, char *argv[]) {
	size_t i;
	int j;
	uint8_t zero[1 << 12];
	uint8_t varstr[1 << 12];
	char *test_strs[256];
	(void)(argc);
	(void)(argv);

	srand(time(NULL));
	
	memset(zero, 0, ARRAY_SIZE(zero));
	plan_tests(1+2*256+1);

	diag("++++varstr++++");

	ok1(bmsg_encode_varstr(NULL, 0, "asdf", 4) == 1);

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
		vint_size = bmsg_encode_varstr(varstr, ARRAY_SIZE(varstr), test_strs[i], slen);
		ok1(vint_size <= 3);
		diag("vint_size = %zu, slen = %zu", vint_size, slen);

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

	diag("----varstr----\n#");
	return exit_status();
}
