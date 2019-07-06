#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<iomanip>

using namespace std;

class account
{
	int balance,acno;
	char lname[20],city[20],pass[20];
	public:
		char name[20];
		void create(int pr)
		{
			
			acno=pr+1;
			cout<<endl<<endl<<"Enter your first name : ";
			cin>>name;
			cout<<"Enter your last name : ";
			cin>>lname;
			cout<<"Enter city name : ";
			cin>>city;
			cout<<"Enter the amount to be deposited : ";
			cin>>balance;
			cout<<"Choose a password : ";
			cin>>pass;
			cout<<endl<<"Your account number is "<<acno;
			cout<<endl<<endl<<"Account Created...";
			cin.get();
		}
		void modify()
		{
			cout<<endl<<endl<<"Enter your first name :";
			cin>>name;
			cout<<"Enter your last name :";
			cin>>lname;
			cout<<"Enter city name :";
			cin>>city;
			cout<<"Enter a password : ";
			cin>>pass;
		}
		void info()
		{
			cout<<endl<<"Account No.: "<<acno;
			cout<<endl<<"Account Holder Name : "<<name<<" "<<lname;
			cout<<endl<<"City name: "<<city<<endl;
		}
		void dep(int amt)
		{
			balance=balance+amt;
		}
		int wid(int amt)
		{
			if(amt>balance)
				return 0;
			else
			{
				balance=balance-amt;
				return 1;
			}
		}
		int gacno()
		{
			return acno;
		}
		int balinfo()
		{
			cout<<endl<<"Your Account Balance is "<<balance<<" Rs";
		}
		int verifypass(char key[20])
		{
			if(strcmp(key,pass)==0)
				return 1;
			else
				return 0;
		}
		void changepass()
		{
			char tempa[20],tempb[20];
			m:cout<<endl<<"Enter new password: ";
			cin>>tempa;
			cout<<"Confirm new password: ";
			cin>>tempb;
			if(strcmp(tempa,tempb)==0)
			{
				strcpy(pass,tempb);
				cout<<"\n\n\t Password changed successfully...";
			}
			else
			{
				cout<<"\nPasswords do not match ";
				goto m;
			}
		}
		
		void list(int p=0)
		{
			if(p==0)
				cout<<" "<<setw(12)<<left<<acno<<setw(10)<<left<<name<<setw(19)<<left<<lname<<balance<<endl;
			if(p==1)
				cout<<" "<<setw(12)<<left<<acno<<setw(10)<<left<<name<<setw(19)<<left<<lname<<balance<<setw(7)<<" "<<setw(20)<<left<<setfill(' ')<<pass<<endl;
		}
		~account(){}
};

void css()
{
	system("cls");
	cout<<endl<<"--------------------------------------------------------------";
	cout<<endl<<"                         THE C++ BANK                         ";
	cout<<endl<<"--------------------------------------------------------------"<<endl;
}	
void waccount()
{
	css();
	account ac;
	int pr,i=0;
	ifstream fin;
	fin.open("accounts.dat",ios::binary);
	while(!fin.eof())
	{
		fin.read(reinterpret_cast<char *> (&ac), sizeof(account));
		pr=ac.gacno();
		if(pr!=50001 && i==0)
			pr=50000;
		i++;
	}
	fin.close();
	fstream fout;
	fout.open("accounts.dat",ios::binary|ios::app);
	ac.create(pr);
	fout.write(reinterpret_cast<char *> (&ac), sizeof(account));
	fout.close();
	cin.get();
}

void transfer(int racc,int amt)
{
	account ac;
	fstream file;
    file.open("accounts.dat", ios::binary|ios::in|ios::out);
	if(!file)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	while(!file.eof())
	{
		file.read(reinterpret_cast<char *> (&ac), sizeof(account));
		if(ac.gacno()==racc)
		{
			ac.dep(amt);
			css();
			cout<<endl<<"Rs "<<amt<<" transfered sucessfully to ";
			ac.info();
		}
		int pos=(-1)*static_cast<int>(sizeof(ac));
		file.seekp(pos,ios::cur);
		file.write(reinterpret_cast<char *> (&ac), sizeof(account));
	}
	file.close();
}

