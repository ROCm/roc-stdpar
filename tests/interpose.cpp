#include <cstdint>
#include <cstdlib>
#include <new>
#include <malloc.h>
#include <memory.h>

extern "C" void* __libc_calloc(std::size_t, std::size_t);
extern "C" void __libc_cfree(void*);
extern "C" void __libc_free(void*);
extern "C" void* __libc_malloc(std::size_t);
extern "C" void* __libc_memalign(std::size_t, std::size_t);
extern "C" void* __libc_realloc(void*, std::size_t);
extern "C" int __posix_memalign(void**, std::size_t, std::size_t);

int main()
{
    try {
        if (auto p = std::aligned_alloc(8u, 42)) std::free(p);
        if (auto p = std::calloc(1, 42)) std::free(p);
        if (auto p = std::malloc(42)) std::free(p);
        if (auto p = memalign(8, 42)) std::free(p);
        if (void* p; posix_memalign(&p, 8, 42) == 0) std::free(p);
        if (auto p = std::realloc(std::malloc(42), 42)) std::free(p);
        if (auto p = reallocarray(std::calloc(1, 42), 1, 42)) std::free(p);
        if (auto p = new std::uint8_t) delete p;
        if (auto p = new (std::align_val_t{8}) std::uint8_t) {
            ::operator delete(p, std::align_val_t{8});
        }
        if (auto p = new (std::nothrow) std::uint8_t) delete p;
        if (auto p = new (std::align_val_t{8}, std::nothrow) std::uint8_t) {
            ::operator delete(p, std::align_val_t{8});
        }
        if (auto p = new std::uint8_t[42]) delete[] p;
        if (auto p = new (std::align_val_t{8}) std::uint8_t[42]) {
            ::operator delete[](p, std::align_val_t{8});
        }
        if (auto p = new (std::nothrow) std::uint8_t[42]) delete[] p;
        if (auto p = new (std::align_val_t{8}, std::nothrow) std::uint8_t[42]) {
            ::operator delete[](p, std::align_val_t{8});
        }
        if (auto p = __builtin_calloc(1, 42)) __builtin_free(p);
        if (auto p = __builtin_malloc(42)) __builtin_free(p);
        if (auto p = __builtin_operator_new(42)) __builtin_operator_delete(p);
        if (auto p = __builtin_operator_new(42, std::align_val_t{8})) {
            __builtin_operator_delete(p, std::align_val_t{8});
        }
        if (auto p = __builtin_operator_new(42, std::nothrow)) {
            __builtin_operator_delete(p);
        }
        if (auto p =
            __builtin_operator_new(42, std::align_val_t{8}, std::nothrow)) {
            __builtin_operator_delete(p, std::align_val_t{8});
        }
        if (auto p = __builtin_realloc(__builtin_malloc(42), 41)) {
            __builtin_free(p);
        }
        if (auto p = __libc_calloc(1, 42)) __libc_free(p);
        if (auto p = __libc_malloc(42)) __libc_free(p);
        if (auto p = __libc_memalign(8, 42)) __libc_free(p);
    }
    catch (...) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}