#ifndef __htk_vector_h__
#define __htk_vector_h__

#include <htk/exception.h>
#include <htk/initializer_list.h>
#include <htk/iterator.h>
#include <htk/memory.h>
#include <htk/stdexcept.h>
#include <htk/utility.h>

#include <vector>



namespace htk
{
    void assert_(bool cond, const char *msg)
    {
        if (!cond)
            throw invalid_operation(msg);
    }
    /*
        We're going to implement a simple iterator, that uses a simple allocator,
        and implments all public members of the vector on CPP reference.

        Goal: It needs to function, and be just past a toy. It needs to work
        with the std::algorithms.
    */
    template <typename VectorT>
    class vector_iterator
    {
    public:
        using iterator_category = random_access_iterator_tag;
        using container = const VectorT;
        using difference_type = typename container::difference_type;
        using value_type = typename container::value_type;
        using pointer = typename container::pointer;
        using reference = typename container::reference;

        vector_iterator()
            : ptr_(nullptr), vec_(nullptr)
        {
        }

        vector_iterator(pointer ptr, container *vec)
            : ptr_(ptr), vec_(vec)
        {
        }

        vector_iterator(const vector_iterator &rhs)
            : ptr_(rhs.ptr_), vec_(rhs.vec_)
        {
        }

        // we need to increment / decrement / dereference
        const reference operator*() const
        {
            assert_(ptr_ != nullptr, "empty iterator");
            assert_(ptr_ >= vec_->data_.first && ptr_ < vec_->data_.last, "out of bounds");
            return *ptr_;
        }

        reference operator*()
        {
            return const_cast<reference>(const_cast<const vector_iterator *>(this)->operator*());
        }

        pointer operator->() const
        {
            assert_(ptr_ != nullptr, "empty iterator");
            assert_(ptr_ >= vec_->data_.first && ptr_ < vec_->data_.last, "out of bounds");
            return ptr_;
        }

        vector_iterator &operator++()
        {
            assert_(ptr_ != nullptr, "empty iterator");
            assert_(ptr_ < vec_->data_.last, "cannot increment past end");
            ++ptr_;
            return *this;
        }

        vector_iterator operator++(int)
        {
            vector_iterator tmp{ *this };
            ++(*this);
            return tmp;
        }

        vector_iterator operator+=(const difference_type val)
        {
            assert_(ptr_ != nullptr, "empty iterator");
            const auto next = ptr_ + val;
            assert_(next < vec_->data_.last && next >= vec_->data_.first, "seek out of bounds");
            ptr_ += val;
            return *this;
        }

        vector_iterator operator+(const difference_type val)
        {
            vector_iterator tmp{ *this };
            return tmp += val;
        }

        vector_iterator &operator--()
        {
            assert_(ptr_ != nullptr, "empty iterator");
            assert_(ptr_ > vec_->data_.first, "cannot decrement past start");
            --ptr_;
            return *this;
        }

        vector_iterator operator--(int)
        {
            vector_iterator tmp{ *this };
            --(*this);
            return tmp;
        }

        vector_iterator operator-=(const difference_type val)
        {
            assert_(ptr_ != nullptr, "empty iterator");
            const auto next = ptr_ + val;
            assert_(next < vec_->data_.last && next >= vec_->data_.first, "seek out of bounds");
            ptr_ -= val;
            return *this;
        }

        vector_iterator operator-(const difference_type val)
        {
            vector_iterator tmp{ *this };
            return tmp -= val;
        }

        bool operator<(const vector_iterator &rhs) const
        {
            assert_(vec_ == rhs.vec_, "incompatible iterators");
            return ptr_ < rhs.ptr_;
        }

        bool operator==(const vector_iterator &rhs) const
        {
            assert_(vec_ == rhs.vec_, "incompatible iterators");
            return ptr_ == rhs.ptr_;
        }

        bool operator!=(const vector_iterator &rhs) const
        {
            return !(*this == rhs);
        }

        const pointer &ptr() const { return ptr_; }
        pointer &ptr() { return ptr_; }

    private:
        pointer ptr_;
        container *vec_;
    };


