#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <sstream>

using namespace std;

const int employeesMax = 100;

// Employee Class definition

class Employee {
private:
    int id;
    string name;
    double payRate;
    int dependents;
    bool type;

public:
    // Employee Constructor
    Employee() : id(0), name(""), payRate(0.0), dependents(0), type(false) {}

    // Employee getters
    int getId() const { return id; }
    string getName() const { return name; }
    double getPayRate() const { return payRate; }
    int getDependents() const { return dependents; }
    bool getType() const { return type; }

    // Employee setters
    void setDataEmployees(int idFile, const string& nameFile, double payRateFile, int dependentsFile, bool typeFile) {
        id = idFile;
        name = nameFile;
        payRate = payRateFile;
        dependents = dependentsFile;
        type = typeFile;
    }
};

// Transaction class definition
class Transaction {
private:
    int id;
    double hours;

public:
    //Transaction constructor
    Transaction() : id(0), hours(0.0) {}

    // Transaction getter
    int getId() const { return id; }
    double getHours() const { return hours; }
    //Transaction setter
    void setDataTransactions(int idFile, double hoursFile) {
        id = idFile;
        hours = hoursFile;
    }
};

// Function Prototypes
void projectStart();
void projectEnd();
void display();
void errorReport(const Employee employees[], int employeeCount, const Transaction transactions[], int transactionCount);
void createPayrollFile(const Employee employees[], int employeeCount, const Transaction transactions[], int transactionCount);
double calculateGrossPay(bool type, double payRate, double hours);
void readEmployees(Employee employees[], int& count, const string& filename);
void readTransaction(Transaction transactions[], int& count, const string& filename);

// Function for error report
void errorReport(const Employee employees[], int employeeCount, const Transaction transactions[], int transactionCount) {
    cout << "-------------------------\n";
    cout << "Error and Control Report\n";
    cout << "-------------------------\n";
    cout << "The following items could not be processed:\n";

    // Matches employee and transaction file IDs, if no match, cout as error
    for (int i = 0; i < transactionCount; i++) {
        bool found = false;
        for (int j = 0; j < employeeCount; j++) {
            if (transactions[i].getId() == employees[j].getId()) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Transaction ID: " << transactions[i].getId() << "\n";
        }
    }
}

// Function for display
void display() {
    cout << "Screen Display\n";
    cout << "-------------------------\n";
    cout << "Payroll Processor\n";
    cout << "Written By: Dustin Nguyen\n";
    cout << "-------------------------\n";
    cout << "PAYROLL PROCESSOR\n";
    cout << "-------------------------\n";
    cout << "Processing Complete. See Reports.\n";
    cout << "-------------------------\n";
}

// Function to calculate gross pay
double calculateGrossPay(bool type, double payRate, double hours) {
    if (type) {
        return payRate;
    }
    else {
        if (hours <= 40) {
            return payRate * hours;
        }
        else {
            double regularPay = payRate * 40;
            double overtimePay = payRate * 1.5 * (hours - 40);
            return regularPay + overtimePay;
        }
    }
}

// Function to create payroll file
void createPayrollFile(const Employee employees[], int employeeCount, const Transaction transactions[], int transactionCount) {
    ofstream payrollFile("payroll.txt");

    double totalGrossPay = 0;
    double totalNetPay = 0;
    payrollFile << "-------------------------\n"
        << "Monthly Payroll Report\n"
        << "-------------------------\n"
        << left << setw(5) << "ID"
        << setw(20) << "Name"
        << setw(10) << "Tax"
        << setw(15) << "Insurance"
        << setw(15) << "Gross Pay"
        << setw(15) << "Net Pay" << endl;

    //Loop to print all valid data using matching ids
    for (int i = 0; i < employeeCount; i++) {
        int transactionIndex = -1;
        for (int j = 0; j < transactionCount; j++) {
            if (transactions[j].getId() == employees[i].getId()) {
                transactionIndex = j;
                break;
            }
        }

        if (transactionIndex == -1) {
            continue;
        }

        double hours = transactions[transactionIndex].getHours();
        double grossPay = calculateGrossPay(employees[i].getType(), employees[i].getPayRate(), hours);
        totalGrossPay += grossPay;
        double tax = grossPay * 0.15;
        double insurance = employees[i].getDependents() * 30;
        double netPay = grossPay - tax - insurance;
        totalNetPay += netPay;

        payrollFile << left << setw(5) << employees[i].getId()
            << setw(20) << employees[i].getName()
            << fixed << setprecision(2)
            << setw(10) << tax
            << setw(15) << insurance
            << setw(15) << grossPay
            << setw(15) << netPay << endl;
    }

    payrollFile << "-------------------------\n"
        << "Number of Employees: "
        << employeeCount
        << endl;
    payrollFile << "Total Gross Pay: $"
        << totalGrossPay
        << endl;
    payrollFile << "Total Net Pay: $"
        << totalNetPay
        << "\n-------------------------\n";

    payrollFile.close();
}

//Reads data from employee file
void readEmployees(Employee employees[], int& count, const string& filename) {
    ifstream file(filename);

    int id;
    string firstName, lastName;
    double payRate;
    int dependents;
    bool type;
    string line;

    while (getline(file, line) && count < employeesMax) {
        istringstream iss(line);
        if (iss >> id >> firstName >> lastName >> payRate >> dependents >> type) {
            string fullName = firstName + " " + lastName;
            employees[count].setDataEmployees(id, fullName, payRate, dependents, type);
            count++;
        }
    }

    file.close();
}

// Reads data from transaction file
void readTransaction(Transaction transactions[], int& count, const string& filename) {
    ifstream file(filename);

    int id;
    double hours;

    while (file >> id >> hours && count < employeesMax) {
        transactions[count].setDataTransactions(id, hours);
        count++;
    }

    file.close();
}

int main()
{
    projectStart();

    Employee employees[employeesMax];
    Transaction transactions[employeesMax];
    int employeeCount = 0;
    int transactionCount = 0;

    display();

    readEmployees(employees, employeeCount, "employee.txt");
    readTransaction(transactions, transactionCount, "transaction.txt");

    errorReport(employees, employeeCount, transactions, transactionCount);

    createPayrollFile(employees, employeeCount, transactions, transactionCount);

    projectEnd();
    return 0;
}

void projectStart()
{
    cout << "----------------------------------------------" << endl;
    cout << "Project #8" << endl;
    cout << "Written by: Dustin Nguyen" << endl;
    cout << "Objectives: Classes, I/O, sort" << endl;
    cout << "----------------------------------------------" << endl;
}

void projectEnd()
{
    cout << "----------------------------------------------" << endl;
    cout << "End of Project #8" << endl;
    cout << "----------------------------------------------" << endl;
}