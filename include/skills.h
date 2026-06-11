#ifndef SKILLS_H
#define SKILLS_H

#include <stddef.h>

typedef struct {
    char *name;
    char *description;
    char *version;
    char *directory;
} skill_metadata_t;

typedef struct {
    skill_metadata_t *skills;
    size_t count;
} skill_list_t;

skill_list_t* skills_discover(const char *base_path);
void skills_free_list(skill_list_t *list);

char* skill_view_content(const char *base_path, const char *skill_name, const char *file_path);

#endif
