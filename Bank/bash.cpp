#include <cstdio>
#include <iostream>

#include "bash.h"

#define CLS			system("cls");
#define PAUSE		system("pause");
#define ERR_MSG		cout << "�����ʽ�������������룡\n";
#define FUNC_MSG	cout << "��ѡ��һ������:\n";

using namespace std;

namespace BASH
{
	void IO::mainWindow()
	{
		cout << "��ӭʹ�������Ż���" << endl;
		PAUSE;
		while (1)
		{
			welcome();
		}
	}

	void IO::welcome()
	{
		CLS;
		FUNC_MSG;
		cout << "1.���н���\n2.�˻�����\n3.���й���\n4.������������\n0.�˳�\n";
		cout << ">>";
		cin >> choice;
		switch (choice)
		{
		case 1:
			while(f1_trade());
			break;
		case 2:
			while(f2_account());
			break;
		case 3:
			while(f3_bank());
			break;
		case 4:
			f4_server();
			break;
		case 0:
			exit(0);
		default:
			ERR_MSG;
			PAUSE;
		}
	}

	int IO::f1_trade()
	{
		string password;
		//unsigned long long amount;
		double amount;
		CLS;
		cout << "���н��״���ϵͳ" << endl;

		cout << "��忨�����뿨�ţ���";
		cin >> str;
		if (!fc.FetchCard(str.c_str())) {
			cout << "���Ų����ڣ�\n";
			PAUSE;
			return 0;
		}
		cardID = fc.GetCardID(str.c_str());

		cout << "���������룺";
		cin >> str;
		if (!fc.VerifyCardSC(cardID, str)) {
			cout << "�������\n";
			return 0;
		}
		else {
			cout << "��֤ͨ����\n";
			PAUSE;
		}
		do {
			CLS;
			FUNC_MSG;
			cout << "1.���\n2.ȡ��\n3.��ѯ\n4.ת��\n5.�޸�����\n0.ȡ��\n";
			cout << ">>";
			cin >> choice;
			switch (choice) {
			case 1:
				cout << "����" << endl << ">>";
				if (cin >> amount) {
					if (fc.Deposit(cardID, amount * 100)) {
						cout << "����ɹ�!" << endl;
						cout << "���:\t\t" << fc.CardBalance(cardID) << endl;
					}
					else cout << "ʧ��" << endl;
				}
				else {
					cout << "��ʽ����\n";
					cin.clear();
					getchar();
				}
				PAUSE;
				break;
			case 2:
				cout << "ȡ���" << endl << ">>";
				if (cin >> amount) {
					if (fc.Withdraw(cardID, amount * 100)) {
						cout << "ȡ���ɹ�����ȡ�" << endl;
						cout << "���:\t\t" << fc.CardBalance(cardID) << endl;
					}
					else {
						cout << "���㣡" << endl;
					}
				}
				else {
					cout << "��ʽ����\n";
					cin.clear();
					getchar();
				}
				PAUSE;
				break;
			case 3:
				cout << "���п���Ϣ\n";
				cout << "���ţ�\t\t" << fc.GetCardNo(cardID) << "\t"
					 << fc.GetCardBankName(cardID) << endl;
				cout << "�ֿ��ˣ�\t" << fc.GetCardUserName(cardID) << endl;
				cout << "���:\t\t" << fc.CardBalance(cardID) << endl;
				PAUSE;
				break;
			case 4:
				cout << "δʵ��" << endl;
				PAUSE;
				break;
			case 5:
				do {
					cout << "�����������룺" << endl << ">>";
					cin >> password;
					cout << "���ٴ����������룺" << endl << ">>";
					cin >> str;
				} while (password != str);
				if (fc.ChangeCardSC(cardID, password))
					cout << "�޸ĳɹ���\n";
				PAUSE;
				return 1;
			case 0:
				break;
			default:
				ERR_MSG;
				PAUSE;
			}
		} while (choice != 0);
		
		return 0;
	}

	int IO::f2_account()
	{
		CLS;
		cout << "�û��˻�����ϵͳ\n";
		FUNC_MSG;
		cout << "1.����\n2.��������˻���Ϣ\n3.���ָ���˻���Ϣ\n0.����\n";
		cin >> choice;
		switch (choice) {
		case 1:
			f2_1_newCard();
			break;
		case 2:
			break;
		case 3:
			cout << "��ʹ�ý��״���ϵͳ" << endl;
			PAUSE;
			break;
		case 0:
			return 0;
		default:
			ERR_MSG;
			PAUSE;
		}
		return 1;
	}

	int IO::f3_bank()
	{
		CLS;
		cout << "���й���\n";
		FUNC_MSG;
		cout << "1.��������ʵ��\n2.������Ϣ\n0.����\n";
		cin >> choice; getchar();
		switch (choice) {
		case 1:
			fc.ListBanks();
			cout << "�������������ƣ�" << endl << ">>";
			getline(cin, str);
			fc.CreateBank(str.c_str());	// doesn't check duplication
			PAUSE;
			break;

		case 2:
			fc.ListBanks();
			// show bank status here
			PAUSE;
			break;

		case 0:
			return 0;
		default:
			ERR_MSG;
			PAUSE;
		}
		return 1;
	}

	void IO::f4_server()
	{
		CLS;
		getchar();
		PAUSE;
	}

	void IO::f2_1_newCard()
	{
		cout << "�������û�֤�����룺" << endl << ">>";
		cin >> str; getchar();
		if (fc.FetchUser(str.c_str())) { // user exists
			userID = fc.GetUserID(str.c_str());
			cout << "�����û���¼��\n������" << fc.GetUserName(userID) << endl;
			PAUSE;
		}
		else { // new user
			userID = fc.CreateUser(NULL, str.c_str());
			cout << "�������û�������" << endl << ">>";
			getline(cin, str);
			fc.UpdateUser(userID, str.c_str());
		}

		do {
			cout << "��ѡ�񿪿����У�" << endl;
			fc.ListBanks();
			cout << ">>";
			cin >> bankID;
		} while (!fc.ShowBank(bankID));
		
		cout << "�����뿨�ţ�" << endl << ">>";
		cin >> str;
		while (fc.FetchCard(str.c_str())) { // card number exists
			cout << "�Բ��𣬴˿��Ѵ��ڣ����������뿨�ţ�" << endl << ">>";
			cin >> str;
		}
		string cardNo = str;

		string password;
		do {
			cout << "���������룺" << endl << ">>";
			cin >> password;
			cout << "���ٴ��������룺" << endl << ">>";
			cin >> str;
		} while (password != str);

		fc.CreateCard(cardNo.c_str(), password, userID, bankID);
		PAUSE;
	}

}	// namespace BASH