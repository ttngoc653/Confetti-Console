// ConfettiClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConfettiClient.h"
#include<afxsock.h>
#include<thread>
#include<string>
#include<string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

CSocket client;
char str_ip[16];

void program() {
	AfxSocketInit(NULL);
	
	char msg[255];
	int length_msg = 0;

	cout << "Please input IP address of server: ";
	cin.getline(str_ip,254);

	cout << "Nhap ten nguoi choi: ";
	while (true)
	{
		client.Create();

		client.Connect(CA2W(str_ip), 1760);

		cin.getline(msg, 254);

		length_msg = strlen(msg);

		client.Send(&length_msg, sizeof(int), 0);
		client.Send(msg, length_msg, 0);

		client.Receive(&length_msg, sizeof(int), 0);
		client.Receive(msg, length_msg, 0);
		msg[length_msg] = '\0';

		cout << "Ket qua trung: " << msg << endl;

		if (strcmp("1", msg) == 0)
		{
			cout << "Hien tai dang cho du nguoi moi choi duoc nhe." << endl;
			break;
		}
		else
		{
			cout << "Bi trung ten. Yeu cau nhap lai ten nguoi choi: ";
			client.Close();
		}
	}
	while (true)
	{

	}

	client.Close();
	/*while (true)
	{
		cout << "Input message: ";
		cin.getline(msg, 254);
		length_msg = strlen(msg);
		client.Send(&length_msg, sizeof(int), 0);
		client.Send(msg, length_msg, 0);

		client.Receive(&length_msg, sizeof(int), 0);
		client.Receive(msg, length_msg, 0);

		msg[length_msg] = '\0';
		cout << "Receive: " << msg << endl;
	//	delete msg;
	}*/

	client.Close();
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
