// ConfettiServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConfettiServer.h"
#include<afxsock.h>
#include<stdlib.h>
#include<vector>
#include<thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;

struct InfoUser
{
	char * name;
	int socaudung;
};

CSocket server;
vector<CSocket> clients;
vector<InfoUser *> users;
int count_user = 0, count_question = 0;
int temp = 1;

/*source: https://stackoverflow.com/questions/49335001/get-local-ip-address-in-c*/
void getIp()
{
	const char* google_dns_server = "8.8.8.8";
	int dns_port = 53;

	struct sockaddr_in serv;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	//Socket could not be created
	if (sock < 0)
	{
		std::cout << "Socket error" << std::endl;
	}

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(google_dns_server);
	serv.sin_port = htons(dns_port);

	int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
	if (err < 0)
	{
		std::cout << "Error number: " << errno
			<< ". Error message: " << strerror(errno) << std::endl;
	}

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*)&name, &namelen);

	char buffer[15], temp[15];
	const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
	if (p != NULL)
	{
		std::cout << "Local IP address is: " << buffer << std::endl;
	}
	else
	{
		std::cout << "Error number: " << errno
			<< ". Error message: " << strerror(errno) << std::endl;
	}
}

boolean checkEqualUser(char * username) {
	for (int i = 0; i < users.size(); i++)
	{
		if (strcmp(users.at(i)->name,username)==0)
		{
			return true;
		}
	}
	return false;
}

void checkConnect() {
	while (temp != 500)
	{
		for (int i = 0; i < clients.size(); i++)
		{
			if (clients.at(i)==NULL)
			{
				cout << "Client " << i << "disconnected!!!" << endl;
				//clients.erase(clients.begin() + i);
			}
		}
	}
}

void program(){
	
	char msg[255];
	int length_msg = 0;


	AfxSocketInit(NULL);

	getIp();

	server.Create(1760);

	server.Listen();

	cout << "Nhap so nguoi choi: " << endl;
	cin >> count_user;

	cout << "Waiting for client" << endl;
	for (int i = 0; i < count_user;)
	{
		CSocket clientindex;
		if (server.Accept(clientindex)) {
			cout << "Co ket noi toi." << endl;
		}

		clientindex.Receive(&length_msg, sizeof(int), 0);
		clientindex.Receive(msg, length_msg, 0);

		msg[length_msg] = '\0';

		cout << "Da nhan ten: " << msg;

		if (!checkEqualUser(msg))
		{
			cout << endl;
			cout << "Nguoi choi thu " << i << " da ket noi" << endl;

			//clients.push_back(clientindex);
			
			InfoUser *info=(InfoUser *)malloc(sizeof(InfoUser));

			info->name = (char *)malloc(length_msg * sizeof(int));

			strcpy(info->name, msg);
			info->socaudung = 0;
			users.push_back(info);
			
			clientindex.Send(&temp, sizeof(int), 0);
			clientindex.Send("1", 1, 0);

			i++;
		}
		else
		{
			cout << " bi trung ten" << endl;

			clientindex.Send(&temp, sizeof(int), 0);
			clientindex.Send("0", 1, 0);
		}
	}

	thread threadCheck(checkConnect);

	for (int i = 0; i < count_question; i++)
	{
		for (int i = 0; i < count_user; i++)
		{

		}
	}

	temp = 500;
	threadCheck.join();
	
	while(clients.size()!=0)
	{
		clients.at(clients.size()-1).Close();
		clients.pop_back();
	}


	server.Close();

}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: change error code to suit your needs
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
			program();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
