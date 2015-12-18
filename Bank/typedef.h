#pragma once

#include <string>
#include <vector>

#define DATABASE_NAME "BankDatabase.db"

using namespace std;

namespace BANK
{
	typedef unsigned int BankID;

	class IBank
	{
	public:
		void getRec(char **values) {
			ID		= atoi(values[0]);
			name	= values[1];
		}
		/*** return 1 if record exists */
		bool isvalid()		{ return ID > 0; }
		BankID get_ID()		{ return ID; }
		string get_name()	{ return name; }
	protected:
		BankID ID = 0;
		string name;
	};

	typedef std::vector<IBank> BankVec;
}	// namespace BANK

namespace CUSTOMER
{
	typedef unsigned int CustomerID;

	class ICustomer
	{
	public:
		void getRec(char **values) {
			ID				= atoi(values[0]);
			name			= values[1];
			identityCard	= values[1];
		}
		/*** return true if record exists */
		bool isvalid()		{ return ID > 0; }
		CustomerID get_ID()	{ return ID; }
		string get_name()	{ return name; }
		string get_idCard() { return identityCard; }
	protected:
		CustomerID ID = 0;
		string name;
		string identityCard;
	};

	typedef std::vector<ICustomer> CustomerVec;
}	// namespace CUSTOMER

namespace CARD
{
	typedef unsigned int CardID;

	class ICard
	{
	public:
		void getRec(char **values) {
			ID			= atoi(values[0]);
			cardNo		= values[1];
			payPassword	= values[2];
			balance		= atoll(values[3]);
			customerID	= atoi(values[4]);
			bankID		= atoi(values[5]);
		}
		/*** return true if record exists */
		bool isvalid() { return ID > 0; }
		CardID getID() { return ID; }
		string getCardNo() { return cardNo; }
		bool checkPassword(const string inPass) { return payPassword == inPass; }
		unsigned long long getBalance() { return balance; }
		void incBalance(unsigned long long in) { balance += in; }
		/*** return true if withdraw success, false if failed */
		bool decBalance(unsigned long long out) {
			if (balance >= out) {
				balance -= out;
				return true;
			}
			else return false;
		}
		BANK::BankID getBankID() { return bankID; }
		CUSTOMER::CustomerID getUserID() { return customerID; }
	protected:
		CardID ID = 0;
		string cardNo;
		string payPassword;
		unsigned long long balance;
		BANK::BankID bankID;
		CUSTOMER::CustomerID customerID;
	};

	typedef std::vector<ICard> CardVec;
}	// namespace CARD