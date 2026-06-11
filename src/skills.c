#include "skills.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

skill_list_t* skills_discover(const char *base_path) {
    skill_list_t *list = malloc(sizeof(skill_list_t));
    if (!list) return NULL;
    list->skills = NULL;
    list->count = 0;

    DIR *dir = opendir(base_path);
    if (!dir) return list;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            char skill_path[1024];
            snprintf(skill_path, sizeof(skill_path), "%s/%s", base_path, entry->d_name);
            
            struct stat st;
            if (stat(skill_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                char skill_file[2048];
                snprintf(skill_file, sizeof(skill_file), "%s/SKILL.md", skill_path);
                
                FILE *f = fopen(skill_file, "r");
                if (f) {
                    skill_metadata_t *meta = malloc(sizeof(skill_metadata_t));
                    meta->name = strdup(entry->d_name);
                    meta->directory = strdup(entry->d_name);
                    meta->description = strdup("");
                    meta->version = strdup("1.0.0");

                    char line[1024];
                    int in_frontmatter = 0;
                    while (fgets(line, sizeof(line), f)) {
                        if (strncmp(line, "---", 3) == 0) {
                            in_frontmatter = !in_frontmatter;
                            continue;
                        }
                        if (in_frontmatter) {
                            char *desc_ptr = strstr(line, "description:");
                            if (desc_ptr) {
                                desc_ptr += 12;
                                while(*desc_ptr == ' ' || *desc_ptr == ':') desc_ptr++;
                                char *nl = strchr(desc_ptr, '\n');
                                if (nl) *nl = '\0';
                                meta->description = strdup(desc_ptr);
                            }
                            char *ver_ptr = strstr(line, "version:");
                            if (ver_ptr) {
                                ver_ptr += 8;
                                while(*ver_ptr == ' ' || *ver_ptr == ':') ver_ptr++;
                                char *nl = strchr(ver_ptr, '\n');
                                if (nl) *nl = '\0';
                                meta->version = strdup(ver_ptr);
                            }
                        }
                    }
                    fclose(f);

                    list->skills = realloc(list->skills, sizeof(skill_metadata_t) * (list->count + 1));
                    list->skills[list->count++] = *meta;
                    free(meta);
                } else {
                    free(entry->d_name);
                }
            } else {
                free(entry->d_name);
            }
        } else {
            free(entry->d_name);
        }
    }
    closedir(dir);
    return list;
}

void skills_free_list(skill_list_t *list) {
    if (!list) return;
    for (size_t i = 0; i < list->count; i++) {
        free(list->skills[i].name);
        free(list->skills[i].directory);
        free(list->skills[i].description);
        free(list->skills[i].version);
    }
    free(list->skills);
    free(list);
}

char* skill_view_content(const char *base_path, const char *skill_name, const char *file_path) {
    char path[2048];
    if (file_path && strlen(file_path) > 0) {
        snprintf(path, sizeof(path), "%s/%s/%s", base_path, skill_name, file_path);
    } else {
        snprintf(path, sizeof(path), "%s/%s/SKILL.md", base_path, skill_name);
    }

    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, f);
        buffer[length] = '\0';
    }
    fclose(f);
    return buffer;
}
