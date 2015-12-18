#pragma once

#include "typedef.h"
#include "Database.h"

using namespace std;

namespace CUSTOMER
{
	class Customer
	{
	public:
		Customer(SQLite::Database &apdb) : db(apdb) {};

		string getUserName(const CustomerID id) {
			return db.getCustomer(id).get_name();
		}

		string getIdCard(const CustomerID id) {
			return db.getCustomer(id).get_idCard();
		}

		CustomerID getIDviaIdCard(const char* idCard) {
			return db.getCustomer(idCard).get_ID();
		}

		CustomerVec getUsers() {
			return db.getCustomers();
		}

		/*** return true if success, false if user exists */
		bool newUser(const CustomerID id, const char* name, const char* id_card) {
			if (db.getCustomer(id_card).isvalid()) {
				return false;
			}
			else {
				db.createCustomer(id, name, id_card);
				return true;
			}
		}

		/*** return true if success, false if CustomerID not exists */
		bool updateUser(const CustomerID id, const char* name) {
			if (db.getCustomer(id).isvalid()) {
				db.updateCustomer(id, name);
				return true;
			}
			else
				return false;
		}
	private:
		SQLite::CustomerDB db;
	};

}	// namespace CUSTOMER
