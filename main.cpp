#include <cctype>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientFile = "Clients.txt";
const string UsersFile = "Users.txt";

enum enMainMenuOptions 
{
    showClientsList = 1,
    addClient, deleteClient,
    updateClientInfo, findClient, 
    transactions, manageUsers, logout,
};

enum enManageUsersMenu
{
    listUsers = 1, AddUser = 2, deleteUser = 3,
    updateUser = 4, findUser = 5, mainMenu = 6,
};

enum enPermissions 
{
    FullAccess = -1, Show = 1, Add = 2, Delete = 4, 
    UpdateInfo = 8, Find = 16, Transactios = 32, ManageUsers = 64,
};


struct stClientData 
{
    string accountNumber;
    string pinCode;
    string clientName;
    string phone;
    double accountBalance;
    bool markForDelete = false;
};

struct stUsersDate
{
    string userName;
    string passWord;
    enPermissions premission;
    bool IsActive = false;
    bool markForDelete = false;
};

void ShowMainMenu();

stUsersDate user;

int ReadUserIput(const string &message)
{
    int userInput = 0;
    
    while (true)
    {
        cout << message;
        cin >> userInput;
        
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "\n Invalid Input, Try again!" << endl;
            continue;
        }
        
        return userInput;
    }
}

vector<string> SplitString(string s, string separator = "/#/")
{
    vector<string> vString;

    string part = "";
    int pos = 0;

    while ((pos = s.find(separator)) != string::npos)
    {
        part = s.substr(0, pos);

        if (part != "")
        {
            vString.push_back(part);
        }

        s.erase(0, pos + separator.length());
    }

    if (s != "") vString.push_back(s);

    return vString;
}

stClientData ConverLineToRecord(const string &line)
{
    vector<string> vClientData = SplitString(line);
    stClientData client;

    client.accountNumber = vClientData[0];
    client.pinCode = vClientData[1];
    client.clientName = vClientData[2];
    client.phone = vClientData[3];
    client.accountBalance = stod(vClientData[4]);

    return client;
}

vector<stClientData> LoadFileDataToVector(const string &fileName)
{
    vector<stClientData> vClientsData;

    fstream file;
    file.open(fileName, ios::in);

    if (file.is_open())
    {
        string line;
        stClientData Client;
    
        while(getline(file, line))
        {
            if (line != "")
            {
                Client = ConverLineToRecord(line);
                vClientsData.push_back(Client);
            }
        }

        file.close();
    }

    return vClientsData;
}

void ShowClientsList()
{
    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);

    cout << "                   Clients List (" << vClientsData.size() << ") Client(s)" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;
    cout <<
    "|" << left << setw(20) << "Account Number" <<
    "|" << left << setw(13) << "Pin Code" <<
    "|" << left << setw(25) << "Client Name" <<
    "|" << left << setw(13) << "Phone" <<
    "|" << left << setw(20) << "Account Balance" << 
    "|" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;

    for (stClientData &client : vClientsData)
    {
        cout <<
        "|" << left << setw(20) << client.accountNumber <<
        "|" << left << setw(13) << client.pinCode <<
        "|" << left << setw(25) << client.clientName <<
        "|" << left << setw(13) << client.phone <<
        "|" << left << setw(20) << client.accountBalance << 
        "|" << endl;
    }

    cout << "-------------------------------------------------------------------------------------------------" << endl;
    system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");

}

