#include<stdio.h>
#define WINDOWS
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>
#include<windows.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<ctime>
#include<sstream>
#include<fstream>
#include<iomanip>
using namespace std;

string gpwd( void ) 
{
   char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  string current_working_dir(buff);
  return current_working_dir;
}

tm* crtime()
{
	time_t tt;
	struct tm* ti;
	time(&tt);
	ti=localtime(&tt);
	return ti;
}

class passbook
{
	tm* ti;
	char strtm[25];
	int amt,pacno,bal;
	char type;
	public:
		void get(int g_amt,int g_bal,char g_type,int g_acno=0)
		{
			ti=crtime();
			string sri=asctime(ti);
			sri.erase(sri.size()-1);
			strcpy(strtm,sri.c_str()); 
			amt=g_amt;
			pacno=g_acno;
			type=g_type;
			bal=g_bal;
		}
		void disp(int acc)
		{	
			char atri[5];
			if(pacno==acc)
				strcpy(atri,"Self");
			else
				itoa(pacno,atri,10);
			if(type=='d')
				cout<<" "<<setw(30)<<left<<strtm<<setw(19)<<left<<atri<<setw(14)<<left<<amt<<setw(14)<<left<<setfill(' ')<<" "<<setw(14)<<left<<bal<<endl;
			if(type=='c')
				cout<<" "<<setw(30)<<left<<strtm<<setw(19)<<left<<atri<<setw(14)<<left<<setfill(' ')<<" "<<setw(14)<<left<<amt<<setw(14)<<left<<bal<<endl;
		}
};

