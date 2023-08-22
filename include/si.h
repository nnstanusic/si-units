#pragma once
#include <array>
#include <concepts>

namespace si
{

	namespace details
	{
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

	template <class T, details::unit_descriptor descriptor>
	struct unit
	{
		using base_t = unit<T, descriptor>;
		using type = T;
		T value;

		static consteval auto Descriptor()
		{
			return descriptor;
		}

		template <details::unit_descriptor d_other>
			requires same_exponent_c<descriptor, d_other>
		[[nodiscard]] constexpr auto operator=(this auto& v, unit<T, d_other> other)
		{
			constexpr auto factor = details::conversion_factor(d_other, descriptor);
			v.value = factor * other.value;
			return v;
		}

		template <details::unit_descriptor d_other>
			requires same_exponent_c<descriptor, d_other>
		[[nodiscard]] constexpr auto operator+(this auto v, unit<T, d_other> other)
		{
			constexpr auto factor = details::conversion_factor(d_other, descriptor);
			v.value += factor * other.value;
			return v;
		}

		[[nodiscard]] explicit constexpr operator T() const {
			return this->value;
		};

		template <details::unit_descriptor d_other>
			requires same_exponent_c<descriptor, d_other>
		[[nodiscard]] constexpr auto operator-(this auto v, unit<T, d_other> other)
		{
			constexpr auto factor = details::conversion_factor(d_other, descriptor);
			v.value -= factor * other.value;
			return v;
		}

		template <details::unit_descriptor d_other>
		[[nodiscard]] constexpr auto operator*(this auto v, unit<T, d_other> other);

		[[nodiscard]] constexpr auto operator*(this auto v, T factor)
		{
			v.value *= factor;
			return v;
		}

		[[nodiscard]] friend constexpr auto operator*(T factor, unit<T, descriptor> v)
		{
			return v * factor;
		}

		template <details::unit_descriptor d_other>
		[[nodiscard]] constexpr auto operator/(this auto v, unit<T, d_other> other);

		[[nodiscard]] constexpr auto operator/(this auto v, T factor)
		{
			v.value /= factor;
			return v;
		}
	};



	template<class T, details::unit_descriptor d>
	constexpr auto kilo(unit<T, d> u) {
		constexpr auto new_descriptor = details::unit_descriptor{ d.exponent, d.factor * 1000.0f };
		return infer_cast(unit<T, new_descriptor>{u.value / 1000.0f});
	}


	template<class T, details::unit_descriptor d>
	constexpr auto milli(unit<T, d> u) {
		constexpr auto new_descriptor = details::unit_descriptor{ d.exponent, d.factor / 1000.0f };
		return infer_cast(unit<T, new_descriptor>{u.value * 1000.0f});
	}

	//Base units
	template<class T> struct meter : unit<T, details::meter_desc> {};
	template<class T> struct second : unit<T, details::second_desc> {};
	template<class T> struct mols : unit<T, details::mol_desc> {};
	template<class T> struct ampere : unit<T, details::ampere_desc> {};
	template<class T> struct kelvin : unit<T, details::kelvin_desc> {};
	template<class T> struct candela : unit<T, details::candela_desc> {};
	template<class T> struct gram : unit<T, details::gram_desc> {};

	//Kilo units
	template<class T> struct kilo_meter : unit<T, details::meter_desc * 1e3f> {};
	template<class T> struct kilo_second : unit<T, details::second_desc * 1e3f> {};
	template<class T> struct kilo_mols : unit<T, details::mol_desc * 1e3f> {};
	template<class T> struct kilo_ampere : unit<T, details::ampere_desc * 1e3f> {};
	template<class T> struct kilo_kelvin : unit<T, details::kelvin_desc * 1e3f> {};
	template<class T> struct kilo_candela : unit<T, details::candela_desc * 1e3f> {};
	template<class T> struct kilo_gram : unit<T, details::gram_desc * 1e3f> {};

	//mega
	template<class T> struct mega_meter : unit<T, details::meter_desc * 1e6f> {};
	template<class T> struct mega_second : unit<T, details::second_desc * 1e6f> {};
	template<class T> struct mega_mols : unit<T, details::mol_desc * 1e6f> {};
	template<class T> struct mega_ampere : unit<T, details::ampere_desc * 1e6f> {};
	template<class T> struct mega_kelvin : unit<T, details::kelvin_desc * 1e6f> {};
	template<class T> struct mega_candela : unit<T, details::candela_desc * 1e6f> {};
	template<class T> struct ton : unit<T, details::gram_desc * 1e6f> {};

	//Time
	template<class T> struct minute : unit<T, details::second_desc * 60> {};
	template<class T> struct hour : unit<T, details::second_desc * 60 * 60> {};
	template<class T> struct day : unit<T, details::second_desc * 60 * 60 * 24> {};

	//compount 
	template<class T> struct meters_per_second : unit<T, details::velocity_desc> {};
	template<class T> struct meters_per_second_squared : unit<T, details::acceleration_desc> {};

	template<class T> struct SquareMeters : unit<T, details::squared_meter_desc> {};
	template<class T> struct CubicMeters : unit<T, details::cubic_meter_desc> {};

