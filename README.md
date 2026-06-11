# Agent Skills MCP Server (C Version) ⚡

A high-performance, lightweight, and memory-efficient C implementation of the Agent Skills MCP (Model Context Protocol) Server. This server allows AI agents to discover, view, and manage skills using the standard `SKILL.md` format with YAML frontmatter.

This implementation is designed for **pure OpenClaw environments** and is optimized for speed and minimal resource footprint.

## 🚀 Key Features

- **Ultra-Lightweight**: Written in pure C, consuming minimal RAM and CPU.
- **Zero-Dependency (Mostly)**: Uses standard `libcjson` and `libyaml` for reliable parsing.
- **Standard MCP Protocol**: Implements the JSON-RPC 2.0 protocol via `stdin/stdout` for seamless integration with agents.
- **Workspace Context Aware**: Supports the `cwd` parameter, allowing agents to switch between different skill directories dynamically.
- **Automatic Discovery**: Scans directories for `SKILL.md` files and parses metadata automatically.

## 🛠️ Prerequisites

To build this server from source, you need a C compiler (like `gcc`) and the following development libraries:

- `libcjson-dev` (for JSON-RPC communication)
- `libyaml-dev` (for parsing skill frontmatter)

On Ubuntu/Debian, you can install them via:
```bash
sudo apt-get update && sudo apt-get install -y libcjson-dev libyaml-dev
```

## 🔨 Building from Source

1. **Clone the repository**:
   ```bash
   git clone https://github.com/alex-ht/hermes-skills-mcp-server-c.git
   cd hermes-skills-mcp-server-c
   ```

2. **Compile the project**:
   ```bash
   make
   ```

This will generate an executable named `hermes-mcp-server`.

## 🚀 Usage

### Running the Server
Once compiled, you can start the server in interactive mode:
```bash
./hermes-mcp-server
```

### Integrating with OpenClaw
You can register this server as an MCP tool in OpenClaw by adding it to your configuration:

```bash
openclaw mcp set agent-skills '{
  "command": "/absolute/path/to/hermes-mcp-server",
  "args": []
}'
```

### Example Tool Calls

**1. List Skills:**
```json
{
  "jsonrpc": "2.0",
  "method": "tools/call",
  "params": {
    "name": "skills_list",
    "arguments": { "cwd": "./skills" }
  },
  "id": 1
}
```

**2. View a Specific Skill:**
```json
{
  "jsonrpc": "2.0",
  "method": "tools/call",
  "params": {
    "name": "skill_view",
    "arguments": { "name": "my-skill", "cwd": "./skills" }
  },
  "id": 2
}
```

## 📁 Project Structure

- `include/`: Header files (`skills.h`, `mcp_utils.h`).
- `src/`: Source files (`main.c`, `skills.c`, `mcp_utils.c`).
- `Makefile`: Build automation script.

## 📜 License

This project is licensed under the MIT License.
