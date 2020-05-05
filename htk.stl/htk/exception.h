#ifndef __htk_exception_h__
#define __htk_exception_h__

#include <string.h>

#include <vcruntime_exception.h>
#include <exception>

namespace htk
{
    /*
        Interesting that <vcruntime_exception.h> the ctor, has an overload that
        takes an int, which doesn't make a copy of hte message.
    */
    class exception
    {
        const char * const message_;
    public:
        exception()
            :message_(nullptr)
        {
        }

        exception(const char* message)
            :message_(reinterpret_cast<const char *>(::operator new(strnlen(message,256))))
        {
            if (message_ == nullptr)
                exit(-1);
            strncpy_s(const_cast<char *const>(message_), 256, message, 256);
        }

        exception(const exception& e)
            :exception(e.message_)
        {
        }

        ~exception()
        {
            ::operator delete(const_cast<void*>(reinterpret_cast<const void*>(message_)));
        }

        virtual const char* what()
        {
            return message_;
        }

    };

    class bad_alloc : public exception
    {
    public:
        bad_alloc()
            :exception("bad_alloc")
        {
        }
        bad_alloc(const char *message)
            :exception(message)
        {
        }
    };

    class invalid_operation : public exception
    {
    public:
        invalid_operation()
            :exception("invalid operation")
        {

        }

        invalid_operation(const char* message)
            :exception(message)
        {

        }
    };


}

#endif // __htk_exception_h__