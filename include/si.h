#pragma once
#include <array>
#include <concepts>

namespace si
{

	namespace details
	{
		enum class exponent_array_index
		{
			Length,
			Time,
			AmountOfSubstance,
			ElectricCurrent,
			Temperature,
			LuminousIntensity,
			Mass
		};
		using exponent_array = std::array<int, 7>;

		struct unit_descriptor
		{
			exponent_array exponent;
			float factor;
		};

		template <class T, class... TRest>
		consteval auto add_exponents(T a, TRest... b)
		{
			exponent_array output;
			for (size_t i = 0; i < output.size(); ++i)
				output[i] = a[i] + (b[i] + ...);
			return output;
		}

		consteval auto subtract_exponents(exponent_array a, exponent_array b)
		{
			exponent_array output;
			for (size_t i = 0; i < output.size(); ++i)
				output[i] = a[i] - b[i];
			return output;
		}

		consteval auto invert_exponents(exponent_array exponent)
		{
			exponent_array output;
			for (size_t i = 0; i < output.size(); ++i)
				output[i] = -exponent[i];
			return output;
		}

		consteval auto conversion_factor(auto from, auto to)
		{
			return from.factor / to.factor;
		}

		consteval auto operator*(exponent_array a, exponent_array b)
		{
			return add_exponents(a, b);
		}

		consteval auto operator/(exponent_array a, exponent_array b)
		{
			return subtract_exponents(a, b);
		}

		consteval auto operator^(exponent_array a, int exponent)
		{
			for (size_t i = 0; i < a.size(); ++i)
				a[i] = a[i] * exponent;
			return a;
		}

		constexpr auto meter_expo = exponent_array{1, 0, 0, 0, 0, 0, 0};
		constexpr auto second_expo = exponent_array{0, 1, 0, 0, 0, 0, 0};
		constexpr auto mol_expo = exponent_array{0, 0, 1, 0, 0, 0, 0};
		constexpr auto ampere_expo = exponent_array{0, 0, 0, 1, 0, 0, 0};
		constexpr auto kelvin_expo = exponent_array{0, 0, 0, 0, 1, 0, 0};
		constexpr auto candela_expo = exponent_array{0, 0, 0, 0, 0, 1, 0};
		constexpr auto kilogram_expo = exponent_array{0, 0, 0, 0, 0, 0, 1};

		constexpr auto meter_desc = unit_descriptor{.exponent = meter_expo, .factor = 1.0f};
		constexpr auto second_desc = unit_descriptor{.exponent = second_expo, .factor = 1.0f};
		constexpr auto mol_desc = unit_descriptor{.exponent = mol_expo, .factor = 1.0f};
		constexpr auto ampere_desc = unit_descriptor{.exponent = ampere_expo, .factor = 1.0f};
		constexpr auto kelvin_desc = unit_descriptor{.exponent = kelvin_expo, .factor = 1.0f};
		constexpr auto candela_desc = unit_descriptor{.exponent = candela_expo, .factor = 1.0f};
		constexpr auto kilogram_desc = unit_descriptor{.exponent = kilogram_expo, .factor = 1.0f};

		constexpr auto squared_meter_desc = unit_descriptor{.exponent = meter_expo ^ 2, .factor = 1.0f};
		constexpr auto cubic_meter_desc = unit_descriptor{.exponent = meter_expo ^ 3, .factor = 1.0f};

		constexpr auto velocity_desc = unit_descriptor{.exponent = meter_expo / second_expo, .factor = 1.0f};
		constexpr auto acceleration_desc = unit_descriptor{.exponent = velocity_desc.exponent / second_expo, .factor = 1.0f};

		constexpr auto hertz_desc = unit_descriptor{.exponent = second_expo ^ -1, .factor = 1.0f};
		constexpr auto newton_desc = unit_descriptor{.exponent = kilogram_expo * acceleration_desc.exponent, .factor = 1.0f};
		constexpr auto pascal_desc = unit_descriptor{.exponent = newton_desc.exponent / squared_meter_desc.exponent, .factor = 1.0f};
		constexpr auto joule_desc = unit_descriptor{.exponent = newton_desc.exponent * meter_expo, .factor = 1.0f};
		constexpr auto watt_desc = unit_descriptor{.exponent = joule_desc.exponent * (second_expo ^ -1), .factor = 1.0f};
		constexpr auto coulomb_desc = unit_descriptor{.exponent = second_expo * ampere_expo, .factor = 1.0f};
		constexpr auto volt_desc = unit_descriptor{.exponent = watt_desc.exponent / ampere_expo, .factor = 1.0f};
		constexpr auto farad_desc = unit_descriptor{.exponent = coulomb_desc.exponent / volt_desc.exponent, .factor = 1.0f};
		constexpr auto ohm_desc = unit_descriptor{.exponent = volt_desc.exponent / ampere_expo, .factor = 1.0f};
		constexpr auto siemens_desc = unit_descriptor{.exponent = ohm_desc.exponent ^ -1, .factor = 1.0f};
		constexpr auto weber_desc = unit_descriptor{.exponent = volt_desc.exponent * second_expo, .factor = 1.0f};
		constexpr auto tesla_desc = unit_descriptor{.exponent = weber_desc.exponent / squared_meter_desc.exponent, .factor = 1.0f};
		constexpr auto henry_desc = unit_descriptor{.exponent = weber_desc.exponent / ampere_expo, .factor = 1.0f};
		constexpr auto lux_desc = unit_descriptor{.exponent = candela_expo / squared_meter_desc.exponent, .factor = 1.0f};
		constexpr auto katal_desc = unit_descriptor{.exponent = mol_expo / second_expo, .factor = 1.0f};

	};

