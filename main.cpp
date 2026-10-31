#include "database.h" 
#include "account.h"
#include "bank.h"
#include "bankapp.h"

int main()
{
	BankApp bankapp("My first bank", "bank.txt"); 

	while (1)
	{
		bankapp.run(); 
	}
	
    return 0; 
}