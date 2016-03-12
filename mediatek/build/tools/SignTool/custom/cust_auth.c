/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2011. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */


//<20120126-2882-Eric Lin, [secu] The sign tool porting in Linux.

#include <stdio.h>
#include <string.h>


#include <build_info.h>
#include <lib_sign_export.h>
#include <type_defs.h>
#include "../openssl/include/kal_release.h"
#include "../openssl/include/sha1.h"
#include "../openssl/include/rsa.h"



#define 	CUST_RETURN_TRUE		0
#define DUMP_DETAILS_MSG
#define NOT_REMOVE_OPENSSL_FUNC_TEMP


#define SHA1_HASH_LEN       (20)
#define SHA256_HASH_LEN     (32)
#define RSA1024_SIG_LEN     (128)
#define RSA2048_SIG_LEN     (256)


extern unsigned char cust_key_store_e[];
extern unsigned char cust_key_store_n[];
extern unsigned char cust_key_store_d[];
extern void main_openssl(void);
extern int RSA_eay_public_decrypt_ex(int flen, const unsigned char *from,unsigned char *to, RSA *rsa, int padding);


RSA		*custom_test_key;
SHA1Context cust_sha;                /* SHA-1 context                 */

void do_sha1_proc(unsigned char* data_buf,  unsigned int data_len)
{
	char        c;                  /* Character read from file      */
	unsigned int i;
	/*
	 *  Reset the SHA-1 context and process input
	 */
	SHA1Reset(&cust_sha);
	for(i=0;i<data_len;i++)
		SHA1Input(&cust_sha, &data_buf[i], 1);
	if (!SHA1Result(&cust_sha))
	{
		MSG("[SECU_SIGNTOOL]sha: could not compute message digest.\n");
	}
	else
	{
		MSG( "[SECU_SIGNTOOL]%08X %08X %08X %08X %08X" ,
				cust_sha.Message_Digest[0],
				cust_sha.Message_Digest[1],
				cust_sha.Message_Digest[2],
				cust_sha.Message_Digest[3],
				cust_sha.Message_Digest[4]);
	}
	MSG( "\n");
}

// only support 2 digit
int cust_string2hex (unsigned char *instring)
{
	unsigned char *p;
	int i=0,result=0;
	const unsigned char baseTable[]={16,1};

	p = instring;
	for(i=0;i<2;i++)
	{
		if(*p >='0' && *p <='9')
			result |= (*p-'0')*baseTable[i];
		else if(*p >='A' && *p <='Z')
			result |= (*p-'A'+10)*baseTable[i]; 
		else if(*p >='a' && *p <='z')
			result |= (*p-'a'+10)*baseTable[i];
		p++;
	}

	//MSG("%02x",(unsigned char)result); 
	return result;
}

/**************************************************************************
 *  AUTH KEY INIT
 **************************************************************************/
