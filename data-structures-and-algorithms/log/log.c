#include <errno.h>
#include <stdarg.h>
#include <threads.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#include "log.h" 
#include "log_internals.h" 

typedef struct log_module {
    char *name;
    char *buffer;
    int mr_log_pos;
    int file_fd;
    log_config_t config;
} log_module_t;

typedef struct log {
    log_module_t *modules;
    size_t len;
    size_t cap;
} log_t;

static __thread char* hostname_s;
static __thread char* appname_s;
static __thread char* threadname_s;
static __thread int initialized_s = 0;
static __thread log_t *log_s;
static __thread size_t current_module_idx_s;

static void _log_alloc(size_t start, size_t buf_size) {
    for (size_t i=start; i<log_s->cap; i++) {
        log_s->modules[i].buffer = malloc(buf_size*(sizeof(char)));
        if (log_s->modules[i].buffer == NULL) {
            fprintf(stderr, "failed to initialized module buffer: %s\n", strerror(errno));
            exit(1);
        }
    }
}

static int _log_open_fd(const char* filename) {
    int fd = open (filename, O_WRONLY);
    if (fd == -1) {
        fprintf(stderr, "error opening log file: %s\n", strerror(errno));
    }
    return fd;
}

static void _log_set_config(log_config_t *dst, log_config_t *src) {
    dst->log_to_console = src->log_to_console;
    dst->level = src->level;
    dst->filename = NULL;
    if (src->filename != NULL) {
        dst->filename = malloc((strlen(src->filename) + 1) * sizeof(char));
        strcpy(dst->filename, src->filename);
    }
}

void _log_flush(size_t module_idx) {
    if (module_idx >= log_s->len) {
        return;
    }

    if (log_s->modules[module_idx].config.log_to_console == 1) {
        printf("%s", log_s->modules[module_idx].buffer);
    }

    if (log_s->modules[module_idx].file_fd > 0) {
        int seek = lseek(log_s->modules[module_idx].file_fd, 0, SEEK_END);
        if (seek == -1) {
            fprintf(stderr, "error appending to log file: %s.\n", strerror(errno));
        }
        int nr = write(log_s->modules[module_idx].file_fd,
                       log_s->modules[module_idx].buffer,
                       strlen(log_s->modules[module_idx].buffer));
        if (nr == -1) {
            fprintf(stderr,"error writing to log file: %s\n", strerror(errno));
        }
    }

    return;
}

void _mr_log_flush() {
    size_t module_idx = current_module_idx_s;

    // terminate the log
    log_s->modules[module_idx].buffer[log_s->modules[module_idx].mr_log_pos++] = '}';
    log_s->modules[module_idx].buffer[log_s->modules[module_idx].mr_log_pos++] = '\n';
    log_s->modules[module_idx].buffer[log_s->modules[module_idx].mr_log_pos] = '\0';

    _log_flush(current_module_idx_s);

    current_module_idx_s = 0;
    log_s->modules[current_module_idx_s].mr_log_pos = 0;
}

void log_create(const char* appname, const char* threadname) {
    if (initialized_s == 1) {
        return;
    }

    log_s = (log_t*) calloc(1, sizeof(log_t)); 

    hostname_s = malloc(HOST_NAME_MAX*sizeof(char));
    gethostname(hostname_s, HOST_NAME_MAX);

    appname_s = malloc((strlen(appname)+1)*sizeof(char));
    strcpy(appname_s, appname);

    threadname_s = malloc((strlen(threadname)+1)*sizeof(char));
    strcpy(threadname_s, threadname);

    log_s->cap = DEFAULT_MODULES_NUM;
    log_s->len = 0;
    log_s->modules = calloc(log_s->cap, sizeof(log_module_t));
    if (log_s->modules == NULL) {
        fprintf(stderr, "failed to initialize log modules\n");
        exit(1);
    }

    // pre-allocate the buffers for the modules.
    _log_alloc(0, DEFAULT_BUFFER_SIZE);
    initialized_s = 1;
}

int log_find_module(const char* name) {
    for (size_t i=0; i<log_s->len; i++) {
        if (!strcmp(name, log_s->modules[i].name)) return i;
    }
    return -1;
}

