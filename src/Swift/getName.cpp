#include "Swift/getName.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include <fmt/format.h>
#include <string>

namespace Swift {

SplitData splitIntoNames(std::string const& fullyQualifiedName,
                         std::string const& libraryName) {
	SplitData data;

	auto splitted = ObjcSwift::Helpers::split(fullyQualifiedName, "::");
	if (splitted.size() == 1) {
		data.m_name = splitted[0];
		data.m_objcPrefix = libraryName;
		data.m_swiftPrefix = libraryName;
	} else {
		data.m_name = splitted.back();
		// Remove the name
		splitted.pop_back();
		splitted.push_front(libraryName);
		data.m_objcPrefix = fmt::format("{}", fmt::join(splitted, ""));
		data.m_swiftPrefix = fmt::format("{}", fmt::join(splitted, "."));
	}

	return data;
}

}    // namespace Swift

