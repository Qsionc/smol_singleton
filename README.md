# smol::singleton

My Attempt on creating thread-safe wrapper for Singleton Design Pattern for C++.

Main purpose for this project is to test thread safety and learn about CMake packaging.

## Table of contents
1. [Information](#information)
2. [Installation](#installation)
3. [Example usage](#example-usage)
4. [Final thoughts](#final-thoughts)

## Information

I created this project to make first public repository with something slightly useful.<br>
Singleton Wrapper is template class that will control initialisation and destruction of Singleton objects. Interface allows for manual creation and destruction of held object at will. <br>
I created it to avoid necessity of using pattern with static initialisation in static member function. 
This allows for creation of "Singletons" with classes that are not created with Singleton Pattern.

The class still uses static private member, but all static members have default constructors marked `noexcept`, so it should be safe to use.<br>
Class member template function `init(Args&& ... _args)` can throw in case there is any exception during initialisation of new smol object.
Wrapper allows for copying and copy assignment using default constructors, but not moves.

Tests of singleton were performed using `googletest`, you can disable them by setting variable `UNIT_TEST` in CMakeLists.txt to `OFF`. 

## Installation

After download run cmake and build target install.

I recommend installing it locally, for example in .local folder of your home directory. Example after clone:
```shell
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:STRING="${HOME}/.local" # or "home/user-name/.local"
cmake --build . --target install
```

## Example usage

In your `CMakeLists.txt` use `find_package` to find singleton library:
```cmake
find_package(singleton REQUIRED)
# ...
add_executable(some_exec some_source.cpp)
target_link_libraries(some_exec PRIVATE singleton::singleton)
```
Remember to provide proper CMAKE_PREFIX_PATH for find_package engine.
```c++
#include <thread>
#include <singleton.h>

int main()
{
    smol::singleton<int> single;
    std::thread t([](smol::singleton<int> _s) { //singleton is trivially copyable but not movable
        _s.init(42);
    }, single);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Just small synchronisation, use better synchronisation tools
    std::cout << *single << std::endl; // Outputs 42, dereference operator returns int&
    return 0;
}
```

# Final Thoughts

This is my first public repository, and wish to learn more about sharing, creating code and contributing.<br>
Please be considerate - but also share some serious critique, insight. I hope someone finds this useful or at least help me test correctness of my approach.

Comments and suggestions are welcome!
