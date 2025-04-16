#!/usr/bin/env python3

import ctypes
import os
import sys
from ctypes import c_int, c_void_p, c_size_t, c_char_p, POINTER, Structure, byref

# Define error manager structure
class ErrorM(Structure):
    _fields_ = [
        ("error_status", c_int),
        ("error_message", c_char_p)
    ]

# Define garbage collector structure (opaque pointer is fine for our usage)
class GCMemoryC(Structure):
    pass

# Load the library
try:
    lib_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "lib", "libgbxc.so")
    gbxc = ctypes.CDLL(lib_path)
except Exception as e:
    print(f"Failed to load library: {e}")
    sys.exit(1)

# Define function prototypes
gbxc.init_errorm.restype = POINTER(ErrorM)
gbxc.free_errorm.argtypes = [POINTER(ErrorM)]

gbxc.init_gc.argtypes = [POINTER(POINTER(GCMemoryC)), POINTER(ErrorM)]
gbxc.init_gc.restype = c_int

gbxc.gc_alloc.argtypes = [POINTER(ErrorM), POINTER(GCMemoryC), c_size_t]
gbxc.gc_alloc.restype = c_void_p

gbxc.gc_fr.argtypes = [POINTER(ErrorM), POINTER(GCMemoryC), c_void_p]
gbxc.gc_fr.restype = c_int

gbxc.gc_free_ev.argtypes = [POINTER(ErrorM), POINTER(GCMemoryC)]
gbxc.gc_free_ev.restype = c_int

gbxc.gc_destroy_api.argtypes = [POINTER(ErrorM), POINTER(POINTER(GCMemoryC))]
gbxc.gc_destroy_api.restype = c_int

def print_error(err):
    """Print error details if error is not None"""
    if err:
        print(f"Error status: {err.contents.error_status}")
        print(f"Error message: {err.contents.error_message.decode('utf-8')}")

def test_basic_allocation(err, gc):
    """Test basic memory allocation and freeing"""
    print("\n=== Testing Basic Allocation in Python ===")
    
    # Allocate memory
    size = 100
    ptr = gbxc.gc_alloc(err, gc, size)
    if not ptr:
        print_error(err)
        return
    print(f"Memory allocated successfully at address {ptr}")
    
    # Write to memory using ctypes
    message = b"Hello from Python FFI!"
    ctypes.memmove(ptr, message, len(message) + 1)  # +1 for null terminator
    
    # Read back the message
    result = ctypes.cast(ptr, c_char_p).value
    print(f"String stored: {result.decode('utf-8')}")
    
    # Free the memory
    result = gbxc.gc_fr(err, gc, ptr)
    if result != 0:
        print_error(err)
    else:
        print("Memory freed successfully")
    
    # Try to free it again (should fail)
    result = gbxc.gc_fr(err, gc, ptr)
    if result != 0:
        print(f"Expected error when freeing again: {err.contents.error_message.decode('utf-8')}")

def test_multiple_allocations(err, gc):
    """Test multiple allocations and free operations"""
    print("\n=== Testing Multiple Allocations in Python ===")
    
    # Allocate 150 integer blocks
    num_blocks = 150
    int_size = ctypes.sizeof(c_int)
    pointers = []
    
    print(f"Allocating {num_blocks} integer blocks...")
    for i in range(num_blocks):
        ptr = gbxc.gc_alloc(err, gc, int_size)
        if not ptr:
            print(f"Failed at allocation {i}: {err.contents.error_message.decode('utf-8')}")
            break
        
        # Set the value
        int_ptr = ctypes.cast(ptr, POINTER(c_int))
        int_ptr.contents.value = i
        
        pointers.append(ptr)
    
    print(f"Successfully allocated {len(pointers)} blocks")
    
    # Verify data
    all_correct = True
    for i, ptr in enumerate(pointers):
        int_ptr = ctypes.cast(ptr, POINTER(c_int))
        if int_ptr.contents.value != i:
            print(f"Data verification failed at index {i}")
            all_correct = False
            break
    
    if all_correct:
        print("Data verification successful")
    
    # Free every other block
    print("Freeing odd-indexed blocks...")
    for i in range(1, len(pointers), 2):
        if gbxc.gc_fr(err, gc, pointers[i]) != 0:
            print(f"Failed to free block {i}: {err.contents.error_message.decode('utf-8')}")
    
    # Free remaining blocks using gc_free_ev
    print("Freeing all remaining blocks...")
    if gbxc.gc_free_ev(err, gc) != 0:
        print_error(err)
    else:
        print("All blocks freed successfully")

def main():
    # Initialize error manager
    err = gbxc.init_errorm()
    if not err:
        print("Failed to initialize error manager")
        return 1
    
    print("=== Garbage Collector Python FFI Test ===")
    
    # Initialize the garbage collector
    gc_ptr = POINTER(GCMemoryC)()
    if gbxc.init_gc(byref(gc_ptr), err) != 0:
        print(f"Failed to initialize garbage collector: {err.contents.error_message.decode('utf-8')}")
        gbxc.free_errorm(err)
        return 1
    
    print("Garbage collector initialized successfully")
    
    # Run tests
    test_basic_allocation(err, gc_ptr)
    test_multiple_allocations(err, gc_ptr)
    
    # Clean up
    print("\n=== Cleanup ===")
    if gbxc.gc_destroy_api(err, byref(gc_ptr)) != 0:
        print(f"Error during garbage collector destruction: {err.contents.error_message.decode('utf-8')}")
    else:
        print("Garbage collector destroyed successfully")
    
    gbxc.free_errorm(err)
    print("Test suite completed")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())