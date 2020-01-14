#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Coo::Math;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		TEST_METHOD(TestConstructor)
		{
			Vector3 v0;
			Assert::AreEqual(0.0f, v0.x);
			Assert::AreEqual(0.0f, v0.y);
			Assert::AreEqual(0.0f, v0.z);

			Vector3 v1{ 1.0f };
			Assert::AreEqual(1.0f, v1.x);
			Assert::AreEqual(1.0f, v1.y);
			Assert::AreEqual(1.0f, v1.z);

			Vector3 v2{ 1.0f, 2.0f, 3.0f };
			Assert::AreEqual(1.0f, v2.x);
			Assert::AreEqual(2.0f, v2.y);
			Assert::AreEqual(3.0f, v2.z);
		}

		TEST_METHOD(TestNegative)
		{
			Vector3 v0{ 1.0f, 2.0f, 3.0f };
			v0 = -v0;

			Assert::AreEqual(-1.0f, v0.x);
			Assert::AreEqual(-2.0f, v0.y);
			Assert::AreEqual(-3.0f, v0.z);
		}

		TEST_METHOD(TestAddition)
		{
			Vector3 v0{ 1.0f };
			Vector3 v1{ 2.0f, 3.0f, 4.0f };
			Vector3 v2 = v0 + v1;

			Assert::AreEqual(1.0f, v0.x);
			Assert::AreEqual(1.0f, v0.y);
			Assert::AreEqual(1.0f, v0.z);

			Assert::AreEqual(2.0f, v1.x);
			Assert::AreEqual(3.0f, v1.y);
			Assert::AreEqual(4.0f, v1.z);

			Assert::AreEqual(3.0f, v2.x);
			Assert::AreEqual(4.0f, v2.y);
			Assert::AreEqual(5.0f, v2.z);
		}

		TEST_METHOD(TestSubtraction)
		{
			Vector3 v0{ 1.0f };
			Vector3 v1{ 2.0f, 3.0f, 4.0f };
			Vector3 v2 = v0 - v1;

			Assert::AreEqual(1.0f, v0.x);
			Assert::AreEqual(1.0f, v0.y);
			Assert::AreEqual(1.0f, v0.z);

			Assert::AreEqual(2.0f, v1.x);
			Assert::AreEqual(3.0f, v1.y);
			Assert::AreEqual(4.0f, v1.z);

			Assert::AreEqual(-1.0f, v2.x);
			Assert::AreEqual(-2.0f, v2.y);
			Assert::AreEqual(-3.0f, v2.z);
		}

		TEST_METHOD(TestMultiplication)
		{
			Vector3 v0{ 2.0f, 3.0f, 4.0f };
			float s{ 2.0f };
			Vector3 v1 = v0 * s;

			Assert::AreEqual(2.0f, v0.x);
			Assert::AreEqual(3.0f, v0.y);
			Assert::AreEqual(4.0f, v0.z);

			Assert::AreEqual(2.0f, s);

			Assert::AreEqual(4.0f, v1.x);
			Assert::AreEqual(6.0f, v1.y);
			Assert::AreEqual(8.0f, v1.z);
		}

		TEST_METHOD(TestDivision)
		{
			Vector3 v0{ 4.0f, 6.0f, 8.0f };
			float s{ 2.0f };
			Vector3 v1 = v0 / s;

			Assert::AreEqual(4.0f, v0.x);
			Assert::AreEqual(6.0f, v0.y);
			Assert::AreEqual(8.0f, v0.z);

			Assert::AreEqual(2.0f, s);

			Assert::AreEqual(2.0f, v1.x);
			Assert::AreEqual(3.0f, v1.y);
			Assert::AreEqual(4.0f, v1.z);
		}

		TEST_METHOD(TestAddEqual)
		{
			Vector3 v0{ 1.0f };
			Vector3 v1{ 2.0f, 3.0f, 4.0f };
			v0 += v1;

			Assert::AreEqual(2.0f, v1.x);
			Assert::AreEqual(3.0f, v1.y);
			Assert::AreEqual(4.0f, v1.z);

			Assert::AreEqual(3.0f, v0.x);
			Assert::AreEqual(4.0f, v0.y);
			Assert::AreEqual(5.0f, v0.z);
		}

		TEST_METHOD(TestSubEqual)
		{
			Vector3 v0{ 1.0f };
			Vector3 v1{ 2.0f, 3.0f, 4.0f };
			v0 -= v1;

			Assert::AreEqual(2.0f, v1.x);
			Assert::AreEqual(3.0f, v1.y);
			Assert::AreEqual(4.0f, v1.z);

			Assert::AreEqual(-1.0f, v0.x);
			Assert::AreEqual(-2.0f, v0.y);
			Assert::AreEqual(-3.0f, v0.z);
		}

		TEST_METHOD(TestMulEqual)
		{
			Vector3 v0{ 2.0f, 3.0f, 4.0f };
			float s{ 2.0f };
			v0 *= s;

			Assert::AreEqual(2.0f, s);

			Assert::AreEqual(4.0f, v0.x);
			Assert::AreEqual(6.0f, v0.y);
			Assert::AreEqual(8.0f, v0.z);
		}

		TEST_METHOD(TestDivEqual)
		{
			Vector3 v0{ 4.0f, 6.0f, 8.0f };
			float s{ 2.0f };
			v0 /= s;

			Assert::AreEqual(2.0f, s);

			Assert::AreEqual(2.0f, v0.x);
			Assert::AreEqual(3.0f, v0.y);
			Assert::AreEqual(4.0f, v0.z);
		}

		TEST_METHOD(TestDot)
		{
			Vector3 v0{ 1.0f };
			Vector3 v1{ 2.0f,3.0f,4.0f };
			float dot = Dot(v0, v1);

			Assert::AreEqual(1.0f, v0.x);
			Assert::AreEqual(1.0f, v0.y);
			Assert::AreEqual(1.0f, v0.z);

			Assert::AreEqual(2.0f, v1.x);
			Assert::AreEqual(3.0f, v1.y);
			Assert::AreEqual(4.0f, v1.z);

			Assert::AreEqual(9.0f, dot);
		}

		TEST_METHOD(TestMagnitudeSqr)
		{
			Vector3 v0{ 1.0f, 2.0f, 2.0f };
			float magSqr = MagnitudeSqr(v0);

			Assert::AreEqual(1.0f, v0.x);
			Assert::AreEqual(2.0f, v0.y);
			Assert::AreEqual(2.0f, v0.z);

			Assert::AreEqual(9.0f, magSqr);
		}

		TEST_METHOD(TestMagnitude)
		{
			Vector3 v0{ 1.0f, 2.0f, 2.0f };
			float mag = Magnitude(v0);

			Assert::AreEqual(1.0f, v0.x);
			Assert::AreEqual(2.0f, v0.y);
			Assert::AreEqual(2.0f, v0.z);

			Assert::AreEqual(3.0f, mag);
		}

		TEST_METHOD(TestNormalize)
		{
			Vector3 v0{ 3.0f, 4.0f, 0.0f };
			Vector3 v1 = Normalize(v0);

			Assert::AreEqual(3.0f, v0.x);
			Assert::AreEqual(4.0f, v0.y);
			Assert::AreEqual(0.0f, v0.z);

			Assert::AreEqual(0.6f, v1.x);
			Assert::AreEqual(0.8f, v1.y);
			Assert::AreEqual(0.0f, v1.z);
		}

		TEST_METHOD(TestCross) 
		{
			Vector3 v0{1.0f, 2.0f, 3.0f};
			Vector3 v1{1.0f, 5.0f, 7.0f};
			Vector3 v2 = Cross(v0, v1);

			Assert::AreEqual(1.0f, v0.x);
			Assert::AreEqual(2.0f, v0.y);
			Assert::AreEqual(3.0f, v0.z);

			Assert::AreEqual(1.0f, v1.x);
			Assert::AreEqual(5.0f, v1.y);
			Assert::AreEqual(7.0f, v1.z);

			Assert::AreEqual(-1.0f, v2.x);
			Assert::AreEqual(-4.0f, v2.y);
			Assert::AreEqual( 3.0f, v2.z);

		}

		TEST_METHOD(TestDistanceSqr) 
		{
			Vector3 v0{ 2.0f,3.0f,3.0f };
			Vector3 v1{ 1.0f };
			float disSqr = DistanceSqr(v0, v1);

			Assert::AreEqual(2.0f, v0.x);
			Assert::AreEqual(3.0f, v0.y);
			Assert::AreEqual(3.0f, v0.z);

			Assert::AreEqual(1.0f, v1.x);
			Assert::AreEqual(1.0f, v1.y);
			Assert::AreEqual(1.0f, v1.z);

			Assert::AreEqual(9.0f, disSqr);
		}

		TEST_METHOD(TestDistance)
		{
			Vector3 v0{ 2.0f,3.0f,3.0f };
			Vector3 v1{ 1.0f };
			float dis = Distance(v0, v1);

			Assert::AreEqual(2.0f, v0.x);
			Assert::AreEqual(3.0f, v0.y);
			Assert::AreEqual(3.0f, v0.z);

			Assert::AreEqual(1.0f, v1.x);
			Assert::AreEqual(1.0f, v1.y);
			Assert::AreEqual(1.0f, v1.z);

			Assert::AreEqual(3.0f, dis);
		}
	};
} // namespace MathTest