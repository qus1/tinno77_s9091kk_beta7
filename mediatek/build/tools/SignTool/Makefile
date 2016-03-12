ifneq ($(STANDALONE_BUILD),yes)
include ../../../build/Makefile
include ../../../../$(MTK_ROOT_CONFIG_OUT)/ProjectConfig.mk
endif

#//<20121016-15224-Eric Lin, [secu] Implement the signtool related functions.
DEFINE           = 	-D__SECURE_USB_DOWNLOAD__\
	-D__MOTO_USB_DOWNLOAD__  \
	-DOPENSSL_NO_SOCK        \
	-DOPENSSL_NO_BIO         \
	-DOPENSSL_NO_LHASH       \
	-DOPENSSL_NO_ERR         \
	-DOPENSSL_NO_CMS         \
	-DOPENSSL_NO_ENGINE      \
	-DOPENSSL_NO_EC          \
	-DOPENSSL_NO_ECDSA       \
	-DOPENSSL_NO_ECDH        \
	-DOPENSSL_NO_DSA         \
	-DOPENSSL_NO_DH          \
	-DOPENSSL_NO_LOCKING  \
	-DOPENSSL_SMALL_FOOTPRINT
#export CFLAGS := -I inc
export CFLAGS := -I inc -I openssl/include $(DEFINE)
#//>20121016-15224-Eric Lin
export SECURE_LIB := 
export COMMON_LIB := obj/SignLib.a
export EXTRA_CLEAN := 
export SEC_WRAPPER_IMPL := 
export EXTRA_TARGET :=
export OBJ_DIR := obj
OUTPUT_LINK := SignTool

all: precheck build_info extra SignTool

#//<20121016-15224-Eric Lin, [secu] Implement the signtool related functions.
#ifeq ($(CUSTOM_SEC_SIGNTOOL_SUPPORT),yes)
#ifeq ($(STANDALONE_BUILD),yes)
#    SIGN_TOOL_CUSTOM_FOLDER := auth/private	
#else
#    SIGN_TOOL_CUSTOM_FOLDER := ../../../../$(MTK_ROOT_CUSTOM_OUT)/security/signtool
#endif
    OUTPUT_BIN := SignTool_Cust
    DEL_BIN := SignTool_MTK
#    EXTRA_CLEAN :=
#    $(shell rm -rf custom)
#    $(shell \
#        if [ -d $(SIGN_TOOL_CUSTOM_FOLDER) ]; then \
#            ln -s $(SIGN_TOOL_CUSTOM_FOLDER) custom ; \
#        fi; \
    )
    include custom/Makefile
#else
#    OUTPUT_BIN := SignTool_MTK
#    DEL_BIN := SignTool_Cust
#    include auth/Makefile
#endif
#//>20121106-16328-Eric Lin
#//>20121016-15224-Eric Lin

#//<20121016-15224-Eric Lin, [secu] Implement the signtool related functions.
################################################################################################################
##{ARIMA
## SignTool: $(OBJ_DIR)/main.o  $(CUSTOM_OPENSSL_LIST) $(SEC_WRAPPER_IMPL) $(SECURE_LIB)
## 	gcc -m32 obj/main.o    $(SEC_WRAPPER_IMPL) $(SECURE_LIB) $(COMMON_LIB) -o $(OUTPUT_BIN)
## 	ln -s $(OUTPUT_BIN) $(OUTPUT_LINK)
## 
## $(OBJ_DIR)/main.o: core/main.c
## 	gcc -m32 -c core/main.c -o $(OBJ_DIR)/main.o $(CFLAGS)
## 
## 


CUSTOM_OPENSSL_LIST := obj/bn_recp.o obj/bn_exp.o obj/bn_mont.o obj/bn_shift.o obj/rsa_eay.o obj/bn_sqr.o obj/bn_mod.o obj/bn_gf2m.o obj/bn_ctx.o obj/lhash.o obj/bn_div.o obj/rsa_lib.o obj/rand_lib.o obj/mem_clr.o obj/bn_lib.o obj/bn_add.o obj/rsa_pk1.o obj/bn_rand.o obj/ex_data.o obj/bn_gcd.o obj/bget.o obj/bn_mul.o obj/rsa_test.o obj/stack.o obj/bn_word.o obj/bn_nist.o obj/bn_asm.o obj/sha1.o obj/sha2.o

