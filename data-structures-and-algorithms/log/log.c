#include "log.h" 
#include <stdarg.h>
#include <threads.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

typedef struct log_module {
    char *name;
    char *buffer;
    int file_fd;
    log_config_t config;
} log_module_t;

static char* hostname_s;
static char* appname_s;
//TODO need to think about thread names
thread_local static char* threadname_s;
static int initialized_s = 0;
static log_module_t *modules_s;
static size_t modules_len_s;
static size_t modules_cap_s;

int log_open_fd(const char* filename) {
    int fd = open (filename, O_WRONLY);
    if (fd == -1) {
        perror("error opening log file\n");
    }
    return fd;
}

void log_set_thread_name(const char* threadname) {
    threadname_s = malloc((strlen(threadname_s)+1)*sizeof(char));
    strcpy(threadname_s, threadname);
}

void log_init(const char* appname) {
    if (initialized_s == 0) {
        // don't initialized twice
        hostname_s = malloc(HOST_NAME_MAX*sizeof(char));
        gethostname(hostname_s, HOST_NAME_MAX);
        appname_s = malloc((strlen(appname)+1)*sizeof(char));
        strcpy(appname_s, appname);
        threadname_s = malloc((strlen("main")+1)*sizeof(char));
        strcpy(threadname_s, "main");
        initialized_s = 1;
        modules_cap_s = 10;
        modules_len_s = 0;
        modules_s = malloc(modules_cap_s*sizeof(char*)); 
        if (modules_s == NULL) {
            perror("failed to initialized module buffers\n");
        }
        for (size_t i=0; i<modules_cap_s; i++) {
            modules_s[i].buffer = malloc(MAX_BUFFER_SIZE*(sizeof(char)));
            if (modules_s[i].buffer == NULL) {
                perror("failed to initialized module buffer\n");
            }
        }
        LOG_ADD_DEFAULT_MODULE("default", L_INFO);
    }
}

int log_find_module(const char* name) {
    for (size_t i=0; i<modules_len_s; i++) {
        if (!strcmp(name, modules_s[i].name)) return i;
    }
    return -1;
}

int log_add_module(const char* name, log_config_t config) {
    if (initialized_s == 0) {
        perror("logging is not initialized\n");
        exit(1);
    }

    int found_module = log_find_module(name);
    if (found_module >= 0) {
        return found_module;
    }

    if (modules_len_s >= modules_cap_s) {
        modules_s = realloc(modules_s,
                               2*modules_cap_s*sizeof(char*)); 
        if (modules_s == NULL) {
            perror("failed to initialized module buffers\n");
        }
        for (size_t i=modules_cap_s; i<2*modules_cap_s; i++) {
            modules_s[i].buffer = malloc(MAX_BUFFER_SIZE*(sizeof(char)));
            if (modules_s[i].buffer == NULL) {
                perror("failed to initialized module buffer\n");
            }
        }
        modules_cap_s *= 2;
    }

    memcpy(&modules_s[modules_len_s].config, &config, sizeof(log_config_t));
    if (modules_s[modules_len_s].config.filename != NULL) {
        modules_s[modules_len_s].file_fd = log_open_fd(modules_s[modules_len_s].config.filename);
    }
    modules_s[modules_len_s].name = malloc(strlen(name)*sizeof(char));
    strcpy(modules_s[modules_len_s].name, name);

    modules_len_s++;
    return modules_len_s-1;
}

//TODO idx might be out of bounds here.
void _log_flush(size_t idx) {
    if (modules_s[idx].config.log_to_console == 1) {
        printf("%s", modules_s[idx].buffer);
    }
    if (modules_s[idx].file_fd > 0) {
        int seek = lseek(modules_s[idx].file_fd, 0, SEEK_END);
        if (seek == -1) {
            perror("error appending to log file.\n");
        }
        int nr = write(modules_s[idx].file_fd,
                       modules_s[idx].buffer,
                       strlen(modules_s[idx].buffer));
        if (nr == -1) {
            perror("write error\n");
        } 
    }
}

//TODO idx might be out of bounds here.
//TODO the macros and this function should be refactored so that the level
// is one of the parameters. Otherwise lower log levels than what is set cannot be
// skipped.
void log_sprintf(int module_idx,
                 int level,
                 const char* func,
                 const char* file,
                 int line,
                 const char *fmt, ...) {
    va_list args;
    char error[128];

    if (modules_s+module_idx == NULL || modules_s[module_idx].name == NULL) {
        sprintf(error, "logging module %d is not initialized.\n", module_idx);
        perror(error);
        exit(1);
    }

    struct timespec ts;
    struct tm tm;

    clock_gettime(CLOCK_REALTIME, &ts);
    gmtime_r(&(ts.tv_sec), &tm);

    int pos = sprintf(modules_s[module_idx].buffer, LOG_HEADER_FORMAT, LOG_HEADER_VAL);

    va_start(args, fmt);
    // https://stackoverflow.com/questions/150543/forward-an-invocation-of-a-variadic-function-in-c
    // also from "man printf":
    // The functions vprintf(),  vfprintf(),  vdprintf(),  vsprintf(),	vsnprintf()  are
    // equivalent   to	 the   functions   printf(),  fprintf(),  dprintf(),  sprintf(),
    // snprintf(), respectively, except that they are called with a va_list instead of a
    // variable number of arguments.  These functions do not call the va_end macro.  Be‐
    // cause they invoke the va_arg macro, the value of ap is undefined after the  call.
    // See stdarg(3).
    pos += vsnprintf(modules_s[module_idx].buffer+pos, MAX_BUFFER_SIZE, fmt, args);
    if (pos >= MAX_BUFFER_SIZE) {
        modules_s[module_idx].buffer[pos-1] = '\n';
        modules_s[module_idx].buffer[pos] = '\0';
    }
    modules_s[module_idx].buffer[pos] = '\n';
    modules_s[module_idx].buffer[pos+1] = '\0';

    va_end(args);
}

void log_set_Level(int idx, uint8_t lvl) {
    modules_s[idx].config.level = lvl;
}

uint8_t log_get_level(int idx) {
    return modules_s[idx].config.level;
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

void log_close(int idx) {
    if (modules_s[idx].file_fd != -1) {
        // TODO what happens if buffer isn't flushed?
        if (close(modules_s[idx].file_fd == -1)) {
            perror("closing the log file\n");
        }
    }
}