bool SearchForClient(stClientData &Client, string accountNumber)
{
    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);

    for (stClientData &C : vClientsData)
    {
        if (C.accountNumber == accountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

bool ClientExistsByAccountNumber(string accountNumber)
{
    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);

    for (stClientData &Client : vClientsData)
    {
        if (Client.accountNumber == accountNumber)
        {
            return true;
        }
    }

    return false;
}

stClientData ReadClientInfo()
{
    stClientData client;

    cout << "Adding New Client: " << endl;

    cout << "\nEnter Account Number: ";
    getline(cin >> ws, client.accountNumber);

    while (ClientExistsByAccountNumber(client.accountNumber))
    {
        cout << "Client with [" << client.accountNumber << "] already exists, Enter anouther account number: ";
        getline(cin >> ws, client.accountNumber);
    }

    cout << "Enter Pin Code: ";
    getline(cin, client.pinCode);

    cout << "Enter Client Name: ";
    getline(cin, client.clientName);

    cout << "Enter Phone: ";
    getline(cin, client.phone);

    cout << "Enter account balance: ";
    cin >> client.accountBalance;

    return client;
}

string ConvertRecordToLine(const stClientData &client, string separator = "/#/")
{
    string DataLine = "";

    DataLine += client.accountNumber + separator;
    DataLine += client.pinCode + separator;
    DataLine += client.clientName + separator;
    DataLine += client.phone + separator;
    DataLine += to_string(client.accountBalance);

    return DataLine;
}

void AddDataLineToFile(string DataLine, string fileName)
{
    fstream file;
    file.open(fileName, ios::app);

    if (file.is_open())
    {
        file << DataLine << endl;
        file.close();
    }
}

void AddClient()
{
    stClientData client = ReadClientInfo();
    AddDataLineToFile(ConvertRecordToLine(client), ClientFile);
}

void AddNewCliets()
{
    char addMore = 'y';

    do {

        AddClient();

        cout << "\n Do you wanto add more clients? (y/n): ";
        cin >> addMore;

    } while (tolower(addMore) == 'y');
}

void AddClientScrean()
{
    cout << "=========================================" << endl;
    cout << "             Add New Clients" << endl;
    cout << "=========================================" << endl;

    AddNewCliets();
}

string ReadAccountNumber()
{
    string accountNumber;

    cout << "Please enter account number: ";
    getline(cin >> ws, accountNumber);

    return accountNumber;
}

void PrintClientCard(stClientData client)
{
    cout << "\nThe following are the client details:" << endl;
    cout << "============================================" << endl;
    cout << "Account Number: " << client.accountNumber << endl;
    cout << "Pin Code: " << client.pinCode << endl;
    cout << "Client Name: " << client.clientName << endl;
    cout << "Phone: " << client.phone << endl;
    cout << "Account Balance: " << client.accountBalance << endl;
    cout << "============================================" << endl;
}

void MarkClientForDelete( vector<stClientData> &vClientsData ,string accountNumber)
{
    for (stClientData &client : vClientsData)
    {
        if (client.accountNumber == accountNumber)
        {
            client.markForDelete = true;
        }
    }
}

void SaveCleintsDataToFile(vector<stClientData> vClientsData, string fileName)
{
    fstream file;
    file.open(fileName, ios::out);

    if (file.is_open())
    {
        for (stClientData &client : vClientsData)
        {
            if (client.markForDelete == false)
            {
                file << ConvertRecordToLine(client) << endl;
            }
        }

        file.close();
    }
}

void DeleteClientByAccountNumber(vector<stClientData> &vClientData, string accountNumber)
{
    stClientData client;
    char deleteClient = 'y';

    if (SearchForClient( client, accountNumber ))
    {
        PrintClientCard(client);

        cout << "\n Are you sure do you want to delete this client? (y/n): ";
        cin >> deleteClient;

        if (tolower(deleteClient) == 'y')
        {
            MarkClientForDelete(vClientData, accountNumber);
            SaveCleintsDataToFile(vClientData, ClientFile);

            cout << "\nClient Deleted successflly." << endl;
        }

        else 
        {
            cout << "\nClient didn't deleted." << endl;
        }

    }

    else 
    {
        cout << "\n Cleint with [" << accountNumber << "] Not found!" << endl;
    }
    
    system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");
}

void DeleteScrean()
{
    cout << "============================================" << endl;
    cout << "            Delete Client Screan" << endl;
    cout << "============================================" << endl;

    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);
    string accountNumber = ReadAccountNumber();
    DeleteClientByAccountNumber(vClientsData, accountNumber);

}

stClientData ChangeClientRecord(string accountNumber)
{
    stClientData client;

    client.accountNumber = accountNumber;

    cout << endl;

    cout << "Enter Pin Code: ";
    getline(cin >> ws, client.pinCode);

    cout << "Enter Client Name: ";
    getline(cin, client.clientName);

    cout << "Enter Phone: ";
    getline(cin, client.phone);

    cout << "Enter Account Balance: ";
    cin >> client.accountBalance;

    return client;
}

