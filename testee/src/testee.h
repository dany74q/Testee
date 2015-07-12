#pragma once
#include <utility>

namespace testee {
	auto for_each_arg = [](auto const& func, auto&&... args) {
		unsigned _[] {
			(
				func(std::forward<decltype(args)>(args)),
				0u
			)...
		};
	};

	template <typename Func> using TestAndDescription = std::pair<const char*, Func>;

	auto make_test = [](const char* description, auto const& func) {
		return TestAndDescription<decltype(func)>{description, func};
	};

	class TestSuite {
		mutable bool currentTestFailed_ = false;
	public:
		template <typename... Tests>
		TestSuite(const char* name, TestAndDescription<Tests> const&... tests) {
			for_each_arg(
				[this](auto& test) {
					currentTestFailed_ = false;
					SetUp();
					std::printf("Running test %s ... ", test.first);
					test.second(*this);
					std::printf(currentTestFailed_ ? "Failed\n" : "Passed\n");
					TearDown();
				},
				tests...
			);
		}

		void assert(bool expr) const {
			currentTestFailed_ = !expr;
		}

		template <typename T, typename U>
		void assertEQ(T const& first, U const& other) const {
			assert(first == other);
		}

		template <typename T, typename U>
		void assertGT(T const& first, U const& other) const {
			assert(first > other);
		}

		template <typename T, typename U>
		void assertLT(T const& first, U const& other) const {
			assert(first < other);
		}

		template <typename T, typename U>
		void assertLEQ(T const& first, U const& other) const {
			assert(first <= other);
		}

		template <typename T, typename U>
		void assertGEQ(T const& first, U const& other) const {
			assert(first >= other);
		}

		template <typename E, typename F>
		void assertThrows(F const& func) const {
			bool threwExpected = false;
			try {
				func();
			}
			catch (E&) {
				threwExpected = true;
			}
			catch (...) {}
			assert(threwExpected);
		}

		template <typename E, typename F>
		void assertNotThrows(F const& func) const {
			bool didntThrow = true;
			try {
				func();
			}
			catch (E&) {
				didntThrow = false;
			}
			catch (...) {}
			assert(didntThrow);
		}

		virtual void SetUp() const {};
		virtual void TearDown() const {};
	};
}