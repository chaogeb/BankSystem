#pragma once

#include <stdexcept>

#include "sqlite3.h"
#include "typedef.h"

using namespace BANK;
using namespace CUSTOMER;
using namespace CARD;

namespace SQLite
{

	class Exception : public std::runtime_error
	{
	public:
		/**
		* @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
		*
		* @param[in] aErrorMessage The string message describing the SQLite error
		*/
		explicit Exception(const std::string& aErrorMessage) :
			std::runtime_error(aErrorMessage)
		{
		}
	};

	class Database
	{
	public:
		Database();
		Database(const char* apFilename);
		~Database() noexcept; // nothrow
	protected:
		// Shortcut to execute one or multiple SQL statements without results (UPDATE, INSERT, ALTER, COMMIT, CREATE...).
		int exec(const char* apQueries);
		inline int exec(const std::string& aQueries)
		{
			return exec(aQueries.c_str());
		}
		inline void check(const int aRet) const
		{
			if (SQLITE_OK != aRet)
			{
				throw SQLite::Exception(sqlite3_errstr(aRet));
			}
		}
	protected:
		sqlite3 * mpSQLite;
		std::string mFilename;
	};

	class BankDB : public Database
	{
	public:
		BankDB(Database &apdb);
		~BankDB();
		// execute bank table
		IBank	getBank(const BankID id);
		BankVec	getBanks();
		IBank	updateBank(const BankID id, const char* name);
		IBank	createBank(const BankID id, const char* name);
	};

	class CustomerDB : Database
	{
	public:
		CustomerDB(Database &apdb);
		~CustomerDB();
		// execute customer table
		ICustomer	getCustomer(const CustomerID id);
		ICustomer	getCustomer(const char* idCard);
		CustomerVec	getCustomers();
		ICustomer	updateCustomer(const CustomerID id, const char* name);
		ICustomer	createCustomer(const CustomerID id, const char* name, const char* id_card);
	};

	class CardDB : Database
	{
	public:
		CardDB(Database &apdb);
		~CardDB();
		// execute catd table
		ICard	getCard(const CardID id);
		ICard	getCard(const char* cardNo);
		CardVec	getCards();
		/*** update balance if payPassword == NULL, else update S.C */
		ICard	updateCard(const CardID id, const char* payPassword, 
			const unsigned long long balance = 0);
		ICard	createCard(const CardID id, const char* cardNo, const char* payPassword,
			const unsigned long long balance, const BANK::BankID bankID, const CUSTOMER::CustomerID customerID);
		//int deleteCard(const CardID id);
	};

}	// namespace SQLite