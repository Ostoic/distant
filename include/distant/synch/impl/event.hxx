#pragma once
#include "../event.hpp"

#include <boost/winapi/event.hpp>

#include <distant/error/winapi_error.hpp>

#include <random>

namespace distant::synch
{
	namespace detail
	{
		std::wstring generate_random_name()
		{
			std::default_random_engine eng{};
			auto length_gen = std::uniform_int_distribution{10, 100};
			const auto length = length_gen(eng);

			auto char_gen = std::uniform_int_distribution{65, 90};

			std::wstring name = {};
			for (int i = 0; i < length; i++)
				name += static_cast<wchar_t>(char_gen(eng));

			return name;
		}
	}
// class mutex
	inline event::event()
		: event(detail::generate_random_name())
	{
	}

	inline event::event(const std::wstring& name)
		: handle_{boost::winapi::create_event(
			nullptr,
			true,
			false,
			name.data()
		)}
		, name_{name}
	{
		if (this->handle_ == nullptr)
			throw winapi_error("[synch::mutex::{ctor}] Anonymous CreateMutex failed"); 
	}


}