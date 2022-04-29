#include "Swift/getName.hpp"
#include "ObjcSwift/Helpers/typeToStringBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <numeric>
#include <string>
#include <vector>

namespace Swift {
namespace {
std::string getParameterString(std::vector<IR::Type> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, IR::Type const& current) {
		                       return std::move(soFar) +
		                              ObjcSwift::Helpers::buildTypeString(
		                                  current);
	                       });
}

}    // namespace

std::string getClassName(std::string const& cppClassName,
                         std::vector<IR::Type> const& cppTemplateArgs) {
	return ObjcSwift::Helpers::removeCppTemplate(cppClassName) +
	       getParameterString(cppTemplateArgs);
}

std::string getFunctionName(IR::Function const& cppFunction,
                            bool isConstructor) {
	if (!isConstructor) {
		return ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name);
	} else {
		return "init";
	}
}
}    // namespace Swift

