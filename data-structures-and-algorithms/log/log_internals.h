#pragma once

//TODO idx might be out of bounds here.
void _log_flush(size_t module_idx);
void _mr_log_flush();
void _log_init_modules(size_t start, size_t buf_size);
void _log_set_config(log_config_t *dst, log_config_t *src);