void ulogin()
{
	int accn,fl=0;
	char accpass[20],vpass[20];
	y:css();
	cout<<endl<<endl<<"Enter Account Number :";
	cin>>accn;
	if(accn==0)
		return;
	cout<<"Enter password :";
	cin>>accpass;
	x:account ac;
	fstream fout;
    fout.open("accounts.dat", ios::binary|ios::in|ios::out);
	if(!fout)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	while(!fout.eof())
	{
    fout.read(reinterpret_cast<char *> (&ac), sizeof(account));
		if(ac.gacno()==accn && ac.verifypass(accpass)==1)
		{
			fl=1;
			int pos=(-1)*static_cast<int>(sizeof(ac));
			int amt,racc;
			char k,cpass[20];
			css();
			ac.info();
	    	cout<<endl<<"1. Deposit "<<endl<<"2. Check Balance"<<endl<<"3. Withdraw "<<endl<<"4. Transfer Money"<<endl<<"5. Change password"<<endl<<"6. Logout"<<endl<<endl;
	    	cin>>k;
	    	switch(k)
	    	{
    		case '1':cout<<"Enter the amount to be deposited: ";
        			cin>>amt;
					ac.dep(amt);
					cout<<"\n\tRs "<<amt<<" deposited successfully";
					break;
            case '2': ac.balinfo();
					break;
            case '3':cout<<"Enter the amount to be withdraw: ";
		            cin>>amt;
					if(ac.wid(amt)==1)
						cout<<"\n\tRs "<<amt<<" withdrawn sucessfully !";
					else if(ac.wid(amt)==0)
						cout<<"\n\tInsufficient Balance !";
					break;
            case '4':cout<<"Enter Recivers account no: ";
            		cin>>racc;
            		cout<<"Enter the amount to be transfered: ";
        			cin>>amt;
        			cout<<"Press Enter to Confirm ";
        			cin.get();cin.get();
					if(ac.wid(amt)==1)
						transfer(racc,amt);
					else if(ac.wid(amt)==0)
						cout<<"\n\tInsufficient Balance !";
					break;
        	case '5':cout<<"Enter old password: ";
        			cin>>cpass;
        			if(ac.verifypass(cpass)==1)
						ac.changepass();
					else goto x;
					fl=2;
					break;
			case '6':return;
            default: cout<<endl<<"Invalid Entry";
            			break;
			}
			cin.get();cin.get();
			fout.seekp(pos,ios::cur);
			fout.write(reinterpret_cast<char *> (&ac), sizeof(account));
			fout.close();
	   		goto x;
		}
	}
	if(fl==2)
	{
		fl=0;
		goto y;
	}
	if(fl==0)
	{
		css();
		cout<<"\n\nIncorrect account number or password";
		cin.get();cin.get();
		goto y;
	}
		
}

int alogin()
{
	char ukey[15],pkey[15],uname[15],pass[15];
	strcpy(uname,"admin");
	strcpy(pass,"admin");
	cout<<endl<<"Enter Admin username: ";
	cin>>ukey;
	cout<<"Enter Admin password: ";
	cin>>pkey;
	if(strcmp(ukey,uname)==0 && strcmp(pkey,pass)==0)
		return 1;
	else
		return 0;
}

void allacc(int s=0)
{
	account ac;
	ifstream inFile;
	inFile.open("accounts.dat",ios::binary);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	css();
	cout<<"\n\n\t\tACCOUNT HOLDER LIST\n\n";
	cout<<"====================================================\n";
	cout<<"A/c no.      NAME                        Balance\n";
	cout<<"====================================================\n";
	if(s==0)
		while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
			ac.list();
	else if(s==1)
		while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
			ac.list(1);
	inFile.close();
}
void modifyacc()
{
	int key;
	cout<<endl<<"Enter account number : ";
	cin>>key;
	int fl=0;
	account ac;
	fstream file;
    file.open("accounts.dat",ios::binary|ios::in|ios::out);
	if(!file)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	while(!file.eof() && fl==0)
	{
		file.read(reinterpret_cast<char *> (&ac), sizeof(account));
		if(ac.gacno()==key)
		{
			ac.info();
			cout<<"\n\nEnter The New Details of account"<<endl;
			ac.modify();
			int pos=(-1)*static_cast<int>(sizeof(account));
			file.seekp(pos,ios::cur);
		    file.write(reinterpret_cast<char *> (&ac), sizeof(account));
		    cout<<"\n\n\t Record Updated";
		    fl=1;
		}
	}
	file.close();
	if(fl==0)
	{
		cout<<"\n\n Record Not Found ";
		return;
	}
}

