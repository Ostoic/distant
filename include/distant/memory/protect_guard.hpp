#pragma once

#include <distant/agents/process.hpp>
#include <distant/memory/page.hpp>
#include <distant/memory/virtual_memory.hpp>

#include <optional>

namespace distant
{
	namespace memory
	{
		// Todo: multiple pages, etc
		template <typename AddressT>
		class protect_guard
		{
		public:
			explicit protect_guard(const process<vm_op>& process, address<AddressT> address, const page_protection protection, const std::size_t size)
				: process_(&process)
				, page_(address)
				, old_protection_(virtual_protect<AddressT>(process, address, protection, size))
			{}

			~protect_guard() noexcept
			{
				if (process_ != nullptr)
					virtual_protect_noexcept<AddressT>(*this->process_, this->page_.base(), this->old_protection_, this->page_.size());
			}

			protect_guard(protect_guard&& other) noexcept
				: process_(other.process_)
				, page_(other.page_)
				, old_protection_(other.old_protection_)
			{ other.process_ = nullptr; }

			protect_guard& operator=(protect_guard&& other) noexcept
			{
				this->process_ = other.process_;
				other.process_ = nullptr;

				this->page_ = other.page_;
				this->old_protection_ = other.old_protection_;
				return *this;
			};

			protect_guard(const protect_guard&) = delete;
			protect_guard& operator=(const protect_guard&) = delete;

		private:
			const process<vm_op>* process_;
			const page<AddressT> page_;
			const page_protection old_protection_;

		}; // class protect_guard

		template <page_protection Protection, typename AddressT>
		[[nodiscard]] protect_guard<AddressT>
			make_protect_guard(const process<vm_op>& process, const address<AddressT> address, const std::size_t size)
		{
			return protect_guard<AddressT>{process, address, Protection, size};
		}

		template <page_protection Protection>
		[[nodiscard]] protect_guard<dword>
			make_protect_guard(const process<vm_op>& process, const address<dword> address, const std::size_t size)
		{
			return protect_guard<dword>{process, address, Protection, size};
		}

		/// @brief Attempts to protect a region of memory in the given process, but will catch any exceptions thrown by protect_guard.
		template <page_protection Protection, typename AddressT>
		[[nodiscard]] std::optional<protect_guard<dword>>
			try_protect_guard(const process<vm_op>& process, const address<AddressT> address, const std::size_t size = 1)
		{
			try
			{
				protect_guard<AddressT> protect{process, address, Protection, size};
				return std::move(protect);
			}
			catch (winapi_error&) {}
			catch (std::exception&) { throw; }
			return {};
		}

		template <page_protection Protection>
		[[nodiscard]] std::optional<protect_guard<dword>>
			try_protect_guard(const process<vm_op>& process, const address<dword> address, const std::size_t size = 1)
		{
			try
			{
				protect_guard<dword> protect{process, address, Protection, size};
				return std::move(protect);
			}
			catch (winapi_error&) {}
			catch (std::exception&) { throw; }
			return {};
		}
	} // namespace memory

	using memory::make_protect_guard;
	using memory::try_protect_guard;

} // namespace distant
