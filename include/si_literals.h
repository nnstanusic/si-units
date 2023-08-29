#pragma once
#include "si.h"

namespace si::literals
{
	constexpr auto m = meter<float>{ 1 };
	constexpr auto s = second<float>{ 1 };
	constexpr auto mol = mols<float>{ 1 };
	constexpr auto A = ampere<float>{ 1 };
	constexpr auto K = kelvin<float>{ 1 };
	constexpr auto cd = candela<float>{ 1 };
	constexpr auto g = gram<float>{ 1 };
	constexpr auto kg = kilo_gram<float>{ 1 };
	constexpr auto t = ton<float>{ 1 };

	constexpr auto operator""_m(long double x) { return x * m; };
	constexpr auto operator""_s(long double x) { return x * s; };
	constexpr auto operator""_mol(long double x) { return x * mol; };
	constexpr auto operator""_A(long double x) { return x * A; };
	constexpr auto operator""_K(long double x) { return x * K; };
	constexpr auto operator""_cd(long double x) { return x * cd; };
	constexpr auto operator""_g(long double x) { return x * g; };
	constexpr auto operator""_kg(long double x) { return x * kg; };
	constexpr auto operator""_t(long double x) { return x * t; };
}

