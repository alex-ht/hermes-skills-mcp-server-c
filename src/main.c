#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "mcp_utils.h"
#include "skills.h"

void send_mcp_response(int id, cJSON *result) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "jsonrpc", "2.0");
    if (id >= 0) cJSON_AddNumberToObject(response, "id", id);
    cJSON_AddItemToObject(response, "result", result);
    
    char *out = cJSON_PrintUnformatted(response);
    if (out) {
        printf("%s\n", out);
        free(out);
    }
    cJSON_Delete(response);
}

void send_mcp_error(int id, const char *error_msg) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "jsonrpc", "2.0");
    if (id >= 0) cJSON_AddNumberToObject(response, "id", id);
    cJSON_AddStringToObject(response, "error", error_msg);
    
    char *out = cJSON_PrintUnformatted(response);
    if (out) {
        printf("%s\n", out);
        free(out);
    }
    cJSON_Delete(response);
}

int main() {
    char line[8192];
    while (fgets(line, sizeof(line), stdin)) {
        cJSON *root = cJSON_Parse(line);
        if (!root) continue;

        cJSON *id_obj = cJSON_GetObjectItemCaseSensitive(root, "id");
        int req_id = cJSON_IsNumber(id_obj) ? id_obj->valueint : -1;
        
        cJSON *method = cJSON_GetObjectItemCaseSensitive(root, "method");
        
        if (method && strcmp(method->valuestring, "tools/call") == 0) {
            cJSON *params = cJSON_GetObjectItemCaseSensitive(root, "params");
            cJSON *tool_name = cJSON_GetObjectItemCaseSensitive(params, "name");
            cJSON *args = cJSON_GetObjectItemCaseSensitive(params, "arguments");
            
            if (!tool_name) {
                send_mcp_error(req_id, "Missing tool name");
                cJSON_Delete(root);
                continue;
            }

            if (strcmp(tool_name->valuestring, "skills_list") == 0) {
                const char *cwd = cJSON_IsString(cJSON_GetObjectItemCaseSensitive(args, "cwd")) 
                                  ? cJSON_GetObjectItemCaseSensitive(args, "cwd")->valuestring 
                                  : ".";
                skill_list_t *list = skills_discover(cwd);
                
                cJSON *result_content = cJSON_CreateArray();
                cJSON *text_block = cJSON_CreateObject();
                cJSON_AddStringToObject(text_block, "type", "text");
                
                if (list) {
                    cJSON *skills_arr = cJSON_CreateArray();
                    for (size_t i = 0; i < list->count; i++) {
                        cJSON *s = cJSON_CreateObject();
                        cJSON_AddStringToObject(s, "name", list->skills[i].name);
                        cJSON_AddStringToObject(s, "description", list->skills[i].description ? list->skills[i].description : "");
                        cJSON_AddStringToObject(s, "version", list->skills[i].version);
                        cJSON_AddStringToObject(s, "directory", list->skills[i].directory);
                        cJSON_AddItemToArray(skills_arr, s);
                    }
                    cJSON_AddItemToObject(text_block, "skills", skills_arr);
                    skills_free_list(list);
                }
                cJSON_AddItemToArray(result_content, text_block);
                send_mcp_response(req_id, result_content);
                cJSON_Delete(result_content);

            } else if (strcmp(tool_name->valuestring, "skill_view") == 0) {
                cJSON *name = cJSON_GetObjectItemCaseSensitive(args, "name");
                const char *cwd = cJSON_IsString(cJSON_GetObjectItemCaseSensitive(args, "cwd")) 
                                  ? cJSON_GetObjectItemCaseSensitive(args, "cwd")->valuestring 
                                  : ".";
                
                if (cJSON_IsString(name)) {
                    char *content = skill_view_content(cwd, name->valuestring, NULL);
                    if (content) {
                        cJSON *result_content = cJSON_CreateArray();
                        cJSON *text_block = cJSON_CreateObject();
                        cJSON_AddStringToObject(text_block, "type", "text");
                        cJSON_AddStringToObject(text_block, "text", content);
                        cJSON_AddItemToArray(result_content, text_block);
                        
                        send_mcp_response(req_id, result_content);
                        free(content);
                        cJSON_Delete(result_content);
                    } else {
                        send_mcp_error(req_id, "Skill not found");
                    }
                }
            } else {
                send_mcp_error(req_id, "Unknown tool");
            }
        } else {
            cJSON_Delete(root);
            continue;
        }

        cJSON_Delete(root);
    }
    return 0;
}
EOF
