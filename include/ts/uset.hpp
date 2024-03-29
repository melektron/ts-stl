/*
ELEKTRON © 2022
Written by Matteo Reiter
www.elektron.work
16.09.22, 23:26
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree. 

Wrapper types and Include statement for unordered_set STL header.
*/

#pragma once

#include <unordered_set>
#include "wrapper.hpp"

namespace ts
{
    template <class... _Args>
    using uset = wrapper<std::unordered_set<_Args...>>;
};