    template <typename VectorT>
    typename vector_iterator<VectorT>::difference_type operator-(const vector_iterator<VectorT> &r, const vector_iterator<VectorT> &l)
    {
        return r.ptr() - l.ptr();
    }

    /*
        We're going to implement a simple vector, that uses a simple allocator,
        and implments all public members of the vector on CPP reference.

        Goal: It needs to function, and be just past a toy. It needs to work
        with the std::algorithms.
    */


    template <typename T, typename AllocatorT = htk::allocator<T>>
    class vector
    {
        friend vector_iterator;
        /// member types
    public:
        static constexpr size_t initial_capacity = 10;
        using value_type = T;
        using reference = typename AllocatorT::reference;
        using const_reference = typename AllocatorT::const_reference;
        using iterator = vector_iterator<vector<T>>;
        using const_iterator = const vector_iterator<vector<T>>;
        using difference_type = htk::ptrdiff_t;
        using size_type = htk::size_t;
        using allocator_type = AllocatorT;
        using pointer = typename AllocatorT::pointer;
        using const_pointer = typename AllocatorT::const_pointer;

    private:
        struct move_items_tag
        {
        };
        struct copy_items_tag
        {
        };
        struct memcpy_items_tag
        {
        };

        using copy_type = typename htk::conditional<htk::is_trivial_v<T>, memcpy_items_tag,
            typename htk::conditional<htk::is_move_constructible_v<T>, move_items_tag, copy_items_tag>::type>::type;

    private:
        struct data
        {
            pointer first;
            pointer last;
            pointer end_of_container;

            size_t capacity() const
            {
                return end_of_container - first;
            }

            size_t size() const
            {
                return last - first;
            }

            size_t freespace() const
            {
                return end_of_container - last;
            }
        };

        data data_;
        AllocatorT allocator_;



    public:
        vector()
            : data_{ nullptr, nullptr, nullptr }
        {
        }

        vector(const htk::initializer_list<T> &init)
            : data_{ nullptr, nullptr, nullptr }
        {
            ensure_space_at_least(init.size());
            insert(end(), init.begin(), init.end());
        }

        vector(const vector &v) = delete;
        vector(vector &&v)
            : data_{ nullptr, nullptr, nullptr }
        {
            std::swap(data_, v.data_);
        }

        ~vector() noexcept
        {
            destroy(data_.first, data_.last);
            allocator_.deallocate(data_.first, data_.capacity());
        }

    public: // modifiers
        template <typename... Args>
        decltype(auto) emplace_back(Args &&... args)
        {
            ensure_space_at_least(1);
            allocator_.construct(*(data_.last++), htk::forward<Args>(args)...);
            return back();
        }

        template <typename... Args>
        decltype(auto) emplace(const_iterator where, Args &&... args)
        {
            const T value{ htk::forward(args)... };
            return insert(where, 1, htk::forward(args));
        }

        void push_back(const T &item)
        {
            emplace_back(item);
        }

        void push_back(T &&item)
        {
            emplace_back(htk::move(item));
        }

        iterator insert(const_iterator where, size_type count, const T &value)
        {
            // if we're not at the end; and the count is larger than what's there
            // we need to grow
            const auto offset = where.ptr() - data_.first;
            pointer whereptr = data_.first + offset;
            if (where != end() && freespace() < count)
            {
                // we're going to reallocate
                const auto cap = calculate_growth(count);
                const auto curs = size();
                auto first = allocator_.allocate(cap);

                auto last = first + curs + count;
                const auto end = first + cap;

                data next = { first, last, end };
                // we're going to copy

                try
                {
                    move_items(data_.first, data_.first + offset, next.first, copy_type{});
                    auto dest = first;
                    for (size_type i = 0; i < count; ++i, dest++)
                    {
                        allocator_.construct(*(dest), value);
                    }
                    move_items(data_.first + offset, data_.last, dest, copy_type{});
                }
                catch (...)
                {
                    allocator_.deallocate(next.first, next.capacity());
                }

                const auto old = data_;
                data_ = next;

                destroy(old.first, old.last);
                allocator_.deallocate(old.first, old.capacity());
                // we're going to create
            }
            // else we have enough space
            else if (where != end())
            {
                // we just shift the guys down by copying backwards...,
                // and create in place
                auto dest = data_.last + count - 1;
                if constexpr (htk::is_trivial_v<T>)
                {
                    // we can memmove the whole block, not worrying about overlap.
                    const auto first = data_.first + offset;
                    const auto last = data_.last;
                    const auto mdest = data_.first + offset + count;
                    memmove(mdest, first, (last - first) * sizeof(T));
                }
                else
                {
                    const auto el2move = static_cast<size_type>(data_.last - where.ptr());
                    if (count > el2move) // I'm inserting more than elements to move, move it all
                    {
                        move_items(whereptr, data_.last, data_.last, copy_type{});
                        // destroy anything we moved.
                        destroy(whereptr, data_.last);
                    }
                    else
                    {
                        // move only tail items, and copy backwards the overlap
                        auto target = data_.last;
                        move_items(data_.last - count, data_.last, target, copy_type{});
                        copy_backward(whereptr, data_.last - count, data_.last, copy_type{});
                    }
                }
                // move the end pointer
                data_.last += count;
                assign_value(whereptr, whereptr + count, value);
            }
            else
            {
                // insert into the end.
                for (size_type i = 0; i < count; ++i)
                {
                    allocator_.construct(*(data_.last++), value);
                }
            }
            return vector_iterator(data_.first + offset, this);
        }

