/*
ELEKTRON © 2024 - now
Written by melektron
www.elektron.work
01.01.24, 09:40
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree. 

Wrapper types and Include statement for map STL header.
*/

#pragma once

#include <map>
#include "wrapper.hpp"

namespace ts
{
    template <class... _Args>
    using map = wrapper<std::map<_Args...>>;

    template <class... _Args>
    using multimap = wrapper<std::multimap<_Args...>>;
};