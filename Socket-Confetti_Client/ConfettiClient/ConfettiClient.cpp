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
Question * p; char reply[] = "_____", answer;
char msg[255];
int length_msg = 0, number_question = 0, user_id = -1;
int question_current = 0;
bool replied = false;

void xuatCauHoi() {
	while (question_current < number_question)
	{
		p = (Question *)malloc(sizeof(Question));
		if (1 != client.Receive(&question_current, sizeof(int), 0)) {
			client.Receive(&length_msg, sizeof(int), 0);

			if (question_current != 0 && replied)
				cout << "Ban da tra loi " << ((answer != reply[0]) ? "sai" : "dung") << endl;

			client.Receive(p, length_msg, 0);

			cout << "Cau hoi thu " << question_current << ": " << endl;
			cout << p->question << endl;
			cout << "\ta. " << p->answerfirst << endl;
			cout << "\tb. " << p->answersecond << endl;
			cout << "\tc. " << p->answerthirs << endl;
			cout << "\td. " << p->answerfouth << endl;
			cout << "Bat dau tinh gio." << endl;
			cout << "Cau tra loi cua ban (a, b, c hoac d): ";

			replied = false;

			Sleep(60000);

			if (!replied)
			{
				cout << endl << "Het thoi gian tra loi." << endl;
				strcpy(reply, "e");
			}

			answer = p->answertrue;

			// gui ma nguoi choi hien tai
			client.Send(&user_id, sizeof(int), 0);
			//gui cau tra loi
			client.Send(reply, 1, 0);
		}
	}
}

void guiCauTraLoi() {
	while (number_question>question_current)
	{
		cin >> reply;
		replied = true;
		cout << "Cho nguoi cung choi tra loi!" << endl;
	}
}

void program() {
	AfxSocketInit(NULL);
	
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

		// nhan so luong cau hoi can tra lop hoac -1 neu bi trung
		client.Receive(&number_question, sizeof(int), 0);
		if (number_question != -1)
		{
			client.Receive(&user_id, sizeof(int), 0);

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
	thread showQuestion(xuatCauHoi);
	thread inputReply(guiCauTraLoi);
	showQuestion.join();
	inputReply.detach();
	
	// nhan so nguoi choi co diem cao
	client.Receive(&length_msg, sizeof(int));

	cout << "Ban da tra loi " << ((answer != reply[0]) ? "sai" : "dung") << endl;

	cout << "So cau hoi ma nguoi chien thang tra loi dung: " << length_msg<<endl;
	
	int max_user = 0;
	// nhan so nguoi tra loi dung nhieu nhat
	client.Receive(&max_user, sizeof(int));
	
	cout << "Co " << max_user << " nguoi choi tra loi dung nhieu cau nhat:" << endl;
	for (int i = 0; i < max_user; i++)
	{
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
