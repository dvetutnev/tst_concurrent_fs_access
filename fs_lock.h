#pragma once


#include <string>
#include <mutex>
#include <shared_mutex>
#include <memory>


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

namespace v_0_1 {

    class shared_lock
    {
    public:

        shared_lock(const std::string&);
        ~shared_lock();

    private:

        std::shared_ptr<std::shared_mutex> _ptr_mtx_file;
    };

    class unique_lock
    {
    public:

        unique_lock(const std::string&);
        ~unique_lock();

    private:

        std::shared_ptr<std::shared_mutex> _ptr_mtx_file;
    };
}   // namespace v_0_1

struct Lock_v_0_1
{
    using shared_lock = v_0_1::shared_lock;
    using unique_lock = v_0_1::unique_lock;
};


namespace v_0_2 {

    class shared_lock
    {
    public:

        shared_lock(const std::string&);
        ~shared_lock();

    private:

        std::shared_ptr<std::shared_mutex> _ptr_mtx_file;
    };

    class unique_lock
    {
    public:

        unique_lock(const std::string&);
        ~unique_lock();

    private:

        std::shared_ptr<std::shared_mutex> _ptr_mtx_file;
    };
}   // namespace v_0_2

struct Lock_v_0_2
{
    using shared_lock = v_0_2::shared_lock;
    using unique_lock = v_0_2::unique_lock;
};

namespace v_1_0 {

    struct Locks;

    class shared_lock
    {
    public:

        shared_lock(const std::string&);
        ~shared_lock();

    private:

        std::shared_ptr<Locks> _locks;
    };

    class unique_lock
    {
    public:

        unique_lock(const std::string&);
        ~unique_lock();

    private:

        std::shared_ptr<Locks> _locks;
    };
}   // namespace v_1_0

struct TwoLevelLock_v_1_0
{
    using shared_lock = v_1_0::shared_lock;
    using unique_lock = v_1_0::unique_lock;
};
