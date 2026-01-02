/**
 * Copyright (c) 2025 JeepWay
 *
 * This library is free, you can redistribute and modify it
 * under the MIT License, see logger.c for details.
 */

#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#if defined(__GNUC__) || defined(__clang__)
#define __LOGGER_HAS_TYPEOF 1
#else
#define __LOGGER_HAS_TYPEOF 0
#endif

#define MAX_HANDLERS 29
#define LOG_VERSION "0.1.0"
#define ROOT_HANDLER 0
#define ROOT_HANDLER_NAME "root"
#define DEFAULT NULL
#define DEFAULT_LEVEL LOG_INFO
#define DEFAULT_STRAEM stderr
#define DEFAULT_FILE_NAME "logger/program.log"
#define DEFAULT_FILE_MODE "a"
#define DEFAULT_DATE_FORMAT1 "%H:%M:%S"                  // HH:MM:SS
#define DEFAULT_DATE_FORMAT2 "%Y-%m-%d"                  // YYYY-MM-DD
#define DEFAULT_DATE_FORMAT3 "%Y/%m/%d %H:%M:%S"         // YYYY/MM/DD HH:MM:SS
#define DEFAULT_DATE_FORMAT4 "%Y-%m-%d %H:%M:%S"         // YYYY-MM-DD HH:MM:SS
#define DEFAULT_DATE_FORMAT8 "%a, %d %b %Y %H:%M:%S %z"  // RFC 2822
#define DEFAULT_DATE_FORMAT9 "%Y-%m-%dT%H:%M:%S%z"       // ISO 8601

enum LOG_LEVEL {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

static const char *level_strings[] = {"TRACE", "DEBUG", "INFO",
                                      "WARN",  "ERROR", "FATAL"};

static const char *level_colors[] = {"\x1b[94m", "\x1b[36m", "\x1b[32m",
                                     "\x1b[33m", "\x1b[31m", "\x1b[35m"};

typedef struct record record_t;

typedef void (*log_dump_fn)(record_t *rec);
typedef void (*log_fmt_fn)(record_t *rec, const char *time_buf);
typedef void (*log_LockFn)(bool lock, void *fp);

struct record {
    va_list ap;        // parse
    struct tm *time;   // localtime
    int level;         // LOG_LEVEL
    const char *file;  // __FILE__
    int line;          // __LINE__
    const char *msg_fmt;
    const char *hd_name;
    log_fmt_fn hd_fmt_fn;
    void *hd_fp;
    const char *hd_date_fmt;
};

int log_add_file_handler(const char *filename, const char *filemode,
                         size_t level, const char *name);
int log_add_stream_handler(FILE *fp, size_t level, const char *name);

void _log_message(int level, const char *file, int line, const char *msg_fmt,
                  ...);
#define log_trace(...) _log_message(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) _log_message(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) _log_message(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) _log_message(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) _log_message(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) _log_message(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_set_level(const char *name, size_t value);
void log_set_quiet(const char *name, bool value);
void log_set_date_fmt(const char *name, const char *value);
void log_set_fmt_fn(const char *name, log_fmt_fn value);
void log_set_dump_fn(const char *name, log_dump_fn value);

void log_set_lock(log_LockFn fn, void *fp);

// some default log_fmt_fn and log_dump_fn functions
void dump_log(record_t *rec);
void color_fmt1(record_t *rec, const char *time_buf);
void color_fmt2(record_t *rec, const char *time_buf);
void no_color_fmt1(record_t *rec, const char *time_buf);
void no_color_fmt2(record_t *rec, const char *time_buf);

#endif /* LOG_H */