        template <typename It, typename = htk::enable_if_t<htk::is_iterator_v<It>>>
        iterator insert(const_iterator where, It start, It fin)
        {

            // if we're not at the end; and the count is larger than what's there
            // we need to grow
            const auto count = fin - start;
            const auto offset = where.ptr() - data_.first;
            const pointer whereptr = data_.first + offset;
            if (freespace() < count)
            {
                // we're going to reallocate
                const auto cap = calculate_growth(count);
                const auto curs = size();
                auto first = allocator_.allocate(cap);

                auto last = first + curs + count;
                const auto end = first + cap;

                data next = { first, last, end };
                // we're going to copy

                try
                {
                    move_items(data_.first, data_.first + offset, next.first, copy_type{});
                    auto dest = first + offset;
                    while (start != fin)
                        allocator_.construct(*(dest++), *(start++));
                    move_items(data_.first + offset, data_.last, dest, copy_type{});
                }
                catch (...)
                {
                    allocator_.deallocate(next.first, next.capacity());
                }

                const auto old = data_;
                data_ = next;

                destroy(old.first, old.last);
                allocator_.deallocate(old.first, old.capacity());
            }
            // else we have enough space
            else if (where != end())
            {
                // we just shift the guys down by copying backwards...,
                // and create in place
                auto dest = data_.last + count - 1;
                if constexpr (htk::is_trivial_v<T>)
                {
                    // we can memmove the whole block, not worrying about overlap.
                    const auto first = data_.first + offset;
                    const auto last = data_.last;
                    const auto mdest = data_.first + offset + count;
                    memmove(mdest, first, (last - first) * sizeof(T));
                }
                else
                {
                    const auto el2move = static_cast<size_type>(data_.last - whereptr);
                    if (count > el2move) // I'm inserting more than elements to move, move it all
                    {
                        move_items(whereptr, data_.last, data_.last, copy_type{});
                        // destroy anything we moved.
                        destroy(whereptr, data_.last);
                    }
                    else
                    {
                        // move only tail items, and copy backwards the overlap
                        auto target = data_.last;
                        move_items(data_.last - count, data_.last, target, copy_type{});
                        copy_backward(whereptr, data_.last - count, data_.last, copy_type{});
                    }
                }
                // move the end pointer
                data_.last += count;
                assign_values(whereptr, start, fin);
            }
            else
            {
                // insert into the end.
                assign_values(data_.last, start, fin);
                data_.last += count;
            }
            return vector_iterator(data_.first + offset, this);
        }

        void clear()
        {
            // destroy all elements.
            // set last = first
            for (auto begin = data_.first; begin < data_.last; ++begin)
            {
                allocator_.destroy(begin);
            }
            data_.last = data_.first;
        }

        void pop_back()
        {
            allocator_.destroy(data_.last - 1);
            data_.last--;
        }

    public:
        T &back()
        {
            if (empty())
                throw htk::exception("back() called on empty vector");

            return *(data_.last - 1);
        }

