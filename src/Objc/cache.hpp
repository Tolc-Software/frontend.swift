#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

namespace Objc {

struct Cache {
	std::string m_extraFunctionsNamespace = "Tolc_";

	// Functions that are needed globally from Objective-C
	// Will be placed in the root under the namespace m_extraFunctionsNamespace
	std::vector<std::string> m_extraFunctions;

	// The name of the users module we are converting
	std::string m_moduleName;

	// Registered conversion names
	std::set<std::string> m_conversions;
};

}    // namespace Objc
