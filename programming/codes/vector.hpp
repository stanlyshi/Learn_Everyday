#pragma once
#include <utility>
#include <iterator>
#include <stdexcept>
#include <initializer_list>
#define ALG_VECTOR_MAX_SIZE 1000000000

namespace alg
{
    template <typename T>
    class vector
    {
    public:
        /*types*/
        using value_type             = T;
        using reference              = T&;
        using const_reference        = const T&;
        using right_reference        = T&&;
        using iterator               = T*;
        using const_iterator         = const T*;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        /*constructor/destructor*/
        vector() noexcept;
        explicit vector(size_t n);
        vector(const size_t n, const T& value);
        vector(iterator first, iterator last);
        vector(std::initializer_list<T> ilist);
        vector(const vector<T>& other);
        vector(vector<T>&&) noexcept;
        ~vector();

        /*assignment*/
        vector<T>& operator = (const vector<T>& other);
        vector<T>& operator = (vector<T>&& other) noexcept;
        vector<T>& operator = (std::initializer_list<T> ilist);
        void assign(const size_t count, const T& value);
        void assign(iterator first, iterator last);
        void assign(std::initializer_list<T> ilist);

        /*modifiers*/
        void push_back(const T& value);
        void push_back(T&&);
        void pop_back();
        template <class ... Args> void emplace_back(Args&&... args);
        template <class ... Args> iterator emplace(const_iterator pos, Args&&... args);
        iterator insert(const_iterator pos, const T& value);
        iterator insert(const_iterator pos, T&& value);
        iterator insert(const_iterator pos, size_t count, const T& value);
        template <class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last);
        iterator insert(const_iterator pos, std::initializer_list<T> ilist);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void clear() noexcept;
        void swap(vector<T>& other) noexcept;

        /*iterators*/
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        /*capacity*/
        bool empty() const noexcept;
        size_t size() const noexcept;
        size_t max_size() const noexcept;
        size_t capacity() const noexcept;
        void resize(size_t size);
        void resize(size_t size, const T& value);
        void reserve(size_t size);
        void shrink_to_fit();

        /*element access*/
        reference operator [] (size_t pos);
        const_reference operator [] (size_t pos) const;
        reference at(const size_t pos);
        const_reference at(const size_t pos) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;
        iterator data() noexcept;
        const_iterator data() const noexcept;

        /*non member functions*/
        bool operator == (const vector<T>& other) const;
        bool operator != (const vector<T>& other) const;
        bool operator <  (const vector<T>& other) const;
        bool operator <= (const vector<T>& other) const;
        bool operator >  (const vector<T>& other) const;
        bool operator >= (const vector<T>& other) const;

    private:
        //std::unique_ptr<T[]> arr;
        T* arr;
        size_t m_capacity_reserve = 4;
        size_t m_capacity = 4;
        size_t m_size  = 0;

