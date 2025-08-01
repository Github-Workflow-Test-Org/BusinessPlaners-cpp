#include "Header Files/main.h"
#include "Header Files/user_id.h"
#include "Header Files/assets.h"

int id;
int main()
{
	pqxx::connection* conn = new pqxx::connection("dbname=postgres user=Kaloyan password=Zow14194 host=database-business-planers.postgres.database.azure.com port=5432 sslmode=require");

	login(username, password, passwordVer, haveAccount, loginOrSignup, conn);
	assets(answer, typeOfTran, btcAdd, ethAdd, rightsAdd, stocksAdd, conn, btc, eth, rights, stocks);
	free(conn);
}
