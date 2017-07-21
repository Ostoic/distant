#pragma once

#include <distant\windows\error\gle.h>
#include <distant\windows\virtual_allocator.h>

#include <distant\memory\address.h>
#include <distant\memory\vm.h>
#include <distant\memory\protection.h>
#include <distant\process.h>

#include <thread>

namespace distant {
namespace memory  {

	// Represents a memory allocation in virtual memory
	// XXX Allocation is convertible to memory::value, memory::pointer
	// Note that vm allocation uses page granularity. If a 2 byte allocation
	// is made straddling two pages, both pages will be included in that allocation.
	class vm::operation : public  windows::gle,
						  private windows::virtual_allocator
	{
	public:
		enum class action
		{
			// Allocation actions
			reset	= MEM_RESET,
			commit	= MEM_COMMIT,
			reserve = MEM_RESERVE,
			reset_undo = MEM_RESET_UNDO,

			// Deallocation actions
			decommit = MEM_DECOMMIT,
			release  = MEM_RELEASE,
		};

	public: // Type information
		using windows::virtual_allocator::alloc;
		using windows::virtual_allocator::dealloc;

		//using protection_type = memory::protection;
		using address_type	  = memory::address;
		using flag_type		  = operation::action;
		using size_type		  = std::uintptr_t;

	public: // Functions
		operation(const process& p, flag_type flags) :
			m_process(p),
			m_base(memory::null_address),
			m_size(0),
			m_flags(flags) // We are commiting new memory
		{}

		operation(const process& p, address_type base, flag_type flags, size_type size) :
			m_process(p),
			m_base(base),
			m_size(size),
			m_flags(flags)
		{}

		operator address_type() const { return m_base; }

	protected:
		template <typename Operation>
		void perform_allocation(const Operation& op)
		{
			// We need to have at least vm_operation access to perform
			// a distant virtual allocation (create or destroy)
			// If we want portability later, use tag dispatch for 
			// distant::process with windows::process::access_rights
			if (m_process.check_permission(process::access_rights::vm_operation))
			{
				// Perform virtual operation
				auto result = op();

				// Update last error code
				this->update_gle();
			}
			else
			{	// We don't have permission to allocate memory
				// Return an empty allocation at base
			}
		}
		
		void zero()
		{
			m_size = 0;
		}

		bool check_allocation() const
		{
			//MEMORY_BASIC_INFORMATION info;

			//VirtualQueryEx(WoW::handle, (LPVOID)address, &info, sizeof(info));
			//return info.State == MEM_COMMIT;
		}

	protected:
		flag_type m_flags;

		address_type m_base;
		size_type m_size;

		const process& m_process;
	};

	// Set up bitwise operators for use with the allocation::action enum
	DEFINE_ENUM_FLAG_OPERATORS(vm::operation::action);

	class vm::deletion : public vm::operation
	{
	public: // Functions
		deletion(const process& p, address_type base, flag_type flags, size_type size) :
			operation(p, base, flags, size),
			m_op(p, base, flags, size)
		{
			/*if (!(flags == action::decommit ||
				  flags == action::release))
				  throw std::logic_error("Invalid allocation flags for vm deletion");*/

			// Destroy memory allocation with the given parameters
			this->perform_allocation<decltype(m_op)>(m_op);
		}

		deletion(const process& p, flag_type flags, size_type size) :
			operation(p, memory::null_address, flags, size),
			m_op(p, memory::null_address, flags, size)
		{
			/*if (!(flags == action::decommit ||
				  flags == action::release))
				  throw std::logic_error("Invalid allocation flags for vm deletion");
*/
			// Destroy memory allocation with the given parameters
			this->perform_allocation<decltype(m_op)>(m_op);
		}

	protected:
		dealloc<flag_type> m_op;
	};

	class vm::allocation : public vm::operation
	{
	public: // Type info
		using protection_type = protection;

	public: // Functions
		allocation(const process& p, address_type base, flag_type flags, size_type size, protection_type prot) :
			operation(p, base, flags, size),
			m_prot(prot),
			m_op(p, base, flags, size, prot)
		{
			/*if (flags == action::decommit ||
				flags == action::release)
				throw std::logic_error("Invalid allocation flags for vm creation");*/

			// Create memory allocation with the given parameters
			this->perform_allocation<decltype(m_op)>(m_op);
		}

		allocation(const process& p, flag_type flags, size_type size, protection_type prot) :
			operation(p, memory::null_address, flags, size),
			m_prot(prot),
			m_op(p, memory::null_address, flags, size, prot)
		{
			/*if (flags == action::decommit ||
				flags == action::release)
				throw std::logic_error("Invalid allocation flags for vm creation");*/

			// Create memory allocation with the given parameters
			this->perform_allocation<decltype(m_op)>(m_op);
		}

	protected:
		protection_type m_prot;
		alloc<flag_type, protection_type> m_op;
	};


	template <typename... Ts>
	vm::operation operate(Ts... args)
	{
		return vm::operation(args...);
	}

	// Pass everything to the allocation ctor
	template <typename... Ts>
	vm::allocation allocate(Ts... args)
	{
		return vm::allocation(args...);
	}

	// Pass everything to the allocation ctor
	template <typename... Ts>
	vm::deletion deallocate(Ts... args)
	{
		return vm::deletion(args...);
	}

} // end namespace memory
} // end namespace distant
