#include "BankAccount.h"
#include <iostream>
#include <string>

int BankAccount::nextAccountNumber = 1001;
int BankAccount::totalAccounts = 0;

BankAccount::BankAccount() : 
	accountNumber(nextAccountNumber++), accountHolder("Unknown"), balance(0.0) {
	totalAccounts++;

}

BankAccount::BankAccount(std::string name, double amount) : 
	accountNumber(nextAccountNumber++), accountHolder(name), balance(amount) {
	totalAccounts++;
}


BankAccount::~BankAccount() {
	totalAccounts--;
	std::cout << "Destructor called for account " << accountNumber << std::endl;
}

int BankAccount::getAccountNumber() const{
	return accountNumber;
}

std::string BankAccount::getAccountHolder() const {
	return accountHolder;
}

double BankAccount::getBalance() const {
	return balance;
}

void BankAccount::deposit(double amount) {
	if (amount > 0) {
		balance += amount;
	}
}

void BankAccount::withdraw(double amount) {
	if (amount < balance && amount > 0) {
		balance -= amount;
	}
}

void BankAccount::printAccountInfo() const {
	std::cout << "Account Holder : " << accountHolder
		<< "\nAccount Number : " << accountNumber
		<< "\nBalance : " << balance << std::endl;
}

int BankAccount::getTotalAccount() {
	return totalAccounts;
}

void BankAccount::transfer(BankAccount& toAccount, double amount) {
	if (amount > 0 && amount <= balance) {
		balance -= amount;
		toAccount.balance += amount;
		std::cout << "Transferred $" << amount << " from " << accountHolder << " to " << toAccount.accountHolder << std::endl;
	} else {
		std::cout << "Amount Invalid\n";
	}
}