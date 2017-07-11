#pragma once

#include <Windows.h>

namespace distant {

	using handle_type = HANDLE;

	class invalid_handle_t
	{
	public:
		using handle_type = distant::handle_type;

		constexpr operator handle_type () const
		{ return m_handle; }

		constexpr invalid_handle_t() : m_handle(INVALID_HANDLE_VALUE) {}

	private:
		const handle_type m_handle;

	public:
		friend bool operator ==(const invalid_handle_t&, const HANDLE&);
		friend bool operator ==(const handle_type&, const invalid_handle_t&);

		friend bool operator !=(const invalid_handle_t&, const handle_type&);
		friend bool operator !=(const handle_type&, const invalid_handle_t&);
	};

	bool operator ==(const invalid_handle_t& invalid_handle, const invalid_handle_t::handle_type& handle)
	{
		return handle == invalid_handle.m_handle;
	}

	bool operator ==(const invalid_handle_t::handle_type& handle, const invalid_handle_t& invalid_handle)
	{
		return operator ==(invalid_handle, handle);
	}

	bool operator !=(const invalid_handle_t& invalid_handle, const invalid_handle_t::handle_type& handle)
	{
		return !operator ==(invalid_handle, handle);
	}
	bool operator !=(const invalid_handle_t::handle_type& handle, const invalid_handle_t& invalid_handle)
	{
		return !operator ==(invalid_handle, handle);
	}

	static constexpr invalid_handle_t invalid_handle;

} // end namespace distant