#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/getName.hpp"
#include <IR/ir.hpp>
#include <string>
#include <variant>

namespace Objc::Builders {

namespace {
std::string toObjcType(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "bool";
		case BaseType::Char16_t: return "char16_t";
		case BaseType::Char32_t: return "char32_t";
		case BaseType::Char: return "char";
		case BaseType::Complex: return "complex";
		case BaseType::Double: return "double";
		case BaseType::FilesystemPath: return "filesystempath";
		case BaseType::Float: return "float";
		case BaseType::Int: return "int";
		case BaseType::LongDouble: return "long double";
		case BaseType::LongInt: return "long int";
		case BaseType::LongLongInt: return "long long int";
		case BaseType::ShortInt: return "short int";
		case BaseType::SignedChar: return "signed char";
		case BaseType::String: return "*NSString";
		case BaseType::StringView: return "*NSString";
		case BaseType::UnsignedChar: return "unsigned char";
		case BaseType::UnsignedInt: return "unsigned int";
		case BaseType::UnsignedLongInt: return "unsigned long int";
		case BaseType::UnsignedLongLongInt: return "unsigned long long int";
		case BaseType::UnsignedShortInt: return "unsigned short int";
		case BaseType::Void: return "void";
		case BaseType::Wchar_t: return "wchar_t";
	}
	return "";
}
}    // namespace

Objc::Proxy::Type buildType(IR::Type const& type,
                            std::string const& rootModuleName) {
	Objc::Proxy::Type t;
	if (auto baseType = std::get_if<IR::Type::Value>(&type.m_type)) {
		t.m_name = toObjcType(baseType->m_base);
		t.m_toObjc = "";
		t.m_toCpp = "";
	} else if (auto enumType = std::get_if<IR::Type::EnumValue>(&type.m_type)) {
		t.m_name =
		    Objc::getEnumName(enumType->m_representation, rootModuleName);
		t.m_toObjc = "";
		t.m_toCpp = "";
	}
	return t;
}

}    // namespace Objc::Builders
