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
    int file_fd;
    log_config_t config;
} log_module_t;

typedef struct log {
    log_module_t *modules;
    size_t len;
    size_t cap;
} log_t;

static char* hostname_s;
static char* appname_s;
static __thread char* threadname_s;
static __thread int initialized_s = 0;
static __thread log_t *log_s;

int log_open_fd(const char* filename) {
    int fd = open (filename, O_WRONLY);
    if (fd == -1) {
        perror("error opening log file\n");
    }
    return fd;
}

void log_init(const char *appname) {
    log_create(appname, "main");
}

void _log_init_modules(size_t start, size_t buf_size) {
    for (size_t i=start; i<log_s->cap; i++) {
        log_s->modules[i].buffer = malloc(buf_size*(sizeof(char)));
        if (log_s->modules[i].buffer == NULL) {
            perror("failed to initialized module buffer\n");
            exit(1);
        }
    }
}

void log_create(const char* appname, const char* threadname) {
    if (initialized_s == 0) {
        log_s = (log_t*) calloc(1, sizeof(log_t)); 

        hostname_s = malloc(HOST_NAME_MAX*sizeof(char));
        gethostname(hostname_s, HOST_NAME_MAX);

        appname_s = malloc((strlen(appname)+1)*sizeof(char));
        strcpy(appname_s, appname);

        threadname_s = malloc((strlen(threadname)+1)*sizeof(char));
        strcpy(threadname_s, threadname);

        log_s->cap = DEFAULT_MODULES_NUM;
        log_s->len = 0;
        log_s->modules = malloc(log_s->cap * sizeof(log_module_t)); 
        if (log_s->modules == NULL) {
            perror("failed to initialized module buffers\n");
        }

        // pre-allocate the buffers for the modules.
        _log_init_modules(0, DEFAULT_BUFFER_SIZE);
        initialized_s = 1;
    }
}

int log_find_module(const char* name) {
    for (size_t i=0; i<log_s->len; i++) {
        if (!strcmp(name, log_s->modules[i].name)) return i;
    }
    return -1;
}

void _log_set_config(log_config_t *dst, log_config_t *src) {
    dst->log_to_console = src->log_to_console;
    dst->level = src->level;
    dst->filename = NULL;
    if (src->filename != NULL) {
        dst->filename = malloc((strlen(src->filename) + 1) * sizeof(char));
        strcpy(dst->filename, src->filename);
    }
}

int log_add_module(const char* name, log_config_t config) {
    int found_module = 0, old_cap = 0;

    if (initialized_s == 0) {
        perror("logging is not initialized\n");
        exit(1);
    }

    found_module = log_find_module(name);
    if (found_module >= 0) {
        return found_module;
    }

    if (log_s->len > log_s->cap) {
        old_cap = log_s->cap;
        log_s->cap *= 2;
        log_s->modules = realloc(log_s->modules,
                                 log_s->cap * sizeof(log_module_t*)); 
        if (log_s->modules == NULL) {
            perror("failed to initialized module buffers\n");
            exit(1);
        }
        _log_init_modules(old_cap, DEFAULT_BUFFER_SIZE);
    }

    _log_set_config(&log_s->modules[log_s->len].config, &config);
    log_s->modules[log_s->len].file_fd = -1;
    if (log_s->modules[log_s->len].config.filename != NULL) {
        log_s->modules[log_s->len].file_fd = log_open_fd(log_s->modules[log_s->len].config.filename);
    }
    log_s->modules[log_s->len].name = malloc((strlen(name)+1)*sizeof(char));
    strcpy(log_s->modules[log_s->len].name, name);

    log_s->len++;
    return log_s->len-1;
}

void _log_flush(size_t idx) {
    if (idx >= log_s->len) {
        return;
    }

    if (log_s->modules[idx].config.log_to_console == 1) {
        printf("%s", log_s->modules[idx].buffer);
        return;
    }

    if (log_s->modules[idx].file_fd > 0) {
        int seek = lseek(log_s->modules[idx].file_fd, 0, SEEK_END);
        if (seek == -1) {
            perror("error appending to log file.\n");
        }
        int nr = write(log_s->modules[idx].file_fd,
                       log_s->modules[idx].buffer,
                       strlen(log_s->modules[idx].buffer));
        if (nr == -1) {
            perror("write error\n");
        } 
    } else {
        perror("bad file descriptor\n");
    }

    return;
}

