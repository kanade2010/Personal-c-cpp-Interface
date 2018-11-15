#include <string>
#include "JsonStrParse.hh"
#include "Logger.hh"

int main()
{
  std::string jsonStr = "{\"code\":0,\"msg\":\"success\",\"data\":{\"sign\":null,\"token\":\"wfKPCvNs5c-zjH3JhS7HCg\"}}";

  std::string token = JsonParse::getVehicleToken(jsonStr);

  LOG_DEBUG << "token : " << token;

  std::cout << std::endl;
}