void cust_init_key (unsigned char *key_rsa_n, unsigned int nKey_len,
                        unsigned char *key_rsa_d, unsigned int dKey_len,
                        unsigned char *key_rsa_e, unsigned int eKey_len)
{
	unsigned char	need2cov[2];
	int i=0,ken_len_n=0,chkbase=0;


#ifdef NOT_REMOVE_OPENSSL_FUNC_TEMP	
	MSG("[SECU_SIGNTOOL]cust_init_key -enter\n"); 

    /* TODO : the key will pass in, need to judge if the key is for RSA1024 or RSA2048 and save it for later use */    
    /* customer needs to customized this function */
#ifdef DUMP_DETAILS_MSG
    MSG("\n[SECU_SIGNTOOL]===========Dump data start==============================\n");
    MSG("	[SECU_SIGNTOOL]key_rsa_n len :%d , key_rsa_n \r\n",nKey_len);
    for(i=0;i<nKey_len;i++)
    {
   	MSG("%c",key_rsa_n[i]);    
   	//MSG(" 0x%x,",key_rsa_n[i]);
    }
    MSG("\r\n");	
    MSG("	[SECU_SIGNTOOL]key_rsa_d len :%d , key_rsa_d \r\n",dKey_len);
    for(i=0;i<dKey_len;i++)
    {
   	MSG("%c",key_rsa_d[i]);    
   	//MSG(" 0x%x,",key_rsa_d[i]);
    }
    MSG("\r\n");	
    MSG("	[SECU_SIGNTOOL]key_rsa_e len :%d , key_rsa_e \r\n",eKey_len);
    for(i=0;i<eKey_len;i++)
    {
   	//MSG(" 0x%x,",key_rsa_e[i]);
   	MSG("%c",key_rsa_e[i]);
    }
    MSG("\r\n");		
    MSG("[SECU_SIGNTOOL]=============Dump data end============================\n\n");    
#endif


	memset(cust_key_store_n,0,256);
	memset(cust_key_store_d,0,256);
	memset(cust_key_store_e,0,256);


#if 1

	//convert string to hex -N
	//unsigned char cust_key_store_e[256] = {0};
	
	chkbase = 256-(nKey_len>>1);
#if 1
	ken_len_n = nKey_len>>1;
	for(i=0;i<ken_len_n;i++)
	{
		need2cov[0] = key_rsa_n[(i<<1)+0];
		need2cov[1] = key_rsa_n[(i<<1)+1];
		need2cov[2] = 0;
		cust_key_store_n[chkbase+i] = cust_string2hex(need2cov);
	}
	cust_key_store_n[chkbase+i] =0;
#endif
	MSG("\n"); 
#if 1
	//convert string to hex -D
	//unsigned char cust_key_store_d[256] = {0};
	ken_len_n = dKey_len>>1;
	for(i=0;i<ken_len_n;i++)
	{
		need2cov[0] = key_rsa_d[(i<<1)+0];
		need2cov[1] = key_rsa_d[(i<<1)+1];
		need2cov[2] = 0;
		cust_key_store_d[chkbase+i] = cust_string2hex(need2cov);
	}
	cust_key_store_d[chkbase+i] =0;
#endif
	MSG("\n"); 
	//convert string to hex -E
	//unsigned char cust_key_store_n[16] = {0};	
#if 1
	/*
	ken_len_n = eKey_len>>1;
	for(i=0;i<ken_len_n;i++)
	{
		need2cov[0] = key_rsa_e[(i<<1)+0];
		need2cov[1] = key_rsa_e[(i<<1)+1];
		need2cov[2] = 0;
		cust_key_store_e[chkbase+i] = cust_string2hex(need2cov);
	}
	cust_key_store_e[chkbase+i] =0;
	*/
	cust_key_store_e[255] = 0x01 ;
	cust_key_store_e[254] = 0x00;
	cust_key_store_e[253] = 0x01;	
#endif

#endif

	//MSG("main_openssl enter\n"); 
	//main_openssl();
	//MSG("main_openssl exit\n"); 

	// create bget buffer
       create_bget_buf_pool();
	 MSG("[SECU_SIGNTOOL]create_bget_buf_pool\n");    	
	 
	 custom_test_key = RSA_new();
	 MSG("[SECU_SIGNTOOL]RSA_new\n");    	
	 
	 key_mtk_def_img(custom_test_key, 0/*ctext_ex*/);
	 MSG("[SECU_SIGNTOOL]key_mtk_def_img\n");    	

#if 1
	MSG("\n[SECU_SIGNTOOL]NEW key assign\n");
	MSG("\n[SECU_SIGNTOOL]cust_key_store_d[%d]:\n",dKey_len);
	for(i=0;i<256;i++)
	{
		MSG("%02x",(unsigned char)cust_key_store_d[i]); 
	}
	MSG("\n[SECU_SIGNTOOL]cust_key_store_n[%d]:\n",nKey_len);
	for(i=0;i<256;i++)
	{
		MSG("%02x",(unsigned char)cust_key_store_n[i]); 
	}	
	MSG("\n[SECU_SIGNTOOL]cust_key_store_e[%d]:\n",eKey_len);	
	for(i=0;i<256;i++)
	{
		MSG("%02x",(unsigned char)cust_key_store_e[i]); 
	}		
	MSG("\n");	
#endif

#endif
    return;
}

/**************************************************************************
 *  HASH (should support SHA1 and SHA256)
 **************************************************************************/     
