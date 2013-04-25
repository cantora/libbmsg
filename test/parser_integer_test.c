#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

#include <string.h>
#include <time.h>
#include <stdlib.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "parser/integer.h"

int main(int argc, char *argv[]) {
	struct bmsg_parser_uint16 u16_prsr;
	struct bmsg_parser_uint32 u32_prsr;
	struct bmsg_parser_uint64 u64_prsr;
	uint16_t u16_val;
	uint32_t u32_val;
	uint64_t u64_val;
	uint64_t input;
	uint8_t bytes[8];
	size_t i;
	int j;
	(void)(argc);
	(void)(argv);

	srand(time(NULL));

	plan_tests(2*(3+3+4+4+5+5+5+5+6+6));

	diag("++++parser/integer++++\n#");

	for(j = 0; j < 2; j++) {
		diag("j = %d", j);
		input = (uint64_t) rand();
		input |= ((uint64_t) rand()) << 32;
		bmsg_encode_uint64(bytes, input);
	
		bmsg_parser_uint16_init(&u16_prsr);
		bmsg_parser_uint32_init(&u32_prsr);
		bmsg_parser_uint64_init(&u64_prsr);
		ok1(bmsg_parser_uint16_done(&u16_prsr) == 0);
		ok1(bmsg_parser_uint32_done(&u32_prsr) == 0);
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
	
		diag("input = 0x%016" PRIx64, input);
		i = 0;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) == 0);
		ok1(bmsg_parser_uint32_done(&u32_prsr) == 0);
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
	
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) == 0);
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
	
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) == 0);
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
		
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) != 0);
		bmsg_parser_uint32_value(&u32_prsr, &u32_val);
		ok1(u32_val == bmsg_decode_uint32(bytes));
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
	
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) != 0);
		bmsg_parser_uint32_value(&u32_prsr, &u32_val);
		ok1(u32_val == bmsg_decode_uint32(bytes));
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
	
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) != 0);
		bmsg_parser_uint32_value(&u32_prsr, &u32_val);
		ok1(u32_val == bmsg_decode_uint32(bytes));
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
	
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) != 0);
		bmsg_parser_uint32_value(&u32_prsr, &u32_val);
		ok1(u32_val == bmsg_decode_uint32(bytes));
		ok1(bmsg_parser_uint64_done(&u64_prsr) == 0);
	
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) != 0);
		bmsg_parser_uint32_value(&u32_prsr, &u32_val);
		ok1(u32_val == bmsg_decode_uint32(bytes));
		ok1(bmsg_parser_uint64_done(&u64_prsr) != 0);
		bmsg_parser_uint64_value(&u64_prsr, &u64_val);
		ok1(u64_val == bmsg_decode_uint64(bytes));
	
		i++;
		diag("i = %zu", i);
		bmsg_parser_uint16_consume(&u16_prsr, bytes[i]);
		bmsg_parser_uint32_consume(&u32_prsr, bytes[i]);
		bmsg_parser_uint64_consume(&u64_prsr, bytes[i]);
		ok1(bmsg_parser_uint16_done(&u16_prsr) != 0);
		bmsg_parser_uint16_value(&u16_prsr, &u16_val);
		ok1(u16_val == bmsg_decode_uint16(bytes));
		ok1(bmsg_parser_uint32_done(&u32_prsr) != 0);
		bmsg_parser_uint32_value(&u32_prsr, &u32_val);
		ok1(u32_val == bmsg_decode_uint32(bytes));
		ok1(bmsg_parser_uint64_done(&u64_prsr) != 0);
		bmsg_parser_uint64_value(&u64_prsr, &u64_val);
		ok1(u64_val == bmsg_decode_uint64(bytes));
		
	}

	diag("----parser/integer----\n#");

	return exit_status();
}