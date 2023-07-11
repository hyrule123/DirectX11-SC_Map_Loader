#pragma once

#include <regex>
#include <filesystem>

namespace stdfs = std::filesystem;

#ifndef DEBUG_BREAK
#ifdef _DEBUG
#define DEBUG_BREAK DebugBreak()
#else
#define DEBUG_BREAK
#endif
#endif


//#define PRESET constexpr inline const char*

#define PRESET(_VarNameBase, _String) \
namespace _VarNameBase {\
constexpr const char* A = _String;\
constexpr const wchar_t* W = L##_String;\
}


namespace define_Preset
{
	namespace Path
	{
		PRESET(ScriptProj, "./Project/Script");
		PRESET(EngineProj, "./Project/Engine");


#ifdef _DEBUG
		PRESET(Content, "./OutputFile/Bin_Debug/Content");
#else
		PRESET(Content, "./OutputFile/Bin_Release/Content");
#endif

		PRESET(strKey_Script, "strKey_Script.h");
		PRESET(strKey_CShader, "strKey_CShader.h");
		PRESET(strKey_GShader, "strKey_GShader.h");
		PRESET(strKey_Texture, "strKey_Texture.h");
		PRESET(strKey_Prefab, "strKey_Prefab.h");

		PRESET(UserClassInit_CS, "UserClassInitializer_CShader.cpp");
		PRESET(UserClassInit_Script, "UserClassInitializer_Script.cpp");
	}
	
	namespace Keyword
	{
		PRESET(ScriptPrefix, "CScript_");

		PRESET(IncludeBegin, "#include \"");

		PRESET(NameSpace, "namespace ");
		PRESET(ConstexprInlineConstChar, "PRESET(");
		PRESET(EqualDoubleQuotation, ", \"");
		PRESET(EnumClass, "enum class ");
		PRESET(strKey, "strKey_");

		PRESET(DefineSTRKEY, 
R"(#ifndef STRKEY
#define STRKEY constexpr inline const char*
#endif
)");


		PRESET(Head, R"(
#pragma once

#include <Engine/define.h>

//=========================================================
//This Code is Automatically generated by CodeGenerator.exe
//=========================================================

)");


		PRESET(define_T_Constructor_CS, 
			R"(#define CONSTRUCTOR_T(_Type) UserClassMgr::AddBaseCS(strKey_CShader::_Type, Constructor_Ptr_T<_Type>))"
		);
		PRESET(define_T_Constructor_Script, 
			R"(#define CONSTRUCTOR_T(_Type) UserClassMgr::AddBaseScript(strKey_Script::_Type, Constructor_Script_T<_Type>))"
		);

		PRESET(T_Constructor, "CONSTRUCTOR_T(");


	}

	namespace Regex
	{
		namespace arrCharsVarForbidden
		{
			constexpr inline const char* A[] =
			{
				"\\(","\\)",
				"\\{","\\}",
				"\\[","\\]",
				"\\."
			};

			constexpr inline const wchar_t* W[] =
			{
				L"\\(",L"\\)",
				L"\\{",L"\\}",
				L"\\[",L"\\]",
				L"\\."
			};

		}

		namespace g_VarForbiddenChars
		{
			extern std::regex A;
			extern std::wregex W;
			void CreateVarForbiddendRegex();
		}


		PRESET(CShader, R"(CCS_\w+\.h$)");
		PRESET(GShader, R"(\w+\.hlsl$)");

		//[   numthreads   ( %d, %d, %d )   ]
		//[ ] 안에 둘러싸여 있고, 공백 갯수에 상관없이 숫자 3개를 추출
		PRESET(Numthread, R"(\[\s*numthreads\s*\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\).*\])");
	}
}


#include <Engine/CComputeShader.h>
namespace define_ShaderGen
{
	struct tShaderSetting
	{
		stdfs::path FileName[(int)define_Shader::eGS_TYPE::END];
	};
}