void UpdateClientInfo(vector<stClientData> &vClientsData, string accountNumber)
{
    stClientData client;
    char answer = 'n';

    if (SearchForClient(client, accountNumber))
    {
        PrintClientCard(client);

        cout << "Are you sure do you want to update this client info? (y/n): ";
        cin >> answer;

        if (tolower(answer) == 'y')
        {
            for (stClientData &C : vClientsData)
            {
                if (C.accountNumber == accountNumber)
                {
                    C = ChangeClientRecord(accountNumber);
                }
            }

            SaveCleintsDataToFile(vClientsData, ClientFile);

            cout << "\n Client update successflly." << endl;
        }

        else  
        {
            cout << "\n Client didn't update." << endl;
        }
    }

    else 
    {
        cout << "\n Cleint with [" << accountNumber << "] Not found!" << endl;
    }
    
    system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");


}

void UpdateScrean()
{
    cout << "===============================================" << endl;
    cout << "            Update Client Info Screan" << endl;
    cout << "===============================================" << endl;

    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);
    string accountNumber = ReadAccountNumber();

    UpdateClientInfo(vClientsData, accountNumber);
}

void FindClient(vector<stClientData> &vClientsData, string accountNumber)
{
    stClientData client;

    if (SearchForClient(client, accountNumber))
    {
        PrintClientCard(client);
    }

    else  
    {
        cout << "Client with [" << accountNumber << "] Not found!" << endl;
    }

    system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");
}

void FindClientScrean()
{
    cout << "=========================================" << endl;
    cout << "           Find Client Screan" << endl;
    cout << "=========================================" << endl;

    vector<stClientData> vClientData = LoadFileDataToVector(ClientFile);
    string accountNumber = ReadAccountNumber();

    FindClient(vClientData, accountNumber);
}

void ExitScrean()
{
    cout << "======================================" << endl;
    cout << "          Program Ends :-)" << endl;
    cout << "======================================" << endl;
}

// Transactions part.

enum enTransactionsMenuOptions { Deposit = 1, Withdraw, TotalBalances, MainMenu };
void ShowTransactionsMenu();

void DepositAmountToClient(vector<stClientData> &vClientsData)
{
    stClientData client;
    string accountNumber = ReadAccountNumber();
    
    double depositAmount;

    while (! SearchForClient(client, accountNumber))
    {
        cout << "Client with [" << accountNumber << "] Does not exists!" << endl;
        accountNumber = ReadAccountNumber();
    }

    PrintClientCard(client);

    cout << "Please enter deposit amount: " << endl;
    cin >> depositAmount;

    for (stClientData &C : vClientsData)
    {
        if (C.accountNumber == accountNumber)
        {
            C.accountBalance += depositAmount;
        }
    }

    SaveCleintsDataToFile(vClientsData, ClientFile);
}

void ShowDepositScrean()
{
    cout << "====================================" << endl;
    cout << "          Deposti Screan" << endl;
    cout << "====================================" << endl;

    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);

    DepositAmountToClient(vClientsData );
}

void WithdrawAmount(vector<stClientData> &vClientsData)
{
    stClientData client;
    string accountNumber = ReadAccountNumber();

    double withdrawAmount;
    char answer = 'n';

    while (! SearchForClient(client, accountNumber))
    {
        cout << "Client with [" << accountNumber << "] Does not exists!" << endl;
        accountNumber = ReadAccountNumber();
    }

    PrintClientCard(client);

    cout << "Please enter withdraw amount: ";
    cin >> withdrawAmount;

    while (client.accountBalance < withdrawAmount)
    {
        cout << "\n Amount Exceeds the balance, you can withdraw up to: " << client.accountBalance << endl;
        cout << "Please enter withdraw amount: ";
        cin >> withdrawAmount;
    }

    cout << "\n Are you sure do you want perform this transaction? (y/n): ";
    cin >> answer;

    if (tolower(answer) == 'y')
    {
        for (stClientData &C : vClientsData)
        {
            if (C.accountNumber == accountNumber)
            {
                C.accountBalance -= withdrawAmount;
            }
        }
        
        SaveCleintsDataToFile(vClientsData, ClientFile);
        
        cout << "\nThe transaction done successflly." << endl;
    }

    else  
    {
        cout << "\nThe transaction didn't hapened." << endl;
    }

    system("bash -c \"read -n 1 -s -r -p '\nPress any key to go back to Transactions menu ...'\"");
}

