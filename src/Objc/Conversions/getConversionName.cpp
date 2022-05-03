#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/typeToStringBuilder.hpp"
#include <fmt/format.h>
#include <string>
#include <variant>

namespace Objc::Conversions {

Objc::Conversions::Conversion
getConversionEnumName(std::string const& moduleName,
                      std::string const& fullyQualifiedEnumName,
                      std::string const& conversionNamespace) {
	Conversion names;
	auto objcEnum = Objc::getEnumName(fullyQualifiedEnumName, moduleName);
	auto ns = conversionNamespace.empty() ? "" : conversionNamespace + "::";
	names.m_toCpp = fmt::format("{}convertEnum{}ToCpp", ns, objcEnum);
	names.m_toObjc = fmt::format("{}convertEnum{}ToObjc", ns, objcEnum);

	return names;
}

Objc::Conversions::Conversion
getConversionBaseName(IR::BaseType baseType,
                      std::string const& conversionNamespace) {
	Conversion names;
	names.m_toCpp = "";
	names.m_toObjc = "";
	auto ns = conversionNamespace.empty() ? "" : conversionNamespace + "::";
	using IR::BaseType;
	switch (baseType) {
		case BaseType::FilesystemPath: {
			names.m_toCpp =
			    fmt::format("{}convertBaseNSStringToFilesystemPath", ns);
			names.m_toObjc =
			    fmt::format("{}convertBaseFilesystemPathToNSString", ns);
			return names;
		}
		case BaseType::String: {
			names.m_toCpp = fmt::format("{}convertBaseNSStringToString", ns);
			names.m_toObjc = fmt::format("{}convertBaseStringToNSString", ns);
			return names;
		}
		case BaseType::StringView: {
			names.m_toCpp =
			    fmt::format("{}convertBaseNSStringToStringView", ns);
			names.m_toObjc =
			    fmt::format("{}convertBaseStringToNSStringView", ns);
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

Objc::Conversions::Conversion
getConversionContainerName(IR::Type const& containerType,
                           std::string const& conversionNamespace) {
	Conversion names;
	auto typeName = ObjcSwift::Helpers::buildTypeString(containerType);
	auto ns = conversionNamespace.empty() ? "" : conversionNamespace + "::";
	names.m_toCpp = fmt::format("{}convertContainer{}ToCpp", ns, typeName);
	names.m_toObjc = fmt::format("{}convertContainer{}ToObjc", ns, typeName);

	return names;
}
}    // namespace Objc::Conversions
