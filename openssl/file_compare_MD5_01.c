/* compare two files with MD5 hash algorithm */


// gcc -lssl -lcrypto -g -Wall filename.c

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

#define DATA_SIZE 1000

int MD5_hash(FILE *f, unsigned char *c) {

	int i;
	int ret_fread;
	MD5_CTX md5_context;
	unsigned char data[DATA_SIZE];


	// int MD5_Init(MD5_CTX *c);
	MD5_Init (&md5_context);

	while ( (ret_fread = fread (data, 1, DATA_SIZE, f) ) != 0){

		// int MD5_Update(MD5_CTX *c, const void *data, unsigned long len);
		MD5_Update (&md5_context, data, fread_ret);
		
	}

	// int MD5_Final(unsigned char *md, MD5_CTX *c);
	MD5_Final (c, &md5_context);

	for(i = 0; i < MD5_DIGEST_LENGTH; i++)
		printf("%02x", c[i]);

	printf("\n");
}


int main( int argc, char *argv[])
{

	FILE *fp1;
	FILE *fp2;
	unsigned char c1[MD5_DIGEST_LENGTH];
	unsigned char c2[MD5_DIGEST_LENGTH];

	// open files
	fp1 = fopen(argv[1], "r");
	fp2 = fopen(argv[2], "r");


	// compare
	MD5_hash(fp1, c1);
	MD5_hash(fp2, c2);

	// show the results
	printf("%s\n", memcmp(c1, c2, MD5_DIGEST_LENGTH) == 0 ? "same":"different" );
	printf("MD5_DIGEST_LENGTH: %d\n", MD5_DIGEST_LENGTH);

	return 0;
}
