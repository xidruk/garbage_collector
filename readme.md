# XiGC - Lightweight Garbage Collector

A lightweight, cross-language garbage collector written in C with bindings for C++, and Python. XiGC provides simple memory management to help prevent memory leaks in your applications.

## Features

- Memory allocation tracking and automatic cleanup
- Cross-language support (C, C++, Python)
- Comprehensive error handling system
- Dynamic memory container that grows as needed
- Thread-safe memory operations
- API for manual and automatic memory management

## Architecture

XiGC consists of several key components:

1. **Memory Container**: Tracks allocated memory blocks and their status
2. **Error Manager**: Provides detailed error reporting
3. **Function API**: High-level interface for memory operations
4. **Language Bindings**: C++ wrapper and Python FFI interface

## API Overview

### C API

```c
// Initialize the garbage collector
int init_gc(_GC_memory_c **holder, errorm *error_stat);

// Allocate memory through the garbage collector
void *gc_alloc(errorm *error_stat, _GC_memory_c *memory_container, size_t size);

// Free a specific memory block
int gc_fr(errorm *error_stat, _GC_memory_c *memory_container, void *target);

// Free all memory blocks managed by the garbage collector
int gc_free_ev(errorm *error_stat, _GC_memory_c *memory_container);

// Destroy the garbage collector instance
int gc_destroy_api(errorm *error_stat, _GC_memory_c **memory_container);
