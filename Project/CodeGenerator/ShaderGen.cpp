#include "pch.h"

#include "CodeGenFunc.h"

#include "MacroFunc.h"


#include "StringConvert.h"

#include <UtilLib_DLL/json/json.h>
#include <Engine/define.h>
#include <d3dcommon.h>

struct tShaderInfo
{
	string strShaderName;
	int flagPipelineStage;
};

void InitializeShaderInfoJson(Json::Value& _jVal);

void CreateShaderCode()
{
	//first(Key) = json파일의 이름
	map<string, tShaderInfo> mapShaderInfo;

	filesystem::path ShaderDir(u8"./OutputFile/Bin_");
#ifdef _DEBUG
	ShaderDir += "Debug";
#else
	ShaderDir += "Release";
#endif
	ShaderDir /= "Content/Shader";

	filesystem::path GSShaderDir = ShaderDir / "Graphics";

	//Content/Shader/Graphics 폴더 안의 파일을 순회돌아준다.
	filesystem::directory_iterator dirIter(GSShaderDir);
	filesystem::directory_iterator dirIterEnd = std::filesystem::end(dirIter);
	for (dirIter; dirIter != dirIterEnd; ++dirIter)
	{
		const filesystem::directory_entry& file = *dirIter;

		if (false == file.is_directory())
		{
			//파일명을 받아온다.
			string shaderName = file.path().filename().string();

			//확장자가 .cso로 끝나는지 확인한다. 확장자가 cso가 아닐 경우 쉐이더가 아니므로 continue;
			size_t strPos = shaderName.find(JSON_SHADERINFO::strShaderExtension);
			if (string::npos == strPos)
				continue;

			//.cso 확장자를 제거한다.
			shaderName.erase(strPos, string::npos);

			//이름을 통해서 어떤 파이프라인인지를 유추한다.
			int flagPipeline = (int)eSHADER_PIPELINE_STAGE::__NONE;
			for (int i = 0; i < (int)eSHADER_TYPE::END; ++i)
			{
				if (string::npos != shaderName.find(JSON_SHADERINFO::GRAPHICS_SHADER::arrName[i]))
				{
					flagPipeline |= 1 << i;
					break;
				}
			}
			
			//마지막 "_" 위치를 찾아서 쉐이더 관련 정보를 지우고, 이름만 남겨준다.
			//Key 및 Value로 사용할 예정임.
			strPos = shaderName.find_last_of("_");
			if (string::npos != strPos)
			{
				//마지막 _ 위치까지 지워준다.(쉐이더의 이름만 남겨 준다.)
				shaderName.erase((size_t)0, strPos + (size_t)1);
			}
			
			//처음 발견한 쉐이더 이름일 경우 NONE 값을 넣고 초기화해준다.
			auto mapIter = mapShaderInfo.find(shaderName);
			if (mapIter == mapShaderInfo.end())
			{
				mapShaderInfo[shaderName] = tShaderInfo{ shaderName, eSHADER_PIPELINE_STAGE::__NONE };
				
				//새로 생성한 값을 iterator에 등록해준다.(아래에서도 사용함)
				mapIter = mapShaderInfo.find(shaderName);
				mapIter->second.strShaderName += JSON_SHADERINFO::strShaderExtension;
			}

			//발견한 쉐이더 파일의 파이프라인단계를 플래그값에 추가한다.
			mapIter->second.flagPipelineStage |= flagPipeline;
		}
	}
	//생성된 HLSL 파일 순회 끝



	bool bNewShaderDetected = false;
	//map을 순회돌아주면서 파일을 열어본다.
	//파일명은 'Key값'이다.
	Json::Value JsonInfo;
	for (const auto& mapIter : mapShaderInfo)
	{
		//JSON 경로 생성
		filesystem::path JsonPath = GSShaderDir;
		JsonPath /= mapIter.first;
		JsonPath += JSON_SHADERINFO::strJsonExtension;

		ifstream fpJsonIn(JsonPath);
		//파일이 열렸으면 데이터를 읽어온다.
		if (true == fpJsonIn.is_open())
		{
			fpJsonIn >> JsonInfo;
			fpJsonIn.close();
		}
		//파일이 없을 경우 JsonValue를 포맷에 맞게 초기화시킨다.
		else
		{
			InitializeShaderInfoJson(JsonInfo);

			//이름 삽입
			JsonInfo[JSON_SHADERINFO::COMMON_VAL::strShaderName] = mapIter.second.strShaderName;

			//또한 새로운 쉐이더 파일이 발견되었을경우 값을 설정할수 있도록 MessageBox를 띄워준다.
			bNewShaderDetected = true;
		}

		//데이터 확인 후 쉐이더 파이프라인 단계에 차이점이 있을 경우 파이프라인 단계값을 갱신하고 파일로 내보낸다.
		//변경점이 없으면 파일로 내보내지 않는다.
		int origFlag = JsonInfo[JSON_SHADERINFO::COMMON_VAL::ePipelineFlag].asInt();
		
		if (mapIter.second.flagPipelineStage != origFlag)
		{
			ofstream fpJsonOut(JsonPath);
			if (true == fpJsonOut.is_open())
			{
				JsonInfo[JSON_SHADERINFO::COMMON_VAL::ePipelineFlag] = mapIter.second.flagPipelineStage;
				fpJsonOut << JsonInfo;
				fpJsonOut.close();
			}
		}
	}


	//새로운 쉐이더를 발견했을 경우 strKeysShader.h 파일도 다시 작성해준다.
	if (true == bNewShaderDetected)
	{
		std::ofstream fpStrKeyShader("./Project/Script/strKeyShader.h");
		if (true == fpStrKeyShader.is_open())
		{
			string RawLiteral = R"(
#pragma once

//This Code is Automatically generated by CodeGenerator.exe

namespace SHADERS
)";
			fpStrKeyShader << RawLiteral;

			WriteCodeA(fpStrKeyShader, "{");
			WriteCodeA(fpStrKeyShader, "namespace GRAPHICS");
			WriteCodeA(fpStrKeyShader, "{");
			
			for(const auto& GSIter : mapShaderInfo)
			{
				string codeLine = "constexpr const char* ";

				//JSON 파일 명
				string strKeyUpper;
				strKeyUpper.resize(GSIter.first.size());
				transform(GSIter.first.begin(), GSIter.first.end(), strKeyUpper.begin(), ::toupper);
				codeLine += strKeyUpper;
				codeLine += " = \"";

				//CSO 파일 명
				codeLine += GSIter.first + JSON_SHADERINFO::strJsonExtension + "\";";

				WriteCodeA(fpStrKeyShader, codeLine);
			}
			WriteCodeA(fpStrKeyShader, "}");
			WriteCodeA(fpStrKeyShader);


			WriteCodeA(fpStrKeyShader, "namespace COMPUTE");
			WriteCodeA(fpStrKeyShader, "{");


			filesystem::path CSShaderDir(ShaderDir);
			CSShaderDir /= "Compute";
			
			dirIter = filesystem::directory_iterator(CSShaderDir);
			for(;false == dirIter._At_end(); ++dirIter)
			{	
				if (false == (*dirIter).is_directory())
				{
					string codeLine = "constexpr const char* ";

					
					string Key = (*dirIter).path().filename().string();

					string Value = Key;

					//앞의 _와 뒤의 확장자를 지우고 대문자로 만들어서 키값으로 변환해준다.
					size_t strPos = Key.find(JSON_SHADERINFO::strShaderExtension);
					if (string::npos != strPos)
					{
						Key.erase(strPos, string::npos);
					}
					strPos = Key.find_last_of("_");
					if (string::npos != strPos)
					{
						Key.erase((size_t)0, strPos + (size_t)1);
					}

					transform(Key.begin(), Key.end(), Key.begin(), ::toupper);

					codeLine += Key;
					codeLine += " = \"";
					codeLine += Value + "\";";

					WriteCodeA(fpStrKeyShader, codeLine);
				}
			}
			WriteCodeA(fpStrKeyShader, "}");

			WriteCodeA(fpStrKeyShader, "}");
			WriteCodeA(fpStrKeyShader);

			fpStrKeyShader.close();
		}


		MessageBoxA(nullptr, "New Shader Detected.\nDo not close this message box until adjust the values.", "Notification", MB_OK);
	}


}


void InitializeShaderInfoJson(Json::Value& _jVal)
{
	_jVal.clear();

	string comment = "//Enumeration Values are at define.h of Engine project";
	_jVal.setComment(comment, Json::CommentPlacement::commentBefore);

	_jVal[JSON_SHADERINFO::COMMON_VAL::strShaderName] = "";

	_jVal[JSON_SHADERINFO::COMMON_VAL::ePipelineFlag] = eSHADER_PIPELINE_STAGE::__NONE;

	_jVal[JSON_SHADERINFO::GRAPHICS_SHADER::eTopology] = (int)D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	_jVal[JSON_SHADERINFO::GRAPHICS_SHADER::eRSState] = (int)eRASTERIZER_TYPE::CULL_BACK;
	_jVal[JSON_SHADERINFO::GRAPHICS_SHADER::eDSState] = (int)eDEPTHSTENCIL_TYPE::LESS;
	_jVal[JSON_SHADERINFO::GRAPHICS_SHADER::eBState] = (int)eBLENDSTATE_TYPE::DEFAULT;
	_jVal[JSON_SHADERINFO::GRAPHICS_SHADER::eShaderDomain] = (int)eSHADER_DOMAIN::_UNDEFINED;
}
