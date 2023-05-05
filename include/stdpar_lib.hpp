#pragma once

#if defined(__HIP__)
    #warning Using STDPAR Acceleration (temporary debug message)

    #if !defined(__CLANG_HIP_RUNTIME_WRAPPER_H__)
        #define __host__ __attribute__((host))
        #define __device__ __attribute__((device))
        #define __global__ __attribute__((global))
        #define __shared__ __attribute__((shared))
        #define __constant__ __attribute__((constant))
        #define __managed__ __attribute__((managed))
    #endif

    #include <execution>
    #include <functional>
    #include <iterator>
    #include <memory_resource>
    #include <type_traits>
    #include <utility>

    #include <thrust/copy.h>
    #include <thrust/execution_policy.h>
    #include <thrust/fill.h>
    #include <thrust/find.h>
    #include <thrust/for_each.h>
    #include <thrust/inner_product.h>
    #include <thrust/logical.h>
    #include <thrust/reduce.h>
    #include <thrust/scan.h>
    #include <thrust/sort.h>
    #include <thrust/transform.h>
    #include <thrust/transform_reduce.h>

    extern "C"
    __host__
    __attribute__((weak))
    void __stdpar_hidden_delete(void*);

    extern "C"
    __host__
     __attribute__((weak))
    void __stdpar_hidden_free(void*);

    namespace hipcpp
    {
        class Managed_heap final : public ::std::pmr::memory_resource {
            // TODO: add exception handling
            void* do_allocate(::std::size_t n, ::std::size_t) override
            {
                void* r{};
                hipMallocManaged(&r, n);

                return r;
            }

            void do_deallocate(void* p, ::std::size_t, ::std::size_t) override
            {
                if (hipFree(p) != hipSuccess) return __stdpar_hidden_free(p);
            }

            bool do_is_equal(
                const ::std::pmr::memory_resource& x) const noexcept override
            {
                return dynamic_cast<const Managed_heap*>(&x);
            }
        };

        inline Managed_heap mh{};
        inline ::std::pmr::synchronized_pool_resource sph{&mh};
        inline ::std::pmr::polymorphic_allocator<::std::byte> heap{&sph};

        class Managed_free_store final : public ::std::pmr::memory_resource {
            // TODO: add exception handling
            void* do_allocate(::std::size_t n, ::std::size_t) override
            {
                void* r{};
                hipMallocManaged(&r, n);

                return r;
            }

            void do_deallocate(void* p, ::std::size_t, ::std::size_t) override
            {
                if (hipFree(p) != hipSuccess) return __stdpar_hidden_delete(p);
            }

            bool do_is_equal(
                const ::std::pmr::memory_resource& x) const noexcept override
            {
                return dynamic_cast<const Managed_free_store*>(&x);
            }
        };

        inline Managed_free_store mr{};
        inline ::std::pmr::synchronized_pool_resource spr{&mr};
        inline ::std::pmr::polymorphic_allocator<::std::byte> free_store{&spr};
    } // Namespace hipcpp.

    extern "C"
    inline
    __host__
    __attribute__((used))
    void* __stdpar_malloc(std::size_t n)
    {
        if (auto r = hipcpp::heap.allocate(n)) return r;

        throw std::runtime_error{"Failed __stdpar_malloc"};
    }

    extern "C"
    inline
    __host__
    __attribute__((used))
    void* __stdpar_aligned_alloc(std::size_t /*align*/, std::size_t n)
    {   // TODO: deal with alignment.
        return __stdpar_malloc(n);
    }

    extern "C"
    inline
    __host__
    __attribute__((used))
    void __stdpar_free(void* p)
    {
        return hipcpp::heap.deallocate(static_cast<::std::byte*>(p), 0);
    }

    extern "C"
    inline
    __host__
    __attribute__((used))
    void* __stdpar_operator_new(std::size_t n)
    {   // TODO: consider adding the special handling specific to operator new
        if (auto r = hipcpp::free_store.allocate(n)) return r;

        throw std::runtime_error{"Failed __stdpar_malloc"};
    }

    extern "C"
    inline
    __host__
    __attribute__((used))
    void __stdpar_operator_delete(void* p) noexcept
    {
        return hipcpp::free_store.deallocate(static_cast<::std::byte*>(p), 0);
    }

    namespace hipcpp
    {
        template<typename I>
        using IsRandomAccessIterator = ::std::is_same<
            typename ::std::iterator_traits<I>::iterator_category,
            ::std::random_access_iterator_tag>;
        template<typename I>
        inline constexpr bool IsRandomAccessIterator_v =
            IsRandomAccessIterator<I>::value;
        template<typename... Is>
        inline constexpr bool AreRandomAccessIterators_v =
            ::std::conjunction_v<IsRandomAccessIterator<Is>...>;
    }

    namespace std
    {
        // BEGIN ANY_OF
        template<
            typename I,
            typename P,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        bool any_of(
                ::std::execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::any_of(::thrust::device, f, l, ::std::move(p));
        }
        // END ANY_OF

        // BEGIN COPY
        template<
            typename I0,
            typename I1,
            ::std::enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        I1 copy(
            ::std::execution::parallel_unsequenced_policy, I0 fi, I0 li, I1 fo)
        {
            return ::thrust::copy(::thrust::device, fi, li, fo);
        }
        // END COPY

        // BEGIN EXCLUSIVE_SCAN
        template<
            typename I0,
            typename I1,
            typename T,
            ::std::enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        I1 exclusive_scan(
            ::std::execution::parallel_unsequenced_policy,
            I0 fi,
            I0 li,
            I1 fo,
            T x)
        {
            return ::thrust::exclusive_scan(
                ::thrust::device, fi, li, fo, ::std::move(x));
        }

        template<
            typename I0,
            typename I1,
            typename T,
            typename Op,
            ::std::enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        I1 exclusive_scan(
            ::std::execution::parallel_unsequenced_policy,
            I0 fi,
            I0 li,
            I1 fo,
            T x,
            Op op)
        {
            return ::thrust::exclusive_scan(
                ::thrust::device, fi, li, fo, ::std::move(x), ::std::move(op));
        }
        // END EXCLUSIVE_SCAN

        // BEGIN FILL
        template<
            typename I,
            typename T,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void fill(
            ::std::execution::parallel_unsequenced_policy, I f, I l, const T& x)
        {
            return ::thrust::fill(::thrust::device, f, l, x);
        }
        // END FILL

        // BEGIN FIND
        template<
            typename I,
            typename T,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I find(
            ::std::execution::parallel_unsequenced_policy, I f, I l, const T& x)
        {
            return ::thrust::find(::thrust::device, f, l, x);
        }
        // END FIND

        // BEGIN FOR_EACH
        template<
            typename I,
            typename F,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void for_each(
            ::std::execution::parallel_unsequenced_policy, I f, I l, F fn)
        {
            return ::thrust::for_each(::thrust::device, f, l, ::std::move(fn));
        }

        template<
            typename I,
            typename N,
            typename F,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I for_each_n(
            ::std::execution::parallel_unsequenced_policy, I f, N n, F fn)
        {
            return
                ::thrust::for_each_n(::thrust::device, f, n, ::std::move(fn));
        }
        // END FOR_EACH

        // BEGIN REDUCE
        template<
            typename I,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        typename ::std::iterator_traits<I>::value_type reduce(
            ::std::execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::reduce(::thrust::device, f, l);
        }

        template<
            typename I,
            typename T,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        T reduce(::std::execution::parallel_unsequenced_policy, I f, I l, T x)
        {
            return ::thrust::reduce(::thrust::device, f, l, ::std::move(x));
        }

        template<
            typename I,
            typename T,
            typename Op,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        T reduce(
            ::std::execution::parallel_unsequenced_policy, I f, I l, T x, Op op)
        {
            return ::thrust::reduce(
                ::thrust::device, f, l, ::std::move(x), ::std::move(op));
        }
        // END REDUCE

        // BEGIN SORT
        template<
            typename I,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void sort(::std::execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::sort(::thrust::device, f, l);
        }
        // END SORT

        // BEGIN TRANSFORM
        template<
            typename I0,
            typename I1,
            typename F,
            ::std::enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        I1 transform(
            ::std::execution::parallel_unsequenced_policy,
            I0 fi,
            I0 li,
            I1 fo,
            F fn)
        {
            return ::thrust::transform(
                ::thrust::device, fi, li, fo, ::std::move(fn));
        }

        template<
            typename I0,
            typename I1,
            typename I2,
            typename F,
            ::std::enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, I2>>* = nullptr>
        inline
        I2 transform(
            ::std::execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            I2 f2,
            F fn)
        {
            return ::thrust::transform(
                ::thrust::device, f0, l0, f1, f2, ::std::move(fn));
        }
        // END TRANSFORM

        // BEGIN TRANSFORM_REDUCE
        template<
            typename I0,
            typename I1,
            typename T,
            ::std::enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        T transform_reduce(
            ::std::execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            T init)
        {
            return ::thrust::inner_product(
                ::thrust::device,
                f0,
                l0,
                f1,
                ::std::move(init));
        }

        template<
            typename I0,
            typename I1,
            typename T,
            typename F,
            typename G,
            ::std::enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        T transform_reduce(
            ::std::execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            T init,
            F reduce,
            G transform)
        {
            return ::thrust::inner_product(
                ::thrust::device,
                f0,
                l0,
                f1,
                ::std::move(init),
                ::std::move(reduce),
                ::std::move(transform));
        }

        template<
            typename I,
            typename T,
            typename F,
            typename G,
            ::std::enable_if_t<
                ::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        T transform_reduce(
            ::std::execution::parallel_unsequenced_policy,
            I f,
            I l,
            T init,
            F reduce,
            G transform)
        {
            return ::thrust::transform_reduce(
                ::thrust::device,
                f,
                l,
                ::std::move(transform),
                ::std::move(init),
                ::std::move(reduce));
        }
        // END TRANSFORM_REDUCE
    }
#else
    #error "Including the Accelerated Standard Parallel Algorithm Library without HIP is not permitted"
#endif