void log_sprintf(size_t module_idx,
                 int level,
                 const char* func,
                 const char* file,
                 int line,
                 const char *fmt, ...) {
    va_list args;
    char error[128];
    struct timespec ts;
    struct tm tm;

    if (module_idx >= log_s->len) {
        sprintf(error, "logging module %lu is not initialized.\n", module_idx);
        perror(error);
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
    if (pos >= DEFAULT_BUFFER_SIZE-1) {
        log_s->modules[module_idx].buffer[DEFAULT_BUFFER_SIZE-2] = '\n';
        log_s->modules[module_idx].buffer[DEFAULT_BUFFER_SIZE-1] = '\0';
    }
    log_s->modules[module_idx].buffer[pos] = '\n';
    log_s->modules[module_idx].buffer[pos+1] = '\0';

    va_end(args);
}

//TODO this needs functions for encoding labels.
void mr_log_sprintf(size_t module_idx,
                    int level,
                    const char* func,
                    const char* file,
                    int line,
                    const char *msg, ...) {
    va_list args;
    char error[128], *label;
    struct timespec ts;
    struct tm tm;

    if (module_idx >= log_s->len) {
        sprintf(error, "logging module %lu is not initialized.\n", module_idx);
        perror(error);
        return;
    }

    if (log_s->modules[module_idx].config.level < level) {
        return;
    }

    clock_gettime(CLOCK_REALTIME, &ts);
    gmtime_r(&(ts.tv_sec), &tm);

    int pos = 0;
    char *buf = log_s->modules[module_idx].buffer;
    MR_SNPRINTF("{");
    //TODO: make the timestamp format configurable
    //TODO: in fact, this looks really repeatable code, so I assume I can
    //put these values in some vectors and then iterate through them
    MR_SNPRINTF("\"ts\":\""LOG_TIME_FORMAT"\",", LOG_TIME_VALUE(tm, ts));
    MR_SNPRINTF("\"hostname\":\"%s\",", hostname_s);
    MR_SNPRINTF("\"app\":\"%s\",", appname_s);
    MR_SNPRINTF("\"level\":\"%s\",", LOG_LEVEL_VALUE(level));
    MR_SNPRINTF("\"location\":\"%s():%s:%u\",", func, file, line);
    MR_SNPRINTF("\"thread\":\"%s\",", threadname_s);

    va_start(args, msg);
    MR_SNPRINTF("\"msg\":\"%s\"", msg);

    label = va_arg(args, char*);
    if (label != NULL) {
        while(strcmp(label, MR_LOG_TERMINATOR)) {
            MR_SNPRINTF(",");
            MR_SNPRINTF("%s", label);
            label = va_arg(args, char*);
        }
    }
    log_s->modules[module_idx].buffer[pos] = '}';
    log_s->modules[module_idx].buffer[pos+1] = '\n';
    log_s->modules[module_idx].buffer[pos+2] = '\0';

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
        case L_DEFAULT:
            return "Default";
            break;
        case L_CRIT:
            return "Critical";
            break;
        case L_ERR:
            return "Error";
            break;
        case L_INFO:
            return "Info";
            break;
        case L_WARN:
            return "Warn";
            break;
        case L_DBG:
            return "Debug";
            break;
        case L_MEM:
            return "Memory";
            break;
        default:
            return "Undefined";
            break;
    }
}

void log_destroy() {
    for (size_t i=0; i<log_s->cap; i++) {
        if (log_s->modules[i].name != NULL) {
            if (log_s->modules[i].config.filename != NULL && log_s->modules[i].file_fd != -1) {
                if (close(log_s->modules[i].file_fd == -1)) {
                    perror("closing the log file\n");
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

    free(hostname_s);
    free(appname_s);
    free(threadname_s);
}
