#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include<vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::setw;
using std::fixed;
using std::setprecision;

using std::ios;

using std::ofstream;
using std::ifstream;

struct vendingM{
    int cookiesCode; 
	string cookiesName; 
	int cookiesQuantity;
	float cookiesPrice;
};


enum AccountType {
	USER = 1,
	VENDOR
};

struct Account {
	AccountType type;
	string name;
	string password;
    float balance;
};

void load_data(std::vector<Account>& users, std::vector<vendingM>& cookies);
void save_data(const std::vector<Account>& users, const std::vector<vendingM>& cookies);
void registr();
bool login(Account ** acc, std::vector<Account>& users);
void menu(Account * acc, std::vector<vendingM>& cookies);
void vending(Account * acc, std::vector<vendingM>& cookies);
void deposit(Account * acc);
void purchase(Account * acc, vendingM * cookie);
void vendor(std::vector<vendingM>& cookies);

enum LoginChoice {
	REGISTER = 1,
	LOGIN,
	EXIT
};

enum MenuChoice {
    BUY = 1,
    CHECK_BAL,
    DEPOSIT,
    RETURN
};

int main()
{
    int choice;
	Account * acc;
	while (true)
    {
		cout << "                      Welcome to the login page                               \n\n";
		cout << "*******************       << MENU >>       *******************************\n\n\n";
		cout << "1.REGISTER" << endl;
		cout << "2.LOGIN" << endl;
		cout << "3.Exit" << endl;
		cout << "Please select an option (enter 1, 2, 3): ";
		cin>>choice;
		cout<< endl;
		switch(choice)
		{
			case REGISTER:
				registr();
				break;
			case LOGIN:
            {
                std::vector<vendingM> cookies;
                std::vector<Account> users;
                load_data(users, cookies);
		if(login(&acc, users))
                {
		    if(acc->type == USER)
                    {
                        menu(acc, cookies);
                    }
                    else if(acc->type == VENDOR)
                    {
                        vendor(cookies);
                    }
                    save_data(users, cookies);
                }
				break;
            }
			case EXIT:
				return 0;
			default:
				system("cls");
				cout<<"Sorry, you've entered an invalid choice , please try again.\n"<<endl;
		}
	}
}

void registr()
{
    string reguser,regpass,ru,rp;
    system("cls");
    cout<<"Enter the username :";
    cin>>reguser;
    cout<<"\nEnter the password :";
    cin>>regpass;

	int choice;
	do {
		cout << "Select your account type." << endl;
		cout << "1. User" << endl;
		cout << "2. Vendor" << endl;

		cin >> choice;
	} while (choice != 1 && choice != 2);

    ofstream reg("database.txt",ios::app);
    reg<< reguser<< ' ' << regpass << ' ' << choice << ' ' << 0 << endl;
    system("cls");
    cout<<"\nRegistration Successful\nPlease press Enter\n";
    cin.get();
    cin.get();
}

bool login(Account ** acc, std::vector<Account>& users)
{
    bool success;
    string username, pass;        
    cout << "Please enter the following details" << endl;
    cout << "\nUSERNAME :";
    cin >> username;
    cout << "\nPASSWORD :";
    cin >> pass;

    for(int i = 0; i < users.size(); i++)
    {
        success = 
            username == users[i].name && pass == users[i].password ? 
                true : false;
        if(success)
        {
            *acc = &users[i];
            break;
        }
    }

    if(success)
    {
        cout<<"\nHello "<< (**acc).name <<"\n<LOGIN SUCCESSFUL>\nThanks for logging in..\n";
        cin.get();
        cin.get();
        return true;
    }
    else
    {
        cout<<"\nLOGIN ERROR\nPlease check your username and password\n";
        return false;
    }
}

void menu(Account * acc, std::vector<vendingM>& cookies)
{
    int choice;
    while(true)
    {
        cout << "1. Buy Cookies" << endl
            << "2. Check Balance" << endl
            << "3. Deposit Into Account" << endl
            << "4. Return to Main Menu" << endl;
        cin >> choice;
        switch(choice)
        {
            case BUY:
                vending(acc, cookies);
                break;
            case CHECK_BAL:
                cout << "You have RM " << acc->balance << endl; 
                break;
            case DEPOSIT:
                deposit(acc);
                break;
            case RETURN:
                return;
            default:
                cout << "Sorry, you've entered an invalid choice, please try again.\n" << endl;
        }
    }
}

void vending(Account * acc, std::vector<vendingM>& cookies)
{
	cout << "____________________________________________________________________________"<< endl;
	cout << "No." << setw(10) << "Code" << setw(25) << "Cookies" << setw(10) << "\t\tAvailable"
	     << setw(10) << "\tPrice per Item (RM)" << endl;
	cout << "---------------------------------------------------------------------------"<< endl;
    cout << "0."<< setw(10) <<"Cancel Purchase" << endl;
    int count = 1;
	for(int currentCookie=0; currentCookie < cookies.size(); currentCookie++)
	{
		vendingM cookie = cookies[currentCookie];
		cout<< count++ << setw(10) <<cookie.cookiesCode<<setw(25)
			<<fixed<<setprecision(2)<<cookie.cookiesName
			<<setw(15)<<cookie.cookiesQuantity<<setw(20)<<cookie.cookiesPrice<<endl;
	}
    cout << "---------------------------------------------------------------------------"<<endl;
    cout<<"Enter Number: ";
    int choice;
    bool validChoice, inStock;
    do{
        cin>>choice;
        // Cancelling Purchase
        if(choice == 0)
            return;

        validChoice = choice <= cookies.size() && choice > 0;
        inStock = validChoice? cookies[choice - 1].cookiesQuantity > 0 : false; 
        if(!validChoice)
        {
            cout<<"\nInvalid Selection!" << endl
                <<"Select another Cookie: ";
        }
        else if(!inStock)
        {
            cout << "\nOut of Stock!"<<endl;
        }
    }while(!validChoice || !inStock);
    purchase(acc, &cookies[choice-1]);
}

