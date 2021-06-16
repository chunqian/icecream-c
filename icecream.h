/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define LOG_VERSION "0.1.0"
#define IC_PREFIX "🍦"

typedef struct {
  va_list ap;
  const char *fmt;
  const char *file;
  const char *function;
  struct tm *time;
  void *udata;
  int line;
  int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define VA_ARG_1(...) VA_ARG_1_(__VA_ARGS__)
#define VA_ARG_1_(a1, ...) #a1, a1
#define VA_ARG_2(...) VA_ARG_2_(__VA_ARGS__)
#define VA_ARG_2_(a1, a2, ...) #a1, a1, #a2, a2
#define VA_ARG_3(...) VA_ARG_3_(__VA_ARGS__)
#define VA_ARG_3_(a1, a2, a3, ...) #a1, a1, #a2, a2, #a3, a3
#define VA_ARG_4(...) VA_ARG_4_(__VA_ARGS__)
#define VA_ARG_4_(a1, a2, a3, a4, ...) #a1, a1, #a2, a2, #a3, a3, #a4, a4

#define ic(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "", #__VA_ARGS__, #__VA_ARGS__)

#define ic_str(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = \"%s\" ", #__VA_ARGS__, VA_ARG_1(__VA_ARGS__))
#define ic_str2(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = \"%s\", %s = \"%s\" ", #__VA_ARGS__, VA_ARG_2(__VA_ARGS__))
#define ic_str3(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = \"%s\", %s = \"%s\", %s = \"%s\" ", #__VA_ARGS__, VA_ARG_3(__VA_ARGS__))
#define ic_str4(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = \"%s\", %s = \"%s\", %s = \"%s\", %s = \"%s\" ", #__VA_ARGS__, VA_ARG_4(__VA_ARGS__))

#define ic_int(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %ld ", #__VA_ARGS__, VA_ARG_1(__VA_ARGS__))
#define ic_int2(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %ld, %s = %ld ", #__VA_ARGS__, VA_ARG_2(__VA_ARGS__))
#define ic_int3(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %ld, %s = %ld, %s = %ld ", #__VA_ARGS__, VA_ARG_3(__VA_ARGS__))
#define ic_int4(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %ld, %s = %ld, %s = %ld, %s = %ld ", #__VA_ARGS__, VA_ARG_4(__VA_ARGS__))

#define ic_hex(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %#x ", #__VA_ARGS__, VA_ARG_1(__VA_ARGS__))
#define ic_hex2(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %#x, %s = %#x ", #__VA_ARGS__, VA_ARG_2(__VA_ARGS__))
#define ic_hex3(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %#x, %s = %#x, %s = %#x ", #__VA_ARGS__, VA_ARG_3(__VA_ARGS__))
#define ic_hex4(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %#x, %s = %#x, %s = %#x, %s = %#x ", #__VA_ARGS__, VA_ARG_4(__VA_ARGS__))

#define ic_float(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %.4lf ", #__VA_ARGS__, VA_ARG_1(__VA_ARGS__))
#define ic_float2(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %.4lf, %s = %.4lf ", #__VA_ARGS__, VA_ARG_2(__VA_ARGS__))
#define ic_float3(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %.4lf, %s = %.4lf, %s = %.4lf ", #__VA_ARGS__, VA_ARG_3(__VA_ARGS__))
#define ic_float4(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %.4lf, %s = %.4lf, %s = %.4lf, %s = %.4lf ", #__VA_ARGS__, VA_ARG_4(__VA_ARGS__))

#define ic_ptr(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %p ", #__VA_ARGS__, VA_ARG_1(__VA_ARGS__))
#define ic_ptr2(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %p, %s = %p ", #__VA_ARGS__, VA_ARG_2(__VA_ARGS__))
#define ic_ptr3(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %p, %s = %p, %s = %p ", #__VA_ARGS__, VA_ARG_3(__VA_ARGS__))
#define ic_ptr4(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, "%s = %p, %s = %p, %s = %p, %s = %p ", #__VA_ARGS__, VA_ARG_4(__VA_ARGS__))

const char* log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, const char *function, int line, const char *fmt, ...);
void ic_log(int level, const char *file, const char *function, int line, const char *fmt, const char *arg0, ...);

#define MAX_CALLBACKS 32

typedef struct {
  log_LogFn fn;
  void *udata;
  int level;
} Callback;

static struct {
  void *udata;
  log_LockFn lock;
  int level;
  bool quiet;
  Callback callbacks[MAX_CALLBACKS];
} L;


