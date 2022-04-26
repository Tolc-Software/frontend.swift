#include "Objc/getName.hpp"
#include "Objc/Builders/typeToStringBuilder.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <numeric>
#include <string>
#include <vector>

namespace Objc {
namespace {

std::string joinVariableName(std::string qualifiedName,
                             std::string const& rootModuleName) {
	// MyNS::Math, rootModule -> rootModuleMyNsMath
	// This is to avoid naming conflicts when defining namespaces with the
	// same name as the root module
	// This happens if you call your module tensorflow and have a namespace with tensorflow
	auto splitted = ObjcSwift::Helpers::split(qualifiedName, "::");

	splitted.push_front(rootModuleName);
	// If qualifiedName is the root name (global namespace has no name)
	// This will return rootModuleName
	return fmt::format("{}", fmt::join(splitted, ""));
}

std::string getParameterString(std::vector<IR::Type> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, auto const& current) {
		                       return std::move(soFar) +
		                              Objc::Builders::buildTypeString(current);
	                       });
}

}    // namespace

std::string getParameterString(std::vector<IR::Argument> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, auto const& current) {
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

std::string getEnumName(std::string const& qualifiedEnumName,
                        std::string const& moduleName) {
	return joinVariableName(qualifiedEnumName, moduleName);
}
}    // namespace Objc
