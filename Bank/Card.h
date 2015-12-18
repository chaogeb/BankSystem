#pragma once

#include "typedef.h"
#include "Database.h"

using namespace std;

namespace CARD
{
	class Card
	{
	public:
		Card(SQLite::Database &apdb) : db(apdb) {};

		CardID getCardID(const char* cardNo) {
			return db.getCard(cardNo).getID();
		}

		string getCardNo(const CardID id) {
			return db.getCard(id).getCardNo();
		}

		bool checkPassword(const CardID id, const string inPass) {
			return db.getCard(id).checkPassword(inPass);
		}

		unsigned long long getBalance(const CardID id) {
			return db.getCard(id).getBalance();
		}

		BANK::BankID getBankID(const CardID id) {
			return db.getCard(id).getBankID();
		}

		CUSTOMER::CustomerID getUserID(const CardID id) {
			return db.getCard(id).getUserID();
		}

		CardVec getCards() {
			return db.getCards();
		}

		/*** return true if success, false if CardID exists */
		bool newCard(const CardID id, const char* cardNo, const char* payPassword,
			const BANK::BankID bankID, const CUSTOMER::CustomerID userID) {
			if (db.getCard(id).isvalid()) {
				return false;
			}
			else {
				db.createCard(id, cardNo, payPassword, 0, bankID, userID);
				return true;
			}
		}
		
		/*** left payPassword==NULL to update balance
		** else only update password(S.C)
		** return true if success, false if CardID exists */
		bool updateCard(const CardID id, const char* payPassword,
			const unsigned long long balance = 0) {
			if (db.getCard(id).isvalid()) {
				db.updateCard(id, payPassword, balance);
				return true;
			}
			else
				return false;
		}

		/* ´æ¿î */
		bool deposit(const CardID id, unsigned long long amount) {
			if (db.getCard(id).isvalid()) {
				ICard card = db.getCard(id);
				card.incBalance(amount);
				db.updateCard(id, NULL, card.getBalance());
				return true;
			}
			else
				return false;
		}
		/* È¡¿î */
		bool withdraw(const CardID id, unsigned long long amount) {
			if (db.getCard(id).isvalid()) {
				ICard card = db.getCard(id);
				if (card.decBalance(amount)) {
					db.updateCard(id, NULL, card.getBalance());
					return true;
				}
			}
			return false;
		}
	private:
		SQLite::CardDB db;
	};

}	// namespace CARD