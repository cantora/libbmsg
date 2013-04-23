OUTPUT			= libbmsg.a
BUILD			= ./build
MKBUILD			:= $(shell mkdir -p $(BUILD) )

CCAN_DIR		= ./libccan
LIBCCAN			= $(CCAN_DIR)/libccan.a
INCLUDES		= -iquote"include" -iquote"src" -I$(CCAN_DIR) 
LIB				= $(LIBCCAN)

OPTIMIZE		= -ggdb
CXX_FLAGS		= -Wall -Wextra $(INCLUDES) $(OPTIMIZE) $(DEFINES)
CXX_CMD			= gcc $(CXX_FLAGS)

SRCS			= $(notdir $(wildcard ./src/*.c) )
CCAN_MODULES	= tap build_assert endian array_size
OBJECTS			= $(patsubst %.c, $(BUILD)/%.o, $(SRCS) ) 
DEP_FLAGS		= -MMD -MP -MF $(patsubst %.o, %.d, $@)

TESTS 			= $(notdir $(patsubst %.c, %, $(wildcard ./test/*_test.c) ) )
TEST_OUTPUTS	= $(foreach test, $(TESTS), $(BUILD)/$(test))

default: all

.PHONY: all
all: $(LIBCCAN) $(OUTPUT) 

$(OUTPUT): $(OBJECTS) 
	ar rcs $@ $+ 

define cc-template
$(CXX_CMD) $(DEP_FLAGS) -c $< -o $@
endef

$(BUILD)/%.o: src/%.c
	$(cc-template)

$(BUILD)/%.o: test/%.c $(LIBCCAN)
	$(cc-template)

$(CCAN_DIR):
	git clone 'https://github.com/rustyrussell/ccan.git' tmp_ccan
	cd tmp_ccan/tools && ./create-ccan-tree -b make+config ../../$(CCAN_DIR) $(CCAN_MODULES)
	rm -rf tmp_ccan/
	cat $(CCAN_DIR)/Makefile-ccan | sed 's/CCAN_CFLAGS *=/override CCAN_CFLAGS +=/' > $(CCAN_DIR)/tmp.mk \
		&& cp $(CCAN_DIR)/tmp.mk $(CCAN_DIR)/Makefile-ccan && rm $(CCAN_DIR)/tmp.mk

$(LIBCCAN): $(CCAN_DIR)
	cd $(CCAN_DIR) && $(MAKE) $(MFLAGS) 

define test-program-template
$$(BUILD)/$(1): $$(BUILD)/$(1).o $$(LIBCCAN) $$(OUTPUT)
	$(CXX_CMD) $$+ -o $$@

$(1): $$(BUILD)/$(1)
	$(BUILD)/$(1) 
endef

.PHONY: run-tests
run-tests: tests $(foreach test, $(TEST_OUTPUTS), $(notdir $(test) ) )

.PHONY: tests
tests: $(TESTS)

.PHONY: $(TESTS)
$(foreach test, $(TESTS), $(eval $(call test-program-template,$(test)) ) )

.PHONY: libclean
libclean: clean
	rm -rf $(CCAN_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD)
	rm -f $(OUTPUT)

-include $(wildcard $(BUILD)/*.d )