int cust_hash (unsigned char* data_buf,  unsigned int data_len, unsigned char* hash_buf, unsigned int hash_len)
{
	int i,countNumber,shalen,remind_num;
	unsigned int realsize;

	MSG("[SECU_SIGNTOOL]cust_hash -enter\n");    	
#ifdef NOT_REMOVE_OPENSSL_FUNC_TEMP		
    memset(hash_buf, 0x00, hash_len);
    
    if(SHA1_HASH_LEN == hash_len)
    {
        /* =============== */
        /* SHA1            */
        /* =============== */
        
        /* TODO : use cust_hash to generate hash value */    
        /* customer needs to customized this function */
		//MSG("sha1_custom_test [%d]-hash input:\n",SHA1_HASH_LEN);
		//for(i=0;i<data_len;i++)
		//	MSG("%c",data_buf[i]);
		//MSG(" \n");

		//under construction...
		do_sha1_proc(data_buf,data_len);
		//under construction...
	   for(i = 0; i < 5 ; i++)
	   {
			hash_buf[4*i+0] = (unsigned char)(cust_sha.Message_Digest[i]>>24);
			hash_buf[4*i+1] = (unsigned char)(cust_sha.Message_Digest[i]>>16);
			hash_buf[4*i+2] = (unsigned char)(cust_sha.Message_Digest[i]>>8);
			hash_buf[4*i+3] = (unsigned char)(cust_sha.Message_Digest[i]);
		}

		MSG("[SECU_SIGNTOOL]sha1_custom_test-hash output[%d]:\n",SHA1_HASH_LEN);
		for(i=0;i<20;i++)
			MSG("%x",(unsigned char)hash_buf[i]);
	  MSG(" \n");
	  return CUST_RETURN_TRUE;
    }
    else if(SHA256_HASH_LEN == hash_len)
    {
        /* =============== */
        /* SHA256          */
        /* =============== */
        
        /* TODO : use cust_hash to generate hash value */    
        /* customer needs to customized this function */

	//MSG("sha256_custom_test [%d]-hash input:\n",SHA256_HASH_LEN);
	//for(i=0;i<data_len;i++)
	//	MSG("%c",data_buf[i]);
	//MSG(" \n");

//under construction...
	sha256((const unsigned char *) data_buf, data_len, hash_buf);
//under construction...

	MSG("[SECU_SIGNTOOL]sha256_custom_test [%d]-hash output:\n",SHA256_HASH_LEN);
	for(i=0;i<SHA256_HASH_LEN;i++)
		MSG("%x",(unsigned char)hash_buf[i]);
	MSG(" \n");

	 return CUST_RETURN_TRUE;
    }
    else
    {
        /* unsupported, just return hash failed */
		MSG("[SECU_SIGNTOOL]cust_hash other case return -1\n");  
        return -1;
    }    
#else
	 return CUST_RETURN_TRUE;
#endif	
}

/**************************************************************************
 *  RSA Sign (should support RSA1024 and RSA2048)
 **************************************************************************/     
int cust_sign(unsigned char* data_buf,  unsigned int data_len, unsigned char* sig_buf, unsigned int sig_len)
{
	int i,num;

	MSG("[SECU_SIGNTOOL]cust_sign -enter\n");    	

#ifdef NOT_REMOVE_OPENSSL_FUNC_TEMP	
    memset(sig_buf, 0x00, sig_len);
    
    if(RSA1024_SIG_LEN == sig_len)
    {
        unsigned char sha1sum[SHA1_HASH_LEN] = {0};

        /* SHA1 */        
        if( cust_hash(data_buf, data_len, sha1sum, SHA1_HASH_LEN) != 0 )
        {
            /* return sign failed */
			MSG("[SECU_SIGNTOOL]cust_sign hash len return -1\n");  
            return -1;

        }
    
        /* TODO : use cust_sign to sign data buffer with RSA1024 */    
        /* customer needs to customized this function */

	num = RSA_private_encrypt(SHA1_HASH_LEN, sha1sum, sig_buf, custom_test_key, RSA_PKCS1_PADDING);
	if (num != 128)
	{
		MSG("[SECU_SIGNTOOL]RSA_private_encrypt fail:%x\n",num);  
		return -1; 
	}
	else
	{
		MSG("[SECU_SIGNTOOL][1024]RSA_private_encrypt  success:%x\n",num);  	
		MSG("[SECU_SIGNTOOL]cust_sign sig_buf output[%d]:\n",num);
		for(i=0;i<num;i++)
			MSG("%x",(unsigned char)sig_buf[i]);
		MSG(" \n");
		return CUST_RETURN_TRUE;
	}
    }
    else if(RSA2048_SIG_LEN == sig_len)
    {
        unsigned char sha256sum[SHA256_HASH_LEN] = {0};

        /* SHA256 */      
        if( cust_hash(data_buf, data_len, sha256sum, SHA256_HASH_LEN) != 0 )
        {
            /* return sign failed */
			MSG("[SECU_SIGNTOOL]cust_sign hash len return -1\n");  
            return -1;
        }
        
        /* TODO : use cust_sign to sign data buffer with RSA2048*/    
        /* customer needs to customized this function */
		num = RSA_private_encrypt(SHA256_HASH_LEN, sha256sum, sig_buf, custom_test_key, RSA_PKCS1_PADDING);
		if (num != 256)
		{
			MSG("[SECU_SIGNTOOL][2048]cust_sign RSA_private_encrypt return -1. num:%x\n",num);  
			return -1; 
		}
		else
		{
			MSG("[SECU_SIGNTOOL][2048]cust_sign RSA_private_encrypt return pass\n");  
			MSG("[SECU_SIGNTOOL]cust_sign sig_buf output[%d]:\n",num);
			for(i=0;i<num;i++)
				MSG("%x",(unsigned char)sig_buf[i]);
			MSG(" \n");
			return CUST_RETURN_TRUE;
		}	
	
    }
    else
    {
        /* unsupported, just return sign failed */
		MSG("[SECU_SIGNTOOL]cust_sign other case return -1\n");  
        return -1;
    }
#else
	 return CUST_RETURN_TRUE;
#endif 
}

