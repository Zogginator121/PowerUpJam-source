/* Threads compatibility routines for libgcc2 and libobjc.  */
/* Compile this one with gcc.  */
/* Copyright (C) 1997-2016 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#ifndef _GLIBCXX__GLIBCXX_GCC_GTHR_DKP_H
#define _GLIBCXX__GLIBCXX_GCC_GTHR_DKP_H

#define __GTHREADS 1
#define __GTHREADS_CXX0X 1

#ifdef __cplusplus
extern "C" {
#endif

typedef void *                      __gthread_t;
typedef struct { int padding[7]; }  __gthread_cond_t;
typedef struct { int padding; }     __gthread_key_t;
typedef struct { int padding[11]; } __gthread_mutex_t;
typedef struct { int padding; }     __gthread_once_t;
typedef struct { int padding[11]; } __gthread_recursive_mutex_t;
typedef struct timespec             __gthread_time_t;

#define __GTHREAD_HAS_COND 1
#define _GTHREAD_USE_MUTEX_TIMEDLOCK 0

#define __GTHREAD_ONCE_INIT { 0 }
#define __GTHREAD_TIME_INIT { 0, 0 }

#define __GTHREAD_COND_INIT_FUNCTION __gthread_cond_init_function
#define __GTHREAD_MUTEX_INIT_FUNCTION __gthread_mutex_init_function
#define __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION __gthread_recursive_mutex_init_function

typedef struct {
  int (*active) (void);
  int (*create) (__gthread_t *__threadid, void *(*__func) (void*),
                 void *__args);
  int (*join) (__gthread_t __threadid, void **__value_ptr);
  int (*detach) (__gthread_t __threadid);
  int (*equal) (__gthread_t __t1, __gthread_t __t2);
  __gthread_t (*self) (void);
  int (*yield) (void);
  int (*once) (__gthread_once_t *__once, void (*__func) (void));
  int (*key_create) (__gthread_key_t *__key, void (*__dtor) (void *));
  int (*key_delete) (__gthread_key_t __key);
  void * (*getspecific) (__gthread_key_t __key);
  int (*setspecific) (__gthread_key_t __key, const void *__ptr);
  void (*mutex_init_function) (__gthread_mutex_t *__mutex);
  int (*mutex_destroy) (__gthread_mutex_t *__mutex);
  int (*mutex_lock) (__gthread_mutex_t *__mutex);
  int (*mutex_trylock) (__gthread_mutex_t *__mutex);
  int (*mutex_unlock) (__gthread_mutex_t *__mutex);
  int (*recursive_mutex_init_function) (__gthread_recursive_mutex_t *__mutex);
  int (*recursive_mutex_lock) (__gthread_recursive_mutex_t *__mutex);
  int (*recursive_mutex_trylock) (__gthread_recursive_mutex_t *__mutex);
  int (*recursive_mutex_unlock) (__gthread_recursive_mutex_t *__mutex);
  int (*recursive_mutex_destroy) (__gthread_recursive_mutex_t *__mutex);
  void (*cond_init_function) (__gthread_cond_t *__cond);
  int (*cond_broadcast) (__gthread_cond_t *__cond);
  int (*cond_signal) (__gthread_cond_t *__cond);
  int (*cond_wait) (__gthread_cond_t *__cond, __gthread_mutex_t *__mutex);
  int (*cond_timedwait) (__gthread_cond_t *__cond, __gthread_mutex_t *__mutex,
                         const __gthread_time_t *__abs_timeout);
  int (*cond_wait_recursive) (__gthread_cond_t *__cond,
                              __gthread_recursive_mutex_t *__mutex);
  int (*cond_destroy) (__gthread_cond_t* __cond);
} __gthread_impl_t;

extern __gthread_impl_t __gthread_impl;

int
__gthread_active_p (void);

int
__gthread_create (__gthread_t *__threadid, void *(*__func) (void*),
                  void *__args);

int
__gthread_join (__gthread_t __threadid, void **__value_ptr);

int
__gthread_detach (__gthread_t __threadid);

int
__gthread_equal (__gthread_t __t1, __gthread_t __t2);

__gthread_t
__gthread_self (void);

int
__gthread_yield (void);

int
__gthread_once (__gthread_once_t *__once, void (*__func) (void));

int
__gthread_key_create (__gthread_key_t *__key, void (*__dtor) (void *));

int
__gthread_key_delete (__gthread_key_t __key);

void *
__gthread_getspecific (__gthread_key_t __key);

int
__gthread_setspecific (__gthread_key_t __key, const void *__ptr);

void
__gthread_mutex_init_function (__gthread_mutex_t *__mutex);

int
__gthread_mutex_destroy (__gthread_mutex_t *__mutex);

int
__gthread_mutex_lock (__gthread_mutex_t *__mutex);

int
__gthread_mutex_trylock (__gthread_mutex_t *__mutex);

int
__gthread_mutex_unlock (__gthread_mutex_t *__mutex);

int
__gthread_recursive_mutex_init_function (__gthread_recursive_mutex_t *__mutex);

int
__gthread_recursive_mutex_lock (__gthread_recursive_mutex_t *__mutex);

int
__gthread_recursive_mutex_trylock (__gthread_recursive_mutex_t *__mutex);

int
__gthread_recursive_mutex_unlock (__gthread_recursive_mutex_t *__mutex);

int
__gthread_recursive_mutex_destroy (__gthread_recursive_mutex_t *__mutex);

void
__gthread_cond_init_function (__gthread_cond_t *__cond);

int
__gthread_cond_broadcast (__gthread_cond_t *__cond);

int
__gthread_cond_signal (__gthread_cond_t *__cond);

int
__gthread_cond_wait (__gthread_cond_t *__cond, __gthread_mutex_t *__mutex);

int
__gthread_cond_timedwait (__gthread_cond_t *__cond, __gthread_mutex_t *__mutex,
                          const __gthread_time_t *__abs_timeout);

int
__gthread_cond_wait_recursive (__gthread_cond_t *__cond,
                               __gthread_recursive_mutex_t *__mutex);

int
__gthread_cond_destroy (__gthread_cond_t* __cond);

#ifdef __cplusplus
}
#endif

#endif /* ! _GLIBCXX__GLIBCXX_GCC_GTHR_DKP_H */
