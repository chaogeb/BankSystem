
#include "typedef.h"
#include "Facade.h"

using namespace std;

namespace CONTROLLER
{
	FacadeController::FacadeController() :
		dbCon(DATABASE_NAME),
		bankCon(dbCon),
		cusCon(dbCon),
		cardCon(dbCon)
	{
	}

	void FacadeController::CreateBank(const char * name)
	{
		BankVec bv = bankCon.getBanks();
		int id;
		if (bv.size() > 0) {
			id = bv.at(bv.size() - 1).get_ID() + 1;
		}
		else id = 1;

		if (bankCon.newBank(id, name)) {
			printf("Bank record added successfully!\n");
		}
		else {
			printf("Band ID exists!\n");
		}
	}

	bool FacadeController::ShowBank(const BankID id)
	{
		BankVec bv = bankCon.getBanks();
		if (id > bv.size()) {
			printf("Does not match Bank ID!\n");
			return false;
		}
		else {
			printf("%s\n", bankCon.getBankName(id).c_str());
			return true;
		}
	}

	void FacadeController::ListBanks()
	{
		BankVec bv = bankCon.getBanks();
		for each (IBank bank in bv) {
			printf("%d.\t%s\n", bank.get_ID(), bank.get_name().c_str());
		}
	}

	CustomerID FacadeController::CreateUser(const char * name, const char * id_card)
	{
		CustomerVec cv = cusCon.getUsers();
		CustomerID id;
		if (cv.size() > 0) {
			id = cv.at(cv.size() - 1).get_ID() + 1;
		}
		else id = 1;

		if (cusCon.newUser(id, name, id_card)) {
			printf("User record added successfully!\n");
		}
		else {
			printf("User record exists!\n");
		}
		return id;
	}

	bool FacadeController::UpdateUser(const CustomerID id, const char * name)
	{
		return cusCon.updateUser(id, name);
	}

	bool FacadeController::FetchUser(const char * id_card)
	{
		if (GetUserID(id_card) == 0)
			return false;
		else 
			return true;
	}

	CustomerID FacadeController::GetUserID(const char * id_card)
	{
		return cusCon.getIDviaIdCard(id_card);
	}

	string FacadeController::GetUserName(const CustomerID id)
	{
		return cusCon.getUserName(id);
	}

	bool FacadeController::FetchCard(const char * cardNo)
	{
		if (cardCon.getCardID(cardNo) == 0)
			return false;
		else
			return true;
	}

	CardID FacadeController::GetCardID(const char * cardNo)
	{
		return cardCon.getCardID(cardNo);
	}

	bool FacadeController::VerifyCardSC(const CardID id, const string inPass)
	{
		return cardCon.checkPassword(id, inPass);
	}

	/* this is the stractgy to generate card number, could be changed with other algorithms */
	/*char* generateCardNumber(unsigned long offset, char* buff) {
		unsigned long basic = 1000000000;
		basic += offset;
		return ultoa(basic, buff, 10);
	}*/
	bool FacadeController::CreateCard(const char* cardNo, const string payPassword, BANK::BankID bankID, CUSTOMER::CustomerID userID)
	{
		int id;
		//unsigned long cardNo, offset;
		//char buff[11];
		
		if (cardCon.getCardID(cardNo) != 0)
		{
			printf("Card ID already exists!\n");
			return false;
		}
		CardVec cv = cardCon.getCards();
		if (cv.size() > 0) {
			id = cv.at(cv.size() - 1).getID() + 1;
			//cardNo = atol(cv.at(cv.size() - 1).getCardNo().c_str()) + 1;
			//// here needs a stractgy to generate card number
			//ultoa(cardNo, buff, 10);
		}
		else {
			id = 1;
			//generateCardNumber(1, buff);
		}

		if (cardCon.newCard(id, cardNo, payPassword.c_str(), bankID, userID)) {
			printf("Card record added!\n");
			return true;
		}
		else {
			printf("Unable to add new card!\n");
			return false;
		}
	}

	bool FacadeController::ChangeCardSC(const CardID id, const string inPassword)
	{
		return cardCon.updateCard(id, inPassword.c_str());
	}

	string FacadeController::CardBalance(const CardID id)
	{
		char output[20];
		unsigned long long balance = cardCon.getBalance(id);
		unsigned int digit = balance % 100;
		balance /= 100;
		sprintf(output, "%llu.%02d", balance, digit);
		string str = output;
		return str;
	}

	string FacadeController::GetCardBankName(const CardID id)
	{
		BankID bankID = cardCon.getBankID(id);
		return bankCon.getBankName(bankID);
	}

	string FacadeController::GetCardUserName(const CardID id)
	{
		CustomerID userID = cardCon.getUserID(id);
		return cusCon.getUserName(userID);
	}

	string FacadeController::GetCardNo(const CardID id)
	{
		return cardCon.getCardNo(id);
	}

	bool FacadeController::Deposit(const CardID id, const unsigned long long amount)
	{
		return cardCon.deposit(id, amount);
	}

	bool FacadeController::Withdraw(const CardID id, const unsigned long long amount)
	{
		return cardCon.withdraw(id, amount);
	}
	
}