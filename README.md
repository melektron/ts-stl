# ts-stl
ts-stl is a collection of wrapper classes adding thread safety to generic C++ STL container classes using STL features.

## What does ts-stl do?

First of all, __elstd is NOT a replacement for some STL classes__. Instead, ts-stl wrapps STL classes (mostly container classes like unordered_map that have to be provided by the system specific STL implementation) with a safety layer that intercepts method calls and makes sure no two threads access the data simultaniously in a forbidden way. For example, while one thread is writing to the container, all other threads accessing the object may have to wait until the write operation is done.

## Compatability

ts-stl should work with C++ 17 (and possibly higher) in all environments that provide a concurency implementation (standard threading and mutex classes) according to the STL spec. This should include all modern environments on OSes like Windows, Linux and MacOS but may not work on low power systems like microcontrollers.

## Class list

 * None

## Planned Classes
 
 * unordered_map
