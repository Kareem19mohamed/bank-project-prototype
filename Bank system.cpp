#include <iostream>
#include <fstream>
#include "MyLibrary.h"
#include <string>
#include <vector>
#include <iomanip>
#include <conio.h> 
using namespace std;

const string FileName = "Clients.txt";
const string Separator = "#//#";

enum enMainMenu
{
	ShowclientsList = 1,
	Addnewclient = 2,
	DeleteClient = 3,
	updateclient = 4,
	FindClient = 5,
	Transactions = 6,
	Exit = 7
};

enum enTransactions {
	enDeposit = 1,
	enWithdraw = 2,
	enTotalBalances = 3,
	MainMenu = 4
};


struct stClient {
	string AccountNum;
	int Pincode;
	string name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

string ReadString(string messege)
{
	string word;
	cout << messege << ' ';
	getline(cin >> ws, word);
	return word;
}

bool FindClientbyAccNum(string accountnum, vector <stClient> clients, stClient& foundClient);
vector <stClient> LoadDataFromFile(string filename);

stClient ReadClient()
{
	stClient me;
	vector <stClient> clients = LoadDataFromFile(FileName);
	me.AccountNum = ReadString("Enter account number:");

	while (FindClientbyAccNum(me.AccountNum, clients, me))
	{
		cout << me.AccountNum << " already exists, ";
		me.AccountNum = ReadString("Enter another account number:");

	}

	me.Pincode = MyLibrary::ReadNumber("Enter pin code:");
	me.name = ReadString("Enter name:");
	me.Phone = ReadString("Enter phone:");
	me.AccountBalance = MyLibrary::ReadNumber("Enter account balance:");

	return me;
}

vector <string> SplitString(string s, string seperator)
{
	vector <string> Token;
	int pos;
	string word;

	while ((pos = s.find(seperator)) != string::npos)
	{
		word = s.substr(0, pos);

		if (word != seperator)
		{
			Token.push_back(word);
		}
		s.erase(0, pos + seperator.length());
	}

	if (s != seperator)
	{
		Token.push_back(s);
	}

	return Token;
}

stClient ConvertLinetoRecord(string Line)
{
	stClient client;
	vector <string> data = SplitString(Line, Separator);

	client.AccountNum = data[0];
	client.Pincode = stoi(data[1]);
	client.name = data[2];
	client.Phone = data[3];
	client.AccountBalance = stod(data[4]);

	return client;
}

string RecordtoLine(stClient client, string separator)
{
	string s = "";

	s += client.AccountNum + separator;
	s += to_string(client.Pincode) + separator;
	s += client.name + separator;
	s += client.Phone + separator;
	s += to_string(client.AccountBalance);

	return s;
}

void AddDataLinetoFile(string filename, string dataline)
{
	fstream MyFile;
	MyFile.open(filename, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << dataline << endl;
		cout << "Client added successfully\n";
		MyFile.close();
	}
	else
	{
		cout << "Error opening file" << endl;
	}
}

void AddNewClient()
{
	stClient client = ReadClient();
	string s = RecordtoLine(client, Separator);
	AddDataLinetoFile(FileName, s);
}

void AddClient()
{
	char x = 'n';

	do
	{
		AddNewClient();
		cout << "Enter another client? ";
		cin >> x;
		cout << endl;
	} while (x == 'y' || x == 'Y');
}

vector <stClient> LoadDataFromFile(string filename)
{
	string line;
	stClient client;
	vector <stClient> data;
	fstream MyFile;
	MyFile.open(filename, ios::in);
	if (MyFile.is_open())
	{
		while (getline(MyFile, line))
		{
			client = ConvertLinetoRecord(line);
			data.push_back(client);
		}
		MyFile.close();
	}
	else
	{
		cout << "Error opening file" << endl;
	}

	return data;
}

bool FindClientbyAccNum(string accountnum, vector <stClient> clients, stClient& foundClient)
{
	for (stClient& client : clients)
	{
		if (client.AccountNum == accountnum)
		{
			foundClient = client;
			return true;
		}
	}
	return false;
}

void PrintClientCard(string accountnum, vector <stClient> clients)
{
	stClient client;
	if (FindClientbyAccNum(accountnum, clients, client))
	{
		cout << "The Following are the client details: " << endl;
		cout << "-----------------------------------------------------" << endl;
		cout << "Account number: " << client.AccountNum << endl;
		cout << "Pin code: " << to_string(client.Pincode) << endl;
		cout << "Name: " << client.name << endl;
		cout << "Phone: " << client.Phone << endl;
		cout << "Account balance: " << to_string(client.AccountBalance) << endl;
		cout << "-----------------------------------------------------" << endl;
	}
	else
		cout << accountnum << "is not found" << endl;
}

bool MarkForDelete(string accountnum, vector <stClient>& clients)
{
	for (stClient& client : clients)
	{
		if (client.AccountNum == accountnum)
		{
			client.MarkForDelete = true;
			return true;

		}
	}
	return false;
}

void SaveDataToFile(vector <stClient> clients)
{
	string line;
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (stClient& client : clients)
		{
			if (client.MarkForDelete == false)
			{
				line = RecordtoLine(client, Separator);
				MyFile << line << endl;
			}
		}
		MyFile.close();
	}
	else
	{
		cout << "Error opening file" << endl;
	}
}

