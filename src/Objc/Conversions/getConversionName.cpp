#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/getName.hpp"
#include <fmt/format.h>
#include <string>
#include <variant>

namespace Objc::Conversions {

Conversion getConversionEnumName(std::string const& moduleName,
                                 std::string const& fullyQualifiedEnumName,
                                 std::string const& enumName) {
	Conversion names;
	auto objcEnum = Objc::getEnumName(fullyQualifiedEnumName, moduleName);
	names.m_toCpp = fmt::format("convertEnum{}To{}", objcEnum, enumName);
	names.m_toObjc = fmt::format("convertEnum{}To{}", enumName, objcEnum);

	return names;
}
}    // namespace Objc::Conversions
