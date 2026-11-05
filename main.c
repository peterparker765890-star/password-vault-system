#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define USER_FILE "users.dat"
#define TEMP_FILE "users_temp.dat"

/* FUNCTION DECLARATIONS */

void readLine(char text[], int size);
void displayMenu();

void registerUser();
void loginUser();
void changePassword();

int usernameExists(char username[]);
int authenticateUser(char username[], char password[]);
int validPassword(char password[]);

unsigned long long educationalHash(char password[]);

/* INPUT FUNCTION */

void readLine(char text[], int size)
{
    int ch;

    if (fgets(text, size, stdin) != NULL)
    {
        if (strchr(text, '\n') == NULL)
        {
            while ((ch = getchar()) != '\n' && ch != EOF)
            {
                /* Clear remaining input */
            }
        }

        text[strcspn(text, "\n")] = '\0';
    }
    else
    {
        text[0] = '\0';
    }
}

/* EDUCATIONAL PASSWORD HASH */

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

/* PASSWORD VALIDATION */

int validPassword(char password[])
{
    int upper = 0;
    int lower = 0;
    int digit = 0;
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

/* CHECK DUPLICATE USERNAME */

int usernameExists(char username[])
{
    FILE *fp;
    char storedUsername[64];
    unsigned long long storedHash;

    fp = fopen(USER_FILE, "r");

    if (fp == NULL)
        return 0;

    while (fscanf(fp, " %63[^|]|%llu",
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

    unsigned long long hash;
    FILE *fp;

    printf("\n========== USER REGISTRATION ==========\n");

    printf("Enter username: ");
    readLine(username, sizeof(username));

    if (strlen(username) == 0 ||
        strchr(username, '|') != NULL)
    {
        printf("\nInvalid username!\n");
        return;
    }

    if (usernameExists(username))
    {
        printf("\nUSERNAME ALREADY EXISTS!\n");
        return;
    }

    printf("Create password: ");
    readLine(password, sizeof(password));

    if (!validPassword(password))
    {
        printf("\nWEAK PASSWORD!\n");
        printf("Password must contain:\n");
        printf("- Minimum 8 characters\n");
        printf("- At least one uppercase letter\n");
        printf("- At least one lowercase letter\n");
        printf("- At least one digit\n");
        return;
    }

    printf("Confirm password: ");
    readLine(confirmPassword, sizeof(confirmPassword));

    if (strcmp(password, confirmPassword) != 0)
    {
        printf("\nPASSWORDS DO NOT MATCH!\n");
        return;
    }

    hash = educationalHash(password);

    fp = fopen(USER_FILE, "a");

    if (fp == NULL)
    {
        printf("\nUnable to open user database!\n");
        return;
    }

    if (fprintf(fp, "%s|%llu\n", username, hash) < 0)
    {
        printf("\nError saving user record!\n");
        fclose(fp);
        return;
    }

    fclose(fp);

    printf("\nREGISTRATION SUCCESSFUL!\n");
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

/* MODULE 03: PASSWORD CHANGE */

void changePassword()
{
    char username[64];
    char oldPassword[64];
    char newPassword[64];
    char confirmPassword[64];

    char storedUsername[64];

    unsigned long long storedHash;
    unsigned long long newHash;

    FILE *fp;
    FILE *temp;

    int found = 0;
    int writeError = 0;

    printf("\n========== PASSWORD CHANGE ==========\n");

    printf("Enter username: ");
    readLine(username, sizeof(username));

    printf("Enter current password: ");
    readLine(oldPassword, sizeof(oldPassword));

    /* VERIFY CURRENT PASSWORD */

    if (!authenticateUser(username, oldPassword))
    {
        printf("\nAUTHENTICATION FAILED!\n");
        printf("Incorrect username or current password.\n");
        return;
    }

    printf("\nCURRENT PASSWORD VERIFIED!\n");

    /* GET NEW PASSWORD */

    printf("Enter new password: ");
    readLine(newPassword, sizeof(newPassword));

    if (!validPassword(newPassword))
    {
        printf("\nWEAK PASSWORD!\n");
        printf("Minimum 8 characters required.\n");
        printf("Use uppercase, lowercase and digits.\n");
        return;
    }

    /* CHECK SAME PASSWORD */

    if (strcmp(oldPassword, newPassword) == 0)
    {
        printf("\nNEW PASSWORD MUST BE DIFFERENT!\n");
        return;
    }

    /* CONFIRM NEW PASSWORD */

    printf("Confirm new password: ");
    readLine(confirmPassword, sizeof(confirmPassword));

    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printf("\nPASSWORDS DO NOT MATCH!\n");
        return;
    }

    /* OPEN ORIGINAL DATABASE */

    fp = fopen(USER_FILE, "r");

    if (fp == NULL)
    {
        printf("\nUnable to open user database!\n");
        return;
    }

    /* CREATE TEMPORARY DATABASE */

    temp = fopen(TEMP_FILE, "w");

    if (temp == NULL)
    {
        fclose(fp);
        printf("\nUnable to create temporary file!\n");
        return;
    }

    newHash = educationalHash(newPassword);

    /* UPDATE SELECTED USER RECORD */

    while (fscanf(fp, " %63[^|]|%llu",
                  storedUsername, &storedHash) == 2)
    {
        if (strcmp(username, storedUsername) == 0)
        {
            if (fprintf(temp, "%s|%llu\n",
                        storedUsername, newHash) < 0)
            {
                writeError = 1;
                break;
            }

            found = 1;
        }
        else
        {
            if (fprintf(temp, "%s|%llu\n",
                        storedUsername, storedHash) < 0)
            {
                writeError = 1;
                break;
            }
        }
    }

    if (ferror(fp))
        writeError = 1;

    fclose(fp);

    if (fclose(temp) != 0)
        writeError = 1;

    if (!found || writeError)
    {
        remove(TEMP_FILE);

        if (writeError)
            printf("\nDatabase writing error!\n");
        else
            printf("\nUSER ACCOUNT NOT FOUND!\n");

        return;
    }

    /* REPLACE DATABASE WITH UPDATED FILE */

    if (rename(TEMP_FILE, USER_FILE) != 0)
    {
        printf("\nDATABASE UPDATE FAILED!\n");
        printf("Original database has been retained.\n");
        remove(TEMP_FILE);
        return;
    }

    printf("\n========================================\n");
    printf("   PASSWORD CHANGED SUCCESSFULLY!\n");
    printf("========================================\n");

    printf("Your new password has been saved.\n");
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

/* MAIN FUNCTION */

int main()
{
    char input[20];
    char extra;
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

        if (sscanf(input, "%d %c", &choice, &extra) != 1)
        {
            printf("\nINVALID INPUT! ENTER A NUMBER.\n");
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
                changePassword();
                break;

            case 4:
                printf("\nACCOUNT RECOVERY MODULE COMING SOON!\n");
                break;

            case 5:
                printf("\nLOGIN MONITORING MODULE COMING SOON!\n");
                break;

            case 6:
                printf("\nACTIVITY REPORTS MODULE COMING SOON!\n");
                break;

            case 7:
                printf("\nTHANK YOU FOR USING PASSWORD VAULT!\n");
                printf("Exiting system...\n");
                break;

            default:
                printf("\nINVALID CHOICE! TRY AGAIN.\n");
        }

    } while (choice != 7);

    return 0;
}