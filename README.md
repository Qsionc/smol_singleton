#Singleton Wrapper
My Attempt on creating thread-safe wrapper for Singleton Design Pattern for C++.

Main purpose for this project is to test thread safety and learn about CMake packaging.

##Table of contents
* [Information](#information)
* [Installation](#installation)
* [Example usage](#example-usage)
* [Final thoughts](#final-thoughts)

##Information

I created this project to make first public repository with something slightly useful.<br>
Singleton Wrapper is template class that will control initialisation and destruction of Singleton objects. Interface allows for manual creation and destruction of held object at will. <br>
I created it to avoid necessity of using pattern with static initialisation in static member function. 
This allows for creation of "Singletons" with classes that are not created with Singleton Pattern.

The class still uses static private member, but all static members have default constructors marked `noexcept`, so it should be safe to use.<br>
Class member template function `init(Args&& ... _args)` can throw in case there is any exception during initialisation of new singleton object.
Wrapper allows for copying and copy assignment using default constructors, but not moves.

Tests of SingletonWrapper were performed using `googletest`, you can disable them by setting variable `UNIT_TEST` in CMakeLists.txt to `OFF`. 

##Installation

After download run cmake and build target install.

I recommend installing it locally, for example in .local folder of your home directory. Example after clone:
```shell
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:STRING="${HOME}/.local" # or "home/user-name/.local"
cmake --build . --target install
```


##Example usage

```c++
#include <thread>
#include <SingletonWrapper.h>

int main()
{
    singleton::wrapper<int> single;
    std::thread t([](singleton::wrapper<int> _s) { //SingletonWrapper is copyable but not movable
        _s.init(42);
    };
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << *single << std::endl; // Outputs 42
}
```


###Final Thoughts

This is my first public repository, and wish to learn more about sharing, creating code and contributing.<br>
Please be considerate - but also share some serious critique, insight. I hope someone finds this useful or at least help me test correctness of my approach.

Comments and suggestions are welcome!