SignTool: $(OBJ_DIR)/main.o $(CUSTOM_OPENSSL_LIST) $(SEC_WRAPPER_IMPL) $(SECURE_LIB) $(COMMON_LIB)
	gcc -m32 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 $(OBJ_DIR)/main.o $(CUSTOM_OPENSSL_LIST) $(SEC_WRAPPER_IMPL) $(SECURE_LIB) $(COMMON_LIB) -o $(OUTPUT_BIN)
	@echo [Dependency] $(CURDIR)/$(SECURE_LIB)
	@if [ -f $(OUTPUT_LINK) ]; then rm $(OUTPUT_LINK); fi
	ln -s $(OUTPUT_BIN) $(OUTPUT_LINK)

$(OBJ_DIR)/main.o: core/main.c
	gcc -MD -MF dep/main.dep -m32 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -c core/main.c -o $(OBJ_DIR)/main.o $(CFLAGS)

$(OBJ_DIR)/bn_recp.o: openssl/bn_recp.c
	gcc -m32 -c openssl/bn_recp.c -o $(OBJ_DIR)/bn_recp.o $(CFLAGS)

$(OBJ_DIR)/bn_exp.o: openssl/bn_exp.c
	gcc -m32 -c openssl/bn_exp.c -o $(OBJ_DIR)/bn_exp.o $(CFLAGS)

$(OBJ_DIR)/bn_mont.o: openssl/bn_mont.c
	gcc -m32 -c openssl/bn_mont.c -o $(OBJ_DIR)/bn_mont.o $(CFLAGS)

$(OBJ_DIR)/bn_shift.o: openssl/bn_shift.c
	gcc -m32 -c openssl/bn_shift.c -o $(OBJ_DIR)/bn_shift.o $(CFLAGS)
	
$(OBJ_DIR)/rsa_eay.o: openssl/rsa_eay.c
	gcc -m32 -c openssl/rsa_eay.c -o $(OBJ_DIR)/rsa_eay.o $(CFLAGS)

$(OBJ_DIR)/bn_sqr.o: openssl/bn_sqr.c
	gcc -m32 -c openssl/bn_sqr.c -o $(OBJ_DIR)/bn_sqr.o $(CFLAGS)

$(OBJ_DIR)/bn_mod.o: openssl/bn_mod.c
	gcc -m32 -c openssl/bn_mod.c -o $(OBJ_DIR)/bn_mod.o $(CFLAGS)

$(OBJ_DIR)/bn_gf2m.o: openssl/bn_gf2m.c
	gcc -m32 -c openssl/bn_gf2m.c -o $(OBJ_DIR)/bn_gf2m.o $(CFLAGS)

$(OBJ_DIR)/bn_ctx.o: openssl/bn_ctx.c
	gcc -m32 -c openssl/bn_ctx.c -o $(OBJ_DIR)/bn_ctx.o $(CFLAGS)

$(OBJ_DIR)/lhash.o: openssl/lhash.c
	gcc -m32 -c openssl/lhash.c -o $(OBJ_DIR)/lhash.o $(CFLAGS)

$(OBJ_DIR)/bn_div.o: openssl/bn_div.c
	gcc -m32 -c openssl/bn_div.c -o $(OBJ_DIR)/bn_div.o $(CFLAGS)

$(OBJ_DIR)/bn_div.o: openssl/bn_div.c
	gcc -m32 -c openssl/bn_div.c -o $(OBJ_DIR)/bn_div.o $(CFLAGS)

$(OBJ_DIR)/rsa_lib.o: openssl/rsa_lib.c
	gcc -m32 -c openssl/rsa_lib.c -o $(OBJ_DIR)/rsa_lib.o $(CFLAGS)

$(OBJ_DIR)/rand_lib.o: openssl/rand_lib.c
	gcc -m32 -c openssl/rand_lib.c -o $(OBJ_DIR)/rand_lib.o $(CFLAGS)

