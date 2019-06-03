#pragma once

#include <distant/config.hpp>
#include <distant/support/filesystem.hpp>
#include <distant/memory/address.hpp>

namespace distant::agents {

	class module
	{
	public:
		class id;

	public:


	private:
		distant::filesystem::path module_path_;
		distant::address base_address_;
		std::size_t module_size_;
	};

} // namespace distant::agents
