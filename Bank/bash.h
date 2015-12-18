#pragma once

#include "Facade.h"

using namespace CONTROLLER;

namespace BASH
{
	class IO
	{
	public:
		void mainWindow();

	private:
		void welcome();

		int f1_trade();
		int f2_account();
		int f3_bank();
		void f4_server();

		void f2_1_newCard();

	private:
		FacadeController fc;
		BankID bankID;
		CustomerID userID;
		CardID cardID;
		string str;
		unsigned int choice;
	};
}