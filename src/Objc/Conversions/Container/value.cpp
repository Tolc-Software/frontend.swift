#include "Objc/Conversions/Container/value.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace Objc::Conversions::Container {

void convertNSNumber(IR::Type const& type,
                     std::string const& typeName,
                     std::string const& numberWithName,
                     std::vector<std::string>& functions,
                     Objc::Cache& cache) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_baseConversions.m_toCpp.contains(names.m_toCpp)) {
		cache.m_baseConversions.m_toCpp.insert(names.m_toCpp);
		functions.push_back(fmt::format(
		    R"(
{typeName} {toCppName}(id value) {{
  if (value && [value isKindOfClass:[NSNumber class]]) {{
    return [value {typeName}Value];
  }}
  return 0;
}})",
		    fmt::arg("toCppName", names.m_toCpp),
		    fmt::arg("typeName", typeName)));

		functions.push_back(fmt::format(
		    R"(
NSNumber* {toObjcName}({typeName} value) {{
  return [NSNumber numberWith{upperCaseType}:value];
}})",
		    fmt::arg("toObjcName", names.m_toObjc),
		    fmt::arg("typeName", typeName),
		    fmt::arg("upperCaseType", numberWithName)));
	}
}

void valueConversion(IR::Type const& type,
                     IR::BaseType baseType,
                     std::vector<std::string>& functions,
                     Objc::Cache& cache) {
	using IR::BaseType;
	switch (baseType) {
		case BaseType::Int: {
			convertNSNumber(type, "int", "Int", functions, cache);
			break;
		}
		case BaseType::Bool: {
			convertNSNumber(type, "bool", "Bool", functions, cache);
			break;
		}
		case BaseType::Char16_t: {
			break;
		}
		case BaseType::Char32_t: {
			break;
		}
		case BaseType::Char: {
			convertNSNumber(type, "char", "Char", functions, cache);
			break;
		}
		case BaseType::Complex: {
			break;
		}
		case BaseType::Double: {
			convertNSNumber(type, "double", "Double", functions, cache);
			break;
		}
		case BaseType::FilesystemPath: {
			break;
		}
		case BaseType::Float: {
			convertNSNumber(type, "float", "Float", functions, cache);
			break;
		}
		case BaseType::LongDouble: {
			break;
		}
		case BaseType::LongInt: {
			convertNSNumber(type, "long", "Long", functions, cache);
			break;
		}
		case BaseType::LongLongInt: {
			convertNSNumber(type, "long long", "LongLong", functions, cache);
			break;
		}
		case BaseType::ShortInt: {
			convertNSNumber(type, "short", "Short", functions, cache);
			break;
		}
		case BaseType::SignedChar: {
			convertNSNumber(type, "char", "Char", functions, cache);
			break;
		}
		case BaseType::String: {
			break;
		}
		case BaseType::StringView: {
			break;
		}
		case BaseType::UnsignedChar: {
			convertNSNumber(
			    type, "unsigned char", "UnsignedChar", functions, cache);
			break;
		}
		case BaseType::UnsignedInt: {
			convertNSNumber(
			    type, "unsigned int", "UnsignedInt", functions, cache);
			break;
		}
		case BaseType::UnsignedLongInt: {
			convertNSNumber(
			    type, "unsigned long", "UnsignedLong", functions, cache);
			break;
		}
		case BaseType::UnsignedLongLongInt: {
			convertNSNumber(type,
			                "unsigned long long",
			                "UnsignedLongLong",
			                functions,
			                cache);
			break;
		}
		case BaseType::UnsignedShortInt: {
			convertNSNumber(
			    type, "unsigned short", "UnsignedShort", functions, cache);
			break;
		}
		case BaseType::Void: {
			break;
		}
		case BaseType::Wchar_t: {
			break;
		}
	}
}
}    // namespace Objc::Conversions::Container
