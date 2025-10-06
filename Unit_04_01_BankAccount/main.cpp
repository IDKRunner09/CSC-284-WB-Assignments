#include <iostream>
#include "BankAccount.h"


int main() {
	//initiating account1
	BankAccount account1;

	//printing details for account
	account1.printAccountInfo();

	//initiating account2
	BankAccount account2("Shawn", 1500);
	//printing details for account
	account2.printAccountInfo();

	//print total active accounts
	BankAccount::getTotalAccount();
	std::cout << std::endl;
	std::cout << "Total active account: " << BankAccount::getTotalAccount() << std::endl;
	std::cout << std::endl;

	//transfer amount from account 1 to account 2
	account2.transfer(account1, 500.00);
	//printing details for accounts
	account1.printAccountInfo();
	account2.printAccountInfo();
	std::cout << std::endl;
	//deconstructor called manually
	account2.~BankAccount();
	std::cout << std::endl;
	//print total active accounts
	std::cout << "Total active account: " << BankAccount::getTotalAccount() << std::endl;
	std::cout << std::endl;
}
