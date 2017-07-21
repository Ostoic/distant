#pragma once

#include <Windows.h>

#include <distant\memory\address.h>
#include <distant\memory\protection.h>
#include <distant\process.h>

#define UNUSED_TAG(tag) static_cast <void>(tag)

namespace distant {
// Forward declare process
//template <typename>
//class process; 

namespace windows {

	// virtual_operator
	class virtual_allocator
	{
	public:
		using address_type = memory::address;
		using size_type = std::uintptr_t;

	public:
		//static address_type virtual_alloc(const process& p, flags, size_type size, protection:: prot)

	public:
		template <access_rights access_t, typename Flag_Type, typename Protection_Type>
		class alloc
		{
		private:
			const process<access_t>& m_process;
			address_type m_base;
			size_type m_size;

			Flag_Type m_flags;
			Protection_Type m_prot;

		public:
			alloc(const distant::process<access_t>& p, Flag_Type flags, size_type size, Protection_Type prot) :
				m_process(p),
				m_base(memory::null_address),
				m_size(size),
				m_flags(flags),
				m_prot(prot)
			{}

			alloc(const distant::process<access_t>& p, address_type base, Flag_Type flags, size_type size, Protection_Type prot) :
				m_process(p),
				m_base(base),
				m_size(size),
				m_flags(flags),
				m_prot(prot)
			{}

			inline address_type operator () () const
			{
				using under_flag = std::underlying_type_t<Flag_Type>;
				using under_prot = std::underlying_type_t<Protection_Type>;

				address_type result/* =
					reinterpret_cast<address_type>(
						VirtualAllocEx(m_process,
							reinterpret_cast<LPVOID>(m_base),
							m_size,
							static_cast<under_flag>(m_flags),
							static_cast<under_prot>(m_prot)
						)
					)*/;

				return result;
			}
		};

		template <typename Flag_Type>
		class dealloc
		{
		private:
			const process<access_t>& m_process;
			address_type m_base;
			size_type m_size;
			Flag_Type m_flags;

		public:
			dealloc(const process<access_t>& p, Flag_Type flags, size_type size) :
				m_process(p), 
				m_base(memory::null_address),
				m_size(size),
				m_flags(flags)
			{}

			dealloc(const process<access_t>& p, address_type base, Flag_Type flags, size_type size) :
				m_process(p),
				m_base(base),
				m_size(size),
				m_flags(flags)
			{}

			inline bool operator () () const
			{
				using under_flag = std::underlying_type_t<Flag_Type>;

				bool result/* =
					VirtualFreeEx(m_process,
						reinterpret_cast<LPVOID>(m_base),
						m_size,
						static_cast<under_flag>(m_flags)
					)*/;

				return result;
			}
		};
	};

} // end namespace windows
} // end namespace distant