#ifndef __htk_stdexcept_h__
#define __htk_stdexcept_h__

#include <htk/exception.h>

namespace htk
{

    class logic_error : public exception
    {
    public:
        logic_error(const char* what)
            :exception(what)
        {
        }

    };

    class out_of_range : public logic_error
    {
    public:
        out_of_range(const char* what)
            :logic_error(what)
        {
        }
    };

    class invalid_operation : public logic_error
    {
    public:
        invalid_operation()
            :logic_error("invalid operation")
        {

        }

        invalid_operation(const char* message)
            :logic_error(message)
        {

        }
    };
};


#endif // __htk_stdexcept_h__