void ShowWithDrawScreen()
{
    cout << "=========================================" << endl;
    cout << "             Withdraw Screan" << endl;
    cout << "=========================================" << endl;

    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);
    WithdrawAmount(vClientsData);
}

double CountTotalBalance()
{
    vector<stClientData> vClientsData = LoadFileDataToVector(ClientFile);

    double TotalBalances = 0;

    for (stClientData &C : vClientsData)
    {
        TotalBalances += C.accountBalance;
    }

    return TotalBalances;
}

void ShowTotalBalancesScreen()
{
    ShowClientsList();

    cout << "\t\t\t Total Balances = " << CountTotalBalance() << endl;

    system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to Transactions menu ...'\"");
}

void PreformTransactionsMenu(enTransactionsMenuOptions userChoice)
{
    switch (userChoice)
    {
        case enTransactionsMenuOptions::Deposit:
            system("clear");
            ShowDepositScrean();
            ShowTransactionsMenu();
            break;

        case enTransactionsMenuOptions::Withdraw:
            system("clear");
            ShowWithDrawScreen();
            ShowTransactionsMenu();
            break;        
        
        case enTransactionsMenuOptions::TotalBalances:
            system("clear");
            ShowTotalBalancesScreen();
            ShowTransactionsMenu();
            break;        
        
        case enTransactionsMenuOptions::MainMenu:
            ShowMainMenu();
            break;

        default:
            cout << "Invalid input!" << endl;
            system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to Transactions menu ...'\"");
            ShowTransactionsMenu();
    }

}

void ShowTransactionsMenu()
{
    system("clear");

    cout << "===========================================" << endl;
    cout << "          Transactions Menu Screan" << endl;
    cout << "===========================================" << endl;
    cout << "      [1] Deposit." << endl;
    cout << "      [2] Withdraw." << endl;
    cout << "      [3] Total Balances." << endl;
    cout << "      [4] Main Menu." << endl;
    cout << "===========================================" << endl;

    PreformTransactionsMenu((enTransactionsMenuOptions) ReadUserIput("Choose what do you want to do? [1 to 4]: "));
}

// Manage Users part.

void Login();
void ShowManageUsersMenu();

stUsersDate ConverUserDataLineToRecord(string dataLine)
{
    vector<string> vString = SplitString(dataLine);
    stUsersDate userData;

    userData.userName = vString[0];
    userData.passWord = vString[1];
    userData.premission = enPermissions (stoi(vString[2]));

    return userData;
}

vector<stUsersDate> LoadUsersDataFileToVector(string UsersFile)
{
    vector<stUsersDate> vUsersData;

    fstream file;
    file.open(UsersFile, ios::in);

    if (file.is_open())
    {
        stUsersDate user;
        string line;

        while (getline(file, line))
        {
            
            if (line != "")
            {
                user = ConverUserDataLineToRecord(line);
                vUsersData.push_back(user);
            }
        }

        file.close();
    }

    return vUsersData;
}

string ConvertUserRecordToLine(stUsersDate user, string separator = "/#/")
{
    string dataLine = "";

    dataLine += user.userName + separator;
    dataLine += user.passWord + separator;
    dataLine += to_string(user.premission);

    return dataLine;
}

void SaveUsersDataToFile(vector<stUsersDate> vUsersData ,string fileName)
{
    fstream file;
    file.open(fileName, ios::out);

    if (file.is_open())
    {
        for (stUsersDate &user : vUsersData)
        {
            if (! user.markForDelete)
                file << ConvertUserRecordToLine(user) << endl;
        }

        file.close();
    }
}

void PrintUserInfo(stUsersDate user)
{
    cout << "|" << setw(15) << user.userName
         << "|" << setw(10) << user.passWord
         << "|" << setw(15) << user.premission
         << endl;
}

void ShowUsersList()
{
    vector<stUsersDate> vUsersData = LoadUsersDataFileToVector(UsersFile);

    cout << "               Users List (" << vUsersData.size() << ") User(s)" << endl;
    cout << "___________________________________________________________________" << endl;
    cout << "|" << setw(15) << "User Name"
         << "|" << setw(10) << "PassWord"
         << "|" << setw(15) << "permissions" 
         << endl;
    cout << "___________________________________________________________________" << endl;

    for (stUsersDate &user : vUsersData)
    {
        PrintUserInfo(user);
    }

    cout << "___________________________________________________________________" << endl;

    system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");

}

