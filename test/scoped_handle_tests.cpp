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
			const auto assert_valid_handle = [](const auto& handle)
			{
				Assert::IsTrue(handle.valid());
				Assert::IsTrue(!handle.is_closed());
			};

			const auto assert_invalid_handle = [](const auto& handle)
			{
				Assert::IsTrue(!handle.valid());
				Assert::IsTrue(handle.is_closed());
			};

			// Copy constructor
			distant::kernel_handle new_handle = std::move(handle);
			assert_valid_handle(new_handle);
			assert_invalid_handle(handle);

			handle = std::move(new_handle);
			assert_valid_handle(handle);
			assert_invalid_handle(new_handle);

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
			Assert::IsFalse(handle.is_closed());

			handle.close();
			Assert::IsTrue(handle.is_closed());
		}
	};
}