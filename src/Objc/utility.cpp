#include "Objc/utility.hpp"
#include <fmt/format.h>
#include <string>
#include <string_view>

namespace Objc {

// A category is an extension of a class in Objective-C
std::string getCategoryName(char prefix,
                            std::string_view objcClass,
                            std::string_view name) {
	return fmt::format("{}_{}_{}", prefix, objcClass, name);
}

std::string wrapInInterface(std::string_view objcClass,
                            std::string_view category,
                            std::string_view code) {
	return fmt::format(R"(
@interface {}({})
{}
@end
)",
	                   objcClass,
	                   category,
	                   code);
}

std::string wrapInImplementation(std::string_view objcClass,
                                 std::string_view category,
                                 std::string_view code) {
	return fmt::format(R"(
@implementation {}({})
{}
@end
)",
	                   objcClass,
	                   category,
	                   code);
}

std::string getDocumentationString(std::string_view documentation) {
	if (documentation.empty()) {
		return "";
	}
	return fmt::format(R"(
/**
{documentation}
*/)",
	                   fmt::arg("documentation", documentation));
}

}    // namespace Objc
