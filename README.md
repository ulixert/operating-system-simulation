# Operating System Simulation

This repository contains a simple modular operating system simulation written in C, designed to be run locally or inside a Docker container. It features modularized components such as a kernel, user shell, system calls, and process/thread management.

## Features

1. **Interactive User Shell**: Simulated shell to execute commands like `ls`, `touch`, `help`, etc.
2. **Kernel Simulation**: Handles core functionalities like initialization and interrupt handling.
3. **System Calls**: Acts as an interface between user commands and kernel functions.
4. **Process Management**: Simulated process creation, termination, and listing.
5. **Docker Support**: Fully containerized for easy deployment and testing.

---

## Setup Instructions

### Prerequisites

For Local Run:
- GCC installed on your system
- Make installed on your system

For Docker Run:
- Docker installed on your system
- Docker Compose installed

---

## Quick Start

### Local Run

1. Clone this repository:
   ```sh
   git clone https://github.com/yourusername/os-simulation.git
   cd os-simulation
   ```

2. Build the project:
   ```sh
   make
   ```

3. Run the simulation locally:
   ```sh
   ./bin/os_simulation
   ```

4. Clean up build files:
   ```sh
   make clean
   ```

---

### Docker Run

1. Build and start the service in detached mode:
   ```sh
   docker compose up -d
   ```

2. Run the container interactively:
   ```sh
   docker compose run os_simulation
   ```

3. Stop the services:
   ```sh
   docker compose down
   ```

---

## Commands

Once inside the OS simulation, the following commands are available:

- `ls`: List all files.
- `touch <file>`: Create a new file.
- `rm <file>`: Delete a file.
- `mkdir <dir>`: Create a new directory.
- `rmdir <dir>`: Remove a directory.
- `ps`: List all running processes.
- `kill <pid>`: Terminate a process by its ID.
- `help`: Display available commands.
- `exit`: Exit the OS simulation.

---

## Development

### Build Locally
To rebuild the project after making changes:
```sh
make
```

### Build the Docker Image
To rebuild the Docker image after making changes:
```sh
docker compose build
```

### Clean Up
To clean up build files and Docker volumes:
```sh
# Local
make clean

# Docker
docker compose down -v
```

---

## Project Structure

```text
os-simulation/
├── Dockerfile
├── docker-compose.yml
├── Makefile
├── include/
│   ├── user.h
│   ├── system_calls.h
│   ├── kernel.h
│   ├── interrupt.h
│   ├── process.h
│   ├── thread.h
│   └── system_services.h
├── src/
│   ├── user.c
│   ├── system_calls.c
│   ├── kernel.c
│   ├── interrupt.c
│   ├── process.c
│   ├── thread.c
│   └── system_services.c
├── main.c
└── README.md
```

---

## Known Issues

1. **Docker Compose Input Handling**:
   When using `docker compose up`, `stdin` might not behave interactively. To fix, always use:
   ```sh
   docker compose run os_simulation
   ```

2. **Buffering in Non-TTY Mode**:
   The application may exhibit unexpected buffering when not run interactively. To force interactivity, ensure `tty: true` and `stdin_open: true` in `docker-compose.yml`.

---

## Contributing

1. Fork the repository.
2. Create a new branch (`feature-branch-name`).
3. Commit your changes.
4. Push to the branch.
5. Open a Pull Request.

---

## License

This project is licensed under the MIT License. See `LICENSE` for details.