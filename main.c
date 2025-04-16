#include <stdio.h>
#include <string.h>
#include "gdefs.h"
#include "gheader.h"

// Boosting package functions implementation
size_t string_size(const char *ptr) {
    size_t i = 0;
    while (ptr[i]) i++;
    return i;
}

void memory_copy(void *target, const void *src, size_t size) {
    for (size_t i = 0; i < size; i++)
        ((char *)target)[i] = ((const char *)src)[i];
}

// Test function to print error information
void print_error(errorm *err) {
    if (!err) return;
    printf("Error status: %u\n", err->error_status);
    printf("Error message: %s\n", err->error_message);
}

// Test basic allocation and freeing
void test_basic_allocation(errorm *err, _GC_memory_c *gc) {
    printf("\n=== Testing Basic Allocation ===\n");
    
    // Allocate memory
    char *str = gc_alloc(err, gc, 100);
    if (!str) {
        print_error(err);
        return;
    }
    printf("Memory allocated successfully\n");
    
    // Write to memory
    memory_copy(str, "Hello, Garbage Collector!", string_size("Hello, Garbage Collector!") + 1);
    printf("String stored: %s\n", str);
    
    // Free the memory
    int result = gc_fr(err, gc, str);
    if (result != 0) {
        print_error(err);
    } else {
        printf("Memory freed successfully\n");
    }
    
    // Try to free it again (should fail)
    result = gc_fr(err, gc, str);
    if (result != 0) {
        printf("Expected error when freeing again: %s\n", err->error_message);
    }
}

// Test multiple allocations
void test_multiple_allocations(errorm *err, _GC_memory_c *gc) {
    printf("\n=== Testing Multiple Allocations ===\n");
    
    // Allocate several blocks
    int *numbers[150]; // More than initial capacity (128)
    int i;
    
    printf("Allocating 150 integer blocks...\n");
    for (i = 0; i < 150; i++) {
        numbers[i] = gc_alloc(err, gc, sizeof(int));
        if (!numbers[i]) {
            printf("Failed at allocation %d: %s\n", i, err->error_message);
            break;
        }
        *numbers[i] = i;
    }
    
    printf("Successfully allocated %d blocks\n", i);
    printf("Container size after allocations: %lu\n", gc->container_size);
    
    // Verify data
    for (int j = 0; j < i; j++) {
        if (*numbers[j] != j) {
            printf("Data verification failed at index %d\n", j);
            break;
        }
    }
    printf("Data verification successful\n");
    
    // Free every other block
    printf("Freeing odd-indexed blocks...\n");
    for (int j = 1; j < i; j += 2) {
        if (gc_fr(err, gc, numbers[j]) != 0) {
            printf("Failed to free block %d: %s\n", j, err->error_message);
        }
    }
    
    // Free remaining blocks using gc_free_ev
    printf("Freeing all remaining blocks...\n");
    if (gc_free_ev(err, gc) != 0) {
        print_error(err);
    } else {
        printf("All blocks freed successfully\n");
    }
}

// Test error handling
void test_error_handling(errorm *err, _GC_memory_c *gc) {
    printf("\n=== Testing Error Handling ===\n");
    
    // Try to allocate zero bytes
    void *ptr = gc_alloc(err, gc, 0);
    if (!ptr) {
        printf("Expected error when allocating 0 bytes: %s\n", err->error_message);
    }
    
    // Try to free NULL
    int result = gc_fr(err, NULL, NULL);
    if (result != 0) {
        printf("Expected error when freeing NULL: %s\n", err->error_message);
    }
    
    // Try to free a pointer not managed by gc
    int local_var = 10;
    result = gc_fr(err, gc, &local_var);
    if (result != 0) {
        printf("Expected error when freeing unmanaged pointer: %s\n", err->error_message);
    }
}

int main(void) {
    _GC_memory_c *gc = NULL;
    errorm *err = init_errorm();
    
    if (!err) {
        printf("Failed to initialize error manager\n");
        return 1;
    }
    
    printf("=== Garbage Collector Test Suite ===\n");
    
    // Initialize the garbage collector
    if (init_gc(&gc, err) != 0) {
        printf("Failed to initialize garbage collector: %s\n", err->error_message);
        free_errorm(err);
        return 1;
    }
    printf("Garbage collector initialized successfully\n");
    printf("Initial container size: %lu\n", gc->container_size);
    
    // Run tests
    test_basic_allocation(err, gc);
    test_multiple_allocations(err, gc);
    test_error_handling(err, gc);
    
    // Clean up
    printf("\n=== Cleanup ===\n");
    if (gc_destroy_api(err, &gc) != 0) {
        printf("Error during garbage collector destruction: %s\n", err->error_message);
    } else {
        printf("Garbage collector destroyed successfully\n");
    }
    
    free_errorm(err);
    printf("Test suite completed\n");
    
    return 0;
}