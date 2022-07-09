#pragma once

// These are called from a macro externaly to this unit.
// They should be seen as internal functions however, and should not be used directly.
void _log_flush(size_t module_idx);
void _mr_log_flush();
