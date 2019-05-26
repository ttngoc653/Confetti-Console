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

struct Question
{
	char question[254];
	char answerfirst[254];
	char answersecond[254];
	char answerthirs[254];
	char answerfouth[254];
	char answertrue;
};

// The one and only application object

CWinApp theApp;

using namespace std;

CSocket client;
char str_ip[16];

void program() {
	AfxSocketInit(NULL);
	
	char msg[255];
	int length_msg = 0, number_question = 0;

	cout << "Please input IP address of server: ";
	cin.getline(str_ip,254);

	cout << "Nhap ten nguoi choi: ";
	while (true)
	{
		client.Create();

		client.Connect(CA2W(str_ip), 1760);

		// nhap ten nguoi choi
		cin.getline(msg, 254);

		length_msg = strlen(msg);
		
		// gui ten nguoi dung
		client.Send(&length_msg, sizeof(int), 0);
		client.Send(msg, length_msg, 0);

		// nhan so luong cau hoi hoac -1 neu bi trung
		client.Receive(&length_msg, sizeof(int), 0);
		client.Receive(msg, length_msg, 0);
		msg[length_msg] = '\0';

		cout << "Ket qua trung: " << msg << endl;

		if (!strcmp("-1", msg) == 0)
		{
			number_question = atoi(msg);
			cout << "Ban can tra loi " << number_question << " cau hoi." << endl;
			cout << "Hien tai dang cho du nguoi moi choi duoc nhe." << endl;
			break;
		}
		else
		{
			cout << "Bi trung ten. Yeu cau nhap lai ten nguoi choi: ";
			client.Close();
		}
	}

	// nhan cau hoi va tra loi cau hoi
	Question * p; char reply[] = "_____", answer;
	for (int i = 0; i < number_question; i++)
	{
		p = (Question *)malloc(sizeof(Question));
		client.Receive(&length_msg, sizeof(int), 0);

		if (i != 0)
			cout << "Ban da tra loi " << ((answer != reply[0]) ? "sai" : "dung") << endl; 
		
		client.Receive(p, length_msg, 0);


		cout << "Cau hoi thu " << i+1 << ": " << endl;
		cout << p->question << endl;
		cout << "\ta. " << p->answerfirst << endl;
		cout << "\tb. " << p->answersecond << endl;
		cout << "\tc. " << p->answerthirs << endl;
		cout << "\td. " << p->answerfouth << endl;
		cout << "Bat dau tinh gio." << endl;
		cout << "Cau tra loi cua ban (a, b, c hoac d): ";
		cin >> reply;

		answer = p->answertrue;
		
		length_msg = strlen(reply);
		client.Send(&length_msg, sizeof(int), 0);
		client.Send(reply, length_msg, 0);
	}

	client.Receive(&length_msg, sizeof(int));

	cout << "Ban da tra loi " << ((answer != msg[0]) ? "sai" : "dung") << endl;
	
	client.Receive(msg, length_msg);

	int count_user_answer_most_true = atoi(msg);

	cout << "Co " << msg << " nguoi choi tra loi dung nhieu cau nhat:" << endl;
	for (int i = 0; i < count_user_answer_most_true; i++)
	{
		client.Receive(&length_msg, sizeof(int));
		client.Receive(msg, length_msg);

		client.Receive(&length_msg, sizeof(int));
		client.Receive(msg, length_msg);
		msg[length_msg] = '\0';

		cout << "\t" << msg << endl;
	}

	cout << "Ket thuc tro choi" << endl;

	client.Close();

	cin.getline(msg, 254);
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