        const T &back() const
        {
            if (empty())
                throw htk::exception("back() called on empty vector");

            return *(data_.last - 1);
        }

        T &at(size_type index)
        {
            if (data_.last <= data_.first + index)
                throw out_of_range("index out of range");

            return *(data_.first + index);
        }

        const T &at(size_type index) const
        {
            if (data_.last <= data_.first + index)
                throw out_of_range("index out of range");

            return *(data_.first + index);
        }

    public:
        const_iterator cbegin() const
        {
            return iterator(data_.first, this);
        }

        const_iterator cend() const
        {
            return iterator(data_.last, this);
        }

        iterator begin()
        {
            return iterator(data_.first, this);
        }

        iterator end()
        {
            return iterator(data_.last, this);
        }

    public: // capacity
        size_t freespace() const
        {
            return data_.freespace();
        }

        size_t capacity() const noexcept
        {
            return data_.capacity();
        }

        size_t size() const
        {
            return data_.size();
        }

        bool empty() const
        {
            return data_.first == data_.last;
        }

    private:
        bool initialize(size_t size)
        {
            if (data_.end_of_container == nullptr)
            {
                auto alloc_size = htk::max(initial_capacity, size);
                data_.first = allocator_.allocate(alloc_size);
                data_.last = data_.first;
                data_.end_of_container = data_.first + alloc_size;
                return true;
            }
            return false;
        }

        void ensure_space_at_least(size_t count)
        {
            if (initialize(count))
                return;
            // we need to see if we have enough space.
            if (freespace() < count)
            {
                const auto cap = calculate_growth(count);
                grow(cap);
            }
            // we good.
        }

        size_type calculate_growth(size_type count)
        {
            auto cap = capacity();
            auto curs = size();
            while (cap - curs < count)
                cap = cap * 2;
            return cap;
        }

        void check_bounds(const_iterator where)
        {
            if (where < data_.first || where > data_.last)
                throw htk::exception("iterator out of bounds.");
        }

        void grow(size_type cap)
        {
            pointer new_vec = allocator_.allocate(cap);
            pointer dest = new_vec;
            try
            {
                move_items(data_.first, data_.last, dest, copy_type{});
            }
            catch (...)
            {

                // this is wrong. we really need to be keeping track of what was
                // initialized.
                destroy(new_vec, dest);
                allocator_.deallocate(new_vec, cap);
                throw;
            }

            auto old = data_;
            auto curs = size();
            data_ = data{ new_vec, new_vec + curs, new_vec + cap };

            // destroy the old stuff
            destroy(old.first, old.last);
            allocator_.deallocate(old.first, old.capacity());
        }

        // we have to test if it's move constructable
        void move_items(pointer first, pointer last, pointer &dest, move_items_tag)
        {
            while (first != last)
                allocator_.construct(*(dest++), htk::move(*(first++)));
        }

        void move_items(pointer first, pointer last, pointer &dest, memcpy_items_tag)
        {
            memmove(dest, first, (last - first) * sizeof(T));
        }

        void move_items(pointer first, pointer last, pointer &dest, copy_items_tag)
        {
            while (first != last)
                allocator_.construct(*(dest++), *(first++));
        }

        void copy_backward(pointer first, pointer last, pointer dest, move_items_tag)
        {
            while (first != last)
            {
                const auto target = last--;
                *(dest--) = htk::move(*target);
            }
        }

        void copy_backward(pointer first, pointer last, pointer dest, copy_items_tag)
        {
            while (first != last)
            {
                const auto target = last--;
                *(dest--) = *target;
            }
        }

        template <typename It>
        void assign_values(pointer dest, It begin, It end)
        {
            while (begin != end)
            {
                *(dest++) = *(begin++);
            }
        }

        void assign_value(pointer first, pointer last, const T &value)
        {
            while (first != last)
                *(first++) = value;
        }

        template <typename... Args>
        void create(pointer first, pointer last, Args &&... args)
        {
            while (first != last)
                allocator_.construct(*(first++), htk::forward<Args>(args)...);
        }

        void destroy(pointer first, pointer last)
        {
            while (first != last)
                allocator_.destroy(first++);
        }
    };


}

#endif // __htk_vector_h__