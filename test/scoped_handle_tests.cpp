#include "stdafx.h"
#include "CppUnitTest.h"

#include <distant/scoped_handle.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace distant_unit_tests
{
	TEST_CLASS(scoped_handle_tests)
	{
	private:
		void compare_handles(const distant::kernel_handle& lhs, const distant::kernel_handle& rhs)
		{
			const auto run = [](const auto& lhs, const auto& rhs)
			{
				Assert::IsTrue(lhs == rhs);
				Assert::IsTrue(!(lhs != rhs));
				Assert::IsFalse(lhs != rhs);
			};

			run(lhs, rhs);
			run(rhs, lhs);
		}

		template <class Traits>
		void check_assignment_sanity(distant::scoped_handle<Traits>& handle)
		{
			// Copy constructor
			distant::scoped_handle<Traits> new_handle{ handle.native_handle(), distant::access_rights::handle::close_protected};
			compare_handles(handle, new_handle);

			handle = std::move(new_handle);
			compare_handles(new_handle, nullptr);
		}

	public:
		TEST_METHOD(test_semantics)
		{
			distant::kernel_handle handle{ ::OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId()) };

			compare_handles(handle, handle);
			check_assignment_sanity(handle);
		}

		TEST_METHOD(test_operations)
		{
			HANDLE nh = ::OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
			distant::kernel_handle handle{ nh };

			Assert::IsTrue(nh == handle.native_handle());
			Assert::IsFalse(handle.close_protected());
			Assert::IsFalse(handle.closed());

			handle.close();
			Assert::IsTrue(handle.close_protected());
			Assert::IsTrue(handle.closed());
		}
	};
}