	template <details::unit_descriptor A, details::unit_descriptor B>
	concept same_exponent_c = A
	.exponent == B.exponent;

	template <details::unit_descriptor descriptor>
	struct unit
	{
		float value;

		static consteval auto Descriptor()
		{
			return descriptor;
		}

		template <details::unit_descriptor d_other>
			requires same_exponent_c<descriptor, d_other>
		[[nodiscard]] constexpr auto operator=(this auto &v, unit<d_other> other)
		{
			constexpr auto factor = details::conversion_factor(d_other, descriptor);
			v.value = factor * other.value;
			return v;
		}

		template <details::unit_descriptor d_other>
			requires same_exponent_c<descriptor, d_other>
		[[nodiscard]] constexpr auto operator+(this auto v, unit<d_other> other)
		{
			constexpr auto factor = details::conversion_factor(d_other, descriptor);
			v.value += factor * other.value;
			return v;
		}

		template <details::unit_descriptor d_other>
			requires same_exponent_c<descriptor, d_other>
		[[nodiscard]] constexpr auto operator-(this auto v, unit<d_other> other)
		{
			constexpr auto factor = details::conversion_factor(d_other, descriptor);
			v.value -= factor * other.value;
			return v;
		}

		template <details::unit_descriptor d_other>
		[[nodiscard]] constexpr auto operator*(this auto v, unit<d_other> other);

		[[nodiscard]] constexpr auto operator*(this auto v, float factor)
		{
			v.value *= factor;
			return v;
		}

		[[nodiscard]] friend constexpr auto operator*(float factor, unit<descriptor> v)
		{
			return v * factor;
		}

		template <details::unit_descriptor d_other>
		[[nodiscard]] constexpr auto operator/(this auto v, unit<d_other> other);

		[[nodiscard]] constexpr auto operator/(this auto v, float factor)
		{
			v.value /= factor;
			return v;
		}
	};

	struct Meter : unit<details::meter_desc>
	{
	};
	struct Second : unit<details::second_desc>
	{
	};
	struct Mol : unit<details::mol_desc>
	{
	};
	struct Ampere : unit<details::ampere_desc>
	{
	};
	struct Kelvin : unit<details::kelvin_desc>
	{
	};
	struct Candela : unit<details::candela_desc>
	{
	};
	struct Kilogram : unit<details::kilogram_desc>
	{
	};
	struct MetersPerSecond : unit<details::velocity_desc>
	{
	};
	struct MetersPerSquaredSecond : unit<details::acceleration_desc>
	{
	};

	struct SquareMeters : unit<details::squared_meter_desc>
	{
	};
	struct CubicMeters : unit<details::cubic_meter_desc>
	{
	};

	struct Hertz : unit<details::hertz_desc>
	{
	};
	struct Newton : unit<details::newton_desc>
	{
	};
	struct Pascal : unit<details::pascal_desc>
	{
	};
	struct Joule : unit<details::joule_desc>
	{
	};
	struct Watt : unit<details::watt_desc>
	{
	};
	struct Coulomb : unit<details::coulomb_desc>
	{
	};
	struct Volt : unit<details::volt_desc>
	{
	};
	struct Farad : unit<details::farad_desc>
	{
	};
	struct Ohm : unit<details::ohm_desc>
	{
	};
	struct Siemens : unit<details::siemens_desc>
	{
	};
	struct Weber : unit<details::weber_desc>
	{
	};
	struct Tesla : unit<details::tesla_desc>
	{
	};
	struct Henry : unit<details::henry_desc>
	{
	};
	struct Lux : unit<details::lux_desc>
	{
	};
	struct Katal : unit<details::katal_desc>
	{
	};

	namespace details
	{

