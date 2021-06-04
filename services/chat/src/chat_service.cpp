
#include <iostream>

#include "chat.h"

int main(int argc, char* argv[]){
  

  /* Start gRPC service */
  ToolboxServiceImpl tbImpl;
  CommonTools::RunServer(&tbImpl, DeviceToolBox::service_full_name(), "jwt");

  return 0;
}
