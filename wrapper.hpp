/*
ELEKTRON © 2022
Written by Matteo Reiter
www.elektron.work
14.09.22, 18:37
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree.


Thread-safe wraper for any container or class

Some inspiration and examples for TS implementation:
https://stackoverflow.com/a/29988626
*/

#pragma once

#include <shared_mutex>
#include <mutex>
#include <chrono>
#include <cassert>

#include "except.hpp"

using namespace std::chrono_literals;

namespace ts
{

    /**
     * @brief wrapper class referencing a container of type _CT (typically in a ts::wrapper object)
     * and a lock of type _LT that is assosiated with a mutex guarding the container (typically the mutex
     * of a ts::wrapper object). The class provides access operators to access container methods and
     * READ data from the container but not modify it (only methods marked as const can be used).
     * The accessor is allways associated with a mutex but does not necessarly have to own the lock.
     * Whenever a access operator is used, the lock is aquired (or attempted to be acquired) if the accessor
     * doesn't own the lock already. The mutex can manually be unlocked using the unlock() method.
     * When the accessor is deconstructed, the mutex is allways unlocket.
     *
     * Note that the accessor class itself is not thread safe, meaning one instance of accessor is only ever allowed to be used in a single thread.
     *
     * @tparam _CT container type
     * @tparam _MT mutex type
     */
    template <class _CT, class _MT>
    class shared_accessor
    {
    private:
        const _CT &__container;
        std::shared_lock<_MT> __lock;

        std::chrono::milliseconds __lock_timeout;

    public:
        shared_accessor(const _CT &_c, _MT &_mu)
            : __container(_c),
            __lock(_mu, std::defer_lock),
            __lock_timeout(10000)
        {
        }

        /**
         * @brief Set the lock timeout. This timeout is used when accessing
         * the container and trying to aquire a lock. If it is configured to a negative
         * number (preferably -1) the timeout is disabled
         *
         * Default: 10000 ms
         *
         * @param _ms timeout value
         */
        void set_lock_timeout(std::chrono::milliseconds _ms)
        {
            __lock_timeout = _ms;
        }

        /**
         * @brief manually aquire the lock of the underlying container object.
         * If the lock timeout is exceeded, a lock_timeout_error is thrown
         *
         */
        void lock()
        {
            if (__lock)
                return;
            if (__lock_timeout.count() < 0)
            {
                __lock.lock();
                return;
            }
            __lock.try_lock_for(__lock_timeout);
            if (!__lock)
                throw lock_timeout_error("ts-stl/wrapper shared_accessor::lock()");
        }

        /**
         * @brief Releases the lock if it owns it.
         */
        void unlock()
        {
            if (__lock)
                __lock.unlock();
        }

        /**
         * @brief arrow operator can be used to access container object members.
         * When using this operator, lock access will be aquired if the lock is
         * not owned already. If the lock timeout is not configured to -1 and is
         * exceeded without a successfull lock, a ts::lock_timeout_error is thrown.
         */
        const _CT *operator->()
        {
            // just return if lock is already owned
            if (__lock)
                return &__container;

            // otherwise aquire lock
            if (__lock_timeout.count() < 0)
            {
                __lock.lock();
            }
            else
            {
                __lock.try_lock_for(__lock_timeout);
                if (!__lock)
                    throw lock_timeout_error("ts-stl/wrapper shared_accessor::operator->()");
            }

            return &__container;
        }
        /**
         * @brief asterisk operator can be used to access the container object directly.
         * When using this operator, lock access will be aquired if the lock is
         * not owned already. If the lock timeout is not configured to -1 and is
         * exceeded without a successfull lock, a ts::lock_timeout_error is thrown.
         */
        const _CT &operator*()
        {
            // just return if lock is already owned
            if (__lock)
                return __container;

            // otherwise aquire lock
            if (__lock_timeout.count() < 0)
            {
                __lock.lock();
            }
            else
            {
                __lock.try_lock_for(__lock_timeout);
                if (!__lock)
                    throw lock_timeout_error("ts-stl/wrapper shared_accessor::operator*()");
            }

            return __container;
        }
    };

