#pragma once


#include <string>
#include <shared_mutex>


namespace v_0_0 {

    class shared_lock
    {
    public:

        shared_lock(const std::string&);
        ~shared_lock();

    private:

        std::shared_mutex* _ptr_mtx_file;
    };

}   // namespace v_0_0

struct Lock_v_0_0
{
    using shared_lock = v_0_0::shared_lock;
};
