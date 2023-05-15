#pragma once
#include <type_traits>
#include <new>
namespace alg
{
    template<class T>
    typename std::enable_if<std::is_object<T>::value, T*>::type  addressof(T& arg) noexcept
    {
        return reinterpret_cast<T*>(
            &const_cast<char&>(
                reinterpret_cast<const volatile char&>(arg)));
    }

    template<class T>
    typename std::enable_if<!std::is_object<T>::value, T*>::type addressof(T& arg) noexcept
    {
        return &arg;
    }

    template <class>
    constexpr bool is_const_v = false; // determine whether type argument is const qualified
    template <class _Ty>
    constexpr bool is_const_v<const _Ty> = true;

    template <class _Ptr>
    struct allocation_result {
        _Ptr ptr;
        size_t count;
    };

    template< class T >
    class allocator
    {
    public:
        static_assert(!is_const_v<T>, "The C++ Standard forbids containers of const elements "
            "because allocator<const T> is ill-formed.");
        /*types*/
        using value_type                             = T;
        using pointer                                = T*;
        using const_pointer                          = const T*;
        using reference                              = T&;
        using const_reference                        = const T&;
        using size_type                              = size_t;
        using difference_type                        = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        template <class U> struct rebind { typedef allocator<U> other; };

        /*constructors/destructor/assignment*/
        allocator() noexcept = default;
        allocator(const allocator&) noexcept = default;
        template< class U > allocator(const allocator<U>&) noexcept {}
        ~allocator() = default;
        allocator& operator=(const allocator&) = default;

        /*functions*/
        pointer address(reference x) const noexcept;
        const_pointer address(const_reference x) const noexcept;

        T* allocate(size_t n);
        T* allocate(size_t n, const void* hint);
        [[nodiscard]] constexpr allocation_result<T*> allocate_at_least(size_t n);
        void deallocate(T* p, size_t n);

        size_type max_size() const noexcept;

        template< class U, class... Args >
        void construct(U* p, Args&&... args);
        template< class U >
        void destroy(U* p);
    };

    /*Compare the two default distributors.
    Because the default distributor is not state, the two default distributors are always equal.*/
    template< class T1, class T2 >
    bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept {
        return true;
    }

    template< class T1, class T2 >
    bool operator!=(const allocator<T1>&, const allocator<T2>&) noexcept {
        return false;
    }

    template< class T >
    inline typename allocator<T>::pointer allocator<T>::address(reference x) const noexcept {
        return std::addressof(x);
    }

    template< class T >
    inline typename allocator<T>::const_pointer allocator<T>::address(const_reference x) const noexcept {
        return std::addressof(x);
    }

    template< class T >
    inline T* allocator<T>::allocate(size_t n) {
        static_assert(sizeof(T) > 0, "value_type must be complete before calling allocate.");
        if (n == 0) {
            return nullptr;
        } else if (n > static_cast<size_t>(-1) / sizeof(T)) {
            throw std::bad_array_new_length();
        }
        //return new T[n];
        return static_cast<T*>(::operator new(sizeof(T) * n));
    }

    template< class T >
    inline T* allocator<T>::allocate(size_t n, const void*) {
        return allocator<T>::allocate(n);
    }

    template< class T >
    constexpr allocation_result<T*> allocate_at_least(const size_t count) {
        return { allocator<T>::allocate(count), count };
    }

    template< class T >
    inline void allocator<T>::deallocate(T* p, size_t n) {
        ASSERT(p != nullptr || n == 0, "null pointer cannot point to a block of non-zero size");
        // no overflow check on the following multiply; we assume _Allocate did that check
        ::operator delete(p, sizeof(T) * n);
    }

    template< class T >
    inline typename allocator<T>::size_type allocator<T>::max_size() const noexcept {
        //return std::numeric_limits<size_type>::max() / sizeof(T);
        return static_cast<size_type>(-1) / sizeof(T);
    }

    template< class T >
    template< class U, class... Args >
    inline void allocator<T>::construct(U* p, Args&&... args) {
        ::new U(std::forward<Args>(args)...);
    }

    template< class T >
    template< class U >
    inline void allocator<T>::destroy(U* p) {
        p->~U();
    }

} // namespace alg