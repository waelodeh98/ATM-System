#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct st_Client {
	string Account_Number;
	string Pin_Code;
	string Name;
	string Phone;
	double Account_Balance;
	bool Mark_For_Delete = false;
};

enum en_Main_Menu_Options {
	e_Quick_Withdraw = 1, e_Normal_Withdraw = 2, e_Deposit = 3,
	e_Check_Balance = 4, e_Exit = 5
};

const string Clients_File_Name = "Clients.txt";

st_Client Current_Client;

void Login();

void Show_Main_Menu();

void Show_Quick_Withdraw_Screen();

void Show_Normal_Withdraw_Screen();

vector <string> Split_String(string S1, string Delimiter) {
	vector <string> V_Client;
	short Position = 0;
	string S_Word = "";

	while ((Position = S1.find(Delimiter)) != std::string::npos) {
		S_Word = S1.substr(0, Position);

		if (S_Word != "") {
			V_Client.push_back(S_Word);
		}

		S1.erase(0, Position + Delimiter.length());
	}

	if (S1 != "") {
		V_Client.push_back(S1);
	}

	return V_Client;
}

st_Client Convert_Line_To_Record(string Line, string Separator = "#//#") {
	st_Client Client;
	vector <string> V_Client_Data = Split_String(Line, Separator);
	Client.Account_Number = V_Client_Data[0];
	Client.Pin_Code = V_Client_Data[1];
	Client.Name = V_Client_Data[2];
	Client.Phone = V_Client_Data[3];
	Client.Account_Balance = stod(V_Client_Data[4]);

	return Client;
}

string Convert_Record_To_Line(st_Client Client, string Separator = "#//#") {
	string st_Client_Record = "";
	st_Client_Record += Client.Account_Number + Separator;
	st_Client_Record += Client.Pin_Code + Separator;
	st_Client_Record += Client.Name + Separator;
	st_Client_Record += Client.Phone + Separator;
	st_Client_Record += to_string(Client.Account_Balance);
	return st_Client_Record;
}

vector <st_Client> Load_Clients_Data_File(string File_Name) {
	vector <st_Client> V_Clients;

	fstream My_File;
	My_File.open(File_Name, ios::in);

	if (My_File.is_open()) {
		string Line;
		st_Client Client;

		while (getline(My_File, Line)) {
			Client = Convert_Line_To_Record(Line);
			V_Clients.push_back(Client);
		}

		My_File.close();
	}
	return V_Clients;
}

bool Find_Client_By_Account_Number_And_Pin_Code(string Account_Number, string Pin_Code, st_Client& Client) {
	vector <st_Client> V_Clients = Load_Clients_Data_File(Clients_File_Name);

	for (st_Client C : V_Clients) {
		if (C.Account_Number == Account_Number && C.Pin_Code == Pin_Code) {
			Client = C;
			return true;
		}
	}
	return false;
}

vector <st_Client> Save_Clients_Data_To_File(string File_Name, vector <st_Client> V_Clients) {
	fstream My_File;
	My_File.open(File_Name, ios::out);

	string Data_Line;

	if (My_File.is_open()) {

		for (st_Client C : V_Clients) {

			if (C.Mark_For_Delete == false) {

				Data_Line = Convert_Record_To_Line(C);
				My_File << Data_Line << endl;
			}

		}

		My_File.close();
	}
	return V_Clients;
}

bool Load_Client_Info(string Account_Number, string Pin_Code) {
	if (Find_Client_By_Account_Number_And_Pin_Code(Account_Number, Pin_Code, Current_Client))
		return true;
	else
		return false;
}

