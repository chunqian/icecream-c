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
#define PRINT_PREFIX "🍦"

#define MAX_ROW 64
#define MAX_COLUMN 256

typedef struct
{
    va_list ap;
    const char *fmt;
    const char *file;
    const char *function;
    struct tm *time;
    FILE *udata;  // void *udata
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

#define PRINT_ARG_COUNT(...) PRINT_INTERNAL_ARG_COUNT_PRIVATE(0, ##__VA_ARGS__, \
  64, 63, 62, 61, 60, \
  59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
  49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
  39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
  29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
  19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
   9,  8,  7,  6,  5,  4,  3,  2,  1,  0)
#define PRINT_INTERNAL_ARG_COUNT_PRIVATE( \
   _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
  _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
  _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
  _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
  _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
  _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
  _60, _61, _62, _63, _64, N, ...) N

enum {
    FORMAT_FUNC, 
    FORMAT_STR, 
    FORMAT_INT, 
    FORMAT_LONG, 
    FORMAT_HEX, 
    FORMAT_FLOAT, 
    FORMAT_DOUBLE, 
    FORMAT_PTR 
};
static const char *format_colors[] = {
    "",
    "\x1b[32m\"%s\"\x1b[0m",
    "\x1b[34m%d\x1b[0m",
    "\x1b[34m%ld\x1b[0m",
    "\x1b[34m%#x\x1b[0m",
    "\x1b[34m%.2f\x1b[0m",
    "\x1b[34m%.4lf\x1b[0m",
    "\x1b[34m%p\x1b[0m"
};

#define print(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_FUNC], #__VA_ARGS__, ##__VA_ARGS__)
#define print_func(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, 0, format_colors[FORMAT_FUNC], #__VA_ARGS__, #__VA_ARGS__)
#define print_str(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_STR], #__VA_ARGS__, ##__VA_ARGS__)
#define print_int(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_INT], #__VA_ARGS__, ##__VA_ARGS__)
#define print_long(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_LONG], #__VA_ARGS__, ##__VA_ARGS__)
#define print_hex(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_HEX], #__VA_ARGS__, ##__VA_ARGS__)
#define print_float(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_FLOAT], #__VA_ARGS__, ##__VA_ARGS__)
#define print_double(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_DOUBLE], #__VA_ARGS__, ##__VA_ARGS__)
#define print_ptr(...) \
    log_print(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, PRINT_ARG_COUNT(__VA_ARGS__), format_colors[FORMAT_PTR], #__VA_ARGS__, ##__VA_ARGS__)

#define ic(...) print_func(__VA_ARGS__)
#define ic_str(...) print_str(__VA_ARGS__)
#define ic_int(...) print_int(__VA_ARGS__)
#define ic_long(...) print_long(__VA_ARGS__)
#define ic_hex(...) print_hex(__VA_ARGS__)
#define ic_float(...) print_float(__VA_ARGS__)
#define ic_double(...) print_double(__VA_ARGS__)
#define ic_ptr(...) print_ptr(__VA_ARGS__)

const char *log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, FILE *udata, int level);  // void *udata
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, const char *function, int line, const char *fmt, ...);
void log_print(int level, const char *file, const char *function, int line, int args_count, const char *fmt,
            const char *arg0, ...);

#define MAX_CALLBACKS 32

typedef struct
{
    log_LogFn fn;
    FILE *udata;  // void *udata
    int level;
} Callback;

static struct
{
    void *udata;
    log_LockFn lock;
    int level;
    bool quiet;
    Callback callbacks[MAX_CALLBACKS];
} L;

static const char *level_strings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {"\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"};
#endif

