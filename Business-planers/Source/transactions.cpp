#include "../Header Files/transactions.h"
#include "../Header Files/user_id.h"

//Adding a transaction
void addTran(TRAN*& head, string type, double& value)
{
	TRAN* newTran = new TRAN(type, value);
	newTran->next = head;
	head = newTran;
}

// Function to display the transactions related to Bitcoin
void displayBtc(TRAN* head, double& btc)
{
	TRAN* current = head;
	while (current != nullptr)
	{
		cout << "You added " << btc << " to " << current->type << endl;
		current = current->next;
	}
}

// Function to display the transactions related to Ethereum
void displayEth(TRAN* head, double& eth)
{
	TRAN* current = head;
	while (current != nullptr)
	{
		cout << "You added " << eth << " to " << current->type << endl;
		current = current->next;
	}
}

// Function to display the transactions related to Rights
void displayRights(TRAN* head, double& rights)
{
	TRAN* current = head;
	while (current != nullptr)
	{
		cout << "You added " << rights << " to " << current->type << endl;
		current = current->next;
	}
}

// Function to display the transactions related to Stocks
void displayStocks(TRAN* head, double& stocks)
{
	TRAN* current = head;
	while (current != nullptr)
	{
		cout << "You added " << stocks << " to " << current->type << endl;
		current = current->next;
	}
}

// Function to determine the distribution of digital will among heirs
void will(int sumToHundred, int heirsCounter, pqxx::connection* conn)
{
	double percentages[20];
	cout << "How many heirs will you have?" << endl;
	cin >> heirsCounter;
	for (int i = 0; i < heirsCounter; i++)
	{
		cout << "What percentage of your will will be received by heir " << i + 1 << "?" << endl;
		cin >> percentages[i];
		sumToHundred += percentages[i];
	}
	if (sumToHundred != 100)
	{
		cout << "You didn't distribute your will among your heirs correctly." << endl <<  "The sum of the percentages must be exactly 100!Try entering them again." << endl;
		sumToHundred = 0;
		will(sumToHundred, heirsCounter, conn);
	}
	else
	{
		pqxx::work worker(*conn);
		pqxx::result res = worker.exec("SELECT btc, eth, stocks, rights FROM assets WHERE user_id = " + to_string(id));

		pqxx::result::const_iterator row = res.begin();
		double btc = row["btc"].as<double>();
		double eth = row["eth"].as<double>();
		double stocks = row["stocks"].as<double>();
		double rights = row["rights"].as<double>();

		for (int i = 0; i < heirsCounter; i++)
		{
			cout << "Heir " << i + 1 << " is going to receive " << float((percentages[i] / sumToHundred)) * (btc + eth + rights + stocks) << endl;
		}
	}
}

//Function to handle asset transactions and update the database
void assets( string& answer, string& typeOfTran, double& btcAdd, double& ethAdd, double& rightsAdd, double& stocksAdd, pqxx::connection* conn, double& btc, double& eth, double& rights, double& stocks)
{
	TRAN* head = nullptr;

	cout << "Do you want to make a new transaction? It could be outgoing or incoming." << endl << "Use a negative number for an outgoing and a positive one for an incoming transaction" << endl;
	
	cin >> answer;
	if (answer == "Yes" || answer == "yes" || answer == "YES" || answer == "Y" || answer == "y")
	{
		cout << "Our bank supports these types of assets Bitcoin(1), Ethereum(2), Rights(3), Stocks(4)" << endl;
		cout << "In which of these do you want to operate with? Choose the corresponding number." << endl;
		cin >> typeOfTran;

		if (typeOfTran == "1")
		{
			cout << "How much would you like to add to this asset?" << endl;
			cin >> btcAdd;
			btc += btcAdd;
			addTran(head, "Bitcoin", btcAdd);
			displayBtc(head, btc);
		}
		else if (typeOfTran == "2")
		{
			cout << "How much would you like to add to this asset?" << endl;
			cin >> ethAdd;
			eth += ethAdd;
			addTran(head, "Ethereum", ethAdd);
			displayEth(head, eth);
		}
		else if (typeOfTran == "3")
		{
			cout << "How much would you like to add to this asset?" << endl;
			cin >> rightsAdd;
			rights += rightsAdd;
			addTran(head, "Rights", rightsAdd);
			displayRights(head, rights);
		}
		else if (typeOfTran == "4")
		{
			cout << "How much would you like to add to this asset?" << endl;
			cin >> stocksAdd;
			stocks += stocksAdd;
			addTran(head, "Stocks", stocksAdd);
			displayStocks(head, stocks);
		}
		else
		{
			cout << "Our bank doesn't support this type of asset" << endl;
		}
		return assets(answer, typeOfTran, btcAdd, ethAdd, rightsAdd, stocksAdd, conn, btc, eth, rights, stocks);
		cout << endl;
	}
	else if (answer == "N" || answer == "NO" || answer == "no" || answer == "No" || answer == "n")
	{
		pqxx::work worker(*conn);
		pqxx::result res = worker.exec("SELECT btc, eth, stocks, rights FROM assets WHERE user_id = " + to_string(id));
			
		if (!res.empty())
		{
			pqxx::result::const_iterator row = res.begin();
			btc += row["btc"].as<double>();
			eth += row["eth"].as<double>();
			stocks += row["stocks"].as<double>();
			rights += row["rights"].as<double>();
			res = worker.exec("UPDATE assets SET btc = " + to_string(btc) + ", eth = " + to_string(eth) + ", stocks = " + to_string(stocks) + ", rights = " + to_string(rights) + " WHERE user_id = " + to_string(id));
		}
		else
		{
			res = worker.exec("INSERT INTO assets(btc, eth, stocks, rights, user_id) VALUES('" + to_string(btc) + "','" + to_string(eth) + "','" + to_string(stocks) + "','" + to_string(rights) + "','" + to_string(id) + "')");
		}
		worker.commit();
		cout << "Do you want to determine your digital will? (Yes/No)?" << endl;
		cin >> answer;
		if (answer == "Yes" || answer == "yes" || answer == "YES" || answer == "Y" || answer == "y")
		{

			if (btc + eth + rights + stocks < 1)
			{
				cout << "You have to make at least one transaction to continue!" << endl;
				cout << endl;
				assets(answer, typeOfTran, btcAdd, ethAdd, rightsAdd, stocksAdd, conn, btc, eth, rights, stocks);
			}
			cout << endl << "At our bank, your heirs inherit your will, distributing it in percentages determined by you" << endl << endl;
			will(sumToHundred, heirsCounter, conn);
		}	
		else if(answer == "N" || answer == "NO" || answer == "no" || answer == "No" || answer == "n")
			exit(0);
		return;
	}
	else
	{
		cout << "You should type yes or no" << endl;
		assets(answer, typeOfTran, btcAdd, ethAdd, rightsAdd, stocksAdd, conn, btc, eth, rights, stocks);
	}
}