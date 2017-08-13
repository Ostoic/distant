#pragma once

/*!
@copyright 2017 Shaun Ostoic
Distributed under the Apache Software License, Version 2.0.
(See accompanying file LICENSE.md or copy at http://www.apache.org/licenses/LICENSE-2.0)
*/

#include <distant\windows\handle.hpp>
#include <distant\windows\detail\handle_service.hpp>

#include <distant\windows\system\detail\tool_help.hpp>

#include <distant\iterators\snapshot_iterator.hpp>

namespace distant::windows::system {

	template <typename ObjectType>
	class snapshot : 
		public error::gle,
		public windows::detail::handle_service<snapshot<ObjectType>>
	{
	public:
		using object_type = ObjectType;
		using handle_type = windows::handle<snapshot>;
		
		using iterator = iterators::snapshot_iterator<ObjectType>;
		using const_iterator = iterators::snapshot_iterator<ObjectType>;

	public:
		iterator begin();
		iterator end();

		const_iterator cbegin() const;
		const_iterator cend() const;

	public:
		snapshot();
		~snapshot() = default;
	protected:
		handle_type m_handle;
		friend windows::detail::handle_service<snapshot<ObjectType>>;
	};

} // end namespace distant::windows::system

#include <distant\windows\system\detail\snapshot.inl>