/********************************************************************************/
/*										*/
/*			   Load 						*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: load.c 1324 2018-08-31 16:36:12Z kgoldman $			*/
/*										*/
/* (c) Copyright IBM Corporation 2015 - 2019.					*/
/*										*/
/* All rights reserved.								*/
/* 										*/
/* Redistribution and use in source and binary forms, with or without		*/
/* modification, are permitted provided that the following conditions are	*/
/* met:										*/
/* 										*/
/* Redistributions of source code must retain the above copyright notice,	*/
/* this list of conditions and the following disclaimer.			*/
/* 										*/
/* Redistributions in binary form must reproduce the above copyright		*/
/* notice, this list of conditions and the following disclaimer in the		*/
/* documentation and/or other materials provided with the distribution.		*/
/* 										*/
/* Neither the names of the IBM Corporation nor the names of its		*/
/* contributors may be used to endorse or promote products derived from		*/
/* this software without specific prior written permission.			*/
/* 										*/
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		*/
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		*/
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR	*/
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT		*/
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	*/
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		*/
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,	*/
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY	*/
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		*/
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE	*/
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		*/
/********************************************************************************/

/* 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <ibmtss/tss.h>
#include <ibmtss/tssutils.h>
#include <ibmtss/tssresponsecode.h>
#include <ibmtss/Unmarshal_fp.h>

#include "load.h"

extern int load(int argc, char *argv[])
{
    TPM_RC			rc = 0;
    int				i;    /* argc iterator */
    TSS_CONTEXT			*tssContext = NULL;
    Load_In 			in;
    Load_Out 			out;
    TPMI_DH_OBJECT		parentHandle = 0;
    const char			*publicKeyFilename = NULL;
    const char			*privateKeyFilename = NULL;
    const char			*parentPassword = NULL; 
    TPMI_SH_AUTH_SESSION    	sessionHandle0 = TPM_RS_PW;
    unsigned int		sessionAttributes0 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle1 = TPM_RH_NULL;
    unsigned int		sessionAttributes1 = 0;
    TPMI_SH_AUTH_SESSION    	sessionHandle2 = TPM_RH_NULL;
    unsigned int		sessionAttributes2 = 0;
    int tssUtilsVerbose = FALSE;

	FILE *keyHandle_file;

    setvbuf(stdout, 0, _IONBF, 0);      /* output may be going through pipe to log file */
    TSS_SetProperty(NULL, TPM_TRACE_LEVEL, "1");
    
    
    for (i=1 ; (i<argc) && (rc == 0) ; i++) {
	if (strcmp(argv[i],"-hp") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &parentHandle);
	    }
	    else {
		printf("Missing parameter for -hp\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-pwdp") == 0) {
	    i++;
	    if (i < argc) {
		parentPassword = argv[i];
	    }
	    else {
		printf("-pwdp option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-ipu") == 0) {
	    i++;
	    if (i < argc) {
		publicKeyFilename = argv[i];
	    }
	    else {
		printf("-ipu option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-ipr") == 0) {
	    i++;
	    if (i < argc) {
		privateKeyFilename = argv[i];
	    }
	    else {
		printf("-ipr option needs a value\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-se0") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionHandle0);
	    }
	    else {
		printf("Missing parameter for -se0\n");
		printUsage();
	    }
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionAttributes0);
		if (sessionAttributes0 > 0xff) {
		    printf("Out of range session attributes for -se0\n");
		    printUsage();
		}
	    }
	    else {
		printf("Missing parameter for -se0\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-se1") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionHandle1);
	    }
	    else {
		printf("Missing parameter for -se1\n");
		printUsage();
	    }
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionAttributes1);
		if (sessionAttributes1 > 0xff) {
		    printf("Out of range session attributes for -se1\n");
		    printUsage();
		}
	    }
	    else {
		printf("Missing parameter for -se1\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-se2") == 0) {
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionHandle2);
	    }
	    else {
		printf("Missing parameter for -se2\n");
		printUsage();
	    }
	    i++;
	    if (i < argc) {
		sscanf(argv[i],"%x", &sessionAttributes2);
		if (sessionAttributes2 > 0xff) {
		    printf("Out of range session attributes for -se2\n");
		    printUsage();
		}
	    }
	    else {
		printf("Missing parameter for -se2\n");
		printUsage();
	    }
	}
	else if (strcmp(argv[i],"-h") == 0) {
	    printUsage();
	}
	else if (strcmp(argv[i],"-v") == 0) {
	    tssUtilsVerbose = TRUE;
	    TSS_SetProperty(NULL, TPM_TRACE_LEVEL, "2");
	}
	else {
	    printf("\n%s is not a valid option\n", argv[i]);
	    printUsage();
	}
    }
    if (parentHandle == 0) {
	printf("Missing handle parameter -hp\n");
	printUsage();
    }
    if (privateKeyFilename == NULL) {
	printf("Missing private key parameter -ipr\n");
	printUsage();
    }
    if (publicKeyFilename == NULL) {
	printf("Missing private key parameter -ipu\n");
	printUsage();
    }
    if (rc == 0) {
	rc = TSS_File_ReadStructure(&in.inPrivate,
				    (UnmarshalFunction_t)TSS_TPM2B_PRIVATE_Unmarshalu,
				    privateKeyFilename);
    }
    if (rc == 0) {
	rc = TSS_File_ReadStructureFlag(&in.inPublic,
					(UnmarshalFunctionFlag_t)TSS_TPM2B_PUBLIC_Unmarshalu,
					FALSE,			/* NULL not permitted */
					publicKeyFilename);
    }
    if (rc == 0) {
	in.parentHandle = parentHandle;
    }
    /* Start a TSS context */
    if (rc == 0) {
	rc = TSS_Create(&tssContext);
    }
    /* call TSS to execute the command */
    if (rc == 0) {
	rc = TSS_Execute(tssContext,
			 (RESPONSE_PARAMETERS *)&out,
			 (COMMAND_PARAMETERS *)&in,
			 NULL,
			 TPM_CC_Load,
			 sessionHandle0, parentPassword, sessionAttributes0,
			 sessionHandle1, NULL, sessionAttributes1,
			 sessionHandle2, NULL, sessionAttributes2,
			 TPM_RH_NULL, NULL, 0);
    }
    {
	TPM_RC rc1 = TSS_Delete(tssContext);
	if (rc == 0) {
	    rc = rc1;
	}
    }
    if (rc == 0) {
	//printf("Handle %08x\n", out.objectHandle);
	keyHandle_file = fopen("src-prikey-handle.txt", "w");
	fprintf(keyHandle_file, "%08x", out.objectHandle);
	fclose(keyHandle_file);
	if (tssUtilsVerbose) printf("load: success\n");
    }
    else {
	const char *msg;
	const char *submsg;
	const char *num;
	printf("load: failed, rc %08x\n", rc);
	TSS_ResponseCode_toString(&msg, &submsg, &num, rc);
	printf("%s%s%s\n", msg, submsg, num);
	rc = EXIT_FAILURE;
    }
    return rc;
}

static void printUsage(void)
{
    printf("\n");
    printf("load\n");
    printf("\n");
    printf("Runs TPM2_Load\n");
    printf("\n");
    printf("\t-hp\tparent handle\n");
    printf("\t[-pwdp\tpassword for parent key (default empty)]\n");
    printf("\t-ipu\tpublic key file name\n");
    printf("\t-ipr\tprivate key file name\n");
    printf("\n");
    printf("\t-se[0-2] session handle / attributes (default PWAP)\n");
    printf("\t01\tcontinue\n");
    printf("\t20\tcommand decrypt\n");
    printf("\t40\tresponse encrypt\n");
    exit(1);	
}
