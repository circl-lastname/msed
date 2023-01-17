#pragma once

typedef enum {
  ENTRY_TYPE_FILE,
  ENTRY_TYPE_DIR,
  ENTRY_TYPE_ERROR,
} entry_type_t;

typedef struct {
  entry_type_t type;
  char name[81];
} entry_t;

typedef struct {
  unsigned amount;
  entry_t entries[];
} entries_t;

entries_t* dir_read(char* dirname);
