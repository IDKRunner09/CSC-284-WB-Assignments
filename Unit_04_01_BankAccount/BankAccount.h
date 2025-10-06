#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H
#include <string>
class BankAccount {
	private: 
		std::string accountHolder;
		int accountNumber;
		double balance;
		static int totalAccounts, nextAccountNumber;
public:
	BankAccount();
	BankAccount(std::string name, double amount);
	~BankAccount();
	int getAccountNumber() const;
	std::string getAccountHolder() const;
	double getBalance() const;
	void deposit(double amount);
	void withdraw(double amount);
	void printAccountInfo() const;
	static int getTotalAccount();
	void transfer(BankAccount& toAccount, double amount);
};
#endif


