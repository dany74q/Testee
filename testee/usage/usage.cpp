#include "..\src\testee.h"
#include <stdexcept>

using namespace testee;

int main() {
	TestSuite("Example suite",
		make_test("Test Equality",
			[](auto const& _) {
				_.assertEQ(1, 1);
			}
		),
		make_test("Test Lower Than",
			[](auto const& _) {
				_.assertLT(1, 2);
			}
		),
		make_test("Test throws",
			[](auto const& _) {
				_.assertThrows<std::exception>(
					[]() { throw std::exception{}; }
				);
			}
		),
		make_test("Test does not throw",
			[](auto const& _) {
				_.assertNotThrows<std::exception>(
					[]() {  }
				);
			}
		)
	);
}