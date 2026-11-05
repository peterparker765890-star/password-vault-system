#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define USER_FILE "users.dat"

void displayMenu();
void registerUser();
void loginUser();
int usernameExists(char username[]);
int authenticateUser(char username[], char password[]);
unsigned long long educationalHash(char password[]);
int validPassword(char password[]);
void readLine(char text[], int size);

void readLine(char text[], int size)
{
    if (fgets(text, size, stdin) != NULL)
    {
        text[strcspn(text, "\n")] = '\0';
    }
}

unsigned long long educationalHash(char password[])
{
    unsigned long long hash = 14695981039346656037ULL;
    int i;

    for (i = 0; password[i] != '\0'; i++)
    {
        hash ^= (unsigned char)password[i];
        hash *= 1099511628211ULL;
    }

    return hash;
}

int validPassword(char password[])
{
    int upper = 0, lower = 0, digit = 0;
    int i;

    if (strlen(password) < 8)
        return 0;

    for (i = 0; password[i] != '\0'; i++)
    {
        if (isupper((unsigned char)password[i]))
            upper = 1;

        if (islower((unsigned char)password[i]))
            lower = 1;

        if (isdigit((unsigned char)password[i]))
            digit = 1;
    }

    return upper && lower && digit;
}

int usernameExists(char username[])
{
    FILE *fp;
    char storedUsername[64];
    unsigned long long storedHash;

    fp = fopen(USER_FILE, "r");

    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%63[^|]|%llu",
                  storedUsername, &storedHash) == 2)
    {
        if (strcmp(username, storedUsername) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* MODULE 01: USER REGISTRATION */

void registerUser()
{
    char username[64];
    char password[64];
    char confirmPassword[64];

    FILE *fp;
    unsigned long long hash;

    printf("\n========== USER REGISTRATION ==========\n");

    printf("Enter username: ");
    readLine(username, sizeof(username));

    if (strlen(username) == 0 ||
        strchr(username, '|') != NULL)
    {
        printf("Invalid username!\n");
        return;
    }

    if (usernameExists(username))
    {
        printf("Username already exists!\n");
        return;
    }

    printf("Create password: ");
    readLine(password, sizeof(password));

    if (!validPassword(password))
    {
        printf("\nWeak password!\n");
        printf("Minimum 8 characters required.\n");
        printf("Use uppercase, lowercase and digits.\n");
        return;
    }

    printf("Confirm password: ");
    readLine(confirmPassword, sizeof(confirmPassword));

    if (strcmp(password, confirmPassword) != 0)
    {
        printf("Passwords do not match!\n");
        return;
    }

    hash = educationalHash(password);

    fp = fopen(USER_FILE, "a");

    if (fp == NULL)
    {
        printf("Error opening user database!\n");
        return;
    }

    fprintf(fp, "%s|%llu\n", username, hash);

    fclose(fp);

    printf("\nRegistration successful!\n");
    printf("Your account has been created.\n");
}

/* MODULE 02: LOGIN AUTHENTICATION */

int authenticateUser(char username[], char password[])
{
    FILE *fp;
    char storedUsername[64];
    unsigned long long storedHash;
    unsigned long long enteredHash;

    fp = fopen(USER_FILE, "r");

    if (fp == NULL)
        return 0;

    enteredHash = educationalHash(password);

    while (fscanf(fp, " %63[^|]|%llu",
                  storedUsername, &storedHash) == 2)
    {
        if (strcmp(username, storedUsername) == 0 &&
            enteredHash == storedHash)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void loginUser()
{
    char username[64];
    char password[64];

    printf("\n========== LOGIN AUTHENTICATION ==========\n");

    printf("Enter username: ");
    readLine(username, sizeof(username));

    printf("Enter password: ");
    readLine(password, sizeof(password));

    if (authenticateUser(username, password))
    {
        printf("\nLOGIN SUCCESSFUL!\n");
        printf("Welcome, %s!\n", username);
        printf("Access granted to Password Vault.\n");
    }
    else
    {
        printf("\nLOGIN FAILED!\n");
        printf("Invalid username or password.\n");
    }
}

/* MAIN MENU */

void displayMenu()
{
    printf("\n========================================\n");
    printf("          PASSWORD VAULT SYSTEM         \n");
    printf("========================================\n");
    printf("1. User Registration\n");
    printf("2. Login Authentication\n");
    printf("3. Password Change\n");
    printf("4. Account Recovery\n");
    printf("5. Login Attempt Monitoring\n");
    printf("6. User Activity Reports\n");
    printf("7. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

int main()
{
    char input[20];
    int choice;

    printf("\n****************************************\n");
    printf("          PASSWORD VAULT SYSTEM         \n");
    printf("          C PROGRAMMING PROJECT         \n");
    printf("          DEVELOPED BY SABARISH         \n");
    printf("****************************************\n");

    do
    {
        displayMenu();
        readLine(input, sizeof(input));

        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid input! Enter a number.\n");
            choice = 0;
            continue;
        }

        switch (choice)
        {
            case 1:
                registerUser();
                break;

            case 2:
                loginUser();
                break;

            case 3:
                printf("\nPassword change module coming soon!\n");
                break;

            case 4:
                printf("\nAccount recovery module coming soon!\n");
                break;

            case 5:
                printf("\nLogin monitoring module coming soon!\n");
                break;

            case 6:
                printf("\nActivity reports module coming soon!\n");
                break;

            case 7:
                printf("\nThank you for using Password Vault!\n");
                printf("Exiting system...\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 7);

    return 0;
}