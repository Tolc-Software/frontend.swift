#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/Proxy/type.hpp"
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

	void addMemberVariable(Attribute const& variable);

	std::string const& getName() const;

	void setDocumentation(std::string const& documentation);

	void setInherited(std::vector<std::string> const& inherited);

	// Will be managed by a std::shared_ptr on the python side
	// instead of the default std::unique_ptr
	void setAsManagedByShared();

	// Meaning it has no underlying C++ class
	void setAsPurelyStatic();

	void setConversionNames(Objc::Conversions::Conversion const& c);

	std::string getObjcSource() const;
	std::string getObjcHeader() const;

private:
	std::string joinMemberVariables(bool isSource) const;
	std::string getConversions() const;

	// User defined name of the class
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::string m_documentation;
	Objc::Conversions::Conversion m_conversions;

	std::vector<std::string> m_inherited;
	std::vector<Function> m_constructors;
	std::vector<Function> m_functions;
	std::vector<Attribute> m_memberVariables;
	std::vector<Enum> m_enums;

	bool m_isManagedByShared;
	bool m_isPurelyStatic;
};
}    // namespace Objc::Proxy
