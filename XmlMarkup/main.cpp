#include "Markup.hh"

bool CheckXml(CMarkup &xml, bool &isNeedUpgreade, std::string &downloadAddrId)
{

  bool ret = false;
  //LOG_DEBUG << xml.GetDoc() << std::endl;
  if(xml.FindChildElem("soap:Body"))
  {
    LOG_DEBUG << "CheckXml() : Find Elem soap:Body";
    xml.IntoElem();
    if(xml.FindChildElem("ns2:getPublicSoftMaxVersionByNameResponse"))
    {
      LOG_DEBUG << "CheckXml() : Find Elem ns2:getPublicSoftMaxVersionByNameResponse";
      xml.IntoElem();
      if(xml.FindChildElem("return"))
      {
        LOG_DEBUG << "CheckXml() : Find Elem return";
        xml.IntoElem();
        if(xml.FindChildElem("code"))
        {
          LOG_DEBUG << "CheckXml() : Find Elem code";
          std::string code;
          if( (code = xml.GetChildData()) != "0"){
            if(code == "405") LOG_ERROR << "Server : This device is not supported";
            else
            LOG_ERROR << "CheckXml() : check code error, code is " << code;
            return false;
          }
          if(xml.FindChildElem("SoftMaxVersion"))
          {
            LOG_DEBUG << "CheckXml() : Find Elem SoftMaxVersion";
            xml.IntoElem();
            if(xml.FindChildElem("forceUpgrade"))
            {
              if(xml.GetChildData() == "1")
                isNeedUpgreade = true;
              else
                isNeedUpgreade = false;

              LOG_DEBUG << "CheckXml() : forceUpgrade : " << isNeedUpgreade;

              if(xml.FindChildElem("versionDetailId"))
              {
                downloadAddrId = xml.GetChildData();
                LOG_DEBUG << "CheckXml() : versionDetailId : " << downloadAddrId.c_str();
                ret = true;
              }
            }
            if(xml.FindChildElem("versionNo"))
                LOG_DEBUG << "CheckXml() : versionNo : " << xml.GetChildData().c_str();
          }
        }
      }
    }
  }

  return ret;
}

int main()
{

  return 0;
}