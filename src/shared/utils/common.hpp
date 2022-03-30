#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define var auto
#define let const var
#define loop for(;;)
#define noreturn _Noreturn void

extern char *str(const char *format, ...);
