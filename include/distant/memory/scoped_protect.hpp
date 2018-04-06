#pragma once

#include <distant/kernel/process.hpp>
#include <distant/memory/page.hpp>
#include <distant/memory/virtual_memory.hpp>

#include <optional>

namespace distant
{
	namespace memory
	{
		// Todo: multiple pages, etc
		template <typename AddressT>
		class scoped_protect
		{
		public:
			explicit scoped_protect(const process<vm_op>& process, address<AddressT> address, const page_protection protection, const std::size_t size)
				: process_(&process)
				, page_(address)
				, old_protection_(virtual_protect<AddressT>(process, address, protection, size))
			{}

			~scoped_protect() noexcept
			{
				if (process_ != nullptr)
					virtual_protect_noexcept<AddressT>(*this->process_, this->page_.base(), this->old_protection_, this->page_.size());
			}

			scoped_protect(scoped_protect&& other) noexcept
				: process_(other.process_)
				, page_(other.page_)
				, old_protection_(other.old_protection_)
			{ other.process_ = nullptr; }

			scoped_protect& operator=(scoped_protect&& other) noexcept
			{
				this->process_ = other.process_;
				other.process_ = nullptr;

				this->page_ = other.page_;
				this->old_protection_ = other.old_protection_;
				return *this;
			};

			scoped_protect(const scoped_protect&) = delete;
			scoped_protect& operator=(const scoped_protect&) = delete;

		private:
			const process<vm_op>* process_;
			const page<AddressT> page_;
			const page_protection old_protection_;

		}; // class scoped_protect

		template <page_protection Protection, typename AddressT>
		[[nodiscard]]
		scoped_protect<AddressT>
			make_scoped_protect(const process<vm_op>& process, const address<AddressT> address, const std::size_t size)
		{
			return scoped_protect<AddressT>{process, address, Protection, size};
		}

		template <page_protection Protection>
		[[nodiscard]]
		scoped_protect<dword>
			make_scoped_protect(const process<vm_op>& process, const address<dword> address, const std::size_t size)
		{
			return scoped_protect<dword>{process, address, Protection, size};
		}

		/// @brief Attempts to protect a region of memory in the given process, but will catch any exceptions thrown by scoped_protect.
		template <page_protection Protection, typename AddressT>
		[[nodiscard]]
		std::optional<scoped_protect<dword>>
			try_scoped_protect(const process<vm_op>& process, const address<AddressT> address, const std::size_t size = 1)
		{
			try
			{
				scoped_protect<AddressT> protect{process, address, Protection, size};
				return std::move(protect);
			}
			catch (std::system_error&) {}
			catch (std::exception&) { throw; }
			return {};
		}

		template <page_protection Protection>
		[[nodiscard]]
		std::optional<scoped_protect<dword>>
			try_scoped_protect(const process<vm_op>& process, const address<dword> address, const std::size_t size = 1)
		{
			try
			{
				scoped_protect<dword> protect{process, address, Protection, size};
				return std::move(protect);
			}
			catch (std::system_error&) {}
			catch (std::exception&) { throw; }
			return {};
		}
	} // namespace memory
	
	using memory::make_scoped_protect;
	using memory::try_scoped_protect;

} // namespace distant
