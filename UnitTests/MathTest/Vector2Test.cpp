#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Coo::Math;

namespace MathTest
{
	TEST_CLASS(Vector2Test) 
	{
	public:
		TEST_METHOD(TestConstructor) 
		{
			Vector2 v0;
			Assert::AreEqual(0.0f, v0.x);
			Assert::AreEqual(0.0f, v0.y);

			Vector2 v1{ 1.0f };
			Assert::AreEqual(1.0f, v1.x);
			Assert::AreEqual(1.0f, v1.y);
			
			Vector2 v2{ 2.0f, 3.0f };
			Assert::AreEqual(2.0f, v2.x);
			Assert::AreEqual(3.0f, v2.y);
		}

		TEST_METHOD(TestNegative) 
		{
			Vector2 v0{ 1.0f, 2.0f };
			v0 = -v0;

			Assert::AreEqual(-1.0f, v0.x);
			Assert::AreEqual(-2.0f, v0.y);
		}

		TEST_METHOD(TestAddition) 
		{
			Vector2 v0{ 1.0f };
		}
	};
} // namespace MathTest