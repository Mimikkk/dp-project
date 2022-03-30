#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define var auto
#define let const var
#define loop for(;;)

extern char *str(const char *format, ...);
