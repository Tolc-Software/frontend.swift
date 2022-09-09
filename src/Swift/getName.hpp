#pragma once

#include <string>

namespace Swift {

struct SplitData {
	std::string m_name;
	std::string m_objcPrefix;
	std::string m_swiftPrefix;
};

SplitData splitIntoNames(std::string const& fullyQualifiedName,
                         std::string const& libraryName);

}    // namespace Swift
