#include <iostream>
#include <thread>
#include "DevCom.h"
#include "FactoryManager.h"
#include "CommandFactory.h"
#include "tcp_service_808.h"
using namespace std;

int main(int argc, char *argv[])
{
	int ret = 0;
	DevCom *com = new DevCom;
	ret = com->Open();
	if(ret < 0)
		cout << "Open Com fail" << endl;

	std::thread com_send(&DevCom::SendThread, com);
	std::thread com_recv(&DevCom::RecvThread, com);

	ret = FactoryManager::CreateFactory();
	if(ret < 0)
		cout << "Create Factory fail" << endl;

	std::thread com_command_loop(&CommandFactory::EventLoop, FactoryManager::GetFactory(DEV_COM));
	std::thread net_command_loop(&CommandFactory::EventLoop, FactoryManager::GetFactory(DEV_NET));

    std::thread t1([]() {
        TcpService808& service = Singleton<TcpService808>::instance();
        service.run();
    });

	com_send.join();
	com_recv.join();
	t1.join();

	return 0;
}