int delacc()
{
	account ac;
	ifstream fin;
	ofstream fout;
	int key;
	cout<<"Enter the Account Number to be deleted : ";
	cin>>key;
	fin.open("accounts.dat",ios::binary);
	if(!fin)
	{
		cout<<"File could not be open !! Press any Key...";
		return 0;
	}
	fout.open("Temp.dat",ios::binary);
	fin.seekg(0,ios::beg);
	while(fin.read(reinterpret_cast<char *> (&ac), sizeof(account)))
	{
		if(ac.gacno()!=key)
			fout.write(reinterpret_cast<char *> (&ac), sizeof(account));
		if(ac.gacno()==key)
		{
			ac.info();
			cout<<endl<<("Press Enter to Confirm");
			cin.get();cin.get();	
		}
		
	}
    fin.close();
	fout.close();
	remove("accounts.dat");
	rename("Temp.dat","accounts.dat");
	return 1;
}

void delall()
{
	ofstream fout;
	cout<<endl<<"Login for confirmation";
	if(alogin()==1)
	{
		fout.open("accounts.dat",ios::trunc);
		fout.close();
		cout<<"\n\n\n\tAll acounts Deleted !!! ";
		return;
	}
	else
	{
		cout<<"\n\n\tFailed ,Try Again!!!";
		return;
	}
}

void admin()
{
	char z;
	while(1)
	{
		css();
		cout<<endl<<"Enter your Choice"<<endl;
		cout<<endl<<"1.Modify Account"<<endl<<"2.Delete Account"<<endl<<"3.All Account Holders"<<endl<<"4.Delete All Accounts"<<endl<<"5.Logout"<<endl;
		cin>>z;
		switch(z)
		{
			case '1':modifyacc();
					cin.get();
					break;
			case '2': if(delacc()==1)
						cout<<"\n\n\tRecord Deleted...";
					else
						cout<<"\n\n\tFailed, Try Again !!";
					break; 
			case '3':allacc();
					cin.get();
					break;
			case '4':delall();
					cin.get();
					break;
			case 'p':char l;
					cin>>l;
					if(l=='l')		
						allacc(1);
					cin.get();
					break;
			case '5':return;
			default: break;
		}
		cin.get();
	}
}

void front()
{
	cout<<endl<<"                                                  ";
	cout<<endl<<"      ____________________________________________";
	cout<<endl<<"     |                                           |";
	cout<<endl<<"     |               The  C++ Bank               |";
	cout<<endl<<"     |                                           |";
	cout<<endl<<"     |                                           |";
	cout<<endl<<"     |                                           |";
	cout<<endl<<"     |                created by,                |";
	cout<<endl<<"     |                                           |";
	cout<<endl<<"     | Walstan Baptista           Krut Chitre    |";
	cout<<endl<<"     | 160050107503               160050107510   |";
	cout<<endl<<"     |                    &                      |";
	cout<<endl<<"     |                                           |";
	cout<<endl<<"     |             Abhijeet Chaudhry             |";
	cout<<endl<<"     |               160050107509                |";
	cout<<endl<<"     |___________________________________________|";
	cout<<endl<<"                                                  ";
	cin.get();
}
main()
{
	char ch;
	front();
	while(1)
	{
		css();
		cout<<endl<<"1. Create Account";
		cout<<endl<<"2. Login";
		cout<<endl<<"3. Exit";
		cout<<endl<<endl<<"Enter your Choice :";
		cin>>ch;
		switch(ch)
		{
			case '1':waccount();
				break;
			case '2':ulogin();
					break;
			case '9':css();
				if(alogin()==1)
					admin();
				break;
			case '3':	cout<<endl<<endl<<endl;
					cout<<endl<<"--------------------------------------------------------------";
					cout<<endl<<"               THANK YOU FOR USING OUR SERVICES               ";
					cout<<endl<<"--------------------------------------------------------------"<<endl;
					return 0;
			default: break;
		}
	}
}	
