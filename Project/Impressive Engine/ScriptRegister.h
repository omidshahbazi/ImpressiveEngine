/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#include "VirtualMachine.h"

#ifdef EDIT_MODE
#include "ScriptTypesDefinition.h"
#endif

BEGIN_NAMESPACE

#define DECLARE_SCRIPT_REGISTATION() \
	static void DefineInScript(void);

#define BEGIN_DEFINE_SCRIPT_REGISTRATION(ClassName) \
	void ClassName::DefineInScript(void) \
	{

#define END_DEFINE_SCRIPT_REGISTRATION() \
	}

#define CALL_SCRIPT_REGISTRATION(ClassName) \
	ClassName::DefineInScript();

#ifdef EDIT_MODE
#define ADD_KEYWORD(TypeAsString) \
	ScriptTypesDefinition::GetInstance().AddKeyword(TypeAsString);
#else
#define ADD_KEYWORD(TypeAsString)
#endif

#define BEGIN_DEFINE_TYPE(Type, TypeAsString) \
	ADD_KEYWORD(TypeAsString) \
	module(VirtualMachine::GetInstance().GetState()) \
	[ \
		class_<Type>(TypeAsString)

#define BEGIN_DEFINE_TYPE_BASE(Base, Type, TypeAsString) \
	ADD_KEYWORD(TypeAsString) \
	module(VirtualMachine::GetInstance().GetState()) \
	[ \
		class_<Type, Base>(TypeAsString)

#define END_DEFINE_TYPE() \
	];

#define BEGIN_DEFINE_ENUMERATOR(Type, TypeAsString) \
	BEGIN_DEFINE_TYPE(Type, TypeAsString) \
		.enum_(TypeAsString) \
		[

#define END_DEFINE_ENUMERATOR() \
		] \
	END_DEFINE_TYPE()

#define DEFINE_ENUMERATOR_VALUE(Name, Value) \
	value(Name, Value)

#define DEFINE_CONSTRUCTOR(Pointer) \
	.def(Pointer)

#define DEFINE_FUNCTION(Name, Pointer) \
	.def(Name, Pointer)

#define DEFINE_PROPERTY(Name, Pointer) \
	.def_readwrite(Name, Pointer)

#define DEFINE_OPERATOR(Pointer) \
	.def(Pointer)
	
END_NAMESPACE