# Ring Buffer IPC

A lock-free ring buffer implementation for inter-CPU communication using shared memory.
Designed for embedded or low-level systems where atomic instructions and memory barriers are critical.

## Features

- Single Producer / Single Consumer
- Lock-free, uses memory barriers
- Suitable for shared DDR with per-core caches
- Testable via pthread simulation on Linux

## Build and Run

```bash
make
./ring_buffer_test
```

## License

GPL-2.0-only