void AddUsersScreen()
{
    cout << "====================================" << endl;
    cout << "          Add users screen" << endl;
    cout << "====================================" << endl;
}

bool FindUser(string userName, stUsersDate &user)
{
    vector<stUsersDate> vUsersData = LoadUsersDataFileToVector(UsersFile);

    for (stUsersDate &U : vUsersData)
    {
        if (U.userName == userName) 
        {
            user = U;
            return true;
        }
    }

    return false;
}

int GetUserPermisson()
{
    char yes_no = 'n';
    int permission = 0;

    cout << "Do you want to give full access? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y') return -1;

    cout << "Do you want to give access to:" << endl;

    cout << "Show Client List? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y') 
    {
        permission |= enPermissions::Show;
    }

    cout << "Add New Clients? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y')
    {
        permission = permission | enPermissions::Add;
    }

    cout << "Delete Client? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y')
    {
        permission = permission | enPermissions::Delete;
    }

    cout << "Update Client? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y')
    {
        permission = permission | enPermissions::UpdateInfo;
    }
    
    cout << "Find Client? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y')
    {
        permission = permission | enPermissions::Find;
    }

    cout << "Transactions? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y')
    {
        permission = permission | enPermissions::Transactios;
    }

    cout << "Manage Users? (y/n): ";
    cin >> yes_no;

    if (tolower(yes_no) == 'y')
    {
        permission = permission | enPermissions::ManageUsers;
    }

    return permission;
}

bool UserExistsByUserName(string userName)
{
    vector<stUsersDate> vUsersData = LoadUsersDataFileToVector(UsersFile);

    for (stUsersDate &U : vUsersData)
    {
        if (U.userName == userName) return true;
    }

    return false;
}

stUsersDate ReadUserInfo()
{
    stUsersDate user;

    cout << "Adding New User: " << endl;

    cout << "Enter user name: ";
    cin >> user.userName;

    while (UserExistsByUserName(user.userName))
    {
        cout << "User with [" << user.userName <<  "] already exists, Try another one: ";
        cin >> user.userName;
    }

    cout << "enter pass word: ";
    cin >> user.passWord;

    user.premission = (enPermissions) GetUserPermisson();

    return user;
}

void AddOneUser()
{
    stUsersDate user;

    fstream file;
    file.open(UsersFile, ios::app);

    if (file.is_open())
    {
        user = ReadUserInfo(); 
        file << ConvertUserRecordToLine(user) << endl;

        file.close();
    }
}

void AddUsers()
{
    system("clear");
    AddUsersScreen();

    char addMore = 'n';

    do {

        AddOneUser();

        cout << "\nDo you want to add more users? (y/n): ";
        cin >> addMore;

    } while (tolower(addMore) == 'y');
}

void DeleteUserScreen()
{
    cout << "=========================================" << endl;
    cout << "            Delete User Screen" << endl;
    cout << "=========================================" << endl;
}

string ReadUserName()
{
    string userName;
    cout << "Please enter UserName? ";
    cin >>userName;
    return userName;
}

void PrintUserCard(stUsersDate user)
{
    cout << "\nThe following are the user details: " << endl;
    cout << "=========================================" << endl;
    cout << "UserName   : " << user.userName << endl;
    cout << "PassWord   : " << user.passWord << endl;
    cout << "Permission : " << user.premission << endl;
    cout << "=========================================" << endl;
}

void DeleteUser()
{
    DeleteUserScreen();

    vector<stUsersDate> vUsersData = LoadUsersDataFileToVector(UsersFile);
    string userName = ReadUserName();
    stUsersDate userToDelete;

    char yes_no = 'n';

    if (userName == "Admin")
    {
        cout << "You cannot delete this user!" << endl;
    }

    else 
    {
        if (FindUser(userName,userToDelete))
        {
            PrintUserCard(userToDelete);
            
            cout << "\nAre you sure do you wanto to delete this user? (y/n): ";
            cin >> yes_no;
            
            if (tolower(yes_no) == 'y')
            {
                
                for (stUsersDate &user : vUsersData)
                {
                    if (user.userName == userName)
                    {
                        user.markForDelete = true;
                        SaveUsersDataToFile(vUsersData, UsersFile);
                    }
                }
                
                cout << "User deleted successflly." << endl;
            }
            
            else cout << "The user did not deleted!" << endl;

        }
        
        else cout << "\nThis user not found, try again!" << endl;
    }
    
    
    system("bash -c \"read -n 1 -s -r -p '\nPress any key to go back to main menu ...'\"");

    if (yes_no == 'y' && user.userName == userName)
        Login();
}

