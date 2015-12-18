#pragma once

#include "typedef.h"
#include "Database.h"

using namespace std;

namespace BANK
{
	class Bank
	{
	public:
		Bank(SQLite::Database &apdb) : db(apdb) {};

		string getBankName(const BankID id) {
			return db.getBank(id).get_name();
		}

		BankVec getBanks() {
			return db.getBanks();
		}

		/*** return 1 if success, 0 if BankID exists */
		int newBank(const BankID id, const char * name) {
			if (db.getBank(id).isvalid()) { // if id exists
				return 0;
			}
			else {
				db.createBank(id, name);
				return 1;
			}
		}

		/*** return 1 if success, 0 if BankID not exists */
		int updateBank(const BankID id, const char * name) {
			if (db.getBank(id).isvalid()) { // if exists, update record
				db.updateBank(id, name);
				return 1;
			}
			else
				return 0;
		}
	private:
		SQLite::BankDB db;
	};

}	// namespace BANK
