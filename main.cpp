// main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "gdefs.h"
#include "gheader.h"

class GarbageCollectorWrapper {
private:
    _GC_memory_c* gc;
    errorm* err;
    
public:
    GarbageCollectorWrapper() : gc(nullptr), err(nullptr) {
        // Initialize error manager
        err = init_errorm();
        if (!err) {
            std::cerr << "Failed to initialize error manager" << std::endl;
            throw std::runtime_error("Error manager initialization failed");
        }
        
        // Initialize garbage collector
        if (init_gc(&gc, err) != 0) {
            std::cerr << "Failed to initialize garbage collector: " 
                      << err->error_message << std::endl;
            free_errorm(err);
            throw std::runtime_error("Garbage collector initialization failed");
        }
        
        std::cout << "Garbage collector initialized successfully" << std::endl;
        std::cout << "Initial container size: " << gc->container_size << std::endl;
    }
    
    ~GarbageCollectorWrapper() {
        if (gc) {
            std::cout << "\n=== C++ Cleanup ===" << std::endl;
            if (gc_destroy_api(err, &gc) != 0) {
                std::cerr << "Error during garbage collector destruction: " 
                          << err->error_message << std::endl;
            } else {
                std::cout << "Garbage collector destroyed successfully" << std::endl;
            }
        }
        
        if (err) {
            free_errorm(err);
        }
    }
    
    void* allocate(size_t size) {
        void* ptr = gc_alloc(err, gc, size);
        if (!ptr && err) {
            std::cerr << "Allocation error: " << err->error_message << std::endl;
        }
        return ptr;
    }
    
    bool free(void* ptr) {
        int result = gc_fr(err, gc, ptr);
        if (result != 0 && err) {
            std::cout << "Free error: " << err->error_message << std::endl;
            return false;
        }
        return true;
    }
    
    bool freeEverything() {
        int result = gc_free_ev(err, gc);
        if (result != 0 && err) {
            std::cerr << "Free everything error: " << err->error_message << std::endl;
            return false;
        }
        return true;
    }
    
    void printError() {
        if (err) {
            std::cout << "Error status: " << err->error_status << std::endl;
            std::cout << "Error message: " << err->error_message << std::endl;
        }
    }
};

// Test basic allocation and freeing
void testBasicAllocation(GarbageCollectorWrapper& gc) {
    std::cout << "\n=== Testing Basic Allocation in C++ ===" << std::endl;
    
    // Allocate memory
    char* str = static_cast<char*>(gc.allocate(100));
    if (!str) {
        std::cerr << "Memory allocation failed" << std::endl;
        return;
    }
    std::cout << "Memory allocated successfully" << std::endl;
    
    // Write to memory
    const char* message = "Hello from C++!";
    std::strcpy(str, message);
    std::cout << "String stored: " << str << std::endl;
    
    // Free the memory
    if (gc.free(str)) {
        std::cout << "Memory freed successfully" << std::endl;
    }
    
    // Try to free it again (should fail)
    if (!gc.free(str)) {
        std::cout << "Expected error when freeing again" << std::endl;
    }
}

// Test multiple allocations
void testMultipleAllocations(GarbageCollectorWrapper& gc) {
    std::cout << "\n=== Testing Multiple Allocations in C++ ===" << std::endl;
    
    // Allocate several blocks
    const int numBlocks = 150; // More than initial capacity
    std::vector<int*> numbers;
    numbers.reserve(numBlocks);
    
    std::cout << "Allocating " << numBlocks << " integer blocks..." << std::endl;
    for (int i = 0; i < numBlocks; i++) {
        int* num = static_cast<int*>(gc.allocate(sizeof(int)));
        if (!num) {
            std::cerr << "Failed at allocation " << i << std::endl;
            break;
        }
        *num = i;
        numbers.push_back(num);
    }
    
    std::cout << "Successfully allocated " << numbers.size() << " blocks" << std::endl;
    
    // Verify data
    bool dataVerified = true;
    for (size_t i = 0; i < numbers.size(); i++) {
        if (*numbers[i] != static_cast<int>(i)) {
            std::cerr << "Data verification failed at index " << i << std::endl;
            dataVerified = false;
            break;
        }
    }
    
    if (dataVerified) {
        std::cout << "Data verification successful" << std::endl;
    }
    
    // Free every other block
    std::cout << "Freeing odd-indexed blocks..." << std::endl;
    for (size_t i = 1; i < numbers.size(); i += 2) {
        if (!gc.free(numbers[i])) {
            std::cerr << "Failed to free block " << i << std::endl;
        }
    }
    
    // Free remaining blocks
    std::cout << "Freeing all remaining blocks..." << std::endl;
    if (gc.freeEverything()) {
        std::cout << "All blocks freed successfully" << std::endl;
    }
}

// Test error handling
void testErrorHandling(GarbageCollectorWrapper& gc) {
    std::cout << "\n=== Testing Error Handling in C++ ===" << std::endl;
    
    // Try to allocate zero bytes
    void* ptr = gc.allocate(0);
    if (!ptr) {
        std::cout << "Expected error when allocating 0 bytes" << std::endl;
    }
    
    // Try to free a pointer not managed by gc
    int localVar = 10;
    if (!gc.free(&localVar)) {
        std::cout << "Expected error when freeing unmanaged pointer" << std::endl;
    }
}

int main() {
    try {
        std::cout << "=== Garbage Collector C++ Test Suite ===" << std::endl;
        
        // Create garbage collector wrapper
        GarbageCollectorWrapper gc;
        
        // Run tests
        testBasicAllocation(gc);
        testMultipleAllocations(gc);
        testErrorHandling(gc);
        
        std::cout << "C++ test suite completed" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}