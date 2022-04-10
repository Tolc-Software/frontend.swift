#pragma once

#include "Swift/Proxy/attribute.hpp"
#include "Swift/Proxy/class.hpp"
#include "Swift/Proxy/enum.hpp"
#include "Swift/Proxy/function.hpp"
#include <string>
#include <vector>

namespace Swift::Proxy {

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
}    // namespace Swift::Proxy
