#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"
#include <catch2/catch.hpp>
#include <fmt/format.h>

TEST_CASE("Namespaces turn into modules", "[namespaces]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

/*
* MyLib contains a bunch of MyLib functions
*/
namespace MyLib {

int complexFunction() {
	return 5;
}

	namespace We {
		namespace Are {
			namespace Going {
				namespace Pretty {
					namespace Deep {
						std::string meaningOfLife() {
							return "42";
						}
					}
				}
			}
		}
	}
}

)";

	auto objcTestCode = R"(
// Namespaces corresponds to classes
// with {library name} + join(namespaces)
// where functions are static class functions
assert([mMyLib complexFunction] == 5);

// You can nest namespaces arbitrarily deep
NSString* lifeProTips = [mMyLibWeAreGoingPrettyDeep meaningOfLife];
assert([lifeProTips isEqualToString:@"42"]);
)";

	auto swiftTestCode = R"()";

	auto errorCode = stage.runObjcTest(cppCode, objcTestCode, swiftTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Namespaces");
}
