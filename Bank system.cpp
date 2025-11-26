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
    ShowClientsList = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    Transactions = 6,
    Exit = 7
};

enum enTransactions
{
    Deposit = 1,
    Withdraw = 2,
    TotalBalances = 3,
    MainMenu = 4
};

struct stClient
{
    string AccountNum;
    int Pincode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

// ---------------------- Helper Functions ----------------------

string ReadString(string Message)
{
    string Word;
    cout << Message << ' ';
    getline(cin >> ws, Word);
    return Word;
}

vector<string> SplitString(string S, string Separator)
{
    vector<string> Token;
    int Pos;
    string Word;

    while ((Pos = S.find(Separator)) != string::npos)
    {
        Word = S.substr(0, Pos);

        if (Word != Separator)
        {
            Token.push_back(Word);
        }
        S.erase(0, Pos + Separator.length());
    }

    if (S != Separator)
    {
        Token.push_back(S);
    }

    return Token;
}

stClient ConvertLineToRecord(string Line)
{
    stClient Client;
    vector<string> Data = SplitString(Line, Separator);

    Client.AccountNum = Data[0];
    Client.Pincode = stoi(Data[1]);
    Client.Name = Data[2];
    Client.Phone = Data[3];
    Client.AccountBalance = stod(Data[4]);

    return Client;
}

string RecordToLine(stClient Client, string Separator)
{
    string S = "";

    S += Client.AccountNum + Separator;
    S += to_string(Client.Pincode) + Separator;
    S += Client.Name + Separator;
    S += Client.Phone + Separator;
    S += to_string(Client.AccountBalance);

    return S;
}

// ---------------------- File Operations ----------------------

void AddDataLineToFile(string FileName, string DataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        cout << "Client added successfully\n";
        MyFile.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }
}

vector<stClient> LoadDataFromFile(string FileName)
{
    string Line;
    stClient Client;
    vector<stClient> Data;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            Data.push_back(Client);
        }
        MyFile.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }

    return Data;
}