void ShowUpdateScreen()
{
    cout << "=======================================" << endl;
    cout << "        Update User Info Screeen" << endl;
    cout << "=======================================" << endl;
}

stUsersDate ReadUserNewInfo(string userName)
{
    stUsersDate user;

    user.userName = userName;

    cout << "Please enter passWord: ";
    cin >> user.passWord;
    user.premission = (enPermissions) GetUserPermisson();

    return user;
}

void UpdateUserInfo()
{
    ShowUpdateScreen();

    vector<stUsersDate> vUsersData = LoadUsersDataFileToVector(UsersFile);
    stUsersDate user;
    string userName = ReadUserName();

    char yes_no = 'n';

    if (FindUser(userName, user))
    {
        PrintUserCard(user);

        cout << "\nAre you sure do you want to updat this user info? (y/n): ";
        cin >> yes_no;

        if (tolower(yes_no) == 'y')
        {
            for (stUsersDate &U : vUsersData)
            {
                if (U.userName == userName)
                {
                    U = ReadUserNewInfo(userName);
                    SaveUsersDataToFile(vUsersData, UsersFile);
                }
            }

            cout << "\nUser updated successflly." << endl;
        }

        else  cout << "User did not updated!" << endl;
    }

    else cout << "User not found, try again!" << endl;

    system("bash -c \"read -n 1 -s -r -p '\nPress any key to go back to main menu ...'\"");
}

void FindUserScreen()
{
    cout << "===================================" << endl;
    cout << "         Find User Screen" << endl;
    cout << "===================================" << endl;
}

void FindUserWithUserName()
{
    FindUserScreen();

    stUsersDate user;
    string userName = ReadUserName();

    if (FindUser(userName,user))
    {
        PrintUserCard(user);
    }

    else cout << "\nUser Not Found!" << endl;
    
    system("bash -c \"read -n 1 -s -r -p '\nPress any key to go back to main menu ...'\"");
}
    
void PreformManageUsersMenu(enManageUsersMenu userChoice)
{
    switch (userChoice)
    {

        case enManageUsersMenu::listUsers:

            system("clear");
            ShowUsersList();
            ShowManageUsersMenu();
            break;        

        case enManageUsersMenu::AddUser:

            system("clear");
            AddUsers();
            ShowManageUsersMenu();
            break;        

        case enManageUsersMenu::deleteUser:

            system("clear");
            DeleteUser();
            ShowManageUsersMenu();
            break;        

        case enManageUsersMenu::updateUser:

            system("clear");
            UpdateUserInfo();
            ShowManageUsersMenu();
            break;        

        case enManageUsersMenu::findUser:

            system("clear");
            FindUserWithUserName();
            ShowManageUsersMenu();
            break;        

        case enManageUsersMenu::mainMenu:

            system("clear");
            ShowMainMenu();
            break;        

        default:

            cout << "\nInvalid Input! Try again.\n" << endl;
            system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");
            ShowManageUsersMenu();
            
    }
}

void ShowManageUsersMenu()
{
    system("clear");

    cout << "=========================================" << endl;
    cout << "            Manage users Menu" << endl;
    cout << "=========================================" << endl;
    cout << "\t[1] List Users." << endl;
    cout << "\t[2] Add New User." << endl;
    cout << "\t[3] Delete User." << endl;
    cout << "\t[4] Update User." << endl;
    cout << "\t[5] Find User." << endl;
    cout << "\t[6] Main Menu." << endl;
    cout << "==========================================" << endl;

    PreformManageUsersMenu((enManageUsersMenu) ReadUserIput("Choose what do you want to do? [1 ot 6]"));
}

