/*
ELEKTRON © 2022
Written by Matteo Reiter
www.elektron.work
24.09.22, 12:24
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree.

Exception classes for ts-stl
*/

#pragma once

#include <exception>

namespace ts
{
    class lock_timeout_error : public std::exception
    {
    private:
        const char *message = nullptr;

    public:
        lock_timeout_error(const char *_msg)
        {
            message = _msg;
        }

        virtual const char *what() const noexcept override
        {
            return message;
        }
    };
};