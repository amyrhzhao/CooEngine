#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Coo::Math;

namespace MathTest 
{
	TEST_CLASS(QuaternionTest) 
	{
	public:
		TEST_METHOD(TestConstructor) 
		{
			Quaternion q0;
			Assert::AreEqual(0.0f, q0.x);
			Assert::AreEqual(0.0f, q0.y);
			Assert::AreEqual(0.0f, q0.z);
			Assert::AreEqual(1.0f, q0.w);

			Quaternion q1(0.0f, 1.0f, 2.0f, 3.0f);
			Assert::AreEqual(0.0f, q1.x);
			Assert::AreEqual(1.0f, q1.y);
			Assert::AreEqual(2.0f, q1.z);
			Assert::AreEqual(3.0f, q1.w);
		}

		TEST_METHOD(TestStaticFunctions)
		{
			Quaternion zero = Quaternion::Zero();
			Assert::AreEqual(0.0f, zero.x);
			Assert::AreEqual(0.0f, zero.y);
			Assert::AreEqual(0.0f, zero.z);
			Assert::AreEqual(0.0f, zero.w);

			Quaternion identity = Quaternion::Identity();
			Assert::AreEqual(0.0f, identity.x);
			Assert::AreEqual(0.0f, identity.y);
			Assert::AreEqual(0.0f, identity.z);
			Assert::AreEqual(1.0f, identity.w);
		}

		TEST_METHOD(TestBasicCalculations) 
		{
			// operator+ 
			Quaternion q0;
			Quaternion q1(2.0f, 3.0f, 4.0f, 5.0f);
			Quaternion q2 = q0 + q1;
			Assert::AreEqual(0.0f, q0.x);
			Assert::AreEqual(0.0f, q0.y);
			Assert::AreEqual(0.0f, q0.z);
			Assert::AreEqual(1.0f, q0.w);
			Assert::AreEqual(2.0f, q1.x);
			Assert::AreEqual(3.0f, q1.y);
			Assert::AreEqual(4.0f, q1.z);
			Assert::AreEqual(5.0f, q1.w);
			Assert::AreEqual(2.0f, q2.x);
			Assert::AreEqual(3.0f, q2.y);
			Assert::AreEqual(4.0f, q2.z);
			Assert::AreEqual(6.0f, q2.w);

			// operator-
			q2 = q0 - q1;
			Assert::AreEqual(0.0f, q0.x);
			Assert::AreEqual(0.0f, q0.y);
			Assert::AreEqual(0.0f, q0.z);
			Assert::AreEqual(1.0f, q0.w);
			Assert::AreEqual(2.0f, q1.x);
			Assert::AreEqual(3.0f, q1.y);
			Assert::AreEqual(4.0f, q1.z);
			Assert::AreEqual(5.0f, q1.w);
			Assert::AreEqual(-2.0f, q2.x);
			Assert::AreEqual(-3.0f, q2.y);
			Assert::AreEqual(-4.0f, q2.z);
			Assert::AreEqual(-4.0f, q2.w);

			// operator* 
			// operator/
		}
	};

} // namespace MathTest