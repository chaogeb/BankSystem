#pragma once

#include "Bank.h"
#include "Customer.h"
#include "Card.h"
#include "Database.h"

namespace CONTROLLER
{
	class FacadeController
	{
	private:
		SQLite::Database dbCon;
		Bank bankCon;
		Customer cusCon;
		Card cardCon;
	public:
		FacadeController();

		void CreateBank(const char* name);
		/*** return true and show bank info if id is VALID, else false*/
		bool ShowBank(const BankID id);
		void ListBanks();

		CustomerID CreateUser(const char* name, const char* id_card);
		bool UpdateUser(const CustomerID id, const char* name);
		bool FetchUser(const char* id_card);
		CustomerID GetUserID(const char* id_card);
		string GetUserName(const CustomerID id);

		/*** return true if fetched, false if Card ID doesn't exists */
		bool FetchCard(const char* cardNo);
		CardID GetCardID(const char* cardNo);
		/*** verify password, return true if success */
		bool VerifyCardSC(const CardID id, const string inPass);
		bool CreateCard(const char* cardNo, const string payPassword, BANK::BankID bankID, CUSTOMER::CustomerID customerID);
		bool ChangeCardSC(const CardID id, const string inPassword);
		string CardBalance(const CardID id);
		string GetCardBankName(const CardID id);
		string GetCardUserName(const CardID id);
		string GetCardNo(const CardID id);
		bool Deposit(const CardID id, const unsigned long long amount);
		bool Withdraw(const CardID id, const unsigned long long amount);
	};
}