static void stdout_callback(log_Event *ev)
{
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#ifdef LOG_USE_COLOR
    fprintf(ev->udata, "[%s %s%-5s\x1b[0m \x1b[0m%s:%d:\x1b[0m in %s\x1b[0m] ", buf, level_colors[ev->level],
            level_strings[ev->level], ev->file, ev->line, ev->function);
#else
    fprintf(ev->udata, "[%s %-5s %s:%d: in %s] ", buf, level_strings[ev->level], ev->file, ev->line, ev->function);
#endif
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void stdout_print_callback(log_Event *ev)
{
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';

    if (ev->fmt != NULL && strlen(ev->fmt) == 0)
    {
        fprintf(ev->udata, "%s \x1b[0m%s:\x1b[34m%d\x1b[0m in \x1b[34m%s\x1b[0m() ", PRINT_PREFIX, ev->file, ev->line, ev->function);
    }
    else
    {
        fprintf(ev->udata, "%s ", PRINT_PREFIX);
    }

    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void file_callback(log_Event *ev)
{
    char buf[64];
    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    fprintf(ev->udata, "[%s %-5s %s:%d: in %s] ", buf, level_strings[ev->level], ev->file, ev->line, ev->function);
    vfprintf(ev->udata, ev->fmt, ev->ap);
    fprintf(ev->udata, "\n");
    fflush(ev->udata);
}

static void lock(void)
{
    if (L.lock)
    {
        L.lock(true, L.udata);
    }
}

static void unlock(void)
{
    if (L.lock)
    {
        L.lock(false, L.udata);
    }
}

const char *log_level_string(int level) { return level_strings[level]; }

void log_set_lock(log_LockFn fn, void *udata)
{
    L.lock = fn;
    L.udata = udata;
}

void log_set_level(int level) { L.level = level; }

void log_set_quiet(bool enable) { L.quiet = enable; }

int log_add_callback(log_LogFn fn, FILE *udata, int level)
{  // void *udata
    for (int i = 0; i < MAX_CALLBACKS; i++)
    {
        if (!L.callbacks[i].fn)
        {
            L.callbacks[i] = (Callback){fn, udata, level};
            return 0;
        }
    }
    return -1;
}

int log_add_fp(FILE *fp, int level) { return log_add_callback(file_callback, fp, level); }

static void init_event(log_Event *ev, FILE *udata)
{  // void *udata
    if (!ev->time)
    {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}

void log_log(int level, const char *file, const char *function, int line, const char *fmt, ...)
{
    log_Event ev = {
        .fmt = fmt,
        .file = file,
        .function = function,
        .line = line,
        .level = level,
    };

    lock();

    if (!L.quiet && level >= L.level)
    {
        init_event(&ev, stderr);
        va_start(ev.ap, fmt);
        stdout_callback(&ev);
        va_end(ev.ap);
    }

    for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++)
    {
        Callback *cb = &L.callbacks[i];
        if (level >= cb->level)
        {
            init_event(&ev, cb->udata);
            va_start(ev.ap, fmt);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}

void str_trim(char *p_str)
{
    char *p_tmp = p_str;

    while (*p_str != '\0')
    {
        if (*p_str != ' ')
        {
            *p_tmp++ = *p_str;
        }
        ++p_str;
    }
    *p_tmp = '\0';
}

void str_right_trim(char *p_str)
{
    char *p_tmp = p_str + strlen(p_str) - 1;
    while (*p_tmp == ' ')
    {
        *p_tmp = '\0';
        p_tmp--;
    }
}

void str_left_trim(char *p_str)
{
    char *p_tmp = p_str;
    while (*p_tmp == ' ')
    {
        p_tmp++;
    }
    while (*p_tmp != '\0')
    {
        *p_str = *p_tmp;
        p_str++;
        p_tmp++;
    }
    *p_str = '\0';
}

int split(char *dst, int row, int column, char *str, const char *spl)
{
    int n = 0;
    char *result = NULL;
    result = strtok(str, spl);

    while (result != NULL)
    {
        str_left_trim(result);
        str_right_trim(result);
        strcpy(dst + (n * column), result);
        n++;
        result = strtok(NULL, spl);
    }
    return n;
}

void log_print(int level, const char *file, const char *function, int line, int args_count, const char *fmt,
            const char *arg0, ...)
{
    char str[1024] = {0};
    strncpy(str, arg0, strlen(arg0));
    char dst[MAX_ROW][MAX_COLUMN] = {0};
    int cnt = split((char *)dst, MAX_ROW, MAX_COLUMN, str, ",");

    char fmts[1024] = {0};
    for (int i = 0; i < args_count; ++i)
    {
        strcat(fmts, dst[i]);
        strcat(fmts, " = ");
        strcat(fmts, fmt);
        if (i < args_count - 1)
        {
            strcat(fmts, ", ");
        }
    }

    log_Event ev = {
        .fmt = args_count > 0 ? fmts : fmt,
        .file = file,
        .function = function,
        .line = line,
        .level = level,
    };

    lock();

    if (!L.quiet && level >= L.level)
    {
        init_event(&ev, stderr);
        va_start(ev.ap, arg0);
        stdout_print_callback(&ev);
        va_end(ev.ap);
    }

    for (int i = 0; i < MAX_CALLBACKS && L.callbacks[i].fn; i++)
    {
        Callback *cb = &L.callbacks[i];
        if (level >= cb->level)
        {
            init_event(&ev, cb->udata);
            va_start(ev.ap, arg0);
            cb->fn(&ev);
            va_end(ev.ap);
        }
    }

    unlock();
}

#endif  // __ICECREAM_H__
