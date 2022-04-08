#pragma once

#include "ObjcSwift/Proxy/attribute.hpp"
#include "ObjcSwift/Proxy/class.hpp"
#include "ObjcSwift/Proxy/enum.hpp"
#include "ObjcSwift/Proxy/function.hpp"
#include "ObjcSwift/Proxy/typeInfo.hpp"
#include <string>
#include <vector>

namespace ObjcSwift::Proxy {

struct Module {
	explicit Module(std::string const& variableName);

	void addFunction(Function const& function);

	void addClass(Class const& c);

	void addEnum(Enum const& e);

	void addAttribute(Attribute const& a);

	std::string const& getVariableName() const;

	void addSubmodule(std::string const& name,
	                  std::string const& variableName,
	                  std::string const& documentation);

	std::string getObjcSwift() const;

	std::string getObjcHeader() const;
	std::string getObjcSource() const;

	std::string getSwift() const;

private:
	std::string m_variableName;

	struct SubModule {
		std::string name;
		// C++ variable to refer to the submodule
		// E.g. "NS_Nested_Sub"
		std::string variable;
		std::string documentation;
	};
	std::vector<SubModule> m_submodules;

	std::vector<Function> m_functions;
	std::vector<Class> m_classes;
	std::vector<Enum> m_enums;
	std::vector<Attribute> m_attributes;
};
}    // namespace ObjcSwift::Proxy
