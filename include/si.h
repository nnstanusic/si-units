#pragma once
#include <array>
#include <concepts>
#include <format>

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

		consteval auto operator*(unit_descriptor a, float factor)
		{
			return unit_descriptor{ .exponent = a.exponent, .factor = a.factor * factor };
		}

		constexpr auto meter_expo = exponent_array{ 1, 0, 0, 0, 0, 0, 0 };
		constexpr auto second_expo = exponent_array{ 0, 1, 0, 0, 0, 0, 0 };
		constexpr auto mol_expo = exponent_array{ 0, 0, 1, 0, 0, 0, 0 };
		constexpr auto ampere_expo = exponent_array{ 0, 0, 0, 1, 0, 0, 0 };
		constexpr auto kelvin_expo = exponent_array{ 0, 0, 0, 0, 1, 0, 0 };
		constexpr auto candela_expo = exponent_array{ 0, 0, 0, 0, 0, 1, 0 };
		constexpr auto gram_expo = exponent_array{ 0, 0, 0, 0, 0, 0, 1 };

		constexpr auto meter_desc = unit_descriptor{ .exponent = meter_expo, .factor = 1.0f };
		constexpr auto second_desc = unit_descriptor{ .exponent = second_expo, .factor = 1.0f };
		constexpr auto mol_desc = unit_descriptor{ .exponent = mol_expo, .factor = 1.0f };
		constexpr auto ampere_desc = unit_descriptor{ .exponent = ampere_expo, .factor = 1.0f };
		constexpr auto kelvin_desc = unit_descriptor{ .exponent = kelvin_expo, .factor = 1.0f };
		constexpr auto candela_desc = unit_descriptor{ .exponent = candela_expo, .factor = 1.0f };
		constexpr auto gram_desc = unit_descriptor{ .exponent = gram_expo, .factor = 0.001f };

		constexpr auto squared_meter_desc = unit_descriptor{ .exponent = meter_expo ^ 2, .factor = 1.0f };
		constexpr auto cubic_meter_desc = unit_descriptor{ .exponent = meter_expo ^ 3, .factor = 1.0f };

		constexpr auto velocity_desc = unit_descriptor{ .exponent = meter_expo / second_expo, .factor = 1.0f };
		constexpr auto acceleration_desc = unit_descriptor{ .exponent = velocity_desc.exponent / second_expo, .factor = 1.0f };

		constexpr auto hertz_desc = unit_descriptor{ .exponent = second_expo ^ -1, .factor = 1.0f };
		constexpr auto newton_desc = unit_descriptor{ .exponent = gram_expo * acceleration_desc.exponent, .factor = 1.0f };
		constexpr auto pascal_desc = unit_descriptor{ .exponent = newton_desc.exponent / squared_meter_desc.exponent, .factor = 1.0f };
		constexpr auto joule_desc = unit_descriptor{ .exponent = newton_desc.exponent * meter_expo, .factor = 1.0f };
		constexpr auto watt_desc = unit_descriptor{ .exponent = joule_desc.exponent * (second_expo ^ -1), .factor = 1.0f };
		constexpr auto coulomb_desc = unit_descriptor{ .exponent = second_expo * ampere_expo, .factor = 1.0f };
		constexpr auto volt_desc = unit_descriptor{ .exponent = watt_desc.exponent / ampere_expo, .factor = 1.0f };
		constexpr auto farad_desc = unit_descriptor{ .exponent = coulomb_desc.exponent / volt_desc.exponent, .factor = 1.0f };
		constexpr auto ohm_desc = unit_descriptor{ .exponent = volt_desc.exponent / ampere_expo, .factor = 1.0f };
		constexpr auto siemens_desc = unit_descriptor{ .exponent = ohm_desc.exponent ^ -1, .factor = 1.0f };
		constexpr auto weber_desc = unit_descriptor{ .exponent = volt_desc.exponent * second_expo, .factor = 1.0f };
		constexpr auto tesla_desc = unit_descriptor{ .exponent = weber_desc.exponent / squared_meter_desc.exponent, .factor = 1.0f };
		constexpr auto henry_desc = unit_descriptor{ .exponent = weber_desc.exponent / ampere_expo, .factor = 1.0f };
		constexpr auto lux_desc = unit_descriptor{ .exponent = candela_expo / squared_meter_desc.exponent, .factor = 1.0f };
		constexpr auto katal_desc = unit_descriptor{ .exponent = mol_expo / second_expo, .factor = 1.0f };

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
		[[nodiscard]] constexpr auto operator=(this auto& v, unit<d_other> other)
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

		[[nodiscard]] explicit constexpr operator float() const {
			return this->value;
		};

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



	template<details::unit_descriptor d>
	constexpr auto kilo(unit<d> u) {
		constexpr auto new_descriptor = details::unit_descriptor{ d.exponent, d.factor * 1000.0f };
		return infer_cast(unit<new_descriptor>{u.value / 1000.0f});
	}


	template<details::unit_descriptor d>
	constexpr auto milli(unit<d> u) {
		constexpr auto new_descriptor = details::unit_descriptor{ d.exponent, d.factor / 1000.0f };
		return infer_cast(unit<new_descriptor>{u.value * 1000.0f});
	}

	//Base units
	struct meter : unit<details::meter_desc> {};
	struct second : unit<details::second_desc> {};
	struct mols : unit<details::mol_desc> {};
	struct ampere : unit<details::ampere_desc> {};
	struct kelvin : unit<details::kelvin_desc> {};
	struct candela : unit<details::candela_desc> {};
	struct gram : unit<details::gram_desc> {};

	//Kilo units
	struct kilo_meter : unit< details::meter_desc * 1e3f> {};
	struct kilo_second : unit<details::second_desc * 1e3f> {};
	struct kilo_mols : unit<details::mol_desc * 1e3f> {};
	struct kilo_ampere : unit<details::ampere_desc * 1e3f> {};
	struct kilo_kelvin : unit<details::kelvin_desc * 1e3f> {};
	struct kilo_candela : unit<details::candela_desc * 1e3f> {};
	struct kilo_gram : unit<details::gram_desc * 1e3f> {};

	//mega
	struct mega_meter : unit< details::meter_desc * 1e6f> {};
	struct mega_second : unit<details::second_desc * 1e6f> {};
	struct mega_mols : unit<details::mol_desc * 1e6f> {};
	struct mega_ampere : unit<details::ampere_desc * 1e6f> {};
	struct mega_kelvin : unit<details::kelvin_desc * 1e6f> {};
	struct mega_candela : unit<details::candela_desc * 1e6f> {};
	struct ton : unit<details::gram_desc * 1e6f> {};

	//Time
	struct minute : unit<details::second_desc * 60> {};
	struct hour : unit<details::second_desc * 60 * 60> {};
	struct day : unit<details::second_desc * 60 * 60 * 24> {};

	//compount 
	struct MetersPerSecond : unit<details::velocity_desc> {};
	struct MetersPerSecondSquared : unit<details::acceleration_desc> {};

	struct SquareMeters : unit<details::squared_meter_desc> {};
	struct CubicMeters : unit<details::cubic_meter_desc> {};

	struct hertz : unit<details::hertz_desc> {};
	struct newton : unit<details::newton_desc> {};
	struct pascal : unit<details::pascal_desc> {};
	struct joule : unit<details::joule_desc> {};
	struct watt : unit<details::watt_desc> {};
	struct coulomb : unit<details::coulomb_desc> {};
	struct volt : unit<details::volt_desc> {};
	struct farad : unit<details::farad_desc> {};
	struct ohm : unit<details::ohm_desc> {};
	struct siemens : unit<details::siemens_desc> {};
	struct weber : unit<details::weber_desc> {};
	struct tesla : unit<details::tesla_desc> {};
	struct henry : unit<details::henry_desc> {};
	struct lux : unit<details::lux_desc> {};
	struct katal : unit<details::katal_desc> {};

	namespace details
	{

		template <unit_descriptor d> struct inferer { using type = unit<d>; };

		template <> struct inferer<meter::Descriptor()> { using type = meter; };
		template <> struct inferer<second::Descriptor()> { using type = second; };
		template <> struct inferer<mols::Descriptor()> { using type = mols; };
		template <> struct inferer<ampere::Descriptor()> { using type = ampere; };
		template <> struct inferer<kelvin::Descriptor()> { using type = kelvin; };
		template <> struct inferer<candela::Descriptor()> { using type = candela; };
		template <> struct inferer<gram::Descriptor()> { using type = gram; };

		template <> struct inferer<kilo_meter::Descriptor()> { using type = kilo_meter; };
		template <> struct inferer<kilo_second::Descriptor()> { using type = kilo_second; };
		template <> struct inferer<kilo_mols::Descriptor()> { using type = kilo_mols; };
		template <> struct inferer<kilo_ampere::Descriptor()> { using type = kilo_ampere; };
		template <> struct inferer<kilo_kelvin::Descriptor()> { using type = kilo_kelvin; };
		template <> struct inferer<kilo_candela::Descriptor()> { using type = kilo_candela; };
		template <> struct inferer<kilo_gram::Descriptor()> { using type = kilo_gram; };

		template <> struct inferer<mega_meter::Descriptor()> { using type = mega_meter; };
		template <> struct inferer<mega_second::Descriptor()> { using type = mega_second; };
		template <> struct inferer<mega_mols::Descriptor()> { using type = mega_mols; };
		template <> struct inferer<mega_ampere::Descriptor()> { using type = mega_ampere; };
		template <> struct inferer<mega_kelvin::Descriptor()> { using type = mega_kelvin; };
		template <> struct inferer<mega_candela::Descriptor()> { using type = mega_candela; };
		template <> struct inferer<ton::Descriptor()> { using type = ton; };


		template <> struct inferer < unit_descriptor{ .exponent = {0, 0, 0, 0, 0, 0, 0}, .factor = 1.0f } > { using type = float; };
		template <> struct inferer<MetersPerSecond::Descriptor()> { using type = MetersPerSecond; };
		template <> struct inferer<MetersPerSecondSquared::Descriptor()> { using type = MetersPerSecondSquared; };
		template <> struct inferer<SquareMeters::Descriptor()> { using type = SquareMeters; };
		template <> struct inferer<CubicMeters::Descriptor()> { using type = CubicMeters; };
		template <> struct inferer<hertz::Descriptor()> { using type = hertz; };
		template <> struct inferer<newton::Descriptor()> { using type = newton; };
		template <> struct inferer<pascal::Descriptor()> { using type = pascal; };
		template <> struct inferer<joule::Descriptor()> { using type = joule; };
		template <> struct inferer<watt::Descriptor()> { using type = watt; };
		template <> struct inferer<coulomb::Descriptor()> { using type = coulomb; };
		template <> struct inferer<volt::Descriptor()> { using type = volt; };
		template <> struct inferer<farad::Descriptor()> { using type = farad; };
		template <> struct inferer<ohm::Descriptor()> { using type = ohm; };
		template <> struct inferer<siemens::Descriptor()> { using type = siemens; };
		template <> struct inferer<weber::Descriptor()> { using type = weber; };
		template <> struct inferer<tesla::Descriptor()> { using type = tesla; };
		template <> struct inferer<henry::Descriptor()> { using type = henry; };
		template <> struct inferer<lux::Descriptor()> { using type = lux; };
		template <> struct inferer<katal::Descriptor()> { using type = katal; };

		template <unit_descriptor d>
		using inferer_t = typename inferer<d>::type;
	}

	template <details::unit_descriptor d>
	[[nodiscard]] constexpr auto infer_cast(unit<d> value)
	{
		return typename details::inferer<d>::type{ value.value };
	};

	template <details::unit_descriptor descriptor>
	template <details::unit_descriptor d_other>
	constexpr auto unit<descriptor>::operator*(this auto v, unit<d_other> other)
	{
		constexpr auto new_exponent = details::add_exponents(descriptor.exponent, d_other.exponent);
		constexpr auto new_desc = details::unit_descriptor{ new_exponent, descriptor.factor * d_other.factor };
		return infer_cast(unit<new_desc>{v.value* other.value});
	}

	template <details::unit_descriptor descriptor>
	template <details::unit_descriptor d_other>
	constexpr auto unit<descriptor>::operator/(this auto v, unit<d_other> other)
	{
		constexpr auto new_exponent = details::subtract_exponents(descriptor.exponent, d_other.exponent);
		constexpr auto new_desc = details::unit_descriptor{ new_exponent, descriptor.factor / d_other.factor };
		return infer_cast(unit<new_desc>{v.value / other.value});
	}

	template <details::unit_descriptor descriptor>
	[[nodiscard]] constexpr auto operator/(float factor, unit<descriptor> v)
	{
		constexpr auto new_exponent = details::invert_exponents(descriptor.exponent);
		constexpr auto new_desc = details::unit_descriptor{ new_exponent, descriptor.factor };
		return infer_cast(unit<new_desc>{factor / v.value});
	}

	namespace literals
	{
		constexpr auto m = meter{ 1 };
		constexpr auto s = second{ 1 };
		constexpr auto mol = mols{ 1 };
		constexpr auto A = ampere{ 1 };
		constexpr auto K = kelvin{ 1 };
		constexpr auto cd = candela{ 1 };
		constexpr auto g = gram{ 1 };

		constexpr auto operator""_m(long double x) { return x * m; };
		constexpr auto operator""_s(long double x) { return x * s; };
		constexpr auto operator""_mol(long double x) { return x * mol; };
		constexpr auto operator""_A(long double x) { return x * A; };
		constexpr auto operator""_K(long double x) { return x * K; };
		constexpr auto operator""_cd(long double x) { return x * cd; };
		constexpr auto operator""_kg(long double x) { return x * g; };
	}


	template<details::unit_descriptor d, class CharT>
	struct std::formatter<si::unit<d>, CharT> : std::formatter<std::string, CharT>
	{
		template<class FormatContext>
		auto format(si::unit<d> u, FormatContext& fc) const
		{
			constexpr auto values = std::array<std::string, 7>{"m", "s", "mol", "A", "K", "cd", "g"};

			std::string output = std::format("{} ", u.value);
			for (int i = 0; i < d.exponent.size(); ++i)
				if (d.exponent[i] != 0) {
					if(d.exponent[i] == 1)
						output += std::format("{} ", values[i]);
					else
						output += std::format("{}^{} ", values[i], d.exponent[i]);
				}
				

			output.pop_back();

			return std::formatter<std::string, CharT>::format(output, fc);
		}
	};



}