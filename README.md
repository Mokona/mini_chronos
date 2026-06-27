Mini Chronos is a simple profiler for simple needs.

Requires a C++20 compiler.

## How to use

Add the library as a CMake subdirectory and link against `mini_chronos`. All headers are in `src/`.

Declare a `Database` and a `Chronos` instance once, then create `Timer` objects for each measurement point. Timers are declared upfront with an explicit name and reused across calls.

```cpp
#include <mini_chronos.h>
#include <timer.h>

MiniChronos::Database db;
MiniChronos::Chronos<std::chrono::steady_clock> chronos{db};

MiniChronos::Timer t_update{chronos, "update"};
MiniChronos::Timer t_render{chronos, "render"};

// In your loop:
t_update.start();
// ...
t_update.stop();

t_render.start();
// ...
t_render.stop();

// Read results:
for (const auto& data : chronos)
{
    // data.name, data.duration, data.calls
}
```

Path names are plain strings. You can use `::` as a separator if you want to express grouping (`"physics::broadphase"`), but the library treats names as opaque, no hierarchy is enforced.
You can choose other separator or any other naming scheme.

## How to build and launch the tests

Requires CMake 3.20+ and a C++20 compiler. GoogleTest is fetched via git submodule automatically.

```sh
git submodule update --init --recursive
cmake -B build
cmake --build build
./build/mini_chronos_tests
```
