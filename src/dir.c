#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "error.h"
#include "dir.h"

static int compare(const void* entry_1, const void* entry_2) {
  return strcmp(((entry_t*)entry_1)->name, ((entry_t*)entry_2)->name);
}

entries_t* dir_read(char* dirname) {
  DIR* dir = opendir(dirname);
  struct dirent* entry;
  
  if (!dir) {
    entries_t* entries;
    HE((entries = malloc(sizeof(entries_t) + sizeof(entry_t))) == NULL);
    
    entries->amount = 1;
    entries->entries[0].type = ENTRY_TYPE_ERROR;
    strncpy(entries->entries[0].name, strerror(errno), 81);
    
    return entries;
  }
  
  unsigned amount = 0;
  
  while (readdir(dir)) {
    amount++;
  }
  rewinddir(dir);
  
  entries_t* temp_entries;
  HE((temp_entries = malloc(sizeof(entries_t) + sizeof(entry_t) * amount)) == NULL);
  
  unsigned dirs = 0;
  
  unsigned i = 0;
  
  while ((entry = readdir(dir))) {
    switch (entry->d_type) {
      case DT_DIR:
        temp_entries->entries[i].type = ENTRY_TYPE_DIR;
        dirs++;
      break;
      case DT_LNK:;
        char* name;
        HE((name = malloc(strlen(dirname) + 1 + strlen(entry->d_name) + 1)) == NULL);
        
        strcpy(name, dirname);
        strcat(name, "/");
        strcat(name, entry->d_name);
        
        struct stat st;
        int stat_result = stat(name, &st);
        
        free(name);
        
        if (stat_result < 0) {
          temp_entries->entries[i].type = ENTRY_TYPE_ERROR;
        } else {
          if ((st.st_mode & S_IFMT) == S_IFDIR) {
            temp_entries->entries[i].type = ENTRY_TYPE_DIR;
            dirs++;
          } else {
            temp_entries->entries[i].type = ENTRY_TYPE_FILE;
          }
        }
      break;
      default:
        temp_entries->entries[i].type = ENTRY_TYPE_FILE;
      break;
    }
    
    strncpy(temp_entries->entries[i].name, entry->d_name, 81);
    // strncpy does not guaranteee a null terminator.
    temp_entries->entries[i].name[80] = '\0';
    i++;
  }
  
  closedir(dir);
  
  entries_t* entries;
  HE((entries = malloc(sizeof(entries_t) + sizeof(entry_t) * amount)) == NULL);
  entries->amount = amount;
  
  unsigned j = 0;
  unsigned k = 0;
  
  for (i = 0; i < amount; i++) {
    if (temp_entries->entries[i].type == ENTRY_TYPE_DIR) {
      entries->entries[j].type = temp_entries->entries[i].type;
      strcpy(entries->entries[j].name, temp_entries->entries[i].name);
      j++;
    } else {
      entries->entries[dirs+k].type = temp_entries->entries[i].type;
      strcpy(entries->entries[dirs+k].name, temp_entries->entries[i].name);
      k++;
    }
  }
  
  free(temp_entries);
  
  qsort(entries->entries, j, sizeof(entry_t), compare);
  qsort(&entries->entries[dirs], k, sizeof(entry_t), compare);
  
  return entries;
}
