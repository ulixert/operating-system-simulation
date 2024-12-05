# OS Simulation Project README

This README provides updated instructions for the OS Simulation project, reflecting recent changes where interrupts are
implemented and the CPU and kernel continue to operate dynamically after initialization. The CPU loop now uses shorter
sleep intervals, resulting in faster increments of CPU time and more responsive process state transitions.

## Table of Contents

- [Project Overview](#project-overview)
- [Prerequisites](#prerequisites)
- [Project Structure](#project-structure)
- [Quick Start](#quick-start)
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
- [Example Usage](#example-usage)
- [Contributing](#contributing)
- [License](#license)

## Project Overview

The OS Simulation project emulates core functionalities of an operating system in a simplified environment. With
interrupts implemented, the CPU and kernel now run continuously after initialization. A periodic timer interrupt
triggers scheduling, allowing processes and threads to rotate through various states, accumulate CPU time, and terminate
as their time elapses.

By decreasing sleep intervals in the CPU loop (e.g., from 1 second to a few milliseconds), you’ll see processes gain CPU
time and change states much more rapidly, making the simulation feel closer to a real OS.

## Prerequisites

- GCC (GNU Compiler Collection)
- Make
- pthreads library (usually included with GCC)
- Docker (optional, if you choose to run the simulation in a Docker container)

## Project Structure

The project is organized as follows:

- `src/`: Contains the source code files.
- `include/`: Contains header files.
- `bin/`: Compiled binary executables are placed here after building.
- `Makefile`: Build configuration for compiling the project.

## Quick Start

Clone this repository:

   ```sh
   git clone https://github.com/ulixert/os-simulation.git
   cd os-simulation
   ```

You can now build and run the OS Simulation project locally or with Docker.

## Building and Running Locally

### Compilation

To build the OS Simulation project locally:

```
make clean
make
```

- `make clean`: Cleans previous build files.
- `make`: Compiles the source code and builds the executable.

The compiled binary will be placed in the `bin/` directory.

### Running the Simulation

Run the OS Simulation:

```
./bin/os_simulation
```

This starts the simulation and presents the user shell. You can now observe processes and threads dynamically changing.

## Building and Running with Docker

### Docker Setup

Check Docker installation:

```
docker --version
```

### Building the Docker Image

Build the Docker image:

```
docker build -t os_simulation .
```

### Running the Simulation in Docker

When using Docker Compose:

```
docker compose run os_simulation
```

ensures interactive functionality. Alternatively:

```
docker run -it os_simulation
```

runs the simulation in an interactive terminal.

## Available Commands

### File System Commands

- `ls`, `ls -a`
- `touch <file>`
- `rm <file>`
- `mkdir <dir>`
- `rmdir <dir>`
- `cd <dir>` (use `cd` or `cd ~` to go home)
- `mv <src> <dest>`
- `rename <old> <new>`
- `echo "message" > <file>`
- `cat <file>`

### Process and Thread Commands

- `ps`: List all processes and threads.
- `kill <pid>`: Terminate a process.
- `run <command> <time>`: Create a new process (time in seconds, `-1` for infinite).
- `thread <pid> <time>`: Create a new thread in a process (time in seconds, `-1` for infinite).

### Other Commands

- `exit`: Exit the simulation.
- `help`: Display available commands.

## Troubleshooting

### Docker Compose Input Handling

If interactive input fails with:

```
docker compose up
```

use:

```
docker compose run os_simulation
```

for proper interactivity.

## Additional Notes

- Interrupts now drive scheduling, causing processes to run or yield frequently.
- Shortened sleep intervals in the CPU loop result in quicker CPU time increments and more dynamic state changes.
- To gracefully stop the CPU thread and finalize the simulation, call `finalize_cpu()` after `user_shell()` returns.

## Example Usage

Creating and managing processes:

```
run my_process 10
```

List processes:

```
ps
```

Terminate a process:

```
kill 121
```

File operations:

```
touch newfile.txt
```

```
echo "Hello, World!" > newfile.txt
```

```
cat newfile.txt
```

```
rm newfile.txt
```

## Contributing

1. Fork the repository.
2. Create a new branch (`feature-branch-name`).
3. Commit changes.
4. Push to the branch.
5. Open a Pull Request.

## License

Licensed under the MIT License. See `LICENSE` for details.

Enjoy exploring the enhanced OS Simulation project, now with interrupts and more dynamic CPU and kernel behavior!