short Read_Main_Menu_Option() {
	cout << "Choose What Do You Want To Do ? [1 to 5] ? ";

	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void Go_Back_To_Main_Menu() {
	cout << "\n\nPress Any Key To Go Back To Main Menu...";
	system("pause>0");
	Show_Main_Menu();
}

short Read_Quick_Withdraw_Option() {
	short Choice = 0;
	while (Choice < 1 || Choice > 9) {
		cout << "\nChoose What To Do From [1] to [9] ? ";
		cin >> Choice;
	}
	return Choice;
}

short Get_Quick_Withdraw_Amount(short Quick_Withdraw_Option) {
	switch (Quick_Withdraw_Option) {
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

bool Deposit_Balance_To_Client_By_Account_Number(string Account_Number, double Amount, vector <st_Client>& V_Clients) {
	char Answer = 'N';

	cout << "\n\nAre You Sure You Want Perform This Transaction ? Y/N ? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y') {

		for (st_Client& C : V_Clients) {

			if (C.Account_Number == Account_Number) {
				C.Account_Balance += Amount;
				Save_Clients_Data_To_File(Clients_File_Name, V_Clients);
				cout << "\n\nDone Successfully . New Balance Is :  " << C.Account_Balance;
				return true;
			}
		}
		return false;
	}

	return false;
}

void Perform_Quick_Withdraw_Option(short Quick_Withdraw_Option) {
	if (Quick_Withdraw_Option == 9)
		return;

	short With_Draw_Balance = Get_Quick_Withdraw_Amount(Quick_Withdraw_Option);

	if (With_Draw_Balance > Current_Client.Account_Balance) {
		cout << "\n\nThe Amount Exceeds Your Balance, Make Another Choice.\n";
		cout << "Press Any Key To Continue...";
		system("pause>0");
		Show_Quick_Withdraw_Screen();
		return;
	}

	vector <st_Client> V_Clients = Load_Clients_Data_File(Clients_File_Name);

	if (Deposit_Balance_To_Client_By_Account_Number(Current_Client.Account_Number, With_Draw_Balance * -1, V_Clients)) {

		Current_Client.Account_Balance -= With_Draw_Balance;
	}
}

void Show_Quick_Withdraw_Screen() {
	system("cls");
	cout << "==========================================\n";
	cout << "\t\tQuick Withdraw\n";
	cout << "==========================================\n";
	cout << "\t[1] 20\t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "==========================================\n";
	cout << "Your Balance Is " << Current_Client.Account_Balance;
	Perform_Quick_Withdraw_Option(Read_Quick_Withdraw_Option());
}

int Read_Withdraw_Amount() {
	int Amount = 0;
	cout << "\nEnter An Amount Multiple Of 5's ? ";
	cin >> Amount;

	while (Amount % 5 != 0) {
		cout << "\nEnter An Amount Multiple Of 5's ? ";
		cin >> Amount;
	}

	return Amount;
}

void Perform_Normal_Withdraw_Option() {

	int With_Draw_Balance = Read_Withdraw_Amount();

	if (With_Draw_Balance > Current_Client.Account_Balance) {
		cout << "\n\nThe Amount Exceeds Your Balance, Make Another Choice.\n";
		cout << "Press Any Key To Continue...";
		system("pause>0");
		Show_Normal_Withdraw_Screen();
		return;
	}

	vector <st_Client> V_Clients = Load_Clients_Data_File(Clients_File_Name);

	if (Deposit_Balance_To_Client_By_Account_Number(Current_Client.Account_Number, With_Draw_Balance * -1, V_Clients)) {

		Current_Client.Account_Balance -= With_Draw_Balance;
	}
}

void Show_Normal_Withdraw_Screen() {
	system("cls");
	cout << "==========================================\n";
	cout << "\tNormal Withdraw Screen\n";
	cout << "==========================================\n";
	Perform_Normal_Withdraw_Option();
}

double Read_Deposit_Amount() {
	double Amount = 0;
	cout << "\nEnter A Positive Deposit Amount ? ";
	cin >> Amount;

	while (Amount <= 0) {
		cout << "\nEnter A Positive Deposit Amount ? ";
		cin >> Amount;
	}
	return Amount;
}

void Perform_Deposit_Option() {
	double Deposit_Amount = Read_Deposit_Amount();

	vector <st_Client> V_Clients = Load_Clients_Data_File(Clients_File_Name);

	if (Deposit_Balance_To_Client_By_Account_Number(Current_Client.Account_Number, Deposit_Amount, V_Clients)) {

		Current_Client.Account_Balance += Deposit_Amount;
	}
}

void Show_Deposit_Screen() {
	system("cls");
	cout << "==========================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "==========================================\n";
	Perform_Deposit_Option();
}

void Show_Check_Balance_Screen() {
	system("cls");
	cout << "==========================================\n";
	cout << "\tCheck Balance Screen\n";
	cout << "==========================================\n";
	cout << "Your Balance Is " << Current_Client.Account_Balance << "\n";
}

void Perform_Main_Menu_Option(en_Main_Menu_Options Main_Menu_Option) {
	switch (Main_Menu_Option) {
	case en_Main_Menu_Options::e_Quick_Withdraw:
		system("cls");
		Show_Quick_Withdraw_Screen();
		Go_Back_To_Main_Menu();
		break;
	case en_Main_Menu_Options::e_Normal_Withdraw:
		system("cls");
		Show_Normal_Withdraw_Screen();
		Go_Back_To_Main_Menu();
		break;
	case en_Main_Menu_Options::e_Deposit:
		system("cls");
		Show_Deposit_Screen();
		Go_Back_To_Main_Menu();
		break;
	case en_Main_Menu_Options::e_Check_Balance:
		system("cls");
		Show_Check_Balance_Screen();
		Go_Back_To_Main_Menu();
		break;
	case en_Main_Menu_Options::e_Exit:
		system("cls");
		Login();
		break;
	}
}

void Show_Main_Menu() {
	system("cls");
	cout << "==========================================\n";
	cout << "\tATM Main Menu Screen\n";
	cout << "==========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "==========================================\n";
	Perform_Main_Menu_Option((en_Main_Menu_Options)Read_Main_Menu_Option());
}

void Login() {
	bool Login_Faild = false;
	string Account_Number, Pin_Code;
	do {
		system("cls");
		cout << "\n____________________________\n";
		cout << "\tLogin Screen";
		cout << "\n____________________________\n";


		if (Login_Faild) {
			cout << "Invalid Account Number/Pin Code!\n";
		}

		cout << "Enter Account Number ? ";
		cin >> Account_Number;

		cout << "Enter Pin Code ? ";
		cin >> Pin_Code;

		Login_Faild = !Load_Client_Info(Account_Number, Pin_Code);

	} while (Login_Faild);
	Show_Main_Menu();
}

int main() {
	Login();
	system("pause>0");
	return 0;
}