#ifndef DEBUG_H
#define DEBUG_H

#define ERROR(line, ...) do { printf("Error <line %d>: ", line); printf(__VA_ARGS__); } while (0)

#define DEBUG(...) do { if (debugMode) printf(__VA_ARGS__); } while (0)

#endif //DEBUG_H
