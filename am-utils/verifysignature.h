#ifndef VERIFYSIGNATURE_H
#define VERIFYSIGNATURE_H

extern int verifysignature(int argc, char *argv[], unsigned char *data, uint64_t const dataLen, unsigned char *signature, uint64_t const sigLen);
static void printUsage(void);

#endif