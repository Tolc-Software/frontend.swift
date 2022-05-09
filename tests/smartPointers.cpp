#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Smart pointers of custom types work", "[smartPointers]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);
	stage.keepAliveAfterTest();

	auto cppCode = R"(
#include <memory>

struct Data {
  int i = 5;
};

struct SharedData {
  int i = 10;
};

std::unique_ptr<Data> createData() {
  return std::make_unique<Data>();
}

// This moves the data,
// destroying it at the end
// Same as C++
int consumeData(std::unique_ptr<Data> data) {
  return data->i + 20;
}

// std::shared_ptr<SharedData> createSharedData() {
//   return std::make_shared<SharedData>();
// }
)";

	auto objcTestCode = R"(
// std::unique_ptr acts as a normal value
mData* data = [m createData];
assert(data.i == 5);

// This moves the data,
// destroying it at the end
// Same as C++
assert([m consumeData:data] == 25);

// Any access now results
// in undefined behaviour
// NSLog(@"%i", data.i);

// std::shared_ptr acts as a normal value
// mSharedData* sharedData = [m createSharedData];
// assert(sharedData.i == 10);
)";

	auto swiftTestCode = R"()";

	auto errorCode =
	    stage.runObjcSwiftTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Smart Pointers");
}