bool HasPermission( int permissionNumber)
{     
    return ( user.premission == -1 || user.premission & permissionNumber ) ? true : false;
}

void ShowDniedScreen()
{
    cout << "==========================================" << endl;
    cout << "Access Denied," << endl;
    cout << "You don't have permission to do this," << endl;
    cout << "Please contact your admin." << endl;
    cout << "==========================================" << endl;

    system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");
}


void PerformMainMenuOpstions( enMainMenuOptions userChoice)
{
    switch (userChoice)
    {
        case enMainMenuOptions::showClientsList:
                    
            system("clear");
            
            if (HasPermission(enPermissions::Show))
                ShowClientsList();
            else
                ShowDniedScreen();

            ShowMainMenu();
            break;

        case enMainMenuOptions::addClient:
                
            system("clear");
            
            if (HasPermission(enPermissions::Add))
                AddClientScrean();
            else
                ShowDniedScreen();

            ShowMainMenu();
            break;

        case enMainMenuOptions::deleteClient:
                
            system("clear");
            
            if (HasPermission(enPermissions::Delete))
               DeleteScrean();
            else
                ShowDniedScreen();

            ShowMainMenu();
            break;

        case enMainMenuOptions::updateClientInfo:
                
            system("clear");
            
            if (HasPermission(enPermissions::UpdateInfo))
               UpdateScrean();
            else
                ShowDniedScreen();

            ShowMainMenu();
            break;

        case enMainMenuOptions::findClient:
                
            system("clear");
            
            if (HasPermission(enPermissions::Find))
                FindClientScrean();
            else
                ShowDniedScreen();

            ShowMainMenu();
            break;

        case enMainMenuOptions::transactions:
                
            system("clear");
            
            if (HasPermission(enPermissions::Transactios))
                ShowTransactionsMenu();
            else
                ShowDniedScreen();

            ShowMainMenu();
            break;

        case enMainMenuOptions::manageUsers:
                
            system("clear");
            
            if (HasPermission(enPermissions::ManageUsers))
                ShowManageUsersMenu();
            else
                ShowDniedScreen();

            ShowMainMenu();
            break;
            
        case enMainMenuOptions::logout:
            
            Login();
            break;

        default:
            cout << "Invalid input!" << endl;    
            system("bash -c \"read -n 1 -s -r -p 'Press any key to go back to main menu ...'\"");
            ShowMainMenu();
    }
}

void ShowMainMenu()
{
    system("clear");

    cout << "=========================================" << endl;
    cout << "             Main Menu Screan" << endl;
    cout << "=========================================" << endl;
    cout << "\t[1] Show Client List." << endl;
    cout << "\t[2] Add New Client." << endl;
    cout << "\t[3] Delete Client." << endl;
    cout << "\t[4] Update Cleint Info." << endl;
    cout << "\t[5] Finde Client." << endl;
    cout << "\t[6] Transactions." << endl;
    cout << "\t[7] Manage Users." << endl;
    cout << "\t[8] Logout." << endl;
    cout << "=========================================" << endl;

    PerformMainMenuOpstions( (enMainMenuOptions) ReadUserIput("Choose what do you want to do? [1 to 8]: "));
}

string ReadString(string message)
{
    string userInput;

    cout << message;
    cin >> userInput;

    return userInput;
}

void LoginScrean()
{
    cout << "=====================================" << endl;
    cout << "             Login Screen" << endl;
    cout << "=====================================" << endl;
}

bool ActiveUser(string userName, string passWord, stUsersDate &user)
{
    vector<stUsersDate> vUsersData = LoadUsersDataFileToVector(UsersFile);

    for (stUsersDate &U : vUsersData)
    {
        if (U.userName == userName && U.passWord == passWord)
        {
            user = U;
            user.IsActive = true;
            return true;
        }
    }

    return false;
}

void Login()
{
    system("clear");
    LoginScrean();

    while (true)
    {

        string userName = ReadString("Please enter UserName: ");
        string passWord = ReadString("Please enter PassWord:");

        if (ActiveUser(userName, passWord, user))
        {

            ShowMainMenu();
        }

        else  
        {
            system("clear");
            LoginScrean();

            cout << "Invalid UserName / PassWord!" << endl;
        }
    }
}

int main()
{

    Login();
    

    return 0;
}
