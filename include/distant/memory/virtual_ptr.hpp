#pragma once

#include <boost/iterator/iterator_adaptor.hpp>

#include <distant/kernel/fwd.hpp>
#include <distant/memory/address.hpp>
#include <distant/memory/virtual_reference.hpp>

namespace distant::memory
{
	template <typename Element, process_rights Access, typename AddressT = dword>
	class virtual_ptr :
		boost::iterator_adaptor<
			virtual_ptr<Element, Access, AddressT>,
			Element*,
			Element,
			std::random_access_iterator_tag,
			virtual_reference<Element, 
		>
	{
	public:
		virtual_ptr();

		explicit virtual_ptr(const kernel::process<Access>& process, address<AddressT> address = nullptr);

		template <typename OtherT, process_rights OtherAccess, typename OtherAddressT>
		virtual_ptr(const virtual_ptr<OtherT, OtherAccess, OtherAddressT>& pointer);

		//operator=

	private:
		const kernel::process<Access>* process_;
		address<AddressT> address_;
	};

} // namespace distant::memory

// Implementation:
#include "impl/virtual_ptr.hxx"