/**************************************************************************
 *  RSA Verify (should support RSA1024 and RSA2048)
 **************************************************************************/     
int cust_verify (unsigned char* data_buf,  unsigned int data_len, unsigned char* sig_buf, unsigned int sig_len)
{    
	int num,i;	
	unsigned char verfy_buf[32]={0};



	MSG("[SECU_SIGNTOOL]cust_verify enter\n");  

#ifdef NOT_REMOVE_OPENSSL_FUNC_TEMP

    if(RSA1024_SIG_LEN == sig_len)
    {
        unsigned char sha1sum[SHA1_HASH_LEN] = {0};

        /* SHA1 */        
        if( cust_hash(data_buf, data_len, sha1sum, SHA1_HASH_LEN) != 0 )
        {
            /* return verify failed */
			MSG("[SECU_SIGNTOOL]cust_verify hash len return -1\n");  
            return -1;
        }
    
		num = RSA_eay_public_decrypt_ex(sig_len, (const unsigned char *)sig_buf,
										(unsigned char *)verfy_buf, custom_test_key,
										RSA_PKCS1_PADDING);
 	    MSG("	[SECU_SIGNTOOL]numlen :%d , RSA_eay_public_decrypt_ex -data_buf\r\n",num);
 	    for(i=0;i<num;i++)
 	    {
 	   	MSG(" 0x%x,",data_buf[i]);
 	    }
	    MSG(" \n");	
 	    MSG("[SECU_SIGNTOOL]	numlen :%d , RSA_eay_public_decrypt_ex - verfy_buf\r\n",num);
 	    for(i=0;i<num;i++)
 	    {
 	   	MSG(" 0x%x,",verfy_buf[i]);
 	    }
	    MSG("\n");		

		if (memcmp(data_buf, (unsigned char*)verfy_buf, 20) != 0)
		{
			MSG("[SECU_SIGNTOOL]cust_verify RSA_eay_public_decrypt_ex fail\n");
			return -1;
		}
		else
		{
			MSG("[SECU_SIGNTOOL]cust_verify RSA_eay_public_decrypt_ex pass\n");
			return 0;
		}		
        /* TODO : use cust_verify to verify data buffer with RSA1024 */    
        /* customer needs to customized this function */
    }
    else if(RSA2048_SIG_LEN == sig_len)
    {
        unsigned char sha256sum[SHA256_HASH_LEN] = {0};

        /* SHA256 */      
        if( cust_hash(data_buf, data_len, sha256sum, SHA256_HASH_LEN) != 0 )
        {
            /* return verify failed */
			MSG("[SECU_SIGNTOOL]cust_verify sha256 len return -1\n");  
            return -1;
        }
        
        /* TODO : use cust_verify to verify data buffer with RSA2048*/    
        /* customer needs to customized this function */
		num = RSA_eay_public_decrypt_ex(sig_len, (const unsigned char *)sig_buf,
										(unsigned char *)verfy_buf, custom_test_key,
										RSA_PKCS1_PADDING);

 	    MSG("	[SECU_SIGNTOOL]numlen :%d , RSA_eay_public_decrypt_ex -data_buf\r\n",num);
 	    for(i=0;i<num;i++)
 	    {
 	   	MSG(" 0x%x,",data_buf[i]);
 	    }
	    MSG(" \n");	
 	    MSG("	[SECU_SIGNTOOL]numlen :%d , RSA_eay_public_decrypt_ex - verfy_buf\r\n",num);
 	    for(i=0;i<num;i++)
 	    {
 	   	MSG(" 0x%x,",verfy_buf[i]);
 	    }
	    MSG("\n");		
		if (memcmp(data_buf, (unsigned char*)verfy_buf, 32) != 0)
		{
			MSG("[SECU_SIGNTOOL]cust_verify RSA_eay_public_decrypt_ex fail\n");
			return -1;
		}
		else
		{
			MSG("[SECU_SIGNTOOL]cust_verify RSA_eay_public_decrypt_ex pass\n");
			return 0;
		}		
    }
    else
    {
        /* unsupported, just return verify failed */
		MSG("[SECU_SIGNTOOL]cust_verify other case return -1\n");  
        return -1;
    }
#else
	 return CUST_RETURN_TRUE;
#endif 	
}
//>20120126-2882-Eric Lin

