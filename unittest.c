
/**
 * @file test.c
 * @brief unittests
 */

#define UNITTEST_UNIQUE_ID		199
#include "unittest.h"
#include "lmm.h"

unittest_config(
	.name = "lmm"
);

#define DEFAULT_SIZE			( 256 )

/* transparent */
unittest()
{
	uint8_t *arr = (uint8_t *)lmm_malloc(NULL, DEFAULT_SIZE);

	for(int64_t i = 0; i < DEFAULT_SIZE; i++) {
		arr[i] = i & 0xff;
	}

	arr = lmm_realloc(NULL, arr, 2 * DEFAULT_SIZE);

	for(int64_t i = 0; i < DEFAULT_SIZE; i++) {
		assert(arr[i] == (i & 0xff));
	}

	lmm_free(NULL, arr);
}

/* with context */
unittest()
{
	void *mem = malloc(DEFAULT_SIZE);
	lmm_t *lmm = lmm_init(mem, DEFAULT_SIZE);

	assert((void *)lmm == mem, "%p, %p", lmm, mem);

	uint8_t *arr1 = (uint8_t *)lmm_malloc(lmm, 25);
	uint8_t *arr2 = (uint8_t *)lmm_malloc(lmm, 16);
	uint8_t *arr3 = (uint8_t *)lmm_malloc(lmm, 128);
	uint8_t *arr4 = (uint8_t *)lmm_malloc(lmm, 31);

	assert(arr1 != NULL);
	assert(arr2 != NULL);
	assert(arr3 != NULL);
	assert(arr4 != NULL);

	for(int64_t i = 0; i < 25; i++) { arr1[i] = i & 0xff; }
	for(int64_t i = 0; i < 16; i++) { arr2[i] = i & 0xff; }
	for(int64_t i = 0; i < 128; i++) { arr3[i] = i & 0xff; }
	for(int64_t i = 0; i < 31; i++) { arr4[i] = i & 0xff; }

	for(int64_t i = 0; i < 25; i++) { assert(arr1[i] == (i & 0xff)); }
	for(int64_t i = 0; i < 16; i++) { assert(arr2[i] == (i & 0xff)); }
	for(int64_t i = 0; i < 128; i++) { assert(arr3[i] == (i & 0xff)); }
	for(int64_t i = 0; i < 31; i++) { assert(arr4[i] == (i & 0xff)); }

	lmm_free(lmm, arr1);
	lmm_free(lmm, arr2);
	lmm_free(lmm, arr3);
	lmm_free(lmm, arr4);
	free(mem);
}

/* realloc */
unittest()
{
	void *mem = malloc(DEFAULT_SIZE);
	lmm_t *lmm = lmm_init(mem, DEFAULT_SIZE);

	assert((void *)lmm == mem, "%p, %p", lmm, mem);

	uint8_t *arr1 = (uint8_t *)lmm_malloc(lmm, 25);
	uint8_t *arr2 = (uint8_t *)lmm_malloc(lmm, 16);
	uint8_t *arr3 = (uint8_t *)lmm_malloc(lmm, 128);
	uint8_t *arr4 = (uint8_t *)lmm_malloc(lmm, 31);

	assert(arr1 != NULL);
	assert(arr2 != NULL);
	assert(arr3 != NULL);
	assert(arr4 != NULL);

	for(int64_t i = 0; i < 25; i++) { arr1[i] = i & 0xff; }
	for(int64_t i = 0; i < 16; i++) { arr2[i] = i & 0xff; }
	for(int64_t i = 0; i < 128; i++) { arr3[i] = i & 0xff; }
	for(int64_t i = 0; i < 31; i++) { arr4[i] = i & 0xff; }

	arr1 = lmm_realloc(lmm, arr1, 2 * 25);
	arr2 = lmm_realloc(lmm, arr2, 2 * 16);
	arr3 = lmm_realloc(lmm, arr3, 2 * 256);
	arr4 = lmm_realloc(lmm, arr4, 2 * 31);

	for(int64_t i = 0; i < 25; i++) { assert(arr1[i] == (i & 0xff)); }
	for(int64_t i = 0; i < 16; i++) { assert(arr2[i] == (i & 0xff)); }
	for(int64_t i = 0; i < 128; i++) { assert(arr3[i] == (i & 0xff)); }
	for(int64_t i = 0; i < 31; i++) { assert(arr4[i] == (i & 0xff)); }

	lmm_free(lmm, arr1);
	lmm_free(lmm, arr2);
	lmm_free(lmm, arr3);
	lmm_free(lmm, arr4);
	free(mem);
}

int main(int argc, char *argv[])
{
	return(unittest_main(argc, argv));
}

/**
 * end of test.c
 */
