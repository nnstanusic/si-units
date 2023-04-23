#pragma once
#include "si.h"
#include <format>

namespace si{

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

	
	template<class T, class CharT>
		requires std::derived_from<T, typename T::base_t>
	struct std::formatter<T, CharT> : std::formatter<unit<T::Descriptor()>, CharT> {};

	/*template<class CharT >
	typename derived_formater<si::meter, CharT>;*/

}