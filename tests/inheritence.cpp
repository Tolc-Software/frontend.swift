#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Inheritence", "[inheritence]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};

struct Dog : public Pet {
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};
)";

	auto objcTestCode = R"(
fido = m.Dog("Fido")

// Inherits public properties
self.assertEqual(fido.name, "Fido")

// But has its new functions
self.assertEqual(fido.bark(), "woof!")
)";

	[[maybe_unused]] auto swiftTestCode = R"()";

	auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Simple inheritence");
}
