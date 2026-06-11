#include "mcp_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

void mcp_send_response(int request_id, cJSON *response_data, bool is_error) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "jsonrpc", "2.0");
    if (request_id >= 0) cJSON_AddNumberToObject(root, "id", request_id);
    
    if (is_error) {
        cJSON_AddStringToObject(root, "error", response_data);
    } else {
        cJSON_AddItemToObject(root, "result", response_data);
    }
    
    char *out = cJSON_PrintUnformatted(root);
    if (out) {
        printf("%s\n", out);
        free(out);
    }
    cJSON_Delete(root);
}
