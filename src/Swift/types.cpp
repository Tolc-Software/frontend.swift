#include "Swift/types.hpp"
#include <IR/ir.hpp>
#include <string>
#include <variant>

namespace Swift {

namespace {
std::string toSwiftType(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "Bool";
		case BaseType::Char16_t: return "char16t";
		case BaseType::Char32_t: return "char32t";
		case BaseType::Char: return "char";
		case BaseType::Complex: return "complex";
		case BaseType::Double: return "Double";
		case BaseType::FilesystemPath: return "String";
		case BaseType::Float: return "Float";
		case BaseType::Int: return "Int32";
		case BaseType::LongDouble: return "Int64";
		case BaseType::LongInt: return "Int64";
		case BaseType::LongLongInt: return "Int64";
		case BaseType::ShortInt: return "Int32";
		case BaseType::SignedChar: return "signedchar";
		case BaseType::String: return "String";
		case BaseType::StringView: return "String";
		case BaseType::UnsignedChar: return "unsignedchar";
		case BaseType::UnsignedInt: return "unsignedint";
		case BaseType::UnsignedLongInt: return "unsignedlongint";
		case BaseType::UnsignedLongLongInt: return "unsignedlonglongint";
		case BaseType::UnsignedShortInt: return "unsignedshortint";
		case BaseType::Void: return "Void";
		case BaseType::Wchar_t: return "wchart";
	}
	return "";
}
}    // namespace

std::string toSwiftType(IR::Type const& type) {
	if (auto baseType = std::get_if<IR::Type::Value>(&type.m_type)) {
		return toSwiftType(baseType->m_base);
	}
	return "";
}

}    // namespace Swift
