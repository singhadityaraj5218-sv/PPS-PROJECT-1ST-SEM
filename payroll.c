
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Employee {
    int id;
    char name[100];
    double basic;
    double allowances;
    double deductions;
    double netSalary;
};

// Read a line safely
void readLine(const char *prompt, char *buffer, int size) {
    while (1) {
        printf("%s", prompt);

        if (fgets(buffer, size, stdin) == NULL) {
            printf("\nInput error.\n");
            exit(1);
        }

        // Remove newline if present
        size_t len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // Discard remaining characters from the input line
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }

        // Reject empty input
        if (buffer[0] != '\0') {
            return;
        }

        printf("Input cannot be empty. Try again.\n");
    }
}

// Read and validate an integer
int readInt(const char *prompt) {
    char line[100];
    char extra;
    int value;

    while (1) {
        readLine(prompt, line, sizeof(line));

        if (sscanf(line, " %d %c", &value, &extra) == 1) {
            return value;
        }

        printf("Invalid integer. Try again.\n");
    }
}

// Read and validate a non-negative number
double readAmount(const char *prompt) {
    char line[100];
    char extra;
    double value;

    while (1) {
        readLine(prompt, line, sizeof(line));

        if (sscanf(line, " %lf %c", &value, &extra) == 1 &&
            value >= 0 && value <= 1e12) {
            return value;
        }

        printf("Enter a valid amount between 0 and 1000000000000.\n");
    }
}

// Calculate net salary
void calculateSalary(struct Employee *e) {
    e->netSalary = e->basic + e->allowances
                   - e->deductions;
}

// Display an individual salary slip
void generateSalarySlip(struct Employee e) {
    printf("\n========== SALARY SLIP ==========\n");
    printf("Employee ID   : %d\n", e.id);
    printf("Employee Name : %s\n", e.name);
    printf("---------------------------------\n");
    printf("Basic Salary  : Rs. %.2f\n", e.basic);
    printf("Allowances    : Rs. %.2f\n", e.allowances);
    printf("Deductions    : Rs. %.2f\n", e.deductions);
    printf("---------------------------------\n");
    printf("Net Salary    : Rs. %.2f\n", e.netSalary);
    printf("=================================\n");
}

// Display the financial summary
void generateReport(struct Employee *employees, int n) {
    double totalBasic = 0;
    double totalAllowances = 0;
    double totalDeductions = 0;
    double totalNet = 0;

    for (int i = 0; i < n; i++) {
        totalBasic += employees[i].basic;
        totalAllowances += employees[i].allowances;
        totalDeductions += employees[i].deductions;
        totalNet += employees[i].netSalary;
    }

    printf("\n======= FINANCIAL SUMMARY =======\n");
    printf("Total Employees   : %d\n", n);
    printf("Total Basic Salary: Rs. %.2f\n", totalBasic);
    printf("Total Allowances  : Rs. %.2f\n", totalAllowances);
    printf("Total Deductions  : Rs. %.2f\n", totalDeductions);
    printf("Total Net Payroll : Rs. %.2f\n", totalNet);
    printf("=================================\n");
}

int main(void) {
    int n;

    printf("EMPLOYEE PAYROLL MANAGEMENT SYSTEM\n\n");

    do {
        n = readInt("Enter number of employees: ");

        if (n <= 0 || n > 10000) {
            printf("Enter a number between 1 and 10000.\n");
        }
    } while (n <= 0 || n > 10000);

    // Dynamically allocate memory
    struct Employee *employees =
        malloc((size_t)n * sizeof(struct Employee));

    if (employees == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Input employee details
    for (int i = 0; i < n; i++) {
        printf("\n--- Enter details of Employee %d ---\n",
               i + 1);

        employees[i].id = readInt("Employee ID: ");

        readLine("Employee Name: ",
                 employees[i].name,
                 sizeof(employees[i].name));

        employees[i].basic =
            readAmount("Basic Salary: Rs. ");

        employees[i].allowances =
            readAmount("Allowances: Rs. ");

        employees[i].deductions =
            readAmount("Deductions: Rs. ");

        // Prevent negative net salary
        while (employees[i].deductions >
               employees[i].basic + employees[i].allowances) {
            printf("Deductions cannot exceed basic salary "
                   "plus allowances.\n");

            employees[i].deductions =
                readAmount("Enter deductions again: ");
        }

        calculateSalary(&employees[i]);
    }

    // Generate all salary slips
    for (int i = 0; i < n; i++) {
        generateSalarySlip(employees[i]);
    }

    // Generate overall payroll report
    generateReport(employees, n);

    // Release allocated memory
    free(employees);
    employees = NULL;

    return 0;
}

