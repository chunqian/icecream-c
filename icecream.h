/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef __ICECREAM_H__
#define __ICECREAM_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define LOG_VERSION "0.1.0"
#define IC_PREFIX "🍦"

#define MAX_ROW 64
#define MAX_COLUMN 512

typedef struct {
  va_list ap;
  const char *fmt;
  const char *file;
  const char *function;
  struct tm *time;
  FILE *udata; // void *udata
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

#define IC_ARG_COUNT(...) IC_INTERNAL_ARG_COUNT_PRIVATE(0, ##__VA_ARGS__, \
  64, 63, 62, 61, 60, \
  59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
  49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
  39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
  29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
  19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
   9,  8,  7,  6,  5,  4,  3,  2,  1,  0)
#define IC_INTERNAL_ARG_COUNT_PRIVATE( \
   _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
  _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
  _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
  _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
  _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
  _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
  _60, _61, _62, _63, _64, N, ...) N

#define ic(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, 0, "", #__VA_ARGS__, #__VA_ARGS__)
#define ic_str(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, IC_ARG_COUNT(__VA_ARGS__), "\"%s\"", #__VA_ARGS__, ##__VA_ARGS__)
#define ic_int(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, IC_ARG_COUNT(__VA_ARGS__), "%d" , #__VA_ARGS__, ##__VA_ARGS__)
#define ic_long(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, IC_ARG_COUNT(__VA_ARGS__), "%ld" , #__VA_ARGS__, ##__VA_ARGS__)
#define ic_hex(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, IC_ARG_COUNT(__VA_ARGS__), "%#x", #__VA_ARGS__, ##__VA_ARGS__)
#define ic_float(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, IC_ARG_COUNT(__VA_ARGS__), "%.2f", #__VA_ARGS__, ##__VA_ARGS__)
#define ic_double(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, IC_ARG_COUNT(__VA_ARGS__), "%.4lf", #__VA_ARGS__, ##__VA_ARGS__)
#define ic_ptr(...) ic_log(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, IC_ARG_COUNT(__VA_ARGS__), "%p", #__VA_ARGS__, ##__VA_ARGS__)

const char* log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, FILE *udata, int level); // void *udata
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, const char *function, int line, const char *fmt, ...);
void ic_log(int level, const char *file, const char *function, int line, int args_count, const char *fmt, const char *arg0, ...);

#define MAX_CALLBACKS 32

typedef struct {
  log_LogFn fn;
  FILE *udata; // void *udata
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
  if (ev->fmt != NULL && strlen(ev->fmt) == 0) {
    fprintf(
      ev->udata, "%s \x1b[0m%s:%d in %s\x1b[0m() ", IC_PREFIX, ev->file, ev->line, ev->function);
  } else {
    fprintf(
      ev->udata, "%s ", IC_PREFIX);
  }
#else
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


int log_add_callback(log_LogFn fn, FILE *udata, int level) { // void *udata
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


static void init_event(log_Event *ev, FILE *udata) { // void *udata
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

void str_trim(char *p_str) {
  char *p_tmp = p_str;

  while(*p_str != '\0') {
    if (*p_str != ' ') {
      *p_tmp++ = *p_str;
    }
    ++p_str;
  }
  *p_tmp = '\0';
}

void str_right_trim(char *p_str) {
  char *p_tmp = p_str + strlen(p_str) - 1;
  while(*p_tmp == ' ') {
    *p_tmp = '\0';
    p_tmp--;
  }
}

void str_left_trim(char *p_str) {
  char *p_tmp = p_str;
  while(*p_tmp == ' ') {
    p_tmp++;
  }
  while(*p_tmp != '\0') {
    *p_str = *p_tmp;
    p_str++;
    p_tmp++;
  }
  *p_str = '\0';
}

int split(char *dst, int row, int column, char *str, const char *spl) {
  int n = 0;
  char *result = NULL;
  result = strtok(str, spl);

  while( result != NULL )
  {
    str_left_trim(result);
    str_right_trim(result);
    strcpy(dst + (n * column), result);
    n ++;
    result = strtok(NULL, spl);
  }
  return n;
}

void ic_log(int level, const char *file, const char *function, int line, int args_count, const char *fmt, const char *arg0, ...) {

  char str[1024] = {0};
  strncpy(str, arg0, strlen(arg0));
  char dst[MAX_ROW][MAX_COLUMN] = {0};
  int cnt = split(dst, MAX_ROW, MAX_COLUMN, str, ",");

  char fmts[1024] = {0};
  for (int i = 0; i < args_count; ++i) {
    strcat(fmts, dst[i]);
    strcat(fmts, " = ");
    strcat(fmts, fmt);
    if (i < args_count - 1) {
      strcat(fmts, ", ");
    }
  }

  log_Event ev = {
    .fmt   = args_count > 0 ? fmts : fmt,
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

#endif // __ICECREAM_H__
