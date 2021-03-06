
#pragma once

#include /utils/common.h"
#include /utils/Slice.h"
#include /utils/Status.h"

namespace td {

Status setup_signals_alt_stack() TD_WARN_UNUSED_RESULT;

enum class SignalType { Abort, Error, Quit, Pipe, HangUp, User, Other };

Status set_signal_handler(SignalType type, void (*func)(int sig)) TD_WARN_UNUSED_RESULT;

Status set_extended_signal_handler(SignalType type, void (*func)(int sig, void *addr)) TD_WARN_UNUSED_RESULT;

Status set_runtime_signal_handler(int runtime_signal_number, void (*func)(int sig)) TD_WARN_UNUSED_RESULT;

Status ignore_signal(SignalType type) TD_WARN_UNUSED_RESULT;

// writes data to the standard error stream in a signal-safe way
void signal_safe_write(Slice data, bool add_header = true);

void signal_safe_write_signal_number(int sig, bool add_header = true);

void signal_safe_write_pointer(void *p, bool add_header = true);

Status set_default_failure_signal_handler();

}  // namespace td