	template<class T> struct hertz : unit<T, details::hertz_desc> {};
	template<class T> struct newton : unit<T, details::newton_desc> {};
	template<class T> struct pascal : unit<T, details::pascal_desc> {};
	template<class T> struct joule : unit<T, details::joule_desc> {};
	template<class T> struct watt : unit<T, details::watt_desc> {};
	template<class T> struct coulomb : unit<T, details::coulomb_desc> {};
	template<class T> struct volt : unit<T, details::volt_desc> {};
	template<class T> struct farad : unit<T, details::farad_desc> {};
	template<class T> struct ohm : unit<T, details::ohm_desc> {};
	template<class T> struct siemens : unit<T, details::siemens_desc> {};
	template<class T> struct weber : unit<T, details::weber_desc> {};
	template<class T> struct tesla : unit<T, details::tesla_desc> {};
	template<class T> struct henry : unit<T, details::henry_desc> {};
	template<class T> struct lux : unit<T, details::lux_desc> {};
	template<class T> struct katal : unit<T, details::katal_desc> {};

#pragma region Deduction Guidelines
	template<class T> meter(T val) -> meter<T>;
	template<class T> second(T val) -> second<T>;
	template<class T> mols(T val) -> mols<T>;
	template<class T> ampere(T val) -> ampere<T>;
	template<class T> kelvin(T val) -> kelvin<T>;
	template<class T> candela(T val) -> candela<T>;
	template<class T> gram(T val) -> gram<T>;
	template<class T> kilo_meter(T val) -> kilo_meter<T>;
	template<class T> kilo_second(T val) -> kilo_second<T>;
	template<class T> kilo_mols(T val) -> kilo_mols<T>;
	template<class T> kilo_ampere(T val) -> kilo_ampere<T>;
	template<class T> kilo_kelvin(T val) -> kilo_kelvin<T>;
	template<class T> kilo_candela(T val) -> kilo_candela<T>;
	template<class T> kilo_gram(T val) -> kilo_gram<T>;
	template<class T> mega_meter(T val) -> mega_meter<T>;
	template<class T> mega_second(T val) -> mega_second<T>;
	template<class T> mega_mols(T val) -> mega_mols<T>;
	template<class T> mega_ampere(T val) -> mega_ampere<T>;
	template<class T> mega_kelvin(T val) -> mega_kelvin<T>;
	template<class T> mega_candela(T val) -> mega_candela<T>;
	template<class T> ton(T val) -> ton<T>;
	template<class T> minute(T val) -> minute<T>;
	template<class T> hour(T val) -> hour<T>;
	template<class T> day(T val) -> day<T>;
	template<class T> meters_per_second(T val) -> meters_per_second<T>;
	template<class T> meters_per_second_squared(T val) -> meters_per_second_squared<T>;
	template<class T> SquareMeters(T val) -> SquareMeters<T>;
	template<class T> CubicMeters(T val) -> CubicMeters<T>;
	template<class T> hertz(T val) -> hertz<T>;
	template<class T> newton(T val) -> newton<T>;
	template<class T> pascal(T val) -> pascal<T>;
	template<class T> joule(T val) -> joule<T>;
	template<class T> watt(T val) -> watt<T>;
	template<class T> coulomb(T val) -> coulomb<T>;
	template<class T> volt(T val) -> volt<T>;
	template<class T> farad(T val) -> farad<T>;
	template<class T> ohm(T val) -> ohm<T>;
	template<class T> siemens(T val) -> siemens<T>;
	template<class T> weber(T val) -> weber<T>;
	template<class T> tesla(T val) -> tesla<T>;
	template<class T> henry(T val) -> henry<T>;
	template<class T> lux(T val) -> lux<T>;
	template<class T> katal(T val) -> katal<T>;
#pragma endregion Deduction Guidelines

	namespace details
	{

		template <class T, unit_descriptor d> struct inferer { using type = unit<T, d>; };


		template <class T> struct inferer<T, meter<float>::Descriptor()> { using type = meter<T>; };
		template <class T> struct inferer<T, second<float>::Descriptor()> { using type = second<T>; };
		template <class T> struct inferer<T, mols<float>::Descriptor()> { using type = mols<T>; };
		template <class T> struct inferer<T, ampere<float>::Descriptor()> { using type = ampere<T>; };
		template <class T> struct inferer<T, kelvin<float>::Descriptor()> { using type = kelvin<T>; };
		template <class T> struct inferer<T, candela<float>::Descriptor()> { using type = candela<T>; };
		template <class T> struct inferer<T, gram<float>::Descriptor()> { using type = gram<T>; };

