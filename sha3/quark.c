






#include "cpuminer-config.h"
#include "miner.h"

#include <string.h>
#include <stdint.h>
#include "hashblock.h"

static void runhash(void *state, const void *input)
{
//    memcpy(state, init, 32);
    Hash9(state, input);
};

int scanhash_quark(int thr_id, uint32_t *pdata, const uint32_t *ptarget,
	uint32_t max_nonce, unsigned long *hashes_done)
{
	uint32_t n = pdata[19] - 1;
	const uint32_t first_nonce = pdata[19];
	const uint32_t Htarg = ptarget[7];
//	unsigned long stat_ctr = 0;
	//unsigned char hash64[64];
	uint32_t hash64[8] __attribute__((aligned(32)));
	uint32_t endiandata[32];
	
//	char testdata[] = {"\x70\x00\x00\x00\x5d\x38\x5b\xa1\x14\xd0\x79\x97\x0b\x29\xa9\x41\x8f\xd0\x54\x9e\x7d\x68\xa9\x5c\x7f\x16\x86\x21\xa3\x14\x20\x10\x00\x00\x00\x00\x57\x85\x86\xd1\x49\xfd\x07\xb2\x2f\x3a\x8a\x34\x7c\x51\x6d\xe7\x05\x2f\x03\x4d\x2b\x76\xff\x68\xe0\xd6\xec\xff\x9b\x77\xa4\x54\x89\xe3\xfd\x51\x17\x32\x01\x1d\xf0\x73\x10\x00"};

	
	//we need bigendian data...
	//lessons learned: do NOT endianchange directly in pdata, this will all proff-of-works be considered as stale from minerd.... 
	int kk=0;
	for (; kk < 32; kk++)
	{
		be32enc(&endiandata[kk], ((uint32_t*)pdata)[kk]);
	};
	
/*	int jj = 0;
	for (jj=0;jj<80;jj++)
	{
		((uint8_t*)pdata)[jj] = testdata[jj];
	}
*/	
/*
	int ii=0;
	for (; ii < 128; ii++)
	{
		printf ("%.2x",((uint8_t*)pdata)[ii]);
	};
	printf ("\n");

	ii=0;
	for (; ii < 32; ii++)
	{
		printf ("%.8x ",((uint32_t*)pdata)[ii]);
	};
	printf ("\n");
    printf ("\n");
	
	ii=0;
	for (; ii < 32; ii++)
	{
		printf ("%.2x",((uint8_t*)ptarget)[ii]);
	};
	printf ("\n");
	ii=0;
	for (; ii < 8; ii++)
	{
		printf ("%.8x ",((uint32_t*)ptarget)[ii]);
	};
	printf ("\n");	
*/	
	if (opt_debug) {
	
		applog(LOG_DEBUG, "Thr: %02d, firstN: %08x, maxN: %08x, ToDo: %d", thr_id, first_nonce, max_nonce, max_nonce-first_nonce);
	}
	
//	exit(1);
	
	
	
	do {
	
		pdata[19] = ++n;
		be32enc(&endiandata[19], n); 
//		runhash(hash64, pdata);
		runhash(hash64, &endiandata);
        if (((hash64[7]&0xFFFFFF00)==0) && 
				fulltest(hash64, ptarget)) {
            *hashes_done = n - first_nonce + 1;
			return true;
		}
	} while (n < max_nonce && !work_restart[thr_id].restart);
	
	*hashes_done = n - first_nonce + 1;
	pdata[19] = n;
	return 0;
}