    /**
     * @brief wrapper class referencing a container of type _CT (typically in a ts::wrapper object)
     * and a lock of type _LT that is assosiated with a mutex guarding the container (typically the mutex
     * of a ts::wrapper object). The class provides access operators to access container methods and
     * read/write data from or to the container.
     * The accessor is allways associated with a mutex but does not necessarly have to own the lock.
     * Whenever a access operator is used, the lock is aquired (or attempted to be acquired) if the accessor
     * doesn't own the lock already. The mutex can manually be locked or unlocked using the lock() and unlock() methods.
     * When the accessor is deconstructed, the mutex is allways unlocket.
     *
     * Note that the accessor class itself is not thread safe, meaning one instance of accessor is only ever allowed to be used in a single thread.
     *
     * @tparam _CT container type
     * @tparam _MT mutex type
     */
    template <class _CT, class _MT>
    class unique_accessor
    {
    private:
        _CT &__container;
        std::unique_lock<_MT> __lock;

        std::chrono::milliseconds __lock_timeout = 10000;

    public:
        unique_accessor(_CT &_c, _MT &_mu)
            : __container(_c),
            __lock(_mu, std::defer_lock),
            __lock_timeout(10000)
        {
        }

        /**
         * @brief Set the lock timeout. This timeout is used when accessing
         * the container and trying to aquire a lock. If it is configured to a negative
         * number (preferably -1) the timeout is disabled
         *
         * Default: 10000 ms
         *
         * @param _ms timeout value
         */
        void set_lock_timeout(std::chrono::milliseconds _ms)
        {
            __lock_timeout = _ms;
        }

        /**
         * @brief manually aquire the lock of the underlying container object.
         * If the lock timeout is exceeded, a lock_timeout_error is thrown
         *
         */
        void lock()
        {
            if (__lock)
                return;
            if (__lock_timeout.count() < 0)
            {
                __lock.lock();
                return;
            }
            __lock.try_lock_for(__lock_timeout);
            if (!__lock)
                throw lock_timeout_error("ts-stl/wrapper unique_accessor::lock()");
        }

        /**
         * @brief Releases the lock if it owns it.
         */
        void unlock()
        {
            if (__lock)
                __lock.unlock();
        }

        /**
         * @brief arrow operator can be used to access container object members.
         * When using this operator, lock access will be aquired if the lock is
         * not owned already. If the lock timeout is not configured to -1 and is
         * exceeded without a successfull lock, a ts::lock_timeout_error is thrown.
         */
        _CT *operator->()
        {
            // just return if lock is already owned
            if (__lock)
                return &__container;

            // otherwise aquire lock
            if (__lock_timeout.count() < 0)
            {
                __lock.lock();
            }
            else
            {
                __lock.try_lock_for(__lock_timeout);
                if (!__lock)
                    throw lock_timeout_error("ts-stl/wrapper unique_accessor::operator->()");
            }

            return &__container;
        }
        /**
         * @brief asterisk operator can be used to access the container object directly.
         * When using this operator, lock access will be aquired if the lock is
         * not owned already. If the lock timeout is not configured to -1 and is
         * exceeded without a successfull lock, a ts::lock_timeout_error is thrown.
         */
        _CT &operator*()
        {
            // just return if lock is already owned
            if (__lock)
                return __container;

            // otherwise aquire lock
            if (__lock_timeout.count() < 0)
            {
                __lock.lock();
            }
            else
            {
                __lock.try_lock_for(__lock_timeout);
                if (!__lock)
                    throw lock_timeout_error("ts-stl/wrapper unique_accessor::operator*()");
            }

            return __container;
        }
    };


    /**
     * @brief A class wrapping a container of type _T and a protective mutex
     * only granting limited access to the container after calling
     * get_exclusive_access() or get_shared_access()
     *
     * @tparam _T container type
     */
    template <class _T>
    class wrapper
    {
    private:
        _T __container;
        std::shared_timed_mutex __stmutex;

        std::chrono::milliseconds __lock_timeout;

    public:
        typedef std::unique_lock<std::shared_timed_mutex> _ulock_t;
        typedef std::shared_lock<std::shared_timed_mutex> _slock_t;