        inline void reallocate();
    };

    /*constructor/destructor*/
    template <typename T>
    vector<T>::vector() noexcept {
        arr = new T[m_capacity];
    }

    template<typename T>
    vector<T>::vector(size_t n)
        : m_capacity(n << 2), m_size(n)
    {
        if (0 == n)
        {
            m_capacity = m_capacity_reserve;
            reallocate();
        }
        arr = new T[m_capacity];
        for (size_t i = 0; i < n; i++)
            arr[i] = T();
    }

    template<typename T>
    vector<T>::vector(const size_t n, const T& value)
        : m_capacity(n << 2), m_size(n)
    {
        arr = new T[m_capacity];
        for (size_t i = 0; i < n; i++)
        {
            arr[i] = value;
        }
    }

    template<typename T>
    vector<T>::vector(iterator first, iterator last)
        : m_size(last - first), m_capacity(m_size << 2)
    {
        arr = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++)
            arr[i] = *first++;
    }

    template<typename T>
    vector<T>::vector(std::initializer_list<T> ilist)
        :m_capacity(ilist.size() << 2)
    {
        arr = new T[m_capacity];
        for (auto& item : ilist)
            arr[m_size++] = item;
    }

    template<typename T>
    vector<T>::vector(const vector<T>& other)
        : m_capacity(other.m_capacity), m_size(other.m_size)
    {
        arr = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++)
            arr[i] = other.arr[i];
    }
    
    template<typename T>
    vector<T>::vector(vector<T>&& other) noexcept
        : m_capacity(other.m_capacity), m_size(other.m_size)
    {
        arr = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++)
            arr[i] = std::move(other.arr[i]);
    }

    template<typename T>
    vector<T>::~vector()
    {
        if (nullptr != arr)
            delete[] arr;
    }

    /*assignment*/
    template<typename T>
    vector<T>& vector<T>::operator = (const vector<T>& other)
    {
        if (&other == this) {
            return *this;
        }
        if (m_capacity < other.m_capacity) {
            m_capacity = other.m_capacity;
            reallocate();
        }
        m_size = other.m_size;
        for (size_t i = 0; i < m_size; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }

    template<typename T>
    vector<T>& vector<T>::operator = (vector<T>&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }
        if (m_capacity < other.m_capacity) {
            m_capacity = other.m_capacity;
            reallocate();
        }
        m_size = other.m_size;
        for (size_t i = 0; i < other.size; i++) {
            arr[i] = std::move(other.arr[i]);
        }
        other.m_size = 0;
        other.m_capacity = other.m_capacity_reserve;
        return *this;
    }

    template <typename T>
    vector<T>& vector<T>::operator = (std::initializer_list<T> ilist)
    {
        if (ilist.size() > m_capacity) {
            m_capacity = ilist.size() << 2;
            reallocate();
        }
        m_size = 0;
        for (auto& item : ilist)
            arr[m_size++] = item;
        return *this;
    }

    template<typename T>
    inline void vector<T>::assign(const size_t count, const T& value)
    {
        if (count > m_capacity)
        {
            m_capacity = count << 2;
            reallocate();
        }
        m_size = count;
        for (size_t i = 0; i < count; i++)
            arr[i] = value;
    }

    template<typename T>
    inline void vector<T>::assign(iterator first, iterator last)
    {
        size_t newSize = last - first;
        if (newSize > m_capacity) {
            m_capacity = newSize << 2;
            reallocate();
        }
        m_size = newSize;
        for (size_t i = 0; i < newSize; i++, first++)
            arr[i] = *first;
    }

    template<typename T>
    inline void vector<T>::assign(std::initializer_list<T> ilist)
    {
        if (ilist.size() > m_capacity) {
            m_capacity = ilist.size() << 2;
            reallocate();
        }
        m_size = 0;
        for (auto& item : ilist)
            arr[m_size++] = item;
    }

    /*modifiers*/
    template<typename T>
    inline void vector<T>::push_back(const T& value)
    {
        if (m_size == m_capacity) {
            m_capacity <<= 2;
            reallocate();
        }
        arr[m_size++] = value;
    }

    template<typename T>
    inline void vector<T>::push_back(T&& value)
    {
        if (m_size == m_capacity) {
            m_capacity <<= 2;
            reallocate();
        }
        arr[m_size++] = std::move(value);
    }

    template<typename T>
    inline void vector<T>::pop_back()
    {
        m_size--;
    }

    template<typename T>
    template<class ...Args>
    inline void vector<T>::emplace_back(Args&&... args) {
        if (m_size == m_capacity) {
            m_capacity <<= 2;
            reallocate();
        }
        arr[m_size++] = std::move(T(std::forward<Args>(args)...));
    }

    template<typename T>
    template <class ... Args>
    inline vector<T>::iterator vector<T>::emplace(const_iterator pos, Args&&... args)
    {
        size_t offset_pos = pos - arr;
        iterator ipos = &arr[offset_pos];
        if (ipos > arr + m_size) {
            printf("Vector emplace iterator outside range!\n");
            return ipos;
        }
        if (m_size == m_capacity) {
            m_capacity <<= 2;
            reallocate();
            ipos = &arr[offset_pos];
        }
        memmove(ipos + 1, ipos, (m_size - offset_pos) * sizeof(T));
        *ipos = std::move(T(std::forward<Args>(args)...));
        m_size++;
        return ipos;
    }

    template<typename T>
    inline vector<T>::iterator vector<T>::insert(const_iterator pos, const T& value)
    {
        return emplace(pos, std::move(value));
    }

    template<typename T>
    inline vector<T>::iterator vector<T>::insert(const_iterator pos, T&& value)
    {
        return emplace(pos, value);
    }

    template<typename T>
    inline vector<T>::iterator vector<T>::insert(const_iterator pos, size_t count, const T& value)
    {
        size_t offset_pos = pos - arr;
        iterator ipos = &arr[offset_pos];
        if (ipos > arr + m_size || !count) {
            printf("Vector insert iterator outside range, or nothing to be inserted!\n");
            return ipos;
        }
        if (m_size + count > m_capacity) {
            m_capacity = (m_size + count) << 2;
            reallocate();
            ipos = &arr[offset_pos];
        }        
        memmove(ipos + count, ipos, (m_size - offset_pos) * sizeof(T));
        m_size += count;
        for (iterator it = ipos; count--; it++)
            *it = value;
        return ipos;
    }

    template<typename T>
    template<class InputIt>
    inline vector<T>::iterator vector<T>::insert(const_iterator pos, InputIt first, InputIt last)
    {
        size_t offset_pos = pos - arr;
        iterator ipos = &arr[offset_pos];
        size_t count = last - first;
        if (ipos > arr + m_size || !count) {
            printf("Vector insert iterator outside range, or nothing to be inserted!\n");
            return ipos;
        }
        if (m_size + count > m_capacity) {
            m_capacity = (m_size + count) << 2;
            reallocate();
            ipos = &arr[offset_pos];
        }        
        memmove(ipos + count, ipos, (m_size - offset_pos) * sizeof(T));
        for (iterator it = ipos; first != last; it++, first++)
            *it = *first;
        m_size += count;
        return ipos;
    }

    template<typename T>
    inline vector<T>::iterator vector<T>::insert(const_iterator pos, std::initializer_list<T> ilist)
    {
        size_t offset_pos = pos - arr;
        iterator ipos = &arr[offset_pos];
        size_t count = ilist.size();
        if (ipos > arr + m_size || !count) {
            printf("Vector insert iterator outside range, or nothing to be inserted!\n");
            return ipos;
        }
        if (m_size + count > m_capacity) {
            m_capacity = (m_size + count) << 2;
            reallocate();
            ipos = &arr[offset_pos];
        }        
        memmove(ipos + count, ipos, (m_size - offset_pos) * sizeof(T));
        for (auto& item : ilist) {
            *ipos = item;
            ipos++;
        }
        m_size += count;
        return ipos;
    }

    template<typename T>
    inline vector<T>::iterator vector<T>::erase(const_iterator pos)
    {
        iterator ipos = &arr[pos - arr];
        if (ipos > arr + m_size) {
            printf("Vector erase iterator outside range!\n");
            return ipos;
        }
        memmove(ipos, ipos + 1, (m_size - (ipos - arr) - 1) * sizeof(T));
        m_size--;
        return ipos;
    }

    template<typename T>
    inline vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
    {
        iterator ipos = &arr[first - arr];
        if (first == last) return ipos;
        if (last > arr + m_size || first > arr + m_size)
        {
            printf("Vector erase iterator outside range!\n");
            return ipos;
        }
        memmove(ipos, last, (m_size - (last - arr)) * sizeof(T));
        m_size -= last - first;
        return ipos;
    }

    template<typename T>
    inline void vector<T>::clear() noexcept
    {
        m_size = 0;
    }

    template<typename T>
    inline void vector<T>::swap(vector<T>& other) noexcept
    {
        if (&other != this) {
            size_t t_size     = m_size;
            size_t t_capacity = m_capacity;
            T*     t_arr      = arr;

            m_size            = other.m_size;
            m_capacity        = other.m_capacity;
            arr               = other.arr;

            other.m_size      = t_size;
            other.m_capacity  = t_capacity;
            other.arr         = t_arr;
        }
    }

    /*iterators*/
    template<typename T>
    inline vector<T>::iterator vector<T>::begin() noexcept {
        return arr;
    }

    template<typename T>
    inline vector<T>::const_iterator vector<T>::begin() const noexcept {
        return arr;
    }

    template<typename T>
    vector<T>::iterator vector<T>::end() noexcept {
        return arr + m_size;
    }

    template<typename T>
    vector<T>::const_iterator vector<T>::end() const noexcept {
        return const_iterator(arr + m_size);
    }

    template<typename T>
    vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
        return reverse_iterator(arr + m_size);
    }

    template<typename T>
    vector<T>::const_reverse_iterator vector<T>::rbegin() const noexcept {
        return const_reverse_iterator(arr + m_size);
    }

    template<typename T>
    vector<T>::reverse_iterator vector<T>::rend() noexcept {
        return reverse_iterator(arr);
    }

    template<typename T>
    vector<T>::const_reverse_iterator vector<T>::rend() const noexcept {
        return const_reverse_iterator(arr);
    }

    template<typename T>
    inline vector<T>::const_iterator vector<T>::cbegin() const noexcept
    {
        return arr;
    }

    template<typename T>
    vector<T>::const_iterator vector<T>::cend() const noexcept {
        return arr + m_size;
    }

    template<typename T>
    vector<T>::const_reverse_iterator vector<T>::crbegin() const noexcept
    {
        return const_reverse_iterator(arr + m_size);
    }

    template<typename T>
    vector<T>::const_reverse_iterator vector<T>::crend() const noexcept
    {
        return const_reverse_iterator(arr);
    }


    /*capacity*/
    template <typename T>
    bool vector<T>::empty() const noexcept {
        return 0 == m_size;
    }

    template <typename T>
    size_t vector<T>::size() const noexcept {
        return m_size;
    }

    template <typename T>
    size_t vector<T>::max_size() const noexcept {
        return ALG_VECTOR_MAX_SIZE;
    }

    template <typename T>
    size_t vector<T>::capacity() const noexcept {
        return m_capacity;
    }

    template<typename T>
    inline void vector<T>::resize(size_t size)
    {
        if (size > m_size)
        {
            if (size > m_capacity)
            {
                m_capacity = size;
                reallocate();
            }
            for (size_t i = m_size; i < size; i++)
                arr[m_size++] = T();
        }
        else
        {
            m_size = size;
        }
    }

    template<typename T>
    inline void vector<T>::resize(size_t size, const T& value)
    {
        if (size > m_size)
        {
            if (size > m_capacity)
            {
                m_capacity = size;
                reallocate();
            }
            for (size_t i = m_size; i < size; i++)
                arr[m_size++] = value;
        }
        else
        {
            m_size = size;
        }
    }

    template<typename T>
    inline void vector<T>::reserve(size_t size)
    {
        if (size > m_capacity)
        {
            m_capacity = size;
            reallocate();
        }
        else if (size < m_size)
            m_capacity = m_size;
    }

    template<typename T>
    inline void vector<T>::shrink_to_fit()
    {
        m_capacity = m_size;
    }

    template<typename T>
    inline vector<T>::reference vector<T>::operator [] (size_t pos)
    {
        if (pos > m_size)
            printf("Vector subscript out of range!\n");
        return arr[pos];
    }

    template<typename T>
    inline vector<T>::const_reference vector<T>::operator [] (size_t pos) const
    {
        if (pos > m_size)
            printf("Vector subscript out of range!\n");
        return arr[pos];
    }

    template<typename T>
    inline vector<T>::reference vector<T>::at(const size_t pos)
    {
        if (pos < m_size)
            return arr[pos];
        else
            throw std::out_of_range("Invalid vector subscript, accessed position is out of range\n");
    }

    template<typename T>
    inline vector<T>::const_reference vector<T>::at(const size_t pos) const
    {
        if (pos < m_size)
            return arr[pos];
        else
            throw std::out_of_range("Invalid vector subscript, accessed position is out of range\n");
    }

    template<typename T>
    inline vector<T>::reference vector<T>::front()
    {
#ifdef _DEBUG
        if (0 == m_size)
            printf("front() called on empty vector\n");
#endif
        return arr[0];
    }

    template<typename T>
    inline vector<T>::const_reference vector<T>::front() const
    {
#ifdef _DEBUG
        if (0 == m_size)
            printf("front() called on empty vector\n");
#endif
        return arr[0];
    }

    template<typename T>
    inline vector<T>::reference vector<T>::back()
    {
#ifdef _DEBUG
        if (0 == m_size)
            printf("back() called on empty vector\n");
#endif
        return arr[m_size - 1];
    }

    template<typename T>
    inline vector<T>::const_reference vector<T>::back() const
    {
#ifdef _DEBUG
        if (0 == m_size)
            printf("back() called on empty vector\n");
#endif
        return arr[m_size - 1];
    }

    template<typename T>
    inline vector<T>::iterator vector<T>::data() noexcept {
        return arr;
    }

    template<typename T>
    inline vector<T>::const_iterator vector<T>::data() const noexcept {
        return arr;
    }

    template<typename T>
    inline bool vector<T>::operator==(const vector<T>& other) const
    {
        if (m_size != other.m_size) return false;
        for (size_t i = 0; i < m_size; i++) {
            if (arr[i] != other.arr[i]) return false;
        }
        return true;
    }

    template<typename T>
    inline bool vector<T>::operator!=(const vector<T>& other) const
    {
        if (m_size != other.m_size) return true;
        for (size_t i = 0; i < m_size; i++) {
            if (arr[i] != other.arr[i]) return true;
        }
        return false;
    }

    template<typename T>
    inline bool vector<T>::operator<(const vector<T>& other) const
    {
        size_t size = m_size < other.m_size ? m_size : other.m_size;
        for (size_t i = 0; i < size; i++) {
            if (arr[i] != other.arr[i]) return arr[i] < other.arr[i];
        }
        return m_size < other.m_size;
    }

    template<typename T>
    inline bool vector<T>::operator<=(const vector<T>& other) const
    {
        size_t size = m_size < other.m_size ? m_size : other.m_size;
        for (size_t i = 0; i < size; i++) {
            if (arr[i] != other.arr[i]) return arr[i] <= other.arr[i];
        }
        return m_size <= other.m_size;
    }

    template<typename T>
    inline bool vector<T>::operator>(const vector<T>& other) const
    {
        size_t size = m_size < other.m_size ? m_size : other.m_size;
        for (size_t i = 0; i < size; i++) {
            if (arr[i] != other.arr[i]) arr[i] > other.arr[i];
        }
        return m_size > other.m_size;
    }

    template<typename T>
    inline bool vector<T>::operator>=(const vector<T>& other) const
    {
        size_t size = m_size < other.m_size ? m_size : other.m_size;
        for (size_t i = 0; i < size; i++) {
            if (arr[i] != other.arr[i]) return arr[i] > other.arr[i];
        }
        return m_size >= other.m_size;
    }

    /*private fuctions*/
    template<typename T>
    inline void vector<T>::reallocate()
    {
        T* tarr = new T[m_capacity];
        memcpy(tarr, arr, m_size * sizeof(T));
        delete[] arr;
        arr = tarr;
    }

}