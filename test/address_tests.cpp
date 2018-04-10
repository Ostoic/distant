#include "stdafx.h"
#include "CppUnitTest.h"

#include <distant/memory/address.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace distant_unit_tests
{
	TEST_CLASS(address_tests)
	{
	private:
		template <typename T>
		void test_address(T address)
		{
			Assert::IsTrue(address == address);
			Assert::IsTrue(!(address != address));
			Assert::IsTrue(address <= address);
			Assert::IsTrue(address >= address);
			Assert::IsTrue(!(address < address));
			Assert::IsTrue(!(address > address));
		}
	public:
		TEST_METHOD(address_test)
		{
			using namespace distant;
			constexpr address x1 = 2;
			const address x2 = 2;
			address x3 = 2;

			test_address(x1);
			test_address(x2);
			test_address(x3);
		}
	};
}