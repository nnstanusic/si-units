# SI Units Library
This is a C++23 library for working with SI units.

## Features

 *  All calculation are possible during the compile step
 *  Dissallows some operations on different types `+ and -`, allows for generation of new types `* and /`
 *  Some newly generated types can be infered e.g `si::meter / si::second / si::second * si::kilo_gram -> si::newton`

## Installation
To use this library, simply download the header files and include them in your C++ project.

## Usage
Here's an example of how to use the library to calculate the velocity of an object:

```c++

#include <si.h>

int main() {
  using namespace si::literals;

  constexpr auto mass = si::kilo_gram{50}; //si::kilo_gram<int>
  constexpr auto distance = m;  // 1 si::meter constant
  constexpr auto time = 10.0_s; //10 si::seconds literal
  auto velocity = distance / time; 

  static_assert(std::same_as_v<decltype(veclocity), si::meters_per_second<float>>, "Types are not equal");
  std::cout << "Velocity: " << velocity.value << " m/s\n";
  std::cout << "Velocity: " << static_cast<float>(velocity) << " m/s\n";
  std::cout << std::format("Velocity: {}\n", velocity); // >> 0.1 m s^-1
  return 0;
}

```

## Classes
The library provides the following classes for working with SI units:


|  base       | kilo             | mega             | time       | compound                     |
|-------------|------------------|------------------|------------|------------------------------|
| si::meter   | si::kilo_meter   | si::mega_meter   | si::minute | si::meters_per_second        |
| si::second  | si::kilo_second  | si::mega_second  | si::hour   | si::meters_per_second_squared|
| si::mols    | si::kilo_mols    | si::mega_mols    | si::day    | si::SquareMeters             |
| si::ampere  | si::kilo_ampere  | si::mega_ampere  |            | si::CubicMeters              |
| si::kelvin  | si::kilo_kelvin  | si::mega_kelvin  |            | si::hertz                    |
| si::candela | si::kilo_candela | si::mega_candela |            | si::newton                   |
| si::gram    | si::kilo_gram    | ton              |            | si::pascal                   |
|             |                  |                  |            | si::joule                    |
|             |                  |                  |            | si::watt                     |
|             |                  |                  |            | si::coulomb                  |
|             |                  |                  |            | si::volt                     |
|             |                  |                  |            | si::farad                    |
|             |                  |                  |            | si::ohm                      |
|             |                  |                  |            | si::siemens                  |
|             |                  |                  |            | si::weber                    |
|             |                  |                  |            | si::tesla                    |
|             |                  |                  |            | si::henry                    |
|             |                  |                  |            | si::lux                      |
|             |                  |                  |            | si::katal                    |

## Literals
														
## Custom types

## Custom auto infer addition