static const char *level_strings[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif


static void stdout_callback(log_Event *ev) {
  char buf[16];
  buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
  fprintf(
    ev->udata, "[%s %s%-5s\x1b[0m \x1b[0m%s:%d:\x1b[0m in %s\x1b[0m] ",
    buf, level_colors[ev->level], level_strings[ev->level],
    ev->file, ev->line, ev->function);
#else
  fprintf(
    ev->udata, "[%s %-5s %s:%d: in %s] ",
    buf, level_strings[ev->level], ev->file, ev->line, ev->function);
#endif
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}

static void ic_stdout_callback(log_Event *ev) {
  char buf[16];
  buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
  // fprintf(
  //   ev->udata, "[%s %s%-5s\x1b[0m \x1b[0m%s:%d:\x1b[0m in %s\x1b[0m] ",
  //   buf, level_colors[ev->level], level_strings[ev->level],
  //   ev->file, ev->line, ev->function);
  if (ev->fmt != NULL && strlen(ev->fmt) == 0) {
    fprintf(
      ev->udata, "%s \x1b[0m%s:%d in %s\x1b[0m() ", IC_PREFIX, ev->file, ev->line, ev->function);
  } else {
    fprintf(
      ev->udata, "%s ", IC_PREFIX);
  }
#else
  // fprintf(
  //   ev->udata, "[%s %-5s %s:%d: in %s] ",
  //   buf, level_strings[ev->level], ev->file, ev->line, ev->function);
  if (ev->fmt != NULL && strlen(ev->fmt) == 0) {
    fprintf(
      ev->udata, "%s %s:%d in %s() ", IC_PREFIX, ev->file, ev->line, ev->function);
  } else {
    fprintf(
      ev->udata, "%s ", IC_PREFIX);
  }
#endif
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}


static void file_callback(log_Event *ev) {
  char buf[64];
  buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
  fprintf(
    ev->udata, "[%s %-5s %s:%d: in %s] ",
    buf, level_strings[ev->level], ev->file, ev->line, ev->function);
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}


static void lock(void)   {
  if (L.lock) { L.lock(true, L.udata); }
}


static void unlock(void) {
  if (L.lock) { L.lock(false, L.udata); }
}


const char* log_level_string(int level) {
  return level_strings[level];
}


void log_set_lock(log_LockFn fn, void *udata) {
  L.lock = fn;
  L.udata = udata;
}


void log_set_level(int level) {
  L.level = level;
}


void log_set_quiet(bool enable) {
  L.quiet = enable;
}


int log_add_callback(log_LogFn fn, void *udata, int level) {
  for (int i = 0; i < MAX_CALLBACKS; i++) {
    if (!L.callbacks[i].fn) {
      L.callbacks[i] = (Callback) { fn, udata, level };
      return 0;
    }
  }
  return -1;
}


int log_add_fp(FILE *fp, int level) {
  return log_add_callback(file_callback, fp, level);
}


static void init_event(log_Event *ev, void *udata) {
  if (!ev->time) {
    time_t t = time(NULL);
    ev->time = localtime(&t);
  }
  ev->udata = udata;
}


void log_log(int level, const char *file, const char *function, int line, const char *fmt, ...) {
  log_Event ev = {
    .fmt   = fmt,
    .file  = file,
    .function  = function,
    .line  = line,
    .level = level,
  };

  lock();

  if (!L.quiet && level >= L.level) {
    init_event(&ev, stderr);
    va_start(ev.ap, fmt);
    stdout_callback(&ev);
    va_end(ev.ap);
  }

  for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++) {
    Callback *cb = &L.callbacks[i];
    if (level >= cb->level) {
      init_event(&ev, cb->udata);
      va_start(ev.ap, fmt);
      cb->fn(&ev);
      va_end(ev.ap);
    }
  }

  unlock();
}

void ic_log(int level, const char *file, const char *function, int line, const char *fmt, const char *arg0, ...) {
  log_Event ev = {
    .fmt   = fmt,
    .file  = file,
    .function  = function,
    .line  = line,
    .level = level,
  };

  lock();

  if (!L.quiet && level >= L.level) {
    init_event(&ev, stderr);
    va_start(ev.ap, arg0);
    ic_stdout_callback(&ev);
    va_end(ev.ap);
  }

  for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++) {
    Callback *cb = &L.callbacks[i];
    if (level >= cb->level) {
      init_event(&ev, cb->udata);
      va_start(ev.ap, arg0);
      cb->fn(&ev);
      va_end(ev.ap);
    }
  }

  unlock();
}

#endif
