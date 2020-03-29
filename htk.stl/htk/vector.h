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

    template <typename T, size_t InitialCapacity = 50, typename AllocatorT=htk::default_allocator<T>>
    class vector
    {
        /// member types
    public:
        static constexpr size_t initial_capacity = InitialCapacity;
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

    public:

        template<typename ...Args>
        decltype(auto) emplace_back(Args&&...args)
        {
            ensure_space_at_least(1);
            allocator_.construct(*(data_.last++), htk::forward<Args>(args)...);            
        }

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

    private:

        bool initialize(size_t size)
        {
            if (data_.end_of_container == nullptr)
            {
                auto alloc_size = htk::max(initial_capacity, size);
                data_.first = allocator_.allocate(alloc_size * sizeof(T));
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
            pointer new_vec = allocator_.allocate(cap * sizeof(T));
            pointer dest = new_vec;
            try
            {                
                move_items(data_.first, data_.last, dest);
            }
            catch (...)
            {
                destroy(new_vec, dest);
                allocator_.deallocate(new_vec, cap * sizeof(T));
                throw;
            }

            auto old = data_;
            auto curs = size();
            data_ = data{ new_vec,new_vec + curs, new_vec + cap };

            // destroy the old stuff
            destroy(old.first, old.last);
            allocator_.deallocate(old.first, old.capacity());
        }

        void move_items(const_pointer first, const_pointer last, pointer &dest)
        {
            // we're using copy construction. for now.
            while (first != last)
                allocator_.construct(*(dest++), *first++);
        }

        void destroy(pointer first, pointer last)
        {
            while (first != last)
                allocator_.destroy(first);
        }
    };
}

#endif // __htk_vector_h__