#pragma once

#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/function.hpp"
#include <string>
#include <vector>

namespace Objc::Proxy {

class Class {
public:
	explicit Class(std::string const& name,
	               std::string const& fullyQualifiedName);

	void addEnum(Enum const& e);

	void addFunction(Function const& function);

	void addConstructor(Function const& constructor);

	struct MemberVariable {
		// User defined name of the member variable
		std::string m_name;
		// Objc type
		std::string m_type;
		std::string m_documentation;
		bool m_isConst;
		bool m_isStatic;
	};

	void addMemberVariable(MemberVariable const& variable);

	std::string const& getName() const;

	void setDocumentation(std::string const& documentation);

	void setInherited(std::vector<std::string> const& inherited);

	// Will be managed by a std::shared_ptr on the python side
	// instead of the default std::unique_ptr
	void setAsManagedByShared();

	std::string getObjcSource() const;
	std::string getObjcHeader() const;

private:
	std::string joinMemberVariables(bool isSource) const;

	// User defined name of the class
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::string m_documentation;

	std::vector<std::string> m_inherited;
	std::vector<Function> m_constructors;
	std::vector<Function> m_functions;
	std::vector<MemberVariable> m_memberVariables;
	std::vector<Enum> m_enums;

	bool m_isManagedByShared;
};
}    // namespace Objc::Proxy
