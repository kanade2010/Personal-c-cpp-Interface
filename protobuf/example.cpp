#include "user_message.pb.h"
#include <string>
#include <iostream>

int main()
{
  sola::user_info user_info{};
  user_info.set_uid(1);
  user_info.set_name("sola");
  user_info.set_login_in(123456);
  user_info.set_login_out(654321);

  // encode --> bytes stream
  std::string out;
  user_info.SerializeToString(&out);

  std::cout << out.size() << std::endl;
  for(int i = 0; i < out.size(); i++ ){
    printf("%02x ", out[i]);
  }
  std::cout << std::endl;
  //getchar();
  // decode --> user_info structure
  sola::user_info t;
  t.ParseFromArray(out.c_str(), out.size()); // or parseFromString

  std::cout << t.uid() << " " << t.name() << " " << t.login_in() << " " << t.login_out() << std::endl;


  return 0;
}