#pragma once

#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/function.hpp"
#include <string>
#include <vector>

namespace Objc::Proxy {

struct Module {
	explicit Module(std::string const& variableName);

	void addFunction(Function const& function);

	void addClass(Class const& c);

	void addAttribute(Attribute const& a);

	std::string const& getVariableName() const;

	void addSubmodule(std::string const& name,
	                  std::string const& variableName,
	                  std::string const& documentation);

	std::string getObjcHeader() const;
	std::string getObjcSource() const;

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
	std::vector<Attribute> m_attributes;
};
}    // namespace Objc::Proxy