        wrapper(_T &&_stl_init)
            : __container(std::move(_stl_init)),
            __lock_timeout(10000)
        {
        }
        wrapper(const _T &_stl_init)
            : __container(std::copy(_stl_init)),
            __lock_timeout(10000)
        {
        }
        wrapper()
            : __lock_timeout(10000)
            {}
        
        // allow copy, move, assign
        wrapper(const wrapper &_other)
        {
            _slock_t readlock(_other.__stmutex, std::defer_lock);
            assert(("ts-stl/wrapper copy", readlock.try_lock_for(__lock_timeout)));
            __container = std::copy(_other.__container);
            __lock_timeout = _other.__lock_timeout;
        }
        wrapper(wrapper &&_other)
        {
            _ulock_t writelock(_other.__stmutex, std::defer_lock);
            assert(("ts-stl/wrapper move", writelock.try_lock_for(__lock_timeout)));
            __container = std::move(_other.__container);
            __lock_timeout = _other.__lock_timeout;
        }

        wrapper &operator=(const wrapper &_rhs)
        {
            if (this == &_rhs)
                return *this; // don't do self assignment

            _ulock_t lhs_write_lock(__stmutex, std::defer_lock);
            _slock_t rhs_read_lock(_rhs.__stmutex, std::defer_lock);
            std::lock(lhs_write_lock, rhs_read_lock);
            __container = std::copy(_rhs.__container);
            __lock_timeout = _rhs.__lock_timeout;

            return *this;
        }
        wrapper &operator=(wrapper &&_rhs)
        {
            if (this == &_rhs)
                return *this; // don't do self assignment

            _ulock_t lhs_write_lock(__stmutex, std::defer_lock);
            _ulock_t rhs_write_lock(_rhs.__stmutex, std::defer_lock);
            std::lock(lhs_write_lock, rhs_write_lock);
            __container = std::move(_rhs.__container);
            __lock_timeout = _rhs.__lock_timeout;

            return *this;
        }

        /**
         * @brief Set the lock timeout. This timeout is used when accessing
         * the container and trying to aquire a lock. If it is configured to a negative
         * number (preferably -1) the timeout is disabled. This will be passed on to any
         * accessors created by a method.
         *
         * Default: 10000 ms
         *
         * @param _ms timeout value
         */
        void set_lock_timeout(std::chrono::milliseconds _ms)
        {
            __lock_timeout = _ms;
        }

        /**
         * @brief creates a unique accessor to the container and returns it.
         * The unique accessor can then be used to access the container. 
         * If _aquire is true (default) the mutex will be locket (with configured timeout)
         * so it will already be locked at the time of accessing and won't have to be locked then.
         * If a significant amount of time passes between aquireing the accessor (this method)
         * and actually using it, it is recommended to set _aquire to false. This way, the 
         * accessor will only aquire the lock once it is used.
         * 
         * @param _aquire lock aquire flag
         */
        unique_accessor<_T, std::shared_timed_mutex> get_exclusive_access(bool _aquire = true)
        {
            unique_accessor<_T, std::shared_timed_mutex> accessor(__container, __stmutex);
            accessor.set_lock_timeout(__lock_timeout);
            if (_aquire)
                accessor.lock();
            return accessor;
        }

        /**
         * @brief creates a shared accessor to the container and returns it.
         * The shared accessor can then be used to read from the container. 
         * If _aquire is true (default) the mutex will be locket (with configured timeout)
         * so it will already be locked at the time of accessing and won't have to be locked then.
         * If a significant amount of time passes between aquireing the accessor (this method)
         * and actually using it, it is recommended to set _aquire to false. This way, the 
         * accessor will only aquire the lock once it is used.
         * 
         * @param _aquire lock aquire flag
         */
        shared_accessor<_T, std::shared_timed_mutex> get_shared_access(bool _aquire = true)
        {
            shared_accessor<_T, std::shared_timed_mutex> accessor(__container, __stmutex);
            accessor.set_lock_timeout(__lock_timeout);
            if (_aquire)
                accessor.lock();
            return accessor;
        }
    };

};