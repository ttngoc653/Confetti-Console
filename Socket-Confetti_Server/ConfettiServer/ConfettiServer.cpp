// ConfettiServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConfettiServer.h"
#include<afxsock.h>
#include<stdlib.h>
#include<vector>
#include<thread>
#include<fstream>

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

struct Question
{
	char question[254];
	char answerfirst[254];
	char answersecond[254];
	char answerthirs[254];
	char answerfouth[254];
	char answertrue;
};

CSocket server;
CSocket * clients;
vector<InfoUser *> users;
vector<Question *> questions;
int count_user = 0, count_question = 0, users_size_current = 0;
int temp = 1;

char msg[255];
int length_msg = 0;
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
		if (strcmp(users.at(i)->name, username) == 0)
		{
			return true;
		}
	}
	return false;
}

void readQuestionFromFile()
{
	fstream in;
	in.open("data1.txt");
	cout << "Doc file" << endl;
	if (!in.is_open()) {
		cout << " TAP TIN CHUA DU LIEU KHONG TON TAI." << endl;
		return;
	}
	while (!in.eof() && in.is_open())
	{
		Question *p = (Question *)malloc(sizeof(Question));
		char temp[256];
		in.getline(p->question, 256, '\n');
		cout << p->question << endl;
		in.getline(p->answerfirst, 256, '\n');
		cout << p->answerfirst << endl;
		in.getline(p->answersecond, 256, '\n');
		cout << p->answersecond << endl;
		in.getline(p->answerthirs, 256, '\n');
		cout << p->answerthirs << endl;
		in.getline(p->answerfouth, 256, '\n');
		cout << p->answerfouth << endl;
		in.getline(temp, 256, '\n');
		cout << temp << endl;
		p->answertrue = temp[0];
		questions.push_back(p);
		if (in.eof())
			break;
	}
	in.close();
}


void checkConnect() {
	while (temp != 500)
	{
		for (int i = 0; i < count_user; i++)
		{
			if (clients[i] == NULL)
			{
				if (server.Accept(clients[i])) {
					clients[i].Receive(&length_msg, sizeof(int), 0);
					clients[i].Receive(msg, length_msg, 0);

					msg[length_msg] = '\0';

					cout << "Da nhan ten: " << msg;

					temp = 1;
					if (!checkEqualUser(msg))
					{
						cout << endl;
						cout << "Nguoi choi thu " << i << " da ket noi" << endl;

						clients[i];

						InfoUser *info = (InfoUser *)malloc(sizeof(InfoUser));

						info->name = (char *)malloc(length_msg * sizeof(int));

						strcpy(info->name, msg);
						info->socaudung = 0;
						users.push_back(info);

						clients[i].Send(&temp, sizeof(int), 0);
						clients[i].Send("1", 1, 0);
						users_size_current++;
						i++;
					}
					else
					{
						cout << " bi trung ten" << endl;

						clients[i].Send(&temp, sizeof(int), 0);
						clients[i].Send("0", 1, 0);

						clients[i].Close();
					}
				}
			}
		}
	}
}

void program(){
	readQuestionFromFile();

	AfxSocketInit(NULL);

	getIp();

	server.Create(1760);

	server.Listen(5);

	cout << "Nhap so nguoi choi: " << endl;
	cin >> count_user;

	cout << "Nhap so cau hoi: " << endl;
	cin >> count_question;

	clients = new CSocket[count_user];

	cout << "Waiting for client" << endl;
	for (int i = 0; i < count_user;)
	{
		if (server.Accept(clients[i])) {
			cout << "Co ket noi toi." << endl;
		}

		// nhan ten nguoi choi
		clients[i].Receive(&length_msg, sizeof(int), 0);
		clients[i].Receive(msg, length_msg, 0);
		msg[length_msg] = '\0';

		cout << "Da nhan ten: " << msg;
		
		temp = 1;
		if (!checkEqualUser(msg))
		{
			cout << endl;
			cout << "Nguoi choi thu " << i << " da ket noi" << endl;

			clients[i];
			
			InfoUser *info=(InfoUser *)malloc(sizeof(InfoUser));

			info->name = (char *)malloc(length_msg * sizeof(int));

			strcpy(info->name, msg);
			info->socaudung = 0;
			users.push_back(info);
			
			// gui so luong cau hoi
			sprintf(msg, "%d", count_question);
			temp = strlen(msg);
			clients[i].Send(&temp, sizeof(int), 0);
			clients[i].Send(msg, 1, 0);
			users_size_current++;
			i++;
		}
		else
		{
			cout << " bi trung ten" << endl;
			temp = 2;

			// gui ket qua trung ve (-1)
			clients[i].Send(&temp, sizeof(int), 0);
			clients[i].Send("-1", 1, 0);

			clients[i].Close();
		}
	}

	thread threadCheck(checkConnect);

	for (int i = 0; i < count_question; i++)
	{
		int rand_question = rand() % questions.size();
		temp = sizeof(Question);

		// gui cau hoi cho tat ca client con hoat dong
		for (int i = 0; i < count_user; i++)
		{
			if (clients[i]!=NULL)
			{
				// gui cau hoi cho nguoi choi
				clients[i].Send(&temp, sizeof(int));
				clients[i].Send(questions.at(rand_question), temp);
			}
		}

		// nhan cau tra loi cua tat ca client
		for (int i = 0; i < count_user; i++)
		{
			if (clients[i]!=NULL)
			{
				// nhan cau tra loi cua nguoi choi
				clients[i].Receive(&length_msg, sizeof(int));
				clients[i].Receive(msg, length_msg);
				msg[length_msg] = '\0';

				if (msg[0] == questions.at(rand_question)->answertrue)
				{
					users[i]->socaudung++;
				}
			}
		}
	}

	// tim so cau tra loi cao nhat cung nhu so nguoi choi tra loi coan hat
	int maxpoint = 0, count_user_max_point = 0;
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i]->socaudung>users[maxpoint]->socaudung)
		{
			maxpoint = i;
			count_user_max_point = 1;
		}
		else if (users[i]->socaudung == users[maxpoint]->socaudung)
		{
			count_user_max_point++;
		}
	}

	// gui ket qua choi cho nguoi dung
	for (int i = 0; i < count_user; i++)
	{
		if (clients[i]!=NULL)
		{
			// gui so nguoi choi co diem cao nhat
			sprintf(msg, "%d", count_user_max_point);
			length_msg=strlen(msg);
			clients[i].Send(&length_msg, sizeof(int));
			clients[i].Send(msg, length_msg);

			// gui diem cao nhat
			sprintf(msg, "%d", maxpoint);
			length_msg = strlen(msg);
			clients[i].Send(&length_msg, sizeof(int));
			clients[i].Send(msg, length_msg);

			// gui danh sach ten nguoi choi co diem cao nhat
			for (int i = 0; i < users.size(); i++)
			{
				if (users.at(i)->socaudung == users.at(maxpoint)->socaudung)
				{
					length_msg = strlen(users[i]->name);
					clients[i].Send(&length_msg, sizeof(int));
					clients[i].Send(users[i]->name, length_msg);
				}
			}
		}
	}

	temp = 500;
	threadCheck.join();
	
	for (int i = 0; i < users_size_current; i++)
	{
		clients[i].Close();
	}
	server.Close();

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