bool DeleteRecordFromFile(string accountnum, vector <stClient>& clients)
{
	stClient client;
	char omit = 'N';

	if (FindClientbyAccNum(accountnum, clients, client))
	{
		PrintClientCard(accountnum, clients);

		cout << "Are you sure you want to delete this?";
		cin >> omit;


		if (omit == 'y' || omit == 'Y')
		{
			MarkForDelete(accountnum, clients);
			SaveDataToFile(clients);
			clients = LoadDataFromFile(FileName);
			cout << "Client deleted successfully" << endl;;
			return true;
		}
	}
	else
	{
		cout << accountnum << "Is not FOund" << endl;
		return false;
	}

}

stClient ChangeClientRecord(string accountnum)
{
	stClient client;

	client.AccountNum = accountnum;
	client.Pincode = MyLibrary::ReadNumber("Enter pin code:");
	client.name = ReadString("Enter name:");
	client.Phone = ReadString("Enter phone:");
	client.AccountBalance = MyLibrary::ReadNumber("Enter balance:");

	return client;
}

bool UpdateDatainFile(string accountnum, vector <stClient>& clients)
{
	stClient client;
	char update = 'N';

	if (FindClientbyAccNum(accountnum, clients, client))
	{
		PrintClientCard(accountnum, clients);

		cout << "Are you sure you want to update this?";
		cin >> update;

		if (update == 'y' || update == 'Y')
		{
			for (stClient& client : clients)
			{
				if (client.AccountNum == accountnum)
				{
					client = ChangeClientRecord(accountnum);
					break;
				}
			}

		}
		SaveDataToFile(clients);
		cout << "File updated successfully" << endl;
		return true;
	}
	else
	{
		cout << accountnum << "Is not FOund" << endl;
		return false;
	}

}

bool Deposit(string accountnum, vector <stClient>& clients)
{
	stClient client;
	char Deposit ;
	int num;

	if (FindClientbyAccNum(accountnum, clients, client))
	{
		PrintClientCard(accountnum, clients);

		for (stClient& client : clients)
		{
			if (client.AccountNum == accountnum)
			{
				num = MyLibrary::ReadNumber("How much you want to deposit: ");
				cout << "Are you sure you want to deposit this?";
				cin >> Deposit;

				if (Deposit == 'y' || Deposit == 'Y')
				{
					client.AccountBalance += num;
					SaveDataToFile(clients);
					cout << "File updated successfully" << endl;
					return true;
				}

			}
		}
	}
	else
	{
		cout << accountnum << "Is not Found" << endl;
		return false;
	}

}

int EnoughFunds(int balance)
{
	int num;
	string m;

	do
	{
		num = MyLibrary::ReadNumber("Enter how much you want to withdraw: ");
		m = (num > balance) ? "not enough funds" : "";
		cout << m<<endl;

	} while (num > balance);
	return  num;
}

bool Withdraw(string accountnum, vector <stClient>& clients)
{
	stClient client;
	char withdraw;
	int num;

	if (FindClientbyAccNum(accountnum, clients, client))
	{
		PrintClientCard(accountnum, clients);

		for (stClient& client : clients)
		{
			if (client.AccountNum == accountnum)
			{
				num=EnoughFunds(client.AccountBalance);
				cout << "Are you sure you want to withdraw this?";
				cin >> withdraw;

				if (withdraw == 'y' || withdraw == 'Y')
				{
					client.AccountBalance -= num;
					SaveDataToFile(clients);
					cout << "File updated successfully" << endl;
					return true;
				}

			}
		}
	}
	else
	{
		cout << accountnum << "Is not Found" << endl;
		return false;
	}

}

void ClientsBalances(vector <stClient> vClients);
void TotalBalances(vector<stClient> clients)
{
	int num = 0;
	ClientsBalances(clients);
	for (stClient client : clients)
	{
		num += client.AccountBalance;
	}
	printf("Total balances: %d", num);
	cout << endl;
}

void TransactionsScreen();
int ReturnClientInput(int from, int to);
enTransactions OptionsToEnum();