void deposit(Account *acc)
{
    int count_five, count_ten;
    cout << "Your current balance is : RM " << acc->balance << endl;
    do{
        cout << "How many notes of RM 5.00 to deposit?" << endl;
        cin >> count_five;
        cout << "How many notes of RM 10.00 to deposit?" << endl;
        cin >> count_ten;
        if(count_five < 0 || count_ten < 0)
            cout << "Invalid values!"<< endl;
    }while(count_five < 0 || count_ten < 0);
    
    acc->balance += count_five * 5 + count_ten * 10;
    cout << "Your new balance is : RM " << acc->balance << endl;
}

void load_data(std::vector<Account>& users,std::vector<vendingM>& cookies)
{
    ifstream cookies_data("cookies_data.txt");
    vendingM cookie_data;
    while(cookies_data>> cookie_data.cookiesCode
            >> cookie_data.cookiesName
            >> cookie_data.cookiesQuantity
            >> cookie_data.cookiesPrice)
    {
        cookies.push_back(cookie_data);
    }
    cookies_data.close();

    ifstream users_data("database.txt");
    Account acc;
    int type;
    while(users_data>> acc.name
            >> acc.password
            >> type
            >> acc.balance)
    {
        acc.type = type == VENDOR ? VENDOR : USER;
        users.push_back(acc);
    }
    users_data.close();
}

void save_data(const std::vector<Account>& users,const std::vector<vendingM>& cookies)
{
    ofstream database_user("database.txt");
    for(int currentUser = 0; currentUser < users.size(); currentUser++ )
    {
	    Account user = users[currentUser];
        database_user << user.name << " "
            << user.password << " "
            << (int) user.type << " "
            << user.balance << " "
            << "\n";
    }
    database_user.close();

    ofstream database_cookies("cookies_data.txt");
    for(int currentCookie = 0; currentCookie < cookies.size(); currentCookie++)
    {
	    vendingM cookie = cookies[currentCookie];
        database_cookies << cookie.cookiesCode << " "
            << cookie.cookiesName << " "
            << cookie.cookiesQuantity << " "
            << cookie.cookiesPrice << " "
            << "\n";
    }
    database_cookies.close();
}

void purchase(Account *acc, vendingM *cookie)
{
    int quantity, money;
    float totalCost;
    do {
        cout << "Cookie: " << cookie->cookiesName << endl
            << "Price per item: " << cookie->cookiesPrice <<endl
            << "Enter quantity: ";
        cin >> quantity;
        
        if(quantity < 1 || quantity > cookie -> cookiesQuantity)
        {
            cout << "Only " << cookie -> cookiesQuantity
                << " available. Please enter a number between "
                << " 1 and " << cookie -> cookiesQuantity << endl;
        }
    } while(quantity > cookie -> cookiesQuantity);

    totalCost = cookie -> cookiesPrice * quantity;
    if(acc->balance >= totalCost)
    {
        cout << "You bought "<< quantity << " " 
            << cookie->cookiesName << " for RM " 
            << totalCost << endl;
        cookie->cookiesQuantity -= quantity;
        acc->balance -= totalCost;
    }
    else
    {
        cout << "You'll need to deposit RM" << totalCost - acc->balance 
            << " into your account! " <<  endl;
    }
}

void vendor(std::vector<vendingM>& cookies)
{
	int code,stock; //declaration
	char ra; //declaration for restock again
	
	while(true){
        cout<<"------------------------------------------------------------------" << endl;
        cout<<"Code"<<setw(18)<<"Cookies"<<setw(15)<<"\tAvailable"
            <<setw(15)<<"\tPrice per Item (RM)"<< endl;
        cout<<"------------------------------------------------------------------" << endl;
        //list of the data from file
        for(int currentCookie=0;currentCookie < cookies.size(); currentCookie++)
        {
		vendingM cookie = cookies[currentCookie];
                cout<<cookie.cookiesCode<<setw(18)<<fixed<<setprecision(2)
                    <<cookie.cookiesName<<setw(16)<<cookie.cookiesQuantity<<setw(20)
                    <<cookie.cookiesPrice<<endl;
        }
        cout<<"------------------------------------------------------------------";
        
        cout<<endl<<"Enter cookies code that need to be restock: ";
        cin>>code;
        for(int currentCookie=0;currentCookie < cookies.size(); currentCookie++)
        {
	    vendingM cookie = cookies[currentCookie];
            if(code==cookie.cookiesCode)
            {
                cout<<"Enter amount to restock: ";
                cin>>stock;
                cookies[currentCookie].cookiesQuantity += stock;
                cout<<cookie.cookiesName<<" is Restocked" << endl;
                do{
                    cout<<"\nRestock again? Y-yes || N-no: ";
                    cin>>ra;
                    ra = toupper(ra);
                    if (ra=='N')
                    {
                        return;
                    }
                }while(ra != 'N' && ra != 'Y');
            }
        }
    }
}
	

