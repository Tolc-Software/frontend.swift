#pragma once

#pragma once

#include <string>
#include <vector>

namespace Frontend::ObjcSwift {

enum class Language { Swift, ObjectiveC };

/**
* The values in this struct corresponds to the default values
* in configuring what the Frontend should do.
* - There should be no functions or logic here, just values.
* - Every top level value should be commented.
*/
struct Config {
	// Absolute paths to system headers
	Frontend::ObjcSwift::Language m_language =
	    Frontend::ObjcSwift::Language::Swift;

	// Order in which objects should be initialized
	std::vector<size_t> m_definitionOrder = {};
};
}    // namespace Frontend::ObjcSwift
