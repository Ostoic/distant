#include "stdafx.h"
#include "CppUnitTest.h"

#include <distant/scoped_handle.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace distant_unit_tests
{
	TEST_CLASS(scoped_handle_tests)
	{
	private:
		template <class Traits>
		void check_comparison_sanity(const distant::scoped_handle<Traits>& lhs, const distant::scoped_handle<Traits>& rhs)
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
			distant::scoped_handle<Traits> new_handle{ handle.native_handle() };
			check_comparison_sanity(handle, new_handle);

			// Self copy assignment
			handle = std::move(handle);
			check_comparison_sanity(handle, handle);

			// Copy assignment
			new_handle = std::move(handle);
			check_comparison_sanity(handle, new_handle);
		}

		void check_handle_sanity(distant::kernel_handle& lhs, distant::kernel_handle& rhs)
		{
			check_comparison_sanity(lhs, lhs);

			check_assignment_sanity(lhs);
			check_assignment_sanity(rhs);
		}

	public:
		TEST_METHOD(scoped_handle_test)
		{
			distant::kernel_handle lhs = nullptr, rhs = nullptr;
			distant::scoped_handle<distant::kernel_handle_traits> fully_qualified;

			check_handle_sanity(lhs, rhs);
			check_handle_sanity(fully_qualified, rhs);

			lhs.close();
		}
	};
}