void SaveDataToFile(vector<stClient> Clients)
{
    string Line;
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (stClient& Client : Clients)
        {
            if (Client.MarkForDelete == false)
            {
                Line = RecordToLine(Client, Separator);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
    else
    {
        cout << "Error opening file" << endl;
    }
}

// ---------------------- Search & Logic ----------------------

bool FindClientByAccNum(string AccountNum, vector<stClient> Clients, stClient& FoundClient)
{
    for (stClient& Client : Clients)
    {
        if (Client.AccountNum == AccountNum)
        {
            FoundClient = Client;
            return true;
        }
    }
    return false;
}

bool MarkForDelete(string AccountNum, vector<stClient>& Clients)
{
    for (stClient& Client : Clients)
    {
        if (Client.AccountNum == AccountNum)
        {
            Client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

int EnoughFunds(int Balance)
{
    int Num;
    string M;

    do
    {
        Num = MyLibrary::ReadNumber("Enter how much you want to withdraw: ");
        M = (Num > Balance) ? "not enough funds" : "";
        cout << M << endl;

    } while (Num > Balance);
    return Num;
}

// ---------------------- Display Functions ----------------------

void PrintClientCard(string AccountNum, vector<stClient> Clients)
{
    stClient Client;
    if (FindClientByAccNum(AccountNum, Clients, Client))
    {
        cout << "The Following are the client details: " << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << "Account number: " << Client.AccountNum << endl;
        cout << "Pin code: " << to_string(Client.Pincode) << endl;
        cout << "Name: " << Client.Name << endl;
        cout << "Phone: " << Client.Phone << endl;
        cout << "Account balance: " << to_string(Client.AccountBalance) << endl;
        cout << "-----------------------------------------------------" << endl;
    }
    else
        cout << AccountNum << "is not found" << endl;
}

void PrintRecordData(stClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNum;
    cout << "| " << setw(10) << left << Client.Pincode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ConciseData(stClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNum;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<stClient> vClients)
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

void ClientsBalances(vector<stClient> vClients)
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
        ConciseData(Client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

// ---------------------- Core Functionality ----------------------

stClient ReadClient()
{
    stClient Me;
    vector<stClient> Clients = LoadDataFromFile(FileName);
    Me.AccountNum = ReadString("Enter account number:");

    while (FindClientByAccNum(Me.AccountNum, Clients, Me))
    {
        cout << Me.AccountNum << " already exists, ";
        Me.AccountNum = ReadString("Enter another account number:");
    }

    Me.Pincode = MyLibrary::ReadNumber("Enter pin code:");
    Me.Name = ReadString("Enter name:");
    Me.Phone = ReadString("Enter phone:");
    Me.AccountBalance = MyLibrary::ReadNumber("Enter account balance:");

    return Me;
}

void AddNewClient()
{
    stClient Client = ReadClient();
    string S = RecordToLine(Client, Separator);
    AddDataLineToFile(FileName, S);
}

void AddClient()
{
    char X = 'n';

    do
    {
        AddNewClient();
        cout << "Enter another client? ";
        cin >> X;
        cout << endl;
    } while (X == 'y' || X == 'Y');
}

bool DeleteRecordFromFile(string AccountNum, vector<stClient>& Clients)
{
    stClient Client;
    char Omit = 'N';

    if (FindClientByAccNum(AccountNum, Clients, Client))
    {
        PrintClientCard(AccountNum, Clients);

        cout << "Are you sure you want to delete this?";
        cin >> Omit;

        if (Omit == 'y' || Omit == 'Y')
        {
            MarkForDelete(AccountNum, Clients);
            SaveDataToFile(Clients);
            Clients = LoadDataFromFile(FileName);
            cout << "Client deleted successfully" << endl;
            return true;
        }
    }
    else
    {
        cout << AccountNum << "Is not FOund" << endl;
        return false;
    }
    return false;
}

stClient ChangeClientRecord(string AccountNum)
{
    stClient Client;

    Client.AccountNum = AccountNum;
    Client.Pincode = MyLibrary::ReadNumber("Enter pin code:");
    Client.Name = ReadString("Enter name:");
    Client.Phone = ReadString("Enter phone:");
    Client.AccountBalance = MyLibrary::ReadNumber("Enter balance:");

    return Client;
}

bool UpdateDataInFile(string AccountNum, vector<stClient>& Clients)
{
    stClient Client;
    char Update = 'N';

    if (FindClientByAccNum(AccountNum, Clients, Client))
    {
        PrintClientCard(AccountNum, Clients);

        cout << "Are you sure you want to update this?";
        cin >> Update;

        if (Update == 'y' || Update == 'Y')
        {
            for (stClient& Client : Clients)
            {
                if (Client.AccountNum == AccountNum)
                {
                    Client = ChangeClientRecord(AccountNum);
                    break;
                }
            }
        }
        SaveDataToFile(Clients);
        cout << "File updated successfully" << endl;
        return true;
    }
    else
    {
        cout << AccountNum << "Is not FOund" << endl;
        return false;
    }
}

bool Deposit(string AccountNum, vector<stClient>& Clients)
{
    stClient Client;
    char DepositChar;
    int Num;

    if (FindClientByAccNum(AccountNum, Clients, Client))
    {
        PrintClientCard(AccountNum, Clients);

        for (stClient& Client : Clients)
        {
            if (Client.AccountNum == AccountNum)
            {
                Num = MyLibrary::ReadNumber("How much you want to deposit: ");
                cout << "Are you sure you want to deposit this?";
                cin >> DepositChar;

                if (DepositChar == 'y' || DepositChar == 'Y')
                {
                    Client.AccountBalance += Num;
                    SaveDataToFile(Clients);
                    cout << "File updated successfully" << endl;
                    return true;
                }
            }
        }
    }
    else
    {
        cout << AccountNum << "Is not Found" << endl;
        return false;
    }
    return false;
}

bool Withdraw(string AccountNum, vector<stClient>& Clients)
{
    stClient Client;
    char WithdrawChar;
    int Num;

    if (FindClientByAccNum(AccountNum, Clients, Client))
    {
        PrintClientCard(AccountNum, Clients);

        for (stClient& Client : Clients)
        {
            if (Client.AccountNum == AccountNum)
            {
                Num = EnoughFunds(Client.AccountBalance);
                cout << "Are you sure you want to withdraw this?";
                cin >> WithdrawChar;

                if (WithdrawChar == 'y' || WithdrawChar == 'Y')
                {
                    Client.AccountBalance -= Num;
                    SaveDataToFile(Clients);
                    cout << "File updated successfully" << endl;
                    return true;
                }
            }
        }
    }
    else
    {
        cout << AccountNum << "Is not Found" << endl;
        return false;
    }
    return false;
}

void TotalBalances(vector<stClient> Clients)
{
    int Num = 0;
    ClientsBalances(Clients);
    for (stClient Client : Clients)
    {
        Num += Client.AccountBalance;
    }
    printf("Total balances: %d", Num);
    cout << endl;
}

// ---------------------- Menus & Input ----------------------

int ReturnClientInput(int From, int To)
{
    int Num;
    do
    {
        cout << "Choose your service from (" << From << " to " << To << ")" << endl;
        cin >> Num;
        if (Num < From || Num > To)
        {
            cout << "Wrong number enter another one" << endl;
        }
    } while (Num < From || Num > To);

    return Num;
}

enMainMenu ChoiceToEnum(int Option)
{
    return (enMainMenu)Option;
}

enTransactions OptionsToEnum()
{
    int Option = ReturnClientInput(1, 4);
    return (enTransactions)Option;
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

void TransactionsProcess(vector<stClient>& Clients)
{
    string AccountName;
    enTransactions Option;

    do
    {
        TransactionsScreen();
        Option = OptionsToEnum();
        system("cls");
        switch (Option)
        {
        case Deposit:
        {
            AccountName = ReadString("Enter account name: ");
            Deposit(AccountName, Clients);
            cout << "Press any key to return to transaction menu___";
            system("pause>0");
            break;
        }
        case Withdraw:
        {
            AccountName = ReadString("Enter account name: ");
            Withdraw(AccountName, Clients);
            cout << "Press any key to return to transaction menu___";
            system("pause>0");
            break;
        }
        case TotalBalances:
        {
            TotalBalances(Clients);
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
    } while (Option != MainMenu);
}

void BankSystem(vector<stClient>& Clients, enMainMenu Choice)
{
    string AccountNum;
    system("cls");
    switch (Choice)
    {
    case ShowClientsList:
    {
        PrintAllClientsData(Clients);
        cout << "Press any key to return to main menu___";
        system("pause>0");
        break;
    }
    case AddNewClient:
    {
        AddClient();
        cout << "Press any key to return to main menu___";
        system("pause>0");
        break;
    }
    case DeleteClient:
    {
        AccountNum = ReadString("Enter account number:");
        DeleteRecordFromFile(AccountNum, Clients);
        cout << "Press any key to return to main menu___";
        system("pause>0");
        break;
    }
    case UpdateClient:
    {
        AccountNum = ReadString("Enter account number:");
        UpdateDataInFile(AccountNum, Clients);
        cout << "Press any key to return to main menu___";
        system("pause>0");
        break;
    }
    case FindClient:
    {
        AccountNum = ReadString("Enter account number:");
        PrintClientCard(AccountNum, Clients);
        cout << "Press any key to return to main menu___";
        system("pause>0");
        break;
    }
    case Transactions:
    {
        TransactionsProcess(Clients);
        break;
    }
    case Exit:
    {
        break;
    }
    }
    system("cls");
}

void Bank()
{
    int Input;
    enMainMenu Choice;
    vector<stClient> Clients = LoadDataFromFile(FileName);
    do
    {
        MainScreen();

        Input = ReturnClientInput(1, 7);
        Choice = ChoiceToEnum(Input);
        BankSystem(Clients, Choice);
        Clients = LoadDataFromFile(FileName);
    } while (Choice != enMainMenu::Exit);
}

int main()
{
    Bank();

    return 0;
}