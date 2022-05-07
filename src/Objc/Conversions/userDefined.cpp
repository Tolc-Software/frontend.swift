#include "Objc/Conversions/userDefined.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>

namespace Objc::Conversions {

namespace {
std::string getValidFunctionName(std::string const& name) {
	std::string out;
	for (char c : name) {
		switch (c) {
			case '<':
			case '>':
			case ' ':
			case ',': out.push_back('_'); break;
			default: out.push_back(c);
		}
	}
	return out;
}

}    // namespace
Objc::Conversions::Conversion
getUserDefinedConversionNames(std::string const& fullyQualifiedName,
                              std::string const& ns) {
	Objc::Conversions::Conversion names;
	auto validName = getValidFunctionName(fullyQualifiedName);
	names.m_toObjc = fmt::format("convertUserDefined{}ToObjc", validName);
	names.m_toCpp = fmt::format("convertUserDefined{}ToCpp", validName);
	return Objc::Conversions::addNamespace(names, ns);
}
}    // namespace Objc::Conversions
