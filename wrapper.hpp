/*
ELEKTRON © 2022
Written by Matteo Reiter
www.elektron.work
14.09.22, 18:37
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree.


Thread-safe wraper for any container or class
*/

#pragma once

#include <shared_mutex>
#include <chrono>

namespace ts
{
    template <class _T>
    class wrapper
    {
    private:
        _T __container;
        std::shared_timed_mutex __stmutex;

    public:
        typedef std::unique_lock<std::shared_timed_mutex> _ulock_t;
        typedef std::shared_lock<std::shared_timed_mutex> _slock_t;

        map_wrapper(_T &&_stl_init)
            : __container(std::move(_stl_init))
        {
        }
        map_wrapper(const _T &_stl_init)
            : __container(std::copy(_stl_init))
        {
        }
        map_wrapper() = default;

        // allow copy, move, assign
        map_wrapper(const map_wrapper &_other) = default;
        map_wrapper(const map_wrapper &&_other) = default;

        using namespace std::chrono_literals;

        /**
         * @brief tries to acquire exclusive access to the container
         * by locking the internal mutex. If the lock succeeds, the method
         * returns a unique lock owning the mutex. If the ownershit could not
         * be acquired before the timeout, a lock without ownershit is returned.
         * The ownership status can be checked by explicitly or implicitly casting to
         * bool or by using the owns_lock() method on the lock.
         *
         * After a write or read operation is finished, the internal mutex should be released
         * using the release() method on the lock or by calling it's destructor (aka make it go
         * out of skope -> prefered).
         */
        _ulock_t get_exclusive_access(std::chrono::milliseconds _timeout = 1000ms);

        /**
         * @brief tries to acquire shared access to the container
         * by locking the internal mutex. If the lock succeeds, the method
         * returns a unique lock owning the mutex. If the ownershit could not
         * be acquired before the timeout, a lock without ownershit is returned.
         * The ownership status can be checked by explicitly or implicitly casting to
         * bool or by using the owns_lock() method on the lock.
         *
         * After a write or read operation is finished, the internal mutex should be released
         * using the release() method on the lock or by calling it's destructor (aka make it go
         * out of skope -> prefered).
         */
        _ulock_t get_shared_access(std::chrono::milliseconds _timeout = 1000ms);

        /**
         * @brief arrow operator can be used to access the underlying object.
         * When using this operator, it is assumed that propper access has been aquired
         * before using get_exclusive_access() or get_shared_access()
         */
        _T *operator->() const
        {
            return &__container;
        }

        _T &operator*() const
        {
            return __container;
        }
    };

};