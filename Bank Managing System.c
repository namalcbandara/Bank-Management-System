#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100

// ANSI Color Codes for terminal UI
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

typedef struct {
    int acc_no;
    char name[100];
    double balance;
} Customer;

// Global variables
Customer bank[MAX_CUSTOMERS];
int customer_count = 0;
int next_acc_no = 1001;

// Function prototypes
void create_account();
void remove_account();
void deposit();
void withdraw();
void show_balance();
void list_customers();
int find_account(int acc_no);

int main() {
    int choice;

    while (1) {
        printf("\n");
        printf(CYAN "=========================================\n" RESET);
        printf(YELLOW "      BANK MANAGEMENT SYSTEM\n" RESET);
        printf(CYAN "=========================================\n" RESET);
        printf(GREEN "1." RESET " Register New Customer\n");
        printf(GREEN "2." RESET " Remove Customer\n");
        printf(GREEN "3." RESET " Deposit Funds\n");
        printf(GREEN "4." RESET " Withdraw Funds\n");
        printf(GREEN "5." RESET " Check Balance\n");
        printf(GREEN "6." RESET " List All Customers\n");
        printf(RED "7." RESET " Exit System\n");
        printf(CYAN "=========================================\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);

        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // clear buffer
            printf(RED "Invalid input! Please enter a valid number.\n" RESET);
            continue;
        }

        switch (choice) {
            case 1: create_account(); break;
            case 2: remove_account(); break;
            case 3: deposit(); break;
            case 4: withdraw(); break;
            case 5: show_balance(); break;
            case 6: list_customers(); break;
            case 7:
                printf(BLUE "\nThank you for using the Bank Management System. Goodbye!\n\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice! Please select an option between 1 and 7.\n" RESET);
        }
    }
    return 0;
}

// Helper function to find array index of an account number
int find_account(int acc_no) {
    for (int i = 0; i < customer_count; i++) {
        if (bank[i].acc_no == acc_no) {
            return i;
        }
    }
    return -1; // Not found
}

void create_account() {
    if (customer_count >= MAX_CUSTOMERS) {
        printf(RED "Bank capacity reached! Cannot add more customers.\n" RESET);
        return;
    }

    Customer new_cust;
    new_cust.acc_no = next_acc_no++;

    printf(YELLOW "Enter Customer Name: " RESET);
    getchar(); // clear newline left by previous scanf
    fgets(new_cust.name, sizeof(new_cust.name), stdin);
    new_cust.name[strcspn(new_cust.name, "\n")] = 0; // remove trailing newline

    printf(YELLOW "Enter Initial Deposit Amount: $" RESET);
    scanf("%lf", &new_cust.balance);

    if (new_cust.balance < 0) {
        printf(RED "Initial balance cannot be negative. Setting to $0.00\n" RESET);
        new_cust.balance = 0;
    }

    bank[customer_count++] = new_cust;

    printf(GREEN "\n>>> Customer registered successfully! <<<\n" RESET);
    printf(CYAN "-----------------------------------------\n" RESET);
    printf(BLUE "Account Number : %d\n" RESET, new_cust.acc_no);
    printf(BLUE "Account Holder : %s\n" RESET, new_cust.name);
    printf(BLUE "Initial Balance: $%.2lf\n" RESET, new_cust.balance);
    printf(CYAN "-----------------------------------------\n" RESET);
}

void remove_account() {
    int acc_no, index;
    printf(YELLOW "Enter Account Number to remove: " RESET);
    scanf("%d", &acc_no);

    index = find_account(acc_no);
    if (index == -1) {
        printf(RED "Error: Account Number %d not found in the system.\n" RESET, acc_no);
        return;
    }

    // Shift remaining customers left to fill the gap
    for (int i = index; i < customer_count - 1; i++) {
        bank[i] = bank[i + 1];
    }
    customer_count--;

    printf(GREEN "\n>>> Account Number %d has been successfully removed. <<<\n" RESET, acc_no);
}

void deposit() {
    int acc_no, index;
    double amount;

    printf(YELLOW "Enter Account Number: " RESET);
    scanf("%d", &acc_no);

    index = find_account(acc_no);
    if (index == -1) {
        printf(RED "Error: Account Number %d not found.\n" RESET, acc_no);
        return;
    }

    printf(YELLOW "Enter amount to deposit: $" RESET);
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf(RED "Error: Deposit amount must be greater than zero.\n" RESET);
        return;
    }

    bank[index].balance += amount;

    printf(GREEN "\n>>> Deposit successful! <<<\n" RESET);
    printf(BLUE "Account Holder : %s\n" RESET, bank[index].name);
    printf(BLUE "New Balance    : $%.2lf\n" RESET, bank[index].balance);
}

void withdraw() {
    int acc_no, index;
    double amount;

    printf(YELLOW "Enter Account Number: " RESET);
    scanf("%d", &acc_no);

    index = find_account(acc_no);
    if (index == -1) {
        printf(RED "Error: Account Number %d not found.\n" RESET, acc_no);
        return;
    }

    printf(YELLOW "Enter amount to withdraw: $" RESET);
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf(RED "Error: Withdrawal amount must be greater than zero.\n" RESET);
        return;
    }

    if (bank[index].balance < amount) {
        printf(RED "Error: Insufficient funds! Current balance is $%.2lf\n" RESET, bank[index].balance);
        return;
    }

    bank[index].balance -= amount;

    printf(GREEN "\n>>> Withdrawal successful! <<<\n" RESET);
    printf(BLUE "Account Holder   : %s\n" RESET, bank[index].name);
    printf(BLUE "Remaining Balance: $%.2lf\n" RESET, bank[index].balance);
}

void show_balance() {
    int acc_no, index;

    printf(YELLOW "Enter Account Number: " RESET);
    scanf("%d", &acc_no);

    index = find_account(acc_no);
    if (index == -1) {
        printf(RED "Error: Account Number %d not found.\n" RESET, acc_no);
        return;
    }

    printf(CYAN "\n---------- ACCOUNT DETAILS ----------\n" RESET);
    printf(BLUE "Account Number : %d\n" RESET, bank[index].acc_no);
    printf(BLUE "Account Holder : %s\n" RESET, bank[index].name);
    printf(BLUE "Current Balance: $%.2lf\n" RESET, bank[index].balance);
    printf(CYAN "-------------------------------------\n" RESET);
}

void list_customers() {
    if (customer_count == 0) {
        printf(RED "\nNo customers currently registered in the system.\n" RESET);
        return;
    }

    printf(CYAN "\n========================================================\n" RESET);
    printf(YELLOW "%-15s %-25s %-15s\n" RESET, "Account No", "Customer Name", "Balance ($)");
    printf(CYAN "========================================================\n" RESET);

    for (int i = 0; i < customer_count; i++) {
        printf("%-15d %-25s %-15.2lf\n", bank[i].acc_no, bank[i].name, bank[i].balance);
    }
    printf(CYAN "========================================================\n" RESET);
}
