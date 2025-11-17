#ifndef UTILS_H
#define UTILS_H

int colorNameToCode(const char *name);
char* readConfigValue(const char *path, const char *key);
int writeConfigValue(const char *path, const char *key, const char *value);

#endif
