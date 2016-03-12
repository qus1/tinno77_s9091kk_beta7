
SEC_WRAPPER_IMPL := $(OBJ_DIR)/cust_auth.o

extra:

#//<2012/12/14-18741-EricLin, [secu] Fix sign tool issue.
$(SEC_WRAPPER_IMPL): custom/cust_auth.c
	gcc -m32 -c -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64  custom/cust_auth.c -o $(SEC_WRAPPER_IMPL) $(CFLAGS)
#//>2012/12/14-18741-EricLin

