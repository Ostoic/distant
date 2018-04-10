#include "stdafx.h"
#include "CppUnitTest.h"

#include <distant/process.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static constexpr bool is_32bit() noexcept
{ return sizeof(void*) == 4; }

static constexpr bool is_64bit() noexcept
{ return sizeof(void*) == 8; }

namespace distant_unit_tests
{
	TEST_CLASS(process_tests)
	{
	public:
		TEST_METHOD(process_semantics)
		{
			using namespace distant;

			auto current = current_process();
			const process<> system_process(0);

			// Test equality operators
			Assert::IsTrue(current == current);
			Assert::IsTrue(system_process != current);

			const auto test_preconditions = [](auto& p)
			{
				p.handle_count();
				p.is_32bit();
				p.is_64bit();
				p.is_being_debugged();
				p.file_path();
				p.name();
			};

			test_preconditions(current);
			// Test move semantics
			auto p1 = std::move(current);
			Assert::IsFalse(current.valid());
			Assert::IsTrue(p1.valid());

			Assert::ExpectException<std::exception>(
				[&current, test_preconditions]() {test_preconditions(current); }
			);

			// Test valid precondition assertions in process_base.
			test_preconditions(p1);
		}

		TEST_METHOD(process_basic)
		{
			using namespace distant;

			const auto current = current_process();

			Assert::IsTrue(current.id() > 0);
			Assert::IsTrue(current.valid());
			Assert::IsTrue(current.is_active());
			Assert::IsFalse(current.is_zombie());
			Assert::IsTrue(current.access_rights() == process_rights::all_access);
			Assert::IsTrue(reinterpret_cast<int>(current.handle().native_handle()) == -1);

			// Check if architecture detection works
			if constexpr (is_32bit())
				Assert::IsTrue(current.is_32bit());
			else if constexpr (is_64bit())
				Assert::IsTrue(current.is_64bit());
			else 
				Assert::Fail(L"distant::process was not able to detect the current architecture");
		}
	};
}