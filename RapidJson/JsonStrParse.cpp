#include "rapidjson/document.h"
#include "JsonStrParse.hh"
#include "Logger.hh"

std::string JsonParse::getVehicleToken(const std::string &jsonStr)
{
	rapidjson::Document document;
	if (document.Parse(jsonStr.c_str()).HasParseError() || !document.HasMember("data")) 
	{
		LOG_ERROR << "getVehicleToken error , document.Parse error or unHasMember data";
		return "";
	}
 
	const rapidjson::Value &jsonValueObject = document["data"];
	if(!jsonValueObject.IsObject())
	{
		LOG_ERROR << "getVehicleToken error , jsonValueObject not a Object";
		return "";
	}
 
	if(!jsonValueObject.HasMember("token"))
	{
		LOG_ERROR << "getVehicleToken error , jsonValueObject have't Member Object";
		return "";
	}
	
	const rapidjson::Value &jsonValue = jsonValueObject["token"];
	
	return jsonValue.GetString();
}
