#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/getName.hpp"
#include <fmt/format.h>
#include <string>
#include <variant>

namespace Objc::Conversions {

Objc::Conversions::Conversion
getConversionEnumName(std::string const& moduleName,
                      std::string const& conversionNamespace,
                      std::string const& fullyQualifiedEnumName,
                      std::string const& enumName) {
	Conversion names;
	auto objcEnum = Objc::getEnumName(fullyQualifiedEnumName, moduleName);
	names.m_toCpp = fmt::format(
	    "{}::convertEnum{}To{}", conversionNamespace, objcEnum, enumName);
	names.m_toObjc = fmt::format(
	    "{}::convertEnum{}To{}", conversionNamespace, enumName, objcEnum);

	return names;
}

Objc::Conversions::Conversion
getConversionBaseName(std::string const& conversionNamespace,
                      IR::BaseType baseType) {
	Conversion names;
	names.m_toCpp = "";
	names.m_toObjc = "";
	using IR::BaseType;
	switch (baseType) {
		case BaseType::FilesystemPath: {
			names.m_toCpp = fmt::format("{}::convertNSStringToFilesystemPath",
			                            conversionNamespace);
			names.m_toObjc = fmt::format("{}::convertFilesystemPathToNSString",
			                             conversionNamespace);
			return names;
		}
		case BaseType::String: {
			names.m_toCpp =
			    fmt::format("{}::convertNSStringToString", conversionNamespace);
			names.m_toObjc =
			    fmt::format("{}::convertStringToNSString", conversionNamespace);
			return names;
		}
		case BaseType::StringView: {
			names.m_toCpp = fmt::format("{}::convertNSStringToStringView",
			                            conversionNamespace);
			names.m_toObjc = fmt::format("{}::convertStringToNSStringView",
			                             conversionNamespace);
			return names;
		}

		case BaseType::Bool:
		case BaseType::Char16_t:
		case BaseType::Char32_t:
		case BaseType::Char:
		case BaseType::Complex:
		case BaseType::Double:
		case BaseType::Float:
		case BaseType::Int:
		case BaseType::LongDouble:
		case BaseType::LongInt:
		case BaseType::LongLongInt:
		case BaseType::ShortInt:
		case BaseType::SignedChar:
		case BaseType::UnsignedChar:
		case BaseType::UnsignedInt:
		case BaseType::UnsignedLongInt:
		case BaseType::UnsignedLongLongInt:
		case BaseType::UnsignedShortInt:
		case BaseType::Void:
		case BaseType::Wchar_t: return names;
	}

	return names;
}
}    // namespace Objc::Conversions
