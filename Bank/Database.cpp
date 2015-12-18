#include "Database.h"

namespace SQLite
{
	Database::Database() :
		mpSQLite(NULL),
		mFilename(NULL)
	{
	}

	Database::Database(const char* apFilename) :
		mpSQLite(NULL),
		mFilename(apFilename)
	{
		// 打开数据库, 创建连接
		int ret = sqlite3_open(apFilename, &mpSQLite);

		if (SQLITE_OK != ret)
		{
			sqlite3_close(mpSQLite); // close is required even in case of error on opening
			throw SQLite::Exception(sqlite3_errstr(ret));
		}

		exec("CREATE TABLE IF NOT EXISTS BANK\
				('id' int, 'name' varchar(20))");
		exec("CREATE TABLE IF NOT EXISTS USER\
				('id' int, 'name' varchar(20), 'identity_card' varchar(20))");
		exec("CREATE TABLE IF NOT EXISTS CARD\
				('id' int, 'card_no' varchar(20), 'SC' varchar(20), 'balance' unsigned long long integer,\
				'user_id' int, 'bank_id' int)");
	}

	// Close the SQLite database connection.
	Database::~Database() noexcept // nothrow
	{
		const int ret = sqlite3_close(mpSQLite);

		// Only case of error is SQLITE_BUSY: "database is locked" (some statements are not finalized)
		// Never throw an exception in a destructor :
		if (SQLITE_OK != ret)
		{
			printf("database is locked\n");
			getchar(); exit(-1);
		}
	}

	int Database::exec(const char* apQueries)
	{
		const int ret = sqlite3_exec(mpSQLite, apQueries, NULL, NULL, NULL);
		check(ret);

		// Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE only)
		return sqlite3_changes(mpSQLite);
	}

	BankDB::BankDB(Database &apdb) : Database(apdb) {}

	BankDB::~BankDB() { mpSQLite = NULL; }

	int getBank_callback(void *bank, int nColumn, char **colValues, char **colNames)
	{
		((IBank*)bank)->getRec(colValues);
		return 0;
	}
	IBank		BankDB::getBank(const BankID id)
	{
		IBank bank;
		char sql[100];
		sprintf(sql, "SELECT * FROM BANK WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getBank_callback, &bank, NULL);

		return bank;
	}

	int getBanks_callback(void *banklist, int nColumn, char **colValues, char **colNames)
	{
		IBank bank;
		bank.getRec(colValues);
		((BankVec*)banklist)->push_back(bank);
		return 0;
	}
	BankVec		BankDB::getBanks()
	{
		BankVec banklist;
		char sql[100];
		sprintf(sql, "SELECT * FROM BANK");
		sqlite3_exec(mpSQLite, sql, &getBanks_callback, &banklist, NULL);

		return banklist;
	}

	IBank		BankDB::updateBank(const BankID id, const char * name)
	{
		char sql[100];
		sprintf(sql, "UPDATE BANK SET id=%d,name='%s' WHERE id=%d", id, name, id);
		exec(sql);
		return getBank(id);
	}

	IBank		BankDB::createBank(const BankID id, const char * name)
	{
		char sql[100];
		sprintf(sql, "INSERT INTO BANK VALUES (%d, '%s')", id, name);
		exec(sql);
		return getBank(id);
	}


	CustomerDB::CustomerDB(Database & apdb) : Database(apdb) {}

	CustomerDB::~CustomerDB() { mpSQLite = NULL; }

	int getCustomer_callback(void *user, int nColumn, char **colValues, char **colNames)
	{
		((ICustomer*)user)->getRec(colValues);
		return 0;
	}
	ICustomer	CustomerDB::getCustomer(const CustomerID id)
	{
		ICustomer user;
		char sql[100];
		sprintf(sql, "SELECT * FROM USER WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getCustomer_callback, &user, NULL);

		return user;
	}
	ICustomer	CustomerDB::getCustomer(const char* idCard)
	{
		ICustomer user;
		char sql[100];
		sprintf(sql, "SELECT * FROM USER WHERE identity_card='%s' LIMIT 1", idCard);
		sqlite3_exec(mpSQLite, sql, &getCustomer_callback, &user, NULL);

		return user;
	}

	int getCustomers_callback(void *userlist, int nColumn, char **colValues, char **colNames)
	{
		ICustomer user;
		user.getRec(colValues);
		((CustomerVec*)userlist)->push_back(user);
		return 0;
	}
	CustomerVec	CustomerDB::getCustomers()
	{
		CustomerVec userlist;
		char sql[100];
		sprintf(sql, "SELECT * FROM USER");
		sqlite3_exec(mpSQLite, sql, &getCustomers_callback, &userlist, NULL);

		return userlist;
	}

	ICustomer	CustomerDB::updateCustomer(const CustomerID id, const char * name)
	{
		char sql[100];
		sprintf(sql, "UPDATE USER SET id=%d,name='%s' WHERE id=%d", id, name, id);
		exec(sql);
		return getCustomer(id);
	}

	ICustomer	CustomerDB::createCustomer(const CustomerID id, const char * name, const char * id_card)
	{
		char sql[100];
		sprintf(sql, "INSERT INTO USER VALUES (%d, '%s', '%s')", id, name, id_card);
		exec(sql);
		return getCustomer(id);
	}


	CardDB::CardDB(Database & apdb) : Database(apdb) {}

	CardDB::~CardDB() { mpSQLite = NULL; }

	int getCard_callback(void *card, int nColumn, char **colValues, char **colNames)
	{
		((ICard*)card)->getRec(colValues);
		return 0;
	}
	ICard		CardDB::getCard(const CardID id)
	{
		ICard card;
		char sql[100];
		sprintf(sql, "SELECT * FROM CARD WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getCard_callback, &card, NULL);

		return card;
	}
	ICard		CardDB::getCard(const char * cardNo)
	{
		ICard card;
		char sql[100];
		sprintf(sql, "SELECT * FROM CARD WHERE card_no='%s' LIMIT 1", cardNo);
		sqlite3_exec(mpSQLite, sql, &getCard_callback, &card, NULL);

		return card;
	}

	int getCards_callback(void *cardlist, int nColumn, char **colValues, char **colNames)
	{
		ICard card;
		card.getRec(colValues);
		((CardVec*)cardlist)->push_back(card);
		return 0;
	}
	CardVec		CardDB::getCards()
	{
		CardVec cardlist;
		char sql[100];
		sprintf(sql, "SELECT * FROM CARD");
		sqlite3_exec(mpSQLite, sql, &getCards_callback, &cardlist, NULL);

		return cardlist;
	}

	/*** update balance if payPassword == NULL, else only update S.C */
	ICard		CardDB::updateCard(const CardID id, const char * payPassword,
			const unsigned long long balance)
	{
		char sql[300];
		if (payPassword == NULL) { // update balance
			sprintf(sql, "UPDATE CARD SET balance=%llu WHERE id=%d",
				balance, id);
		}
		else { // update password
			sprintf(sql, "UPDATE CARD SET SC='%s' WHERE id=%d",
				payPassword, id);
		}
		exec(sql);
		return getCard(id);
	}

	ICard		CardDB::createCard(const CardID id, const char * cardNo, const char * payPassword,
			const unsigned long long balance, const BANK::BankID bankID, const CUSTOMER::CustomerID customerID)
	{
		char sql[200];
		sprintf(sql, "INSERT INTO CARD VALUES (%d, '%s', '%s', %llu, %d, %d)",
			id, cardNo, payPassword, balance, bankID, customerID);
		exec(sql);
		return getCard(id);
	}

}	// namespace SQLite