class account 
{
	int balance,acno;
	char lname[20],city[20],pass[20];
	public:
		char name[20];
		int create(int pr)
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
			return balance;
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
		int dep(int amt)
		{
			balance=balance+amt;
			return balance;
		}
		int wid(int amt)
		{
			if(amt>balance)
				return -1;
			else
			{
				balance=balance-amt;
				return balance;
			}
		}
		int gacno()
		{
			return acno;
		}
		int bal()
		{
			return balance;
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
	tm* ti;
	ti=crtime();
	char buff1[80],buff2[80];
	strftime (buff2,80,"%I:%M %p.",ti);
	strftime(buff1,80,"%A, %B %d, %Y",ti);
	system("cls");
	cout<<"--------------------------------------------------------------------------------------  ";
	cout<<endl<<"                                   THE C++ BANK                                         ";
	cout<<endl<<"--------------------------------------------------------------------------------------  ";
	cout<<endl<<"                                                               "<<buff1;

}	
void waccount()
{
	css();
	passbook pbk;
	account ac;
	int pr,i=0;
	ifstream fin;
	ofstream fout,foutp;
	p:fin.open("accounts.dat",ios::binary);
	if(!fin)
	{
		fout.open("accounts.dat",ios::binary|ios::app);
		fout.close();
		goto p;
	}
	while(!fin.eof())
	{
		fin.read(reinterpret_cast<char *> (&ac), sizeof(account));
		pr=ac.gacno();
		if(pr!=50001 && i==0)
			pr=50000;
		i++;
	}
	fin.close();
	fout.open("accounts.dat",ios::binary|ios::app);
	int tmpbal=ac.create(pr);
	fout.write(reinterpret_cast<char *> (&ac), sizeof(account));
	fout.close();

	ostringstream ss;
	ss<<gpwd()<<"\\Passbooks\\"<<pr+1<<".dat";
	string geek = ss.str();
	
	if (CreateDirectory("Passbooks", NULL))
	{}
	if (ERROR_ALREADY_EXISTS == GetLastError())
	  	foutp.open(geek.c_str(),ios::binary);
	  	pbk.get(tmpbal,tmpbal,'c',pr+1);
		foutp.write(reinterpret_cast<char *> (&pbk), sizeof(passbook));
		foutp.close();
	cout<<endl<<endl<<"Account Created...";
	cin.get();
	cin.get();
}

int transfer(int accn,int racc,int amt)
{
	passbook pbk;
	account ac;
	int fl=0;
	fstream file,foutp;
    file.open("accounts.dat", ios::binary|ios::in|ios::out);
	if(!file)
	{
		cout<<"File could not be open !! Press any Key...";
		return -1;
	}
	ostringstream ss;
	ss<<gpwd()<<"\\Passbooks\\"<<racc<<".dat";
	string geek = ss.str();
	
	if (CreateDirectory("Passbooks", NULL))
	{}
	if (ERROR_ALREADY_EXISTS == GetLastError())
	  	foutp.open(geek.c_str(),ios::binary|ios::app);
	while(!file.eof())
	{
		file.read(reinterpret_cast<char *> (&ac), sizeof(account));
		if(ac.gacno()==racc)
		{
			ac.dep(amt);
			css();
			cout<<endl<<"Rs "<<amt<<" transfered sucessfully to ";
			ac.info();
			pbk.get(amt,ac.bal(),'c',accn);
			foutp.write(reinterpret_cast<char *> (&pbk), sizeof(passbook));
			foutp.close();
			fl=1;
		}
		int pos=(-1)*static_cast<int>(sizeof(ac));
		file.seekp(pos,ios::cur);
		file.write(reinterpret_cast<char *> (&ac), sizeof(account));
	}
	file.close();
	if(fl==0)
		return -1;
	else
		return 0;
}

void displaypsbk(int accn)
{
	passbook pbk;
	ostringstream ss;
	ss<<gpwd()<<"\\Passbooks\\"<<accn<<".dat";
	string geek = ss.str();
	ifstream fin;
	fin.open(geek.c_str(),ios::binary);
	css();
	cout<<endl;
	cout<<"--------------------------------------------------------------------------------------\n";
	cout<<" Date/Time                    Particulars         DEBITS       CREDITS       BALANCE  \n";
	cout<<"--------------------------------------------------------------------------------------\n";
	while(fin.read(reinterpret_cast<char *> (&pbk), sizeof(passbook)))
		pbk.disp(accn);
}


void ulogin()
{
	int accn,fl=0,ctr=0;
	char accpass[20],vpass[20];
	y:css();
	cout<<endl<<endl<<"Enter Account Number :";
	cin>>accn;
	if(accn==0)
		return;
	cout<<"Enter password :";
	cin>>accpass;
	passbook pbk;
	x:account ac;
	fstream fout,foutp;
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
			ostringstream ss;
			ss<<gpwd()<<"\\Passbooks\\"<<accn<<".dat";
			string geek = ss.str();
			if (CreateDirectory("Passbooks", NULL))
			{}
			if (ERROR_ALREADY_EXISTS == GetLastError())
				foutp.open(geek.c_str(),ios::binary|ios::app);
			fl=1;
			int pos=(-1)*static_cast<int>(sizeof(ac));
			int amt,racc,z,t;
			char k,cpass[20];
			css();
			ac.info();
	    	cout<<endl<<"\t1. Deposit "<<endl<<"\t2. Check Balance"<<endl<<"\t3. Withdraw "<<endl<<"\t4. Transfer Money"<<endl<<"\t5. Check Passbook"<<endl<<"\t6. Change password"<<endl<<"\t7. Logout"<<endl<<endl;
	    	cin>>k;
	    	switch(k)
	    	{
    		case '1':cout<<"Enter the amount to be deposited: ";
        			cin>>amt;
					ac.dep(amt);
					pbk.get(amt,ac.bal(),'c',accn);
					foutp.write(reinterpret_cast<char *> (&pbk), sizeof(passbook));
					foutp.close();
					cout<<"\n\tRs "<<amt<<" deposited successfully";
					break;
            case '2': cout<<endl<<"Your account balance is : "<<ac.bal()<<" Rs";
					break;
            case '3':cout<<"Enter the amount to be withdraw: ";
		            cin>>amt;
		            z=ac.wid(amt);
		            if(z!=-1)
		            {
						pbk.get(amt,ac.bal(),'d',accn);
						foutp.write(reinterpret_cast<char *> (&pbk), sizeof(passbook));
						foutp.close();
						cout<<"\n\tRs "<<amt<<" withdrawn sucessfully !";
					}
					else if(z==-1)
						cout<<"\n\tInsufficient Balance !";
					break;
            case '4':cout<<"Enter Recivers account no: ";
            		cin>>racc;
            		cout<<"Enter the amount to be transfered: ";
        			cin>>amt;
        			cout<<"Press Enter to Confirm ";
        			cin.get();cin.get();
        			if(racc==accn)
        			{
        				z=ac.wid(amt);
        				if(z==-1)
        				{	cout<<endl<<"Insufficient Balance !";
        					cin.get();
        					goto x;	}
        				pbk.get(amt,ac.bal(),'d',accn);
						foutp.write(reinterpret_cast<char *> (&pbk), sizeof(passbook));
						ac.dep(amt);
						pbk.get(amt,ac.bal(),'c',accn);
						foutp.write(reinterpret_cast<char *> (&pbk), sizeof(passbook));
						foutp.close();
						cout<<endl<<"Ammount transfered to self !";
					}
					else
					{
						if(amt<=ac.bal())
						{
							t=transfer(accn,racc,amt);
							if(t!=-1)
							{
								ac.wid(amt);
								pbk.get(amt,ac.bal(),'d',racc);
								foutp.write(reinterpret_cast<char *> (&pbk), sizeof(passbook));
								foutp.close();
							}
							else
								cout<<"\n\tAccount Not Found !";
						}
						else if(z==-1)
							cout<<"\n\tInsufficient Balance !";
					}
					break;
			case '5':displaypsbk(accn);
						cin.get();cin.get();
						goto x;
        	case '6':cout<<"Enter old password: ";
        			cin>>cpass;
        			if(ac.verifypass(cpass)==1)
						ac.changepass();
					else goto x;
					fl=2;
					break;
			case '7':return;
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
		ctr++;
		if(ctr==3)
			return;
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
	cout<<"======================================================================================\n";
	cout<<"A/c no.      NAME                        Balance    \n";
	cout<<"======================================================================================\n";
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
	ostringstream ss;
	ss<<gpwd()<<"\\Passbooks\\"<<key<<".dat";
	string geek = ss.str();
	remove(geek.c_str());
	return 1;
}

void delall()
{
	ofstream fout;
	ifstream fin;
	cout<<endl<<"Login for confirmation";
	if(alogin()==1)
	{
		fout.open("accounts.dat",ios::trunc);
		fout.close();
		int ctr=0,key=50000;
		w:ctr++;
		v:key=key+1;
		string geek;
		ostringstream ss;
		ss<<gpwd()<<"\\Passbooks\\"<<key<<".dat";
		geek=ss.str();
		fin.open(geek.c_str());
		fin.close();
		if(!fin && ctr<70)
			goto w;
		else if(remove(geek.c_str())==0)
			goto v;
		else if(ctr>70)
			cout<<endl<<"Please delete passbooks manually !";
		else
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
		cout<<endl<<"\tEnter your Choice"<<endl;
		cout<<endl<<"\t1.Modify Account"<<endl<<"\t2.Delete Account"<<endl<<"\t3.All Account Holders"<<endl<<"\t4.Delete All Accounts"<<endl<<"\t5.Logout"<<endl;
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
	//front();
	while(1)
	{
		css();
		cout<<endl<<"\t1. Create Account";
		cout<<endl<<"\t2. Login";
		cout<<endl<<"\t3. Exit";
		cout<<endl<<endl<<"\tEnter your Choice :";
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
		//	case '4':delall();
		//			break;
			case '3':	cout<<endl<<endl<<endl;
					cout<<endl<<"--------------------------------------------------------------------------------------";
					cout<<endl<<"                           THANK YOU FOR USING OUR SERVICES                           ";
					cout<<endl<<"--------------------------------------------------------------------------------------"<<endl;
					return 0;
			default: break;
		}
	}
}	
