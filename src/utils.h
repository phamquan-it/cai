#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void trim(char *str);
void to_lower(char *str);
int starts_with(const char *str, const char *prefix);
void clear_screen();

#endif
