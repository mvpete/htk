#ifndef __htk_vector_h__
#define __htk_vector_h__

#include <htk/memory.h>
#include <htk/utility.h>

#include <vector>

namespace htk
{
    /*
        We're going to implement a simple vector, that uses a simple allocator,
        and implments all public members of the vector on CPP reference.

        Goal: It needs to function, and be just past a toy. It needs to work
        with the std::algorithms.
    */

    template <typename T, typename AllocatorT=htk::allocator<T>>
    class vector
    {
        /// member types
    public:
        static constexpr size_t initial_capacity = 10;
        using value_type = T;
        using allocator_type = AllocatorT;
        using size_type = htk::size_t;
        using difference_type = htk::ptrdiff_t;
        using reference = typename AllocatorT::reference;
        using const_reference = typename AllocatorT::const_reference;
        using pointer = typename AllocatorT::pointer;
        using const_pointer = typename AllocatorT::const_pointer;
        using iterator = pointer;
        using const_iterator = const_pointer;

    private:
        struct move_items_tag {};
        struct copy_items_tag {};
        struct memcpy_items_tag {};

        using copy_type = typename htk::conditional<htk::is_trivial_v<T>, memcpy_items_tag, 
                          typename htk::conditional<htk::is_move_constructible_v<T>, move_items_tag, copy_items_tag>::type>::type;

    private:
        struct data
        {
            iterator first;
            iterator last;
            iterator end_of_container;

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

        } data_;
        AllocatorT allocator_;



    public:
        vector()
            :data_{ nullptr,nullptr,nullptr }
        {
        }

        ~vector() noexcept
        {
            destroy(data_.first, data_.last);
            allocator_.deallocate(data_.first, data_.capacity());
        }

    public: // modifiers

        template<typename ...Args>
        decltype(auto) emplace_back(Args&&...args)
        {
            ensure_space_at_least(1);
            allocator_.construct(*(data_.last++), htk::forward<Args>(args)...);        
            return back();
        }

        void push_back(const T& item)
        {
            emplace_back(item);
        }

        void push_back(const T&& item)
        {
            emplace_back(htk::move(item));
        }

    public:

        T& back()
        {
            if (empty())
                throw htk::exception("back() called on empty vector");

            return *(data_.last - 1);
        }

        const T& back() const
        {
            if (empty())
                throw htk::exception("back() called on empty vector");

            return *(data_.last - 1);
        }

    public: // capacity

        size_t freespace() const
        {
            return data_.freespace();
        }

        size_t capacity() const
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
                auto cap = capacity()*2;
                auto curs = size();
                while (cap - curs < count)
                    cap = cap * 2;
                grow(cap);
            }
            // we good.
        }

        void grow(size_t cap)
        {
            pointer new_vec = allocator_.allocate(cap);
            pointer dest = new_vec;
            try
            {                
                move_items(data_.first, data_.last, dest, copy_type{});
            }
            catch (...)
            {
                destroy(new_vec, dest);
                allocator_.deallocate(new_vec, cap);
                throw;
            }

            auto old = data_;
            auto curs = size();
            data_ = data{ new_vec,new_vec + curs, new_vec + cap };

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

        void move_items(pointer first, pointer last, pointer& dest, memcpy_items_tag)
        {
            memmove(dest, first, (last - first));
        }

        void move_items(pointer first, pointer last, pointer& dest, copy_items_tag)
        {
            while (first != last)
                allocator_.construct(*(dest++), *(first++));
        }

        void destroy(pointer first, pointer last)
        {
            while (first != last)
                allocator_.destroy(first++);
        }
    };
}

#endif // __htk_vector_h__