		template <class T> struct inferer<T, kilo_meter<float>::Descriptor()> { using type = kilo_meter<T>; };
		template <class T> struct inferer<T, kilo_second<float>::Descriptor()> { using type = kilo_second<T>; };
		template <class T> struct inferer<T, kilo_mols<float>::Descriptor()> { using type = kilo_mols<T>; };
		template <class T> struct inferer<T, kilo_ampere<float>::Descriptor()> { using type = kilo_ampere<T>; };
		template <class T> struct inferer<T, kilo_kelvin<float>::Descriptor()> { using type = kilo_kelvin<T>; };
		template <class T> struct inferer<T, kilo_candela<float>::Descriptor()> { using type = kilo_candela<T>; };
		template <class T> struct inferer<T, kilo_gram<float>::Descriptor()> { using type = kilo_gram<T>; };

		template <class T> struct inferer<T, mega_meter<float>::Descriptor()> { using type = mega_meter<T>; };
		template <class T> struct inferer<T, mega_second<float>::Descriptor()> { using type = mega_second<T>; };
		template <class T> struct inferer<T, mega_mols<float>::Descriptor()> { using type = mega_mols<T>; };
		template <class T> struct inferer<T, mega_ampere<float>::Descriptor()> { using type = mega_ampere<T>; };
		template <class T> struct inferer<T, mega_kelvin<float>::Descriptor()> { using type = mega_kelvin<T>; };
		template <class T> struct inferer<T, mega_candela<float>::Descriptor()> { using type = mega_candela<T>; };
		template <class T> struct inferer<T, ton<float>::Descriptor()> { using type = ton<T>; };

		template <class T> struct inferer < T, unit_descriptor{ .exponent = {0, 0, 0, 0, 0, 0, 0}, .factor = 1.0f } > { using type = float; };
		template <class T> struct inferer<T, meters_per_second<float>::Descriptor()> { using type = meters_per_second<T>; };
		template <class T> struct inferer<T, meters_per_second_squared<float>::Descriptor()> { using type = meters_per_second_squared<T>; };
		template <class T> struct inferer<T, SquareMeters<float>::Descriptor()> { using type = SquareMeters<T>; };
		template <class T> struct inferer<T, CubicMeters<float>::Descriptor()> { using type = CubicMeters<T>; };
		template <class T> struct inferer<T, hertz<float>::Descriptor()> { using type = hertz<T>; };
		template <class T> struct inferer<T, newton<float>::Descriptor()> { using type = newton<T>; };
		template <class T> struct inferer<T, pascal<float>::Descriptor()> { using type = pascal<T>; };
		template <class T> struct inferer<T, joule<float>::Descriptor()> { using type = joule<T>; };
		template <class T> struct inferer<T, watt<float>::Descriptor()> { using type = watt<T>; };
		template <class T> struct inferer<T, coulomb<float>::Descriptor()> { using type = coulomb<T>; };
		template <class T> struct inferer<T, volt<float>::Descriptor()> { using type = volt<T>; };
		template <class T> struct inferer<T, farad<float>::Descriptor()> { using type = farad<T>; };
		template <class T> struct inferer<T, ohm<float>::Descriptor()> { using type = ohm<T>; };
		template <class T> struct inferer<T, siemens<float>::Descriptor()> { using type = siemens<T>; };
		template <class T> struct inferer<T, weber<float>::Descriptor()> { using type = weber<T>; };
		template <class T> struct inferer<T, tesla<float>::Descriptor()> { using type = tesla<T>; };
		template <class T> struct inferer<T, henry<float>::Descriptor()> { using type = henry<T>; };
		template <class T> struct inferer<T, lux<float>::Descriptor()> { using type = lux<T>; };
		template <class T> struct inferer<T, katal<float>::Descriptor()> { using type = katal<T>; };

		template <class T, unit_descriptor d>
		using inferer_t = typename inferer<T, d>::type;
	}

	template <class T, details::unit_descriptor d>
	[[nodiscard]] constexpr auto infer_cast(unit<T, d> value)
	{
		return typename details::inferer<T, d>::type{ value.value };
	};

	template <class T, details::unit_descriptor descriptor>
	template <details::unit_descriptor d_other>
	constexpr auto unit<T, descriptor>::operator*(this auto v, unit<T, d_other> other)
	{
		constexpr auto new_exponent = details::add_exponents(descriptor.exponent, d_other.exponent);
		constexpr auto new_desc = details::unit_descriptor{ new_exponent, descriptor.factor * d_other.factor };
		return infer_cast(unit<T, new_desc>{v.value* other.value});
	}

	template <class T, details::unit_descriptor descriptor>
	template <details::unit_descriptor d_other>
	constexpr auto unit<T, descriptor>::operator/(this auto v, unit<T, d_other> other)
	{
		constexpr auto new_exponent = details::subtract_exponents(descriptor.exponent, d_other.exponent);
		constexpr auto new_desc = details::unit_descriptor{ new_exponent, descriptor.factor / d_other.factor };
		return infer_cast(unit<T, new_desc>{v.value / other.value});
	}

	template <class T, details::unit_descriptor descriptor>
	[[nodiscard]] constexpr auto operator/(T factor, unit<T, descriptor> v)
	{
		constexpr auto new_exponent = details::invert_exponents(descriptor.exponent);
		constexpr auto new_desc = details::unit_descriptor{ new_exponent, descriptor.factor };
		return infer_cast(unit<T, new_desc>{factor / v.value});
	}

	namespace literals
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


}