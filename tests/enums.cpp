#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Testing enums", "[enums]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
enum Unscoped {
	Under,
	Uboat,
};

enum class Scoped {
	Sacred,
	Snail,
};

class EnumTest {
public:
	explicit EnumTest(Scoped _s) : s(_s) {};

	Scoped s;
};

Unscoped f(Unscoped u) {
	return u;
}

namespace NS {
	// Documentation describing the enum
	enum class Deep {
		Double,
		Down,
	};
}

)";

	auto objcTestCode = R"(
// C++11 enums work
mWithConstructor* ten = [[mWithConstructor alloc] init];
assert([ten getV] == 10);
scoped = m.Scoped.Snail
enumTest = m.EnumTest(scoped)
assert(enumTest.s == scoped)

// Aswell as legacy enums
unscoped = m.Unscoped.Uboat
u = m.f(unscoped)
assert(u == unscoped)

// Enums under namespaces are available
// under the corresponding submodule
deep = m.NS.Deep.Down
assert(deep != m.NS.Deep.Double)

// Documentation carries over from C++
// self.assertIn("Documentation describing the enum", m.NS.Deep.__doc__)
)";

	auto swiftTestCode = R"(
// C++11 enums work
scoped = m.Scoped.Snail
enumTest = m.EnumTest(scoped)
assert(enumTest.s == scoped)

// Aswell as legacy enums
unscoped = m.Unscoped.Uboat
u = m.f(unscoped)
assert(u == unscoped)

// Enums under namespaces are available
// under the corresponding submodule
deep = m.NS.Deep.Down
assert(deep != m.NS.Deep.Double)

// Documentation carries over from C++
// self.assertIn("Documentation describing the enum", m.NS.Deep.__doc__)
)";
	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Enums");
}
