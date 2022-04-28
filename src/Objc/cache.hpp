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

	struct Conversions {
		// Names of conversion functions for types
		// Used to check if a function has been registered already
		std::set<std::string> m_toCpp;
	};

	Conversions m_baseConversions;
};

}    // namespace Objc
