#pragma once
#include "si.h"
#include <format>

namespace si {


	template<class T, details::unit_descriptor d, class CharT>
	struct std::formatter<si::unit<T, d>, CharT> :
		std::formatter<T, CharT>
	{

		constexpr auto parse(auto& context)
		{
			return std::formatter<T, CharT>::parse(context);
		}


		template<class FormatContext>
		auto format(si::unit<T, d> u, FormatContext& fc) const
		{
			constexpr auto values = std::array<std::string, 7>{"m", "s", "mol", "A", "K", "cd", "g"};

			auto x = std::formatter<T, CharT>::format(u.value, fc);

			std::string output = " ";
			for (int i = 0; i < d.exponent.size(); ++i)
				if (d.exponent[i] != 0) {					
					if (d.exponent[i] == 1)
						output += std::format("{} ", values[i]);
					else
						output += std::format("{}^{} ", values[i], d.exponent[i]);
				}
			output.pop_back();

			std::copy(output.begin(), output.end(), x);

			return x;
		}
	};


	template<class T, class CharT>
		requires std::derived_from<T, typename T::base_t>
	struct std::formatter<T, CharT> :
		std::formatter<unit<typename T::type, T::Descriptor()>, CharT>
	{};

	/*template<class CharT >
	typename derived_formater<si::meter, CharT>;*/

}