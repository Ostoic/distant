#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\handle.hpp>
#include <distant\detail\handle_service.hpp>

#include <distant\system\snapshot_iterator.hpp>
#include <distant\kernel\object.hpp>

namespace distant::system {

	// system::snapshot models the ForwardRange concept
	template <typename ObjectType>
	class snapshot : public kernel::object
	{
	public:
		using object_type = ObjectType;
		using handle_type = handle<snapshot>;
		
		using iterator = iterators::snapshot_iterator<ObjectType>;
		using const_iterator = iterators::snapshot_iterator<ObjectType>;

	public:
		iterator begin();
		iterator end();

		iterator begin() const;
		iterator end() const;

	public:
		explicit snapshot();

		// Not copy constructible
		snapshot(const snapshot&) = delete;

		// Not copy assignable
		snapshot& operator=(const snapshot&) = delete;
	};

} // end namespace distant::system

#include <distant\system\detail\snapshot.hxx>
