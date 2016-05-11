
/**
 * @fn lmm.c
 *
 * @brief malloc with local context
 */
#ifndef _LMM_H_INCLUDED
#define _LMM_H_INCLUDED

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE		200112L
#endif

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include <stdint.h>
#include <stdlib.h>


/* roundup */
#define _lmm_cutdown(x, base)		( (x) & ~((base) - 1) )
#define _lmm_roundup(x, base)		( ((x) + (base) - 1) & ~((base) - 1) )

/* constants */
#define LMM_MIN_BASE_SIZE			( 128 )
#define LMM_DEFAULT_BASE_SIZE		( 1024 )

/**
 * @struct lmm_s
 */
struct lmm_s {
	uint8_t need_free;
	uint8_t pad[7];
	void *ptr;
	void *lim;
};
typedef struct lmm_s lmm_t;

/**
 * @fn lmm_init
 */
static inline
lmm_t *lmm_init(
	void *base,
	size_t base_size)
{
	if(base != NULL && base_size > LMM_MIN_BASE_SIZE) {
		struct lmm_s *lmm = (struct lmm_s *)base;
		lmm->need_free = 0;
		lmm->ptr = base + sizeof(struct lmm_s);
		lmm->lim = base + _lmm_cutdown(base_size, 16);
		return((lmm_t *)lmm);
	} else {
		struct lmm_s *lmm = (struct lmm_s *)malloc(LMM_DEFAULT_BASE_SIZE);
		lmm->need_free = 1;
		lmm->ptr = (void *)(lmm + 1);
		lmm->lim = (void *)lmm + LMM_DEFAULT_BASE_SIZE;
		return((lmm_t *)lmm);
	}

	/* never reaches here */
	return(NULL);
}

/**
 * @fn lmm_clean
 */
static inline
void lmm_clean(
	lmm_t *lmm)
{
	if(lmm != NULL && lmm->need_free == 1) {
		free(lmm);
	}
	return;
}

/**
 * @fn lmm_reserve_mem
 */
static inline
void *lmm_reserve_mem(
	lmm_t *lmm,
	void *ptr,
	uint64_t size)
{
	uint64_t *sp = (uint64_t *)ptr;
	size = _lmm_roundup(size, 16);
	*sp = size;
	lmm->ptr = (void *)(sp + 1) + size;
	return((void *)(sp + 1));
}

/**
 * @fn lmm_malloc
 */
static inline
void *lmm_malloc(
	lmm_t *lmm,
	size_t size)
{
	if(lmm != NULL && lmm->ptr + size + sizeof(uint64_t) < lmm->lim) {
		return(lmm_reserve_mem(lmm, lmm->ptr, size));
	} else {
		return(malloc(size));
	}
}

/**
 * @fn lmm_realloc
 */
static inline
void *lmm_realloc(
	lmm_t *lmm,
	void *ptr,
	size_t size)
{
	if(lmm == NULL) {
		return(realloc(ptr, size));
	}

	/* check if prev mem (ptr) is inside mm */
	if((void *)lmm < ptr && ptr < lmm->lim) {
		
		uint64_t prev_size = *((uint64_t *)ptr - 1);
		if(ptr + prev_size == lmm->ptr && ptr + size < lmm->lim) {
			return(lmm_reserve_mem(lmm, ptr - sizeof(uint64_t), size));
		}

		void *np = malloc(size);
		if(np == NULL) { return(NULL); }

		memcpy(np, ptr, prev_size);
		return(np);
	}

	/* pass to library realloc */
	return(realloc(ptr, size));
}

/**
 * @fn lmm_free
 */
static inline
void lmm_free(
	lmm_t *lmm,
	void *ptr)
{
	if(lmm != NULL && (void *)lmm < ptr && ptr < lmm->lim) {
		/* no need to free */
		uint64_t prev_size = *((uint64_t *)ptr - 1);
		if(ptr + prev_size == lmm->ptr) {
			lmm->ptr = ptr - sizeof(uint64_t);
		}
		return;
	}

	free(ptr);
	return;
}

#endif /* _LMM_H_INCLUDED */
/**
 * end of lmm.c
 */
