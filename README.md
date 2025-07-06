# ringbuffer-ipc

A lock-free, production-grade **ring buffer** implementation in C for inter-CPU/process communication using shared memory, featuring:

- **Unity unit tests** for robust coverage
- **Real-world demo**: Two separate processes (writer/reader) using POSIX shared memory (`shm_open`/`mmap`)
- **Modern build system**: All build outputs go to `build/`

---

## 📁 Project Structure
- include/ # API header (ring_buffer.h)

- src/ # Ring buffer implementation

- test/ # Unit tests & Unity framework

- demo/ # Real-world demo: producer/consumer via shared memory

- build/ # All build outputs (created by Makefile)

- LICENSE # GPLv2 (Linux kernel style)

- Makefile # Build system

- README.md



---

## 🚀 Quick Start

### Build everything

```
make

### Run unit tests
```
./build/ring_buffer_test
```

Run the real shared-memory demo
Open two terminals:

Terminal 1 (producer):
```
./build/shm_writer
```
Terminal 2 (consumer):
```
./build/shm_reader
```