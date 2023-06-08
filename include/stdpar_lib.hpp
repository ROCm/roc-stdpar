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
    #include <memory>
    #include <memory_resource>
    #include <type_traits>
    #include <utility>

    #include <thrust/adjacent_difference.h>
    #include <thrust/copy.h>
    #include <thrust/count.h>
    #include <thrust/equal.h>
    #include <thrust/execution_policy.h>
    #include <thrust/extrema.h>
    #include <thrust/fill.h>
    #include <thrust/find.h>
    #include <thrust/for_each.h>
    #include <thrust/generate.h>
    #include <thrust/inner_product.h>
    #include <thrust/iterator/discard_iterator.h>
    #include <thrust/logical.h>
    #include <thrust/merge.h>
    #include <thrust/mismatch.h>
    #include <thrust/partition.h>
    #include <thrust/reduce.h>
    #include <thrust/remove.h>
    #include <thrust/replace.h>
    #include <thrust/reverse.h>
    #include <thrust/scan.h>
    #include <thrust/set_operations.h>
    #include <thrust/sort.h>
    #include <thrust/swap.h>
    #include <thrust/transform.h>
    #include <thrust/transform_reduce.h>
    #include <thrust/transform_scan.h>
    #include <thrust/uninitialized_copy.h>
    #include <thrust/uninitialized_fill.h>
    #include <thrust/unique.h>

    extern "C"
    __host__
    __attribute__((weak))
    void __stdpar_hidden_delete(void*);

    extern "C" void __libc_free(void*);

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
                if (hipFree(p) != hipSuccess) return __libc_free(p);
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
        // BEGIN ADJACENT_DIFFERENCE
        // https://en.cppreference.com/w/cpp/algorithm/adjacent_difference
        template<
            typename I,
            typename O,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O adjacent_difference(
            execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::adjacent_difference(::thrust::device, fi, li, fo);
        }

        template<
            typename I,
            typename O,
            typename Op,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O adjacent_difference(
            execution::parallel_unsequenced_policy, I fi, I li, O fo, Op op)
        {
            return ::thrust::adjacent_difference(
                ::thrust::device, fi, li, fo, ::std::move(op));
        }
        // END ADJACENT_DIFFERENCE

        // BEGIN ADJACENT_FIND
        // https://en.cppreference.com/w/cpp/algorithm/adjacent_find
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I adjacent_find(execution::parallel_unsequenced_policy, I f, I l)
        {
            if (f == l) return l;

            const auto r = ::thrust::mismatch(
                ::thrust::device, f + 1, l, f, not_equal_to<>{});

            return (r.first == l) ? l : r.second;
        }

        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I adjacent_find(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            if (f == l) return l;

            const auto r = ::thrust::mismatch(
                ::thrust::device, f + 1, l, f, not_fn(::std::move(p)));

            return (r.first == l) ? l : r.second;
        }
        // END ADJACENT_FIND

        // BEGIN ALL_OF
        // https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        bool all_of(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::all_of(f, l, ::std::move(p));
        }
        // END ALL_OF

        // BEGIN ANY_OF
        // https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        bool any_of(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::any_of(::thrust::device, f, l, ::std::move(p));
        }
        // END ANY_OF

        // BEGIN COPY
        // https://en.cppreference.com/w/cpp/algorithm/copy
        template<
            typename I,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O copy(execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::copy(::thrust::device, fi, li, fo);
        }
        // END COPY

        // BEGIN COPY_IF
        // https://en.cppreference.com/w/cpp/algorithm/copy
        template<
            typename I,
            typename O,
            typename P,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O copy_if(execution::parallel_unsequenced_policy, I fi, I li, O fo, P p)
        {
            return
                ::thrust::copy_if(::thrust::device, fi, li, fo, ::std::move(p));
        }
        // END COPY_IF

        // BEGIN COPY_N
        // https://en.cppreference.com/w/cpp/algorithm/copy_n
        template<
            typename I,
            typename N,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O copy_n(execution::parallel_unsequenced_policy, I fi, N n, O fo)
        {
            return ::thrust::copy_n(::thrust::device, fi, n, fo);
        }
        // END COPY_N

        // BEGIN COUNT
        // https://en.cppreference.com/w/cpp/algorithm/count
        template<typename I, typename T>
        inline
        typename iterator_traits<I>::difference_type count(
            execution::parallel_unsequenced_policy, I f, I l, const T& x)
        {
            return ::thrust::count(::thrust::device, f, l, x);
        }
        // END COUNT

        // BEGIN COUNT_IF
        // https://en.cppreference.com/w/cpp/algorithm/count
        template<typename I, typename P>
        inline
        typename iterator_traits<I>::difference_type count_if(
            execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::count_if(::thrust::device, f, l, ::std::move(p));
        }
        // END COUNT_IF

        // BEGIN DESTROY
        // https://en.cppreference.com/w/cpp/memory/destroy
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void destroy(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::for_each(f, l, [](auto& x) {
                destroy_at(addressof(x));
            });
        }
        // END DESTROY

        // BEGIN DESTROY_N
        // https://en.cppreference.com/w/cpp/memory/destroy_n
        template<
            typename I,
            typename N,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void destroy_n(execution::parallel_unsequenced_policy, I f, N n)
        {
            return ::thrust::for_each_n(f, n, [](auto& x) {
                destroy_at(addressof(x));
            });
        }
        // END DESTROY_N

        // BEGIN EQUAL
        // https://en.cppreference.com/w/cpp/algorithm/equal
        template<
            typename I0,
            typename I1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        bool equal(execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1)
        {
            return ::thrust::equal(::thrust::device, f0, l0, f1);
        }

        template<
            typename I0,
            typename I1,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        bool equal(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1, R r)
        {
            return
                ::thrust::equal(::thrust::device, f0, l0, f1, ::std::move(r));
        }

        template<
            typename I0,
            typename I1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        bool equal(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1, I1 l1)
        {
            if (l0 - f0 != l1 - f1) return false;

            return ::thrust::equal(::thrust::device, f0, l0, f1);
        }

        template<
            typename I0,
            typename I1,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        bool equal(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            I1 l1,
            R r)
        {
            if (l0 - f0 != l1 - f1) return false;

            return ::thrust::equal(
                ::thrust::device, f0, l0, f1, ::std::move(r));
        }
        // END EQUAL

        // BEGIN EXCLUSIVE_SCAN
        // https://en.cppreference.com/w/cpp/algorithm/exclusive_scan
        template<
            typename I,
            typename O,
            typename T,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O exclusive_scan(
            execution::parallel_unsequenced_policy, I fi, I li, O fo, T x)
        {
            return ::thrust::exclusive_scan(
                ::thrust::device, fi, li, fo, ::std::move(x));
        }

        template<
            typename I,
            typename O,
            typename T,
            typename Op,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O exclusive_scan(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            T x,
            Op op)
        {
            return ::thrust::exclusive_scan(
                ::thrust::device, fi, li, fo, ::std::move(x), ::std::move(op));
        }
        // END EXCLUSIVE_SCAN

        // BEGIN FILL
        // https://en.cppreference.com/w/cpp/algorithm/fill
        template<
            typename I,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void fill(execution::parallel_unsequenced_policy, I f, I l, const T& x)
        {
            return ::thrust::fill(::thrust::device, f, l, x);
        }
        // END FILL

        // BEGIN FILL_N
        // https://en.cppreference.com/w/cpp/algorithm/fill_n
        template<
            typename I,
            typename N,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void fill_n(
            execution::parallel_unsequenced_policy, I f, N n, const T& x)
        {
            return ::thrust::fill_n(::thrust::device, f, n, x);
        }
        // END FILL_N

        // BEGIN FIND
        // https://en.cppreference.com/w/cpp/algorithm/find
        template<
            typename I,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I find(execution::parallel_unsequenced_policy, I f, I l, const T& x)
        {
            return ::thrust::find(::thrust::device, f, l, x);
        }
        // END FIND

        // BEGIN FIND_END
        // https://en.cppreference.com/w/cpp/algorithm/find_end
        // TODO: UNIMPLEMENTED IN THRUST
        // END FIND_END

        // BEGIN FIND_FIRST_OF
        // https://en.cppreference.com/w/cpp/algorithm/find_first_of
        // TODO: UNIMPLEMENTED IN THRUST
        // END FIND_FIRST_OF

        // BEGIN FIND_IF
        // https://en.cppreference.com/w/cpp/algorithm/find
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I find_if(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::find_if(::thrust::device, f, l, ::std::move(p));
        }
        // END FIND_IF

        // BEGIN FIND_IF_NOT
        // https://en.cppreference.com/w/cpp/algorithm/find
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I find_if_not(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return
                ::thrust::find_if_not(::thrust::device, f, l, ::std::move(p));
        }
        // END FIND_IF_NOT

        // BEGIN FOR_EACH
        // https://en.cppreference.com/w/cpp/algorithm/for_each
        template<
            typename I,
            typename F,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void for_each(execution::parallel_unsequenced_policy, I f, I l, F fn)
        {
            return ::thrust::for_each(::thrust::device, f, l, ::std::move(fn));
        }
        // END FOR_EACH

        // BEGIN FOR_EACH_N
        // https://en.cppreference.com/w/cpp/algorithm/for_each_n
        template<
            typename I,
            typename N,
            typename F,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I for_each_n(execution::parallel_unsequenced_policy, I f, N n, F fn)
        {
            return
                ::thrust::for_each_n(::thrust::device, f, n, ::std::move(fn));
        }
        // END FOR_EACH_N

        // BEGIN GENERATE
        // https://en.cppreference.com/w/cpp/algorithm/generate
        template<
            typename I,
            typename G,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void generate(execution::parallel_unsequenced_policy, I f, I l, G g)
        {
            return ::thrust::generate(::thrust::device, f, l, ::std::move(g));
        }
        // END GENERATE

        // BEGIN GENERATE_N
        // https://en.cppreference.com/w/cpp/algorithm/generate_n
        template<
            typename I,
            typename N,
            typename G,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void generate_n(execution::parallel_unsequenced_policy, I f, N n, G g)
        {
            return ::thrust::generate_n(::thrust::device, f, n, ::std::move(g));
        }
        // END GENERATE_N

        // BEGIN INCLUDES
        // https://en.cppreference.com/w/cpp/algorithm/includes
        template<
            typename I0,
            typename I1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        bool includes(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1, I1 l1)
        {
            ::thrust::discard_iterator<> cnt{0};

            return ::thrust::set_difference(
                ::thrust::device, f1, l1, f0, l0, cnt) == cnt;
        }

        template<
            typename I0,
            typename I1,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        bool includes(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            I1 l1,
            R r)
        {
            ::thrust::discard_iterator<> cnt{0};

            return ::thrust::set_difference(
                ::thrust::device, f1, l1, f0, l0, cnt, ::std::move(r)) == cnt;
        }
        // END INCLUDES

        // BEGIN INCLUSIVE_SCAN
        // https://en.cppreference.com/w/cpp/algorithm/inclusive_scan
        template<
            typename I,
            typename O,
            typename T,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O inclusive_scan(
            execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::inclusive_scan(::thrust::device, fi, li, fo);
        }

        template<
            typename I,
            typename O,
            typename Op,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O inclusive_scan(
            execution::parallel_unsequenced_policy, I fi, I li, O fo, Op op)
        {
            return ::thrust::inclusive_scan(
                ::thrust::device, fi, li, fo, ::std::move(op));
        }

        template<
            typename I,
            typename O,
            typename Op,
            typename T,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O inclusive_scan(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            Op op,
            T x)
        {   // TODO: this is highly inefficient due to rocThrust not exposing
            //       this particular interface where the user provides x.
            if (fi == li) return fo;

            auto lo =
                ::thrust::inclusive_scan(::thrust::device, fi, li, fo, op);

            return ::thrust::transform(
                ::thrust::device,
                fo,
                lo,
                fo,
                [op = ::std::move(op), x = ::std::move(x)](auto&& y) {
                return op(x, y);
            });
        }
        // END INCLUSIVE_SCAN

        // BEGIN INPLACE_MERGE
        // https://en.cppreference.com/w/cpp/algorithm/inplace_merge
        // TODO: UNIMPLEMENTED IN THRUST
        // END INPLACE_MERGE

        // BEGIN IS_HEAP
        // https://en.cppreference.com/w/cpp/algorithm/is_heap
        // TODO: UNIMPLEMENTED IN THRUST
        // END IS_HEAP

        // BEGIN IS_HEAP_UNTIL
        // https://en.cppreference.com/w/cpp/algorithm/is_heap_until
        // TODO: UNIMPLEMENTED IN THRUST
        // END IS_HEAP_UNTIL

        // BEGIN IS_PARTITIONED
        // https://en.cppreference.com/w/cpp/algorithm/is_partitioned
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        bool is_partitioned(
            execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::is_partitioned(
                ::thrust::device, f, l, ::std::move(p));
        }
        // END IS_PARTITIONED

        // BEGIN IS_SORTED
        // https://en.cppreference.com/w/cpp/algorithm/is_sorted
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        bool is_sorted(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::is_sorted(::thrust::device, f, l);
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        bool is_sorted(execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            return ::thrust::is_sorted(::thrust::device, f, l, ::std::move(r));
        }
        // END IS_SORTED

        // BEGIN IS_SORTED_UNTIL
        // https://en.cppreference.com/w/cpp/algorithm/is_sorted_until
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I is_sorted_until(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::is_sorted_until(::thrust::device, f, l);
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I is_sorted_until(execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            return ::thrust::is_sorted_until(
                ::thrust::device, f, l, ::std::move(r));
        }
        // END IS_SORTED_UNTIL

        // BEGIN LEXICOGRAPHICAL_COMPARE
        // https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
        template<
            typename I0,
            typename I1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        bool lexicographical_compare(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1, I1 l1)
        {
            if (f0 == l0) return f1 != l1;
            if (f1 == l1) return false;

            const auto n0 = l0 - f0;
            const auto n1 = l1 - f1;
            const auto n = ::std::min(n0, n1);

            const auto m = ::thrust::mismatch(::thrust::device, f0, f0 + n, f1);

            if (m.first == f0 + n) return n0 < n1;

            return *m.first < *m.second;
        }

        template<
            typename I0,
            typename I1,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        bool lexicographical_compare(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            I1 l1,
            R r)
        {
            if (f0 == l0) return f1 != l1;
            if (f1 == l1) return false;

            const auto n0 = l0 - f0;
            const auto n1 = l1 - f1;
            const auto n = ::std::min(n0, n1);

            const auto m = ::thrust::mismatch(
                ::thrust::device,
                f0,
                f0 + n,
                f1,
                [=](auto&& x, auto&& y) { return !r(x, y) && !r(y, x); });

            if (m.first == f0 + n) return n0 < n1;

            return r(*m.first, *m.second);
        }
        // END LEXICOGRAPHICAL_COMPARE

        // BEGIN MAX_ELEMENT
        // https://en.cppreference.com/w/cpp/algorithm/max_element
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I max_element(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::max_element(::thrust::device, f, l);
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I max_element(execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            return
                ::thrust::max_element(::thrust::device, f, l, ::std::move(r));
        }
        // END MAX_ELEMENT

        // BEGIN MERGE
        // https://en.cppreference.com/w/cpp/algorithm/merge
        template<
            typename I0,
            typename I1,
            typename O,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O merge(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            I1 l1,
            O fo)
        {
            return ::thrust::merge(::thrust::device, f0, l0, f1, l1, fo);
        }

        template<
            typename I0,
            typename I1,
            typename O,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O merge(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            I1 l1,
            O fo,
            R r)
        {
            return ::thrust::merge(
                ::thrust::device, f0, l0, f1, l1, fo, ::std::move(r));
        }
        // END MERGE

        // BEGIN MIN_ELEMENT
        // https://en.cppreference.com/w/cpp/algorithm/min_element
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I min_element(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::min_element(::thrust::device, f, l);
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I min_element(execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            return
                ::thrust::min_element(::thrust::device, f, l, ::std::move(r));
        }
        // END MIN_ELEMENT

        // BEGIN MINMAX_ELEMENT
        // https://en.cppreference.com/w/cpp/algorithm/minmax_element
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        pair<I, I> minmax_element(
            execution::parallel_unsequenced_policy, I f, I l)
        {
            auto [m, M] = ::thrust::minmax_element(::thrust::device, f, l);

            return {::std::move(m), ::std::move(M)};
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        pair<I, I> minmax_element(
            execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            auto [m, M] = ::thrust::minmax_element(
                ::thrust::device, f, l, ::std::move(r));

            return {::std::move(m), ::std::move(M)};
        }
        // END MINMAX_ELEMENT

        // BEGIN MISMATCH
        // https://en.cppreference.com/w/cpp/algorithm/mismatch
        template<
            typename I0,
            typename I1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        pair<I0, I1> mismatch(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1)
        {
            auto [m0, m1] = ::thrust::mismatch(::thrust::device, f0, l0, f1);

            return {::std::move(m0), ::std::move(m1)};
        }

        template<
            typename I0,
            typename I1,
            typename P,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        pair<I0, I1> mismatch(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1, P p)
        {
            auto [m0, m1] = ::thrust::mismatch(
                ::thrust::device, f0, l0, f1, ::std::move(p));

            return {::std::move(m0), ::std::move(m1)};
        }

        template<
            typename I0,
            typename I1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        pair<I0, I1> mismatch(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1, I1 l1)
        {
            const auto n = ::std::min(l0 - f0, l1 - f1);

            auto [m0, m1] =
                ::thrust::mismatch(::thrust::device, f0, f0 + n, f1);

            return {::std::move(m0), ::std::move(m1)};
        }

        template<
            typename I0,
            typename I1,
            typename P,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        pair<I0, I1> mismatch(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            I1 l1,
            P p)
        {
            const auto n = ::std::min(l0 - f0, l1 - f1);

            auto [m0, m1] = ::thrust::mismatch(
                ::thrust::device, f0, f0 + n, f1, ::std::move(p));

            return {::std::move(m0), ::std::move(m1)};
        }
        // END MISMATCH

        // BEGIN MOVE
        // https://en.cppreference.com/w/cpp/algorithm/move
        template<
            typename I,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O move(execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::copy(
                ::thrust::device,
                make_move_iterator(fi),
                make_move_iterator(li),
                fo);
        }
        // END MOVE

        // BEGIN NONE_OF
        // https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        bool none_of(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::none_of(::thrust::device, f, l, ::std::move(p));
        }
        // END NONE_OF

        // BEGIN NTH_ELEMENT
        // https://en.cppreference.com/w/cpp/algorithm/nth_element
        // TODO: UNIMPLEMENTED IN THRUST
        // END NTH_ELEMENT

        // BEGIN PARTIAL_SORT
        // https://en.cppreference.com/w/cpp/algorithm/partial_sort
        // TODO: UNIMPLEMENTED IN THRUST
        // END PARTIAL_SORT

        // BEGIN PARTIAL_SORT_COPY
        // https://en.cppreference.com/w/cpp/algorithm/partial_sort_copy
        // TODO: UNIMPLEMENTED IN THRUST
        // END PARTIAL_SORT_COPY

        // BEGIN PARTITION
        // https://en.cppreference.com/w/cpp/algorithm/partition
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I partition(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::partition(::thrust::device, f, l, ::std::move(p));
        }
        // END PARTITION

        // BEGIN PARTITION_COPY
        // https://en.cppreference.com/w/cpp/algorithm/partition_copy
        template<
            typename I,
            typename O0,
            typename O1,
            typename P,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I, O0, O1>>* = nullptr>
        inline
        pair<O0, O1> partition_copy(
            execution::parallel_unsequenced_policy,
            I f,
            I l,
            O0 fo0,
            O1 fo1,
            P p)
        {
            auto [r0, r1] = ::thrust::partition_copy(
                ::thrust::device, f, l, fo0, fo1, ::std::move(p));

            return {::std::move(r0), ::std::move(r1)};
        }
        // END PARTITION_COPY

        // BEGIN REDUCE
        // https://en.cppreference.com/w/cpp/algorithm/reduce
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        typename iterator_traits<I>::value_type reduce(
            execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::reduce(::thrust::device, f, l);
        }

        template<
            typename I,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        T reduce(execution::parallel_unsequenced_policy, I f, I l, T x)
        {
            return ::thrust::reduce(::thrust::device, f, l, ::std::move(x));
        }

        template<
            typename I,
            typename T,
            typename Op,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        T reduce(execution::parallel_unsequenced_policy, I f, I l, T x, Op op)
        {
            return ::thrust::reduce(
                ::thrust::device, f, l, ::std::move(x), ::std::move(op));
        }
        // END REDUCE

        // BEGIN REMOVE
        // https://en.cppreference.com/w/cpp/algorithm/remove
        template<
            typename I,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I remove(execution::parallel_unsequenced_policy, I f, I l, const T& x)
        {
            return ::thrust::remove(::thrust::device, f, l, x);
        }
        // END REMOVE

        // BEGIN REMOVE_COPY
        // https://en.cppreference.com/w/cpp/algorithm/remove_copy
        template<
            typename I,
            typename O,
            typename T,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O remove_copy(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            const T& x)
        {
            return ::thrust::remove_copy(::thrust::device, fi, li, fo, x);
        }
        // END REMOVE_COPY

        // BEGIN REMOVE_COPY_IF
        // https://en.cppreference.com/w/cpp/algorithm/remove_copy
        template<
            typename I,
            typename O,
            typename P,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O remove_copy_if(
            execution::parallel_unsequenced_policy, I fi, I li, O fo, P p)
        {
            return ::thrust::remove_copy_if(
                ::thrust::device, fi, li, fo, ::std::move(p));
        }
        // END REMOVE_COPY_IF

        // BEGIN REMOVE_IF
        // https://en.cppreference.com/w/cpp/algorithm/remove_copy
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I remove_if(execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::remove_if(::thrust::device, f, l, ::std::move(p));
        }
        // END REMOVE_IF

        // BEGIN REPLACE
        // https://en.cppreference.com/w/cpp/algorithm/replace
        template<
            typename I,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void replace(
            execution::parallel_unsequenced_policy,
            I f,
            I l,
            const T& x,
            const T& y)
        {
            return ::thrust::replace(::thrust::device, f, l, x, y);
        }
        // END REPLACE

        // BEGIN REPLACE_COPY
        // https://en.cppreference.com/w/cpp/algorithm/replace_copy
        template<
            typename I,
            typename O,
            typename T,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        void replace_copy(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            const T& x,
            const T& y)
        {
            return ::thrust::replace_copy(::thrust::device, fi, li, fo, x, y);
        }
        // END REPLACE_COPY

        // BEGIN REPLACE_COPY_IF
        // https://en.cppreference.com/w/cpp/algorithm/replace_copy
        template<
            typename I,
            typename O,
            typename P,
            typename T,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        void replace_copy_if(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            P p,
            const T& x)
        {
            return ::thrust::replace_copy_if(
                ::thrust::device, fi, li, fo, ::std::move(p), x);
        }
        // END REPLACE_COPY_IF

        // BEGIN REPLACE_IF
        // https://en.cppreference.com/w/cpp/algorithm/replace
        template<
            typename I,
            typename P,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void replace_if(
            execution::parallel_unsequenced_policy, I f, I l, P p, const T& x)
        {
            return
                ::thrust::replace_if(::thrust::device, f, l, ::std::move(p), x);
        }
        // END REPLACE_IF

        // BEGIN REVERSE
        // https://en.cppreference.com/w/cpp/algorithm/reverse
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void reverse(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::reverse(::thrust::device, f, l);
        }
        // END REVERSE

        // BEGIN REVERSE_COPY
        // https://en.cppreference.com/w/cpp/algorithm/reverse
        template<
            typename I,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        void reverse_copy(
            execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::reverse_copy(::thrust::device, fi, li, fo);
        }
        // END REVERSE

        // BEGIN ROTATE
        // https://en.cppreference.com/w/cpp/algorithm/rotate
        // TODO: UNIMPLEMENTED IN THRUST
        // END ROTATE

        // BEGIN ROTATE_COPY
        // https://en.cppreference.com/w/cpp/algorithm/rotate_copy
        // TODO: UNIMPLEMENTED IN THRUST
        // END ROTATE_COPY

        // BEGIN SET_DIFFERENCE
        // https://en.cppreference.com/w/cpp/algorithm/set_difference
        template<
            typename I0,
            typename I1,
            typename O,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_difference(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo)
        {
            return ::thrust::set_difference(
                ::thrust::device, fi0, li0, fi1, li1, fo);
        }

        template<
            typename I0,
            typename I1,
            typename O,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_difference(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo, R r)
        {
            return ::thrust::set_difference(
                ::thrust::device, fi0, li0, fi1, li1, fo, ::std::move(r));
        }
        // END SET_DIFFERENCE

        // BEGIN SET_INTERSECTION
        // https://en.cppreference.com/w/cpp/algorithm/set_intersection
        template<
            typename I0,
            typename I1,
            typename O,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_intersection(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo)
        {
            return ::thrust::set_intersection(
                ::thrust::device, fi0, li0, fi1, li1, fo);
        }

        template<
            typename I0,
            typename I1,
            typename O,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_intersection(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo, R r)
        {
            return ::thrust::set_intersection(
                ::thrust::device, fi0, li0, fi1, li1, fo, ::std::move(r));
        }
        // END SET_INTERSECTION

        // BEGIN SET_SYMMETRIC_DIFFERENCE
        // https://en.cppreference.com/w/cpp/algorithm/set_symmetric_difference
        template<
            typename I0,
            typename I1,
            typename O,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_symmetric_difference(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo)
        {
            return ::thrust::set_symmetric_difference(
                ::thrust::device, fi0, li0, fi1, li1, fo);
        }

        template<
            typename I0,
            typename I1,
            typename O,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_symmetric_difference(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo, R r)
        {
            return ::thrust::set_symmetric_difference(
                ::thrust::device, fi0, li0, fi1, li1, fo, ::std::move(r));
        }
        // END SET_SYMMETRIC_DIFFERENCE

        // BEGIN SET_UNION
        // https://en.cppreference.com/w/cpp/algorithm/set_union
        template<
            typename I0,
            typename I1,
            typename O,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_union(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo)
        {
            return
                ::thrust::set_union(::thrust::device, fi0, li0, fi1, li1, fo);
        }

        template<
            typename I0,
            typename I1,
            typename O,
            typename R,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O set_union(I0 fi0, I0 li0, I1 fi1, I1 li1, O fo, R r)
        {
            return ::thrust::set_union(
                ::thrust::device, fi0, li0, fi1, li1, fo, ::std::move(r));
        }
        // END SET_UNION

        // BEGIN SORT
        // https://en.cppreference.com/w/cpp/algorithm/sort
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void sort(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::sort(::thrust::device, f, l);
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void sort(execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            return ::thrust::sort(::thrust::device, f, l, ::std::move(r));
        }
        // END SORT

        // BEGIN STABLE_PARTITION
        // https://en.cppreference.com/w/cpp/algorithm/stable_partition
        template<
            typename I,
            typename P,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I stable_partition(
            execution::parallel_unsequenced_policy, I f, I l, P p)
        {
            return ::thrust::stable_partition(
                ::thrust::device, f, l, ::std::move(p));
        }
        // END STABLE_PARTITION

        // BEGIN STABLE_SORT
        // https://en.cppreference.com/w/cpp/algorithm/stable_sort
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void stable_sort(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::stable_sort(::thrust::device, f, l);
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void stable_sort(execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            return
                ::thrust::stable_sort(::thrust::device, f, l, ::std::move(r));
        }
        // END STABLE_SORT

        // BEGIN SWAP_RANGES
        // https://en.cppreference.com/w/cpp/algorithm/swap_ranges
        template<
            typename I0,
            typename I1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        I1 swap_ranges(
            execution::parallel_unsequenced_policy, I0 f0, I0 l0, I1 f1)
        {
            return ::thrust::swap_ranges(::thrust::device, f0, l0, f1);
        }
        // END SWAP_RANGES

        // BEGIN TRANSFORM
        // https://en.cppreference.com/w/cpp/algorithm/transform
        template<
            typename I,
            typename O,
            typename F,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O transform(
            execution::parallel_unsequenced_policy, I fi, I li, O fo, F fn)
        {
            return ::thrust::transform(
                ::thrust::device, fi, li, fo, ::std::move(fn));
        }

        template<
            typename I0,
            typename I1,
            typename O,
            typename F,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1, O>>* = nullptr>
        inline
        O transform(
            execution::parallel_unsequenced_policy,
            I0 fi0,
            I0 li0,
            I1 fi1,
            O fo,
            F fn)
        {
            return ::thrust::transform(
                ::thrust::device, fi0, li0, fi1, fo, ::std::move(fn));
        }
        // END TRANSFORM

        // BEGIN TRANSFORM_EXCLUSIVE_SCAN
        // https://en.cppreference.com/w/cpp/algorithm/transform_exclusive_scan
        template<
            typename I,
            typename O,
            typename T,
            typename Op0,
            typename Op1,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O transform_exclusive_scan(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            T x,
            Op0 op0,
            Op1 op1)
        {
            return ::thrust::transform_exclusive_scan(
                ::thrust::device,
                fi,
                li,
                fo,
                ::std::move(op1),
                ::std::move(x),
                ::std::move(op0));
        }
        // END TRANSFORM_EXCLUSIVE_SCAN

        // BEGIN TRANSFORM_INCLUSIVE_SCAN
        // https://en.cppreference.com/w/cpp/algorithm/transform_inclusive_scan
        template<
            typename I,
            typename O,
            typename Op0,
            typename Op1,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O transform_inclusive_scan(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            Op0 op0,
            Op1 op1)
        {
            return ::thrust::transform_inclusive_scan(
                ::thrust::device,
                fi,
                li,
                fo,
                ::std::move(op1),
                ::std::move(op0));
        }

        template<
            typename I,
            typename O,
            typename Op0,
            typename Op1,
            typename T,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O transform_inclusive_scan(
            execution::parallel_unsequenced_policy,
            I fi,
            I li,
            O fo,
            Op0 op0,
            Op1 op1,
            T x)
        {   // TODO: this is inefficient.
            if (fi == li) return fo;

            auto lo = ::thrust::transform_inclusive_scan(
                ::thrust::device,
                fi,
                li,
                fo,
                ::std::move(op1),
                op0);

            return ::thrust::transform(
                ::thrust::device,
                fo,
                lo,
                fo,
                [op0 = ::std::move(op0), x = ::std::move(x)](auto&& y) {
                return op0(x, y);
            });
        }
        // END TRANSFORM_INCLUSIVE_SCAN

        // BEGIN TRANSFORM_REDUCE
        // https://en.cppreference.com/w/cpp/algorithm/transform_reduce
        template<
            typename I0,
            typename I1,
            typename T,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        T transform_reduce(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            T x)
        {
            return ::thrust::inner_product(
                ::thrust::device, f0, l0, f1, ::std::move(x));
        }

        template<
            typename I0,
            typename I1,
            typename T,
            typename Op0,
            typename Op1,
            enable_if_t<
                ::hipcpp::AreRandomAccessIterators_v<I0, I1>>* = nullptr>
        inline
        T transform_reduce(
            execution::parallel_unsequenced_policy,
            I0 f0,
            I0 l0,
            I1 f1,
            T x,
            Op0 op0,
            Op1 op1)
        {
            return ::thrust::inner_product(
                ::thrust::device,
                f0,
                l0,
                f1,
                ::std::move(x),
                ::std::move(op0),
                ::std::move(op1));
        }

        template<
            typename I,
            typename T,
            typename Op0,
            typename Op1,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        T transform_reduce(
            execution::parallel_unsequenced_policy,
            I f,
            I l,
            T x,
            Op0 op0,
            Op1 op1)
        {
            return ::thrust::transform_reduce(
                ::thrust::device,
                f,
                l,
                ::std::move(op1),
                ::std::move(x),
                ::std::move(op0));
        }
        // END TRANSFORM_REDUCE

        // BEGIN UNINITIALIZED_COPY
        // https://en.cppreference.com/w/cpp/memory/uninitialized_copy
        template<
            typename I,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O uninitialized_copy(
            execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::uninitialized_copy(::thrust::device, fi, li, fo);
        }
        // END UNINITIALIZED_COPY

        // BEGIN UNINITIALIZED_COPY_N
        // https://en.cppreference.com/w/cpp/memory/uninitialized_copy
        template<
            typename I,
            typename N,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O uninitialized_copy(
            execution::parallel_unsequenced_policy, I fi, N n, O fo)
        {
            return ::thrust::uninitialized_copy_n(::thrust::device, fi, n, fo);
        }
        // END UNINITIALIZED_COPY_N

        // BEGIN UNINITIALIZED_DEFAULT_CONSTRUCT
        // https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void uninitialized_default_construct(
            execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::for_each(::thrust::device, f, l, [](auto& x) {
                auto p = const_cast<void*>(
                    static_cast<const volatile void*>((addressof(x))));
                ::new (p) typename iterator_traits<I>::value_type;
            });
        }
        // END UNINITIALIZED_DEFAULT_CONSTRUCT

        // BEGIN UNINITIALIZED_DEFAULT_CONSTRUCT_N
        // https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct_n
        template<
            typename I,
            typename N,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void uninitialized_default_construct_N(
            execution::parallel_unsequenced_policy, I f, N n)
        {
            return ::thrust::for_each_n(::thrust::device, f, n, [](auto& x) {
                auto p = const_cast<void*>(
                    static_cast<const volatile void*>((addressof(x))));
                ::new (p) typename iterator_traits<I>::value_type;
            });
        }
        // END UNINITIALIZED_DEFAULT_CONSTRUCT_N

        // BEGIN UNINITIALIZED_FILL
        // https://en.cppreference.com/w/cpp/memory/uninitialized_fill
        template<
            typename I,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void uninitialized_fill(
            execution::parallel_unsequenced_policy, I f, I l, const T& x)
        {
            return ::thrust::uninitialized_fill(::thrust::device, f, l, x);
        }
        // END UNINITIALIZED_FILL

        // BEGIN UNINITIALIZED_FILL_N
        // https://en.cppreference.com/w/cpp/memory/uninitialized_fill_n
        template<
            typename I,
            typename N,
            typename T,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void uninitialized_fill(
            execution::parallel_unsequenced_policy, I f, N n, const T& x)
        {
            return ::thrust::uninitialized_fill_n(::thrust::device, f, n, x);
        }
        // END UNINITIALIZED_FILL_N

        // BEGIN UNINITIALIZED_MOVE
        // https://en.cppreference.com/w/cpp/memory/uninitialized_move
        template<
            typename I,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O uninitialized_move(
            execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::uninitialized_copy(
                ::thrust::device,
                make_move_iterator(fi),
                make_move_iterator(li),
                fo);
        }
        // END UNINITIALIZED_MOVE

        // BEGIN UNINITIALIZED_MOVE_N
        // https://en.cppreference.com/w/cpp/memory/uninitialized_move_n
        template<
            typename I,
            typename N,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O uninitialized_move_n(
            execution::parallel_unsequenced_policy, I fi, N n, O fo)
        {
            return ::thrust::uninitialized_copy_n(
                ::thrust::device, make_move_iterator(fi), n, fo);
        }
        // END UNINITIALIZED_MOVE_N

        // BEGIN UNINITIALIZED_VALUE_CONSTRUCT
        // https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void uninitialized_value_construct(
            execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::for_each(::thrust::device, f, l, [](auto& x) {
                auto p = const_cast<void*>(
                    static_cast<const volatile void*>((addressof(x))));
                ::new (p) typename iterator_traits<I>::value_type{};
            });
        }
        // END UNINITIALIZED_VALUE_CONSTRUCT

        // BEGIN UNINITIALIZED_VALUE_CONSTRUCT_N
        // https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct_n
        template<
            typename I,
            typename N,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        void uninitialized_value_construct_n(
            execution::parallel_unsequenced_policy, I f, N n)
        {
            return ::thrust::for_each_n(::thrust::device, f, n, [](auto& x) {
                auto p = const_cast<void*>(
                    static_cast<const volatile void*>((addressof(x))));
                ::new (p) typename iterator_traits<I>::value_type{};
            });
        }
        // END UNINITIALIZED_VALUE_CONSTRUCT_N

        // BEGIN UNIQUE
        // https://en.cppreference.com/w/cpp/algorithm/unique
        template<
            typename I,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I unique(execution::parallel_unsequenced_policy, I f, I l)
        {
            return ::thrust::unique(::thrust::device, f, l);
        }

        template<
            typename I,
            typename R,
            enable_if_t<::hipcpp::IsRandomAccessIterator_v<I>>* = nullptr>
        inline
        I unique(execution::parallel_unsequenced_policy, I f, I l, R r)
        {
            return ::thrust::unique(::thrust::device, f, l, ::std::move(r));
        }
        // END UNIQUE

        // BEGIN UNIQUE_COPY
        // https://en.cppreference.com/w/cpp/algorithm/unique_copy
        template<
            typename I,
            typename O,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O unique_copy(execution::parallel_unsequenced_policy, I fi, I li, O fo)
        {
            return ::thrust::unique_copy(::thrust::device, fi, li, fo);
        }

        template<
            typename I,
            typename O,
            typename R,
            enable_if_t<::hipcpp::AreRandomAccessIterators_v<I, O>>* = nullptr>
        inline
        O unique_copy(
            execution::parallel_unsequenced_policy, I fi, I li, O fo, R r)
        {
            return ::thrust::unique_copy(
                ::thrust::device, fi, li, fo, ::std::move(r));
        }
        // END UNIQUE_COPY
    }
#else
    #error "Including the Accelerated Standard Parallel Algorithm Library without HIP is not permitted"
#endif