#include "Objc/getName.hpp"
#include "Objc/Builders/typeToStringBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <string>
#include <vector>

namespace Objc {
namespace {
std::string getParameterString(std::vector<IR::Type> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, IR::Type const& current) {
		                       return std::move(soFar) +
		                              Objc::Builders::buildTypeString(current);
	                       });
}

}    // namespace

std::string getParameterString(std::vector<IR::Variable> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, IR::Variable const& current) {
		                       return std::move(soFar) +
		                              Objc::Builders::buildTypeString(
		                                  current.m_type);
	                       });
}

std::string getConstructorExtraName(IR::Function const& f) {
	if (f.m_arguments.empty()) {
		return "";
	}
	return "With" + getParameterString(f.m_arguments);
}

std::string getClassName(IR::Struct const& cppClass,
                         std::string const& moduleName) {
	return moduleName + ObjcSwift::Helpers::removeCppTemplate(cppClass.m_name) +
	       getParameterString(cppClass.m_templateArguments);
}

std::string getFunctionName(IR::Function const& cppFunction,
                            bool isConstructor) {
	if (!isConstructor) {
		return ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name);
	} else {
		return "init" + getConstructorExtraName(cppFunction);
	}
}
}    // namespace Objc
