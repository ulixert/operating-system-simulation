# OS Simulation Project README

This README provides detailed instructions on how to set up, build, and run the OS Simulation project both locally and
using Docker. It also addresses common issues and provides troubleshooting tips.

## Table of Contents

- [Project Overview](#project-overview)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)
- [Building and Running Locally](#building-and-running-locally)
    - [Compilation](#compilation)
    - [Running the Simulation](#running-the-simulation)
- [Building and Running with Docker](#building-and-running-with-docker)
    - [Docker Setup](#docker-setup)
    - [Building the Docker Image](#building-the-docker-image)
- [Available Commands](#available-commands)
    - [File System Commands](#file-system-commands)
    - [Process and Thread Commands](#process-and-thread-commands)
    - [Other Commands](#other-commands)
- [Troubleshooting](#troubleshooting)
    - [Docker Compose Input Handling](#docker-compose-input-handling)
- [Additional Notes](#additional-notes)

## Project Overview

The OS Simulation project is a simplified operating system simulation written in C. It provides a shell interface where
users can execute commands to interact with a simulated file system, manage processes and threads, and perform basic
operations similar to those found in a Unix-like environment.

## Prerequisites

- GCC (GNU Compiler Collection)
- Make
- pthreads library (usually included with GCC)
- Docker (optional, if you choose to run the simulation in a Docker container)

## Project Structure

The project is organized into the following directories and files:

- `src/`: Contains the source code files.
- `include/`: Contains header files.
- `bin/`: Compiled binary executables are placed here after building.
- `Makefile`: Build configuration for compiling the project.

## Building and Running Locally

### Compilation

To build the OS Simulation project locally, navigate to the project's root directory and run:

```
make clean
make
```

- `make clean`: Cleans previous build files.
- `make`: Compiles the source code and builds the executable.

The compiled binary will be placed in the `bin/` directory.

### Running the Simulation

To run the OS Simulation:

```
./bin/os_simulation
```

This will start the simulation and present you with the user shell interface.

## Building and Running with Docker

### Docker Setup

Ensure you have Docker installed on your system. You can verify the installation by running:

```
docker --version
```

### Building the Docker Image

Build the Docker image using the provided `Dockerfile`:

```
docker build -t os_simulation .
```

### Running the Simulation in Docker

When using Docker Compose, you might encounter issues with interactive input not behaving correctly if you use:

```
docker compose up
```

To fix this issue, always use:

```
docker compose run os_simulation
```

This command ensures that the simulation runs interactively, allowing you to enter commands in the shell interface.

Alternatively, you can run the container directly using `docker run`:

```
docker run -it os_simulation
```

## Available Commands

Within the OS Simulation shell, you can use the following commands:

### File System Commands

- `ls`: List unhidden files and directories.
- `ls -a`: List all files and directories, including hidden ones.
- `touch <file>`: Create a new file.
- `rm <file>`: Delete a file.
- `mkdir <dir>`: Create a new directory.
- `rmdir <dir>`: Remove an empty directory.
- `cd <dir>`: Change the current directory. Use `cd` or `cd ~` to return to the project root.
- `mv <src> <dest>`: Move or rename a file or directory.
- `rename <old> <new>`: Rename a file or directory.
- `echo "message" > <file>`: Write a message to a file.
- `cat <file>`: Display the contents of a file.

### Process and Thread Commands

- `ps`: List all processes and threads.
- `kill <pid>`: Terminate a process by its process ID.
- `run <command> <time>`: Create a new process. Time is in seconds; omit or use `-1` for infinite.
- `thread <pid> <time>`: Create a new thread in a process. Time is in seconds; omit or use `-1` for infinite.

### Other Commands

- `exit`: Exit the OS Simulation shell.
- `help`: Display the help message with available commands.

## Troubleshooting

### Docker Compose Input Handling

When using Docker Compose, you might encounter issues with interactive input not behaving correctly if you use:

```
docker compose up
```

To fix this, use the following command instead:

```
docker compose run os_simulation
```

This ensures that the container runs interactively, allowing you to enter commands in the shell.

## Additional Notes

- The simulation includes both system processes and user-created processes and threads.
- Processes and threads may have finite or infinite execution times.
- Use the `ps` command to monitor the state of processes and threads.
- The simulation supports basic file system operations within the project directory.

## Example Usage

### Creating and Managing Processes

1. **Create a new process:**

   ```
   run my_process 10
   ```

2. **List processes and threads:**

   ```
   ps
   ```

3. **Terminate a process:**

   ```
   kill 121
   ```

### File Operations

1. **Create a new file:**

   ```
   touch newfile.txt
   ```

2. **Write to a file:**

   ```
   echo "Hello, World!" > newfile.txt
   ```

3. **Read a file:**

   ```
   cat newfile.txt
   ```

4. **Delete a file:**

   ```
   rm newfile.txt
   ```

--- 

## Contributing

1. Fork the repository.
2. Create a new branch (`feature-branch-name`).
3. Commit your changes.
4. Push to the branch.
5. Open a Pull Request.

## License

This project is licensed under the MIT License. See `LICENSE` for details.


---

Enjoy exploring the OS Simulation project!