$(OBJ_DIR)/mem_clr.o: openssl/mem_clr.c
	gcc -m32 -c openssl/mem_clr.c -o $(OBJ_DIR)/mem_clr.o $(CFLAGS)

$(OBJ_DIR)/bn_lib.o: openssl/bn_lib.c
	gcc -m32 -c openssl/bn_lib.c -o $(OBJ_DIR)/bn_lib.o $(CFLAGS)
	
$(OBJ_DIR)/bn_add.o: openssl/bn_add.c
	gcc -m32 -c openssl/bn_add.c -o $(OBJ_DIR)/bn_add.o $(CFLAGS)

$(OBJ_DIR)/rsa_pk1.o: openssl/rsa_pk1.c
	gcc -m32 -c openssl/rsa_pk1.c -o $(OBJ_DIR)/rsa_pk1.o $(CFLAGS)

$(OBJ_DIR)/bn_rand.o: openssl/bn_rand.c
	gcc -m32 -c openssl/bn_rand.c -o $(OBJ_DIR)/bn_rand.o $(CFLAGS)

$(OBJ_DIR)/ex_data.o: openssl/ex_data.c
	gcc -m32 -c openssl/ex_data.c -o $(OBJ_DIR)/ex_data.o $(CFLAGS)

$(OBJ_DIR)/bn_gcd.o: openssl/bn_gcd.c
	gcc -m32 -c openssl/bn_gcd.c -o $(OBJ_DIR)/bn_gcd.o $(CFLAGS)
	
$(OBJ_DIR)/bget.o: openssl/bget.c
	gcc -m32 -c openssl/bget.c -o $(OBJ_DIR)/bget.o $(CFLAGS)

$(OBJ_DIR)/bn_mul.o: openssl/bn_mul.c
	gcc -m32 -c openssl/bn_mul.c -o $(OBJ_DIR)/bn_mul.o $(CFLAGS)

$(OBJ_DIR)/rsa_test.o: openssl/rsa_test.c
	gcc -m32 -c openssl/rsa_test.c -o $(OBJ_DIR)/rsa_test.o $(CFLAGS)

$(OBJ_DIR)/stack.o: openssl/stack.c
	gcc -m32 -c openssl/stack.c -o $(OBJ_DIR)/stack.o $(CFLAGS)

$(OBJ_DIR)/bn_word.o: openssl/bn_word.c
	gcc -m32 -c openssl/bn_word.c -o $(OBJ_DIR)/bn_word.o $(CFLAGS)

$(OBJ_DIR)/bn_nist.o: openssl/bn_nist.c
	gcc -m32 -c openssl/bn_nist.c -o $(OBJ_DIR)/bn_nist.o $(CFLAGS)

$(OBJ_DIR)/bn_asm.o: openssl/bn_asm.c
	gcc -m32 -c openssl/bn_asm.c -o $(OBJ_DIR)/bn_asm.o $(CFLAGS)

$(OBJ_DIR)/sha1.o: openssl/sha1.c
	gcc -m32 -c openssl/sha1.c -o $(OBJ_DIR)/sha1.o $(CFLAGS)

$(OBJ_DIR)/sha2.o: openssl/sha2.c
	gcc -m32 -c openssl/sha2.c -o $(OBJ_DIR)/sha2.o $(CFLAGS)
	
#}ARIMA
################################################################################################################
#//>20121016-15224-Eric Lin
build_info:
	@echo //building time information > inc/build_info.h
	@echo '#'define BUILD_TIME '"'$(shell date)'"' >> inc/build_info.h

precheck: \
	$(shell \
	if [ -f $(DEL_BIN) ]; then \
		echo "clean" ; \
	else \
		echo ""; \
	fi; \
	) 
	@mkdir -p obj
	@mkdir -p dep

clean:
	rm -rf $(OBJ_DIR)/*o $(EXTRA_CLEAN) $(OUTPUT_LINK) SignTool_MTK SignTool_Cust

$(info [Dependency] $(addprefix $(CURDIR)/,$(MAKEFILE_LIST)))