int log_add_module(const char* name, log_config_t config) {
    int found_module = 0, old_cap = 0;

    // TODO: adding this check for every function is really ugly.
    if (initialized_s == 0) {
        fprintf(stderr,"logging is not initialized\n");
        return -1;
    }

    found_module = log_find_module(name);
    if (found_module >= 0) {
        return found_module;
    }

    if (log_s->len == log_s->cap) {
        old_cap = log_s->cap;
        log_s->cap *= 2;
        log_s->modules = realloc(log_s->modules,
                                 log_s->cap * sizeof(log_module_t)); 
        //TODO valgrind still complains here about:
        // Uninitialised value was created by a heap allocation
        memset(log_s->modules + log_s->len, 0, (log_s->cap - log_s->len) * sizeof(log_module_t));
        if (log_s->modules == NULL) {
            fprintf(stderr, "failed to reallocate memory for modules.\n");
            exit(1);
        }
        _log_alloc(old_cap, DEFAULT_BUFFER_SIZE);
    }

    _log_set_config(&log_s->modules[log_s->len].config, &config);
    log_s->modules[log_s->len].file_fd = -1;
    if (log_s->modules[log_s->len].config.filename != NULL) {
        log_s->modules[log_s->len].file_fd = _log_open_fd(log_s->modules[log_s->len].config.filename);
    }
    log_s->modules[log_s->len].name = malloc((strlen(name)+1)*sizeof(char));
    strcpy(log_s->modules[log_s->len].name, name);

    log_s->len++;
    return log_s->len-1;
}

void log_sprintf(size_t module_idx,
                 int level,
                 const char* func,
                 const char* file,
                 int line,
                 const char *fmt, ...) {
    va_list args;
    struct timespec ts;
    struct tm tm;

    if (module_idx >= log_s->len) {
        fprintf(stderr, "logging module %lu is not initialized.\n", module_idx);
        return;
    }

    if (log_s->modules[module_idx].config.level < level) {
        return;
    }

    clock_gettime(CLOCK_REALTIME, &ts);
    gmtime_r(&(ts.tv_sec), &tm);

    int pos = sprintf(log_s->modules[module_idx].buffer, LOG_HEADER_FORMAT, LOG_HEADER_VAL);

    va_start(args, fmt);
    // https://stackoverflow.com/questions/150543/forward-an-invocation-of-a-variadic-function-in-c
    // also from "man printf":
    // The functions vprintf(),  vfprintf(),  vdprintf(),  vsprintf(),	vsnprintf()  are
    // equivalent   to	 the   functions   printf(),  fprintf(),  dprintf(),  sprintf(),
    // snprintf(), respectively, except that they are called with a va_list instead of a
    // variable number of arguments.  These functions do not call the va_end macro.  Be‐
    // cause they invoke the va_arg macro, the value of ap is undefined after the  call.
    // See stdarg(3).
    //
    // The  functions  snprintf()  and	vsnprintf() write at most size bytes (including the
    // terminating null byte ('\0')) to str.
    //
    // The  functions snprintf() and vsnprintf() do not write more than size bytes (includ‐
    // ing the terminating null byte ('\0')).  If the output  was  truncated  due  to  this
    // limit,  then the return value is the number of characters (excluding the terminating
    // null byte) which would have been written to the final string  if  enough  space	had
    // been  available.   Thus,  a  return  value of size or more means that the output was
    // truncated.  (See also below under NOTES.)
    pos += vsnprintf(log_s->modules[module_idx].buffer+pos, DEFAULT_BUFFER_SIZE-pos-1, fmt, args);
    if (pos > DEFAULT_BUFFER_SIZE-1) {
        log_s->modules[module_idx].buffer[DEFAULT_BUFFER_SIZE-2] = '\n';
        log_s->modules[module_idx].buffer[DEFAULT_BUFFER_SIZE-1] = '\0';
    } else {
        log_s->modules[module_idx].buffer[pos++] = '\n';
        log_s->modules[module_idx].buffer[pos++] = '\0';
    }

    va_end(args);
}

void log_set_Level(int idx, uint8_t lvl) {
    log_s->modules[idx].config.level = lvl;
}

uint8_t log_get_level(int idx) {
    return log_s->modules[idx].config.level;
}

const char* log_get_level_string(uint8_t level) {
    switch(level) {
        case LOG_LEVEL_DEFAULT:
            return "DEFAULT";
            break;
        case LOG_LEVEL_FATAL:
            return "FATAL";
            break;
        case LOG_LEVEL_ERROR:
            return "ERROR";
            break;
        case LOG_LEVEL_INFO:
            return "INFO";
            break;
        case LOG_LEVEL_WARN:
            return "WARN";
            break;
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
            break;
        case LOG_LEVEL_TRACE:
            return "TRACE";
            break;
        default:
            return "Undefined";
            break;
    }
}

