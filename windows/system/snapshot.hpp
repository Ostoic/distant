#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\error\gle.hpp>
#include <distant\windows\handle.hpp>
#include <distant\windows\system\detail\snapshot.hpp>

namespace distant {
namespace windows {
namespace system  {

	// Make this CRTP (Or just tagged?) where any derived class is at least a windows::kernel::object
	// The snapshot ctor should tag dispatch based on the templated type
	// kernel::process should dispatch to CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// Similary, other objects supported by system::snapshot should dispatch to their respective flag.
	template <class ObjectType>
	class snapshot : private error::gle
	{
	public:
		using object_type = ObjectType;
		using handle_type = typename object_type::handle_type;
		
		using iterator = int;	
		using const_iterator = int;

	public:
		using gle::get_last_error;

		iterator begin();
		iterator end();

		const_iterator cbegin();
		const_iterator cend();

	public:
		snapshot() : 
			m_handle(system::detail::get_snapshot(object_type()))
		{ this->update_gle(); }

	protected:
		using gle::update_gle;

		handle_type m_handle;
	};

} // end namespace system
} // end namespace windows
} // end namespace distant