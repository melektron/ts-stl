/*
ELEKTRON © 2022
Written by Matteo Reiter
www.elektron.work
26.10.22, 16:56
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree. 

threadsafe wrapper for std::string
*/

#pragma once

#include <string>
#include "wrapper.hpp"

namespace ts
{
    using string = wrapper<std::string>;
};