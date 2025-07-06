# Ring Buffer IPC

A lock-free ring buffer implementation for inter-CPU communication using shared memory.
Includes unit tests (Unity framework) and a real shared-memory demo (producer/consumer).

## Build

```bash
make
```

## Run unit tests

```bash
./ring_buffer_test
```

## Run real-world demo

Terminal 1:
```bash
./demo/shm_writer
```
Terminal 2:
```bash
./demo/shm_reader
```
