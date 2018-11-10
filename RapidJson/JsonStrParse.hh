#ifndef _JSONSTR_PARSE_HH
#define _JSONSTR_PARSE_HH

namespace JsonParse
{

//get {"code":0,"msg":"success","data":{"sign":null,"token":"wfKPCvNs5c-zjH3JhS7HCg"}} token .
std::string getVehicleToken(const std::string &jsonStr);

}

#endif 