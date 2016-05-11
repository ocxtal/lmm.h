# lmm.h

Thin malloc wrapper, providing small memory allocation on existing region in a context local way, without aggressive memory management.

## Functions

### lmm\_init

Initialize lmm context with a memory block `ptr`. If `ptr == NULL`, memory with size `size` will be malloc'd internally.

```
lmm_t *lmm_init(void *ptr, size_t size);
```

### lmm\_clean

Cleanup context.

```
void lmm_clean(lmm_t *lmm);
```

### lmm\_malloc

Allocate memory with size `size` from memory block in `lmm`. External memory will be malloc'd if size exceeds remaining memory or `lmm == NULL`.

```
void *lmm_malloc(lmm_t *lmm, size_t size);
```

### lmm\_realloc

Reallocate memory. Passed to realloc except `ptr` is the last allocated block in `lmm` and there is room.

```
void *lmm_realloc(lmm_t *lmm, void *ptr, size_t size);
```

### lmm\_free

Free memory block.

```
void lmm_free(lmm_t *lmm, void *ptr);
```

## License

MIT
