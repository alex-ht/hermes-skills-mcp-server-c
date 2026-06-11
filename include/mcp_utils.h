#ifndef MCP_UTILS_H
#define MCP_UTILS_H

#include <stdbool.h>
#include <cjson/cJSON.h>

void mcp_send_response(int request_id, cJSON *response_data, bool is_error);

#endif
