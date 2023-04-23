# SI Units Library
This is a C++23 library for working with SI units.

## Features

 *  All calculation are possible during the compile step
 *  Dissallows some operations on different types `+ and -`, allows for generation of new types `* and /`
 *  Some newly generated types can be infered e.g `si::meter / si::second / si::second * si::kilo_gram -> si::newton`

## Installation
To use this library, simply download the header files and include them in your C++ project. You can then use the classes and functions provided in the library to perform calculations with SI units.

## Usage
Here's an example of how to use the library to calculate the velocity of an object:

```c++

#include <si.h>

int main() {
  using namespace si::literals;

  constexpr auto mass = si::kilo_gram{50.0f};
  constexpr auto distance = m;  // 1 si::meter constant
  constexpr auto time = 10.0_s; //10 si::seconds literal
  auto velocity = distance / time;  // si::meters_per_seconds is automatically inferred
  std::cout << "Velocity: " << velocity.value << " m/s\n";
  std::cout << "Velocity: " << static_cast<float>(velocity) << " m/s\n";
  return 0;
}

```

## Classes
The library provides the following classes for working with SI units:

* `si::meter`
* `si::kilogram`
* `si::second`
* `si::ampere`
* `si::kelvin`
* `si::mol`
* `si::candela`

In addition, the library provides a number of derived units, such as:

* `si::newton (force)`
* `si::joule (energy)`
* `si::watt (power)`