		template <unit_descriptor d>
		struct inferer
		{
			using type = unit<d>;
		};

		template <>
		struct inferer<meter_desc>
		{
			using type = Meter;
		};

		template <>
		struct inferer<unit_descriptor{.exponent = {0, 0, 0, 0, 0, 0, 0}, .factor = 1.0f}>
		{
			using type = float;
		};
		template <>
		struct inferer<MetersPerSecond::Descriptor()>
		{
			using type = MetersPerSecond;
		};
		template <>
		struct inferer<MetersPerSquaredSecond::Descriptor()>
		{
			using type = MetersPerSquaredSecond;
		};
		template <>
		struct inferer<SquareMeters::Descriptor()>
		{
			using type = SquareMeters;
		};
		template <>
		struct inferer<CubicMeters::Descriptor()>
		{
			using type = CubicMeters;
		};
		template <>
		struct inferer<Hertz::Descriptor()>
		{
			using type = Hertz;
		};
		template <>
		struct inferer<Newton::Descriptor()>
		{
			using type = Newton;
		};
		template <>
		struct inferer<Pascal::Descriptor()>
		{
			using type = Pascal;
		};
		template <>
		struct inferer<Joule::Descriptor()>
		{
			using type = Joule;
		};
		template <>
		struct inferer<Watt::Descriptor()>
		{
			using type = Watt;
		};
		template <>
		struct inferer<Coulomb::Descriptor()>
		{
			using type = Coulomb;
		};
		template <>
		struct inferer<Volt::Descriptor()>
		{
			using type = Volt;
		};
		template <>
		struct inferer<Farad::Descriptor()>
		{
			using type = Farad;
		};
		template <>
		struct inferer<Ohm::Descriptor()>
		{
			using type = Ohm;
		};
		template <>
		struct inferer<Siemens::Descriptor()>
		{
			using type = Siemens;
		};
		template <>
		struct inferer<Weber::Descriptor()>
		{
			using type = Weber;
		};
		template <>
		struct inferer<Tesla::Descriptor()>
		{
			using type = Tesla;
		};
		template <>
		struct inferer<Henry::Descriptor()>
		{
			using type = Henry;
		};
		template <>
		struct inferer<Lux::Descriptor()>
		{
			using type = Lux;
		};
		template <>
		struct inferer<Katal::Descriptor()>
		{
			using type = Katal;
		};

		template <unit_descriptor d>
		using inferer_t = typename inferer<d>::type;
	}

	template <details::unit_descriptor d>
	[[nodiscard]] constexpr auto infer_cast(unit<d> value)
	{
		return typename details::inferer<d>::type{value.value};
	};

	template <details::unit_descriptor descriptor>
	template <details::unit_descriptor d_other>
	constexpr auto unit<descriptor>::operator*(this auto v, unit<d_other> other)
	{
		constexpr auto new_exponent = details::add_exponents(descriptor.exponent, d_other.exponent);
		constexpr auto new_desc = details::unit_descriptor{new_exponent, descriptor.factor * d_other.factor};
		return infer_cast(unit<new_desc>{v.value * other.value});
	}

	template <details::unit_descriptor descriptor>
	template <details::unit_descriptor d_other>
	constexpr auto unit<descriptor>::operator/(this auto v, unit<d_other> other)
	{
		constexpr auto new_exponent = details::subtract_exponents(descriptor.exponent, d_other.exponent);
		constexpr auto new_desc = details::unit_descriptor{new_exponent, descriptor.factor / d_other.factor};
		return infer_cast(unit<new_desc>{v.value / other.value});
	}

	template <details::unit_descriptor descriptor>
	[[nodiscard]] constexpr auto operator/(float factor, unit<descriptor> v)
	{
		constexpr auto new_exponent = details::invert_exponents(descriptor.exponent);
		constexpr auto new_desc = details::unit_descriptor{new_exponent, descriptor.factor};
		return infer_cast(unit<new_desc>{factor / v.value});
	}

	namespace literals
	{
		constexpr auto m = Meter{1};
		constexpr auto s = Second{1};
		constexpr auto mol = Mol{1};
		constexpr auto A = Ampere{1};
		constexpr auto K = Kelvin{1};
		constexpr auto cd = Candela{1};
		constexpr auto kg = Kilogram{1};

		constexpr auto operator""_m(long double x) { return x * m; };
		constexpr auto operator""_s(long double x) { return x * s; };
		constexpr auto operator""_mol(long double x) { return x * mol; };
		constexpr auto operator""_A(long double x) { return x * A; };
		constexpr auto operator""_K(long double x) { return x * K; };
		constexpr auto operator""_cd(long double x) { return x * cd; };
		constexpr auto operator""_kg(long double x) { return x * kg; };
	}
}