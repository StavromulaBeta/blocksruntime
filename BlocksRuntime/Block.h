/*
 * Block.h
 *
 * Copyright 2008-2010 Apple, Inc. Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _BLOCK_H_
#define _BLOCK_H_

#if !defined(BLOCK_EXPORT)
#   if defined(__cplusplus)
#       define BLOCK_EXPORT extern "C"
#   else
#       define BLOCK_EXPORT extern
#   endif
#endif

#ifndef _MSC_VER
#include <stdbool.h>
#else
/* MSVC doesn't have <stdbool.h>. Compensate. */
typedef char bool;
#define true (bool)1
#define false (bool)0
#endif


#if defined(__cplusplus)
extern "C" {
#endif

/* Create a heap based copy of a Block or simply add a reference to an existing one.
 * This must be paired with Block_release to recover memory, even when running
 * under Objective-C Garbage Collection.
 */
BLOCK_EXPORT void *_Block_copy(const void *aBlock);

/* Lose the reference, and if heap based and last reference, recover the memory. */
BLOCK_EXPORT void _Block_release(const void *aBlock);

/* the intercept routines that must be used under GC */
BLOCK_EXPORT void _Block_use_GC( void *(*alloc)(const unsigned long, const bool isOne, const bool isObject),
                                  void (*setHasRefcount)(const void *, const bool),
                                  void (*gc_assign_strong)(void *, void **),
                                  void (*gc_assign_weak)(const void *, void *),
                                  void (*gc_memmove)(void *, void *, unsigned long));

/* earlier version, now simply transitional */
BLOCK_EXPORT void _Block_use_GC5( void *(*alloc)(const unsigned long, const bool isOne, const bool isObject),
                                  void (*setHasRefcount)(const void *, const bool),
                                  void (*gc_assign_strong)(void *, void **),
                                  void (*gc_assign_weak)(const void *, void *));

BLOCK_EXPORT void _Block_use_RR( void (*retain)(const void *),
                                 void (*release)(const void *));

/* make a collectable GC heap based Block.  Not useful under non-GC. */
BLOCK_EXPORT void *_Block_copy_collectable(const void *aBlock);

/* thread-unsafe diagnostic */
BLOCK_EXPORT const char *_Block_dump(const void *block);

/* Runtime entry point called by compiler when assigning objects inside copy helper routines */
BLOCK_EXPORT void _Block_object_assign(void *destAddr, const void *object, const int flags);
    /* BLOCK_FIELD_IS_BYREF is only used from within block copy helpers */

/* runtime entry point called by the compiler when disposing of objects inside dispose helper routine */
BLOCK_EXPORT void _Block_object_dispose(const void *object, const int flags);

/* Runtime entry to get total size of a closure */
BLOCK_EXPORT unsigned long int Block_size(void *block_basic);

#if defined(__cplusplus)
}
#endif

/* Type correct macros. */

#define Block_copy(...) ((__typeof(__VA_ARGS__))_Block_copy((const void *)(__VA_ARGS__)))
#define Block_release(...) _Block_release((const void *)(__VA_ARGS__))


#endif