void log_destroy() {
    if (initialized_s == 0) {
        return;
    }

    for (size_t i=0; i<log_s->cap; i++) {
        if (log_s->modules[i].name != NULL) {
            if (log_s->modules[i].config.filename != NULL && log_s->modules[i].file_fd != -1) {
                if (close(log_s->modules[i].file_fd == -1)) {
                    fprintf(stderr, "closing the log file: %s\n", strerror(errno));
                }
                free(log_s->modules[i].config.filename);
            }
            free(log_s->modules[i].name);
        }
        free(log_s->modules[i].buffer);
    }

    free(log_s->modules);
    log_s->len = 0;
    log_s->cap = 0;
    free(log_s);

    free(hostname_s);
    free(appname_s);
    free(threadname_s);
    initialized_s = 0;
}

void mr_snprintf(size_t module_idx, const char *msg, ...) {
    int n = log_s->modules[module_idx].mr_log_pos;
    char *mr_log_buf = log_s->modules[module_idx].buffer;

    va_list args;
    va_start(args, msg);
    if (n == 0) {
        n += sprintf(mr_log_buf, "{");
        n += vsnprintf(mr_log_buf + n,
                       DEFAULT_BUFFER_SIZE - n - 3 /* \"}\n\0 */,
                       msg , args);
    } else if (n > DEFAULT_BUFFER_SIZE - n - 3) {
        // nothing to do we have already overflown.
    } else {
        n += sprintf(mr_log_buf + n, ",");
        n += vsnprintf(mr_log_buf + n,
                       DEFAULT_BUFFER_SIZE - n - 3 /* \"}\n\0 */,
                       msg, args);
    }

    // need to check again if the last write overflowed.
    if (n > DEFAULT_BUFFER_SIZE-3 /* \"}\n\0 */ ) {
        // rewind on "\0" to make room for \"}\n\0
        log_s->modules[module_idx].mr_log_pos = DEFAULT_BUFFER_SIZE-4;
        // TODO: this is pretty hacky. the \" gets pushed out if the buffer overflows.
        // should have some kind of validation functions which truncate stuff and wrap
        // them arround in \"\".
        log_s->modules[module_idx].buffer[log_s->modules[module_idx].mr_log_pos++] = '"';
    } else {
        log_s->modules[module_idx].mr_log_pos = n;
    }
    va_end(args);
}

void mr_log_preamble(size_t module_idx,
                     int level,
                     const char* func,
                     const char* file,
                     int line,
                     const char* msg) {
    struct timespec ts;
    struct tm tm;

    // TODO: what happens if the module is removed?
    // I need an log_find_module_by_idx
    if (module_idx >= log_s->len) {
        fprintf(stderr, "logging module %lu is not initialized.\n", module_idx);
        return;
    }
    current_module_idx_s = module_idx;

    if (log_s->modules[module_idx].config.level < level) {
        return;
    }

    clock_gettime(CLOCK_REALTIME, &ts);
    gmtime_r(&(ts.tv_sec), &tm);

    //TODO: make the timestamp format configurable
    //TODO: in fact, this looks really repeatable code, so I assume I can
    //put these values in some vectors and then iterate through them
    char ts_fmt[128];
    const char *ts_fmt_key = "\"ts\":";
    strcpy(ts_fmt, ts_fmt_key);
    strcat(ts_fmt, "\"");
    strcat(ts_fmt, LOG_TIME_FORMAT);
    strcat(ts_fmt, "\"");
    mr_snprintf(module_idx, ts_fmt, LOG_TIME_VALUE(tm, ts));
    mr_snprintf(module_idx, "\"hostname\":\"%s\"", hostname_s);
    mr_snprintf(module_idx, "\"app\":\"%s\"", appname_s);
    mr_snprintf(module_idx, "\"level\":\"%s\"", LOG_LEVEL_VALUE(level));
    mr_snprintf(module_idx, "\"location\":\"%s():%s:%u\"", func, file, line);
    mr_snprintf(module_idx, "\"thread\":\"%s\"", threadname_s);
    mr_snprintf(module_idx, "\"msg\":\"%s\"", msg);

    return;
}

void mr_log_error(const char* error) {
    mr_snprintf(current_module_idx_s, "\"error\":\"%s\"", error);
}

void mr_log_buffer(const char* label, int len, const char* buf) {
    mr_snprintf(current_module_idx_s,
                       "\"%s\":\"%.*s\"",
                       label, len, buf);
}

void mr_log_string(const char* label, const char* buf) {
    mr_snprintf(current_module_idx_s,
                       "\"%s\":\"%s\"",
                       label, buf);
}

void mr_log_uint64(const char* label, uint64_t val) {
    mr_snprintf(current_module_idx_s,
                       "\"%s\":\"%ld\"",
                       label, val);
}
