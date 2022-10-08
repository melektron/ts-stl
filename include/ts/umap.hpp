/*
ELEKTRON © 2022
Written by Matteo Reiter
www.elektron.work
16.09.22, 23:26
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree. 

Wrapper types and Include statement for unordered_map STL header.
*/

#pragma once

#include <unordered_map>
#include "wrapper.hpp"

namespace ts
{
    template <class... _Args>
    using umap = wrapper<std::unordered_map<_Args...>>;

    template <class... _Args>
    using umultimap = wrapper<std::unordered_multimap<_Args...>>;
};