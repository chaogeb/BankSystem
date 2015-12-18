#include <cstdio>
#include <iostream>

#include "bash.h"

#define CLS			system("cls");
#define PAUSE		system("pause");
#define ERR_MSG		cout << "输入格式错误！请重新输入！\n";
#define FUNC_MSG	cout << "请选择一个功能:\n";

using namespace std;

namespace BASH
{
	void IO::mainWindow()
	{
		cout << "欢迎使用银行门户！" << endl;
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
		cout << "1.银行交易\n2.账户管理\n3.银行管理\n4.启动网银服务\n0.退出\n";
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
		cout << "银行交易处理系统" << endl;

		cout << "请插卡（输入卡号）：";
		cin >> str;
		if (!fc.FetchCard(str.c_str())) {
			cout << "卡号不存在！\n";
			PAUSE;
			return 0;
		}
		cardID = fc.GetCardID(str.c_str());

		cout << "请输入密码：";
		cin >> str;
		if (!fc.VerifyCardSC(cardID, str)) {
			cout << "密码错误！\n";
			return 0;
		}
		else {
			cout << "验证通过！\n";
			PAUSE;
		}
		do {
			CLS;
			FUNC_MSG;
			cout << "1.存款\n2.取款\n3.查询\n4.转账\n5.修改密码\n0.取卡\n";
			cout << ">>";
			cin >> choice;
			switch (choice) {
			case 1:
				cout << "存款金额：" << endl << ">>";
				if (cin >> amount) {
					if (fc.Deposit(cardID, amount * 100)) {
						cout << "存入成功!" << endl;
						cout << "余额:\t\t" << fc.CardBalance(cardID) << endl;
					}
					else cout << "失败" << endl;
				}
				else {
					cout << "格式错误\n";
					cin.clear();
					getchar();
				}
				PAUSE;
				break;
			case 2:
				cout << "取款金额：" << endl << ">>";
				if (cin >> amount) {
					if (fc.Withdraw(cardID, amount * 100)) {
						cout << "取出成功，请取款！" << endl;
						cout << "余额:\t\t" << fc.CardBalance(cardID) << endl;
					}
					else {
						cout << "余额不足！" << endl;
					}
				}
				else {
					cout << "格式错误\n";
					cin.clear();
					getchar();
				}
				PAUSE;
				break;
			case 3:
				cout << "银行卡信息\n";
				cout << "卡号：\t\t" << fc.GetCardNo(cardID) << "\t"
					 << fc.GetCardBankName(cardID) << endl;
				cout << "持卡人：\t" << fc.GetCardUserName(cardID) << endl;
				cout << "余额:\t\t" << fc.CardBalance(cardID) << endl;
				PAUSE;
				break;
			case 4:
				cout << "未实现" << endl;
				PAUSE;
				break;
			case 5:
				do {
					cout << "请输入新密码：" << endl << ">>";
					cin >> password;
					cout << "请再次输入新密码：" << endl << ">>";
					cin >> str;
				} while (password != str);
				if (fc.ChangeCardSC(cardID, password))
					cout << "修改成功！\n";
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
		cout << "用户账户管理系统\n";
		FUNC_MSG;
		cout << "1.开卡\n2.输出所有账户信息\n3.输出指定账户信息\n0.返回\n";
		cin >> choice;
		switch (choice) {
		case 1:
			f2_1_newCard();
			break;
		case 2:
			break;
		case 3:
			cout << "请使用交易处理系统" << endl;
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
		cout << "银行管理\n";
		FUNC_MSG;
		cout << "1.增加银行实体\n2.银行信息\n0.返回\n";
		cin >> choice; getchar();
		switch (choice) {
		case 1:
			fc.ListBanks();
			cout << "输入新银行名称：" << endl << ">>";
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
		cout << "请输入用户证件号码：" << endl << ">>";
		cin >> str; getchar();
		if (fc.FetchUser(str.c_str())) { // user exists
			userID = fc.GetUserID(str.c_str());
			cout << "发现用户记录！\n姓名：" << fc.GetUserName(userID) << endl;
			PAUSE;
		}
		else { // new user
			userID = fc.CreateUser(NULL, str.c_str());
			cout << "请输入用户姓名：" << endl << ">>";
			getline(cin, str);
			fc.UpdateUser(userID, str.c_str());
		}

		do {
			cout << "请选择开卡银行：" << endl;
			fc.ListBanks();
			cout << ">>";
			cin >> bankID;
		} while (!fc.ShowBank(bankID));
		
		cout << "请输入卡号：" << endl << ">>";
		cin >> str;
		while (fc.FetchCard(str.c_str())) { // card number exists
			cout << "对不起，此卡已存在，请重新输入卡号：" << endl << ">>";
			cin >> str;
		}
		string cardNo = str;

		string password;
		do {
			cout << "请输入密码：" << endl << ">>";
			cin >> password;
			cout << "请再次输入密码：" << endl << ">>";
			cin >> str;
		} while (password != str);

		fc.CreateCard(cardNo.c_str(), password, userID, bankID);
		PAUSE;
	}

}	// namespace BASH