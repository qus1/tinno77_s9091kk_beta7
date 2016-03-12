
SEC_WRAPPER_IMPL := $(OBJ_DIR)/cust_auth.o

extra:

$(SEC_WRAPPER_IMPL): custom/cust_auth.c
	gcc -m32 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -c custom/cust_auth.c -o $(SEC_WRAPPER_IMPL) $(CFLAGS)