void TransctionsProcess(vector <stClient>& clients)
{
	string accountname;
	enTransactions option;

	do
	{
		TransactionsScreen();
		option = OptionsToEnum();
		system("cls");
		switch (option)
		{
		case enDeposit:
		{
			accountname = ReadString("Enter account name: ");
			Deposit(accountname, clients);
			cout << "Press any key to return to transaction menu___";
			system("pause>0");
			break;
		}
		case enWithdraw:
		{
			accountname = ReadString("Enter account name: ");
			Withdraw(accountname, clients);
			cout << "Press any key to return to transaction menu___";
			system("pause>0");
			break;
		}
		case enTotalBalances:
		{
			TotalBalances(clients);
			cout << "Press any key to return to transaction menu___";
			system("pause>0");
			break;
		}
		case MainMenu:
		{

			return;
		}
		}
		system("cls");
	} while (option != MainMenu);
}

void PrintRecordData(stClient client)
{
	cout << "| " << setw(15) << left << client.AccountNum;
	cout << "| " << setw(10) << left << client.Pincode;
	cout << "| " << setw(40) << left << client.name;
	cout << "| " << setw(12) << left << client.Phone;
	cout << "| " << setw(12) << left << client.AccountBalance;
}
void ConseiceData(stClient client)
{
	cout << "| " << setw(15) << left << client.AccountNum;
	cout << "| " << setw(40) << left << client.name;
	cout << "| " << setw(12) << left << client.AccountBalance;
}

void ClientsBalances(vector <stClient> vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (stClient Client : vClients)
	{
		ConseiceData(Client);
		cout << endl;
	}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

}

void PrintAllClientsData(vector <stClient> vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	for (stClient Client : vClients)
	{
		PrintRecordData(Client);
		cout << endl;
	}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

}

enMainMenu choiceToEnum(int option)
{
	return (enMainMenu)option;
}

enTransactions OptionsToEnum()
{
	int option = ReturnClientInput(1, 4);
	return (enTransactions)option;
}

void BankSystem(vector <stClient>& clients, enMainMenu choice)
{
	string accountnum;
	system("cls");
	switch (choice)
	{
	case ShowclientsList:
	{
		PrintAllClientsData(clients);
		cout << "Press any key to return to main menu___";
		system("pause>0");
		break;
	}
	case Addnewclient:
	{
		AddClient();
		cout << "Press any key to return to main menu___";
		system("pause>0");
		break;
	}
	case DeleteClient:
	{
		accountnum = ReadString("Enter account number:");
		DeleteRecordFromFile(accountnum, clients);
		cout << "Press any key to return to main menu___";
		system("pause>0");
		break;
	}
	case updateclient:
	{
		accountnum = ReadString("Enter account number:");
		UpdateDatainFile(accountnum, clients);
		cout << "Press any key to return to main menu___";
		system("pause>0");
		break;
	}
	case FindClient:
	{
		accountnum = ReadString("Enter account number:");
		PrintClientCard(accountnum, clients);
		cout << "Press any key to return to main menu___";
		system("pause>0");
		break;
	}
	case Transactions:
	{
		TransctionsProcess(clients);
		break;
	}
	case Exit:
	{
		break;
	}
	}
	system("cls");
}

int ReturnClientInput(int from, int to)
{
	int num;
	do
	{
		cout << "Choose your service from (" << from << " to " << to << ")" << endl;
		cin >> num;
		if (num < from || num>to)
		{
			cout << "Wrong number enter another one" << endl;
		}
	} while (num<from || num>to);

	return num;
}

void MainScreen()
{
	cout << right;
	cout << "---------------------------------------------" << endl;
	cout << setw(30) << "Main menu screen" << endl;
	cout << "---------------------------------------------" << endl;
	cout << setw(25) << "[1] Show Client List." << endl;
	cout << setw(23) << "[2] Add New Client." << endl;
	cout << setw(22) << "[3] Delete Client." << endl;
	cout << setw(22) << "[4] Update Client." << endl;
	cout << setw(20) << "[5] Find Client." << endl;
	cout << setw(21) << "[6] Transactions." << endl;
	cout << setw(13) << "[7] Exit." << endl;
	cout << "---------------------------------------------" << endl;
}

void TransactionsScreen()
{
	cout << right;
	cout << "---------------------------------------------" << endl;
	cout << setw(25) << "Transactions menu screen" << endl;
	cout << "---------------------------------------------" << endl;
	cout << setw(20) << "[1] Deposit." << endl;
	cout << setw(21) << "[2] Withdraw." << endl;
	cout << setw(27) << "[3] Total Balances." << endl;
	cout << setw(22) << "[4] Main menu." << endl;
	cout << "---------------------------------------------" << endl;
}

void Bank()
{
	int input;
	enMainMenu choice;
	vector <stClient> clients = LoadDataFromFile(FileName);
	do
	{
		MainScreen();

		input = ReturnClientInput(1, 7);
		choice = choiceToEnum(input);
		BankSystem(clients, choice);
		clients = LoadDataFromFile(FileName);
	} while (choice != enMainMenu::Exit);

}

int main()
{
	Bank();

	return 0;
}