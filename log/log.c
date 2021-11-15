#include "log.h" 

static char **log_internal_buffer_g;
static int log_initialized_g = 0;
static size_t log_module_len_g = 0;
static size_t log_module_cap_g = 0;
static int log_file_fd_g;
static int log_seek_g;
static uint8_t log_level_g;

void log_open_fd(const char* filename) {
    int fd = open (filename, O_WRONLY);
    if (fd == -1) {
        perror("error opening log file");
    }
    log_file_fd_g = fd;
    log_seek_g = 0;
}

int log_init(log_config_t config) {
    if (log_initialized_g == 0) {
        log_module_cap_g = 10;
        log_internal_buffer_g = malloc(log_module_cap_g*sizeof(char*)); 
        if (log_internal_buffer_g == NULL) {
            perror("failed to initialized module buffers");
        }
        for (size_t i=0; i<log_module_cap_g; i++) {
            log_internal_buffer_g[i] = malloc(MAX_BUFFER_SIZE*(sizeof(char)));
            if (log_internal_buffer_g[i] == NULL) {
                perror("failed to initialized module buffer");
            }
        }
    }
    log_module_len_g++;
    if (log_module_len_g >= log_module_cap_g) {
        log_internal_buffer_g = realloc(log_internal_buffer_g,
                                        2*log_module_cap_g*sizeof(char*)); 
        if (log_internal_buffer_g == NULL) {
            perror("failed to initialized module buffers");
        }
        for (size_t i=log_module_cap_g; i<2*log_module_cap_g; i++) {
            log_internal_buffer_g[i] = malloc(MAX_BUFFER_SIZE*(sizeof(char)));
            if (log_internal_buffer_g[i] == NULL) {
                perror("failed to initialized module buffer");
            }
        }
        log_module_cap_g *= 2;
    }

    memcpy(&log_config_g, &config, sizeof(log_config_t));
    if (log_config_g.filename != NULL) {
        log_open_fd(log_config_g.filename);
    }

    return log_module_len_g-1;
}

void _log_flush() {
    if (log_config_g.log_to_console == 1) {
        for (size_t i=0; i<log_module_len_g; i++) {
            printf("%s", log_internal_buffer_g[i]);
        }
    }
    if (log_file_fd_g > 0) {
        int seek = lseek(log_file_fd_g, 0, SEEK_END);
        if (seek == -1) {
            perror("error appending to log file.");
        }
        for (size_t i=0; i<log_module_len_g; i++) {
            int nr = write(log_file_fd_g,
                           log_internal_buffer_g[i],
                           strlen(log_internal_buffer_g[i]));
            if (nr == -1) {
                perror("write error");
            } 
        }
    }
} 

void log_set_Level(uint8_t lvl) { log_level_g = lvl; };

uint8_t log_get_level() { return log_level_g; }

void log_close(){
    if (log_file_fd_g != -1) {
        // TODO what happens if buffer isn't flushed?
        if (close(log_file_fd_g == -1)) {
            perror("closing the log file");
        }
    }
}
