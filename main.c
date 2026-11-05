#include <stdio.h>

void displayMenu()
{
    printf("\n");
    printf("========================================\n");
    printf("          PASSWORD VAULT SYSTEM         \n");
    printf("========================================\n");
    printf("  1. User Registration\n");
    printf("  2. Login Authentication\n");
    printf("  3. Password Change\n");
    printf("  4. Account Recovery\n");
    printf("  5. Login Attempt Monitoring\n");
    printf("  6. User Activity Reports\n");
    printf("  7. Exit\n");
    printf("========================================\n");
    printf("Enter your choice: ");
}

int main()
{
    int choice;

    printf("\n");
    printf("****************************************\n");
    printf("       PASSWORD VAULT SYSTEM            \n");
    printf("       C PROGRAMMING PROJECT            \n");
    printf("       DEVELOPED BY SABARISH            \n");
    printf("****************************************\n");

    do
    {
        displayMenu();

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input! Enter a number.\n");

            while (getchar() != '\n');

            continue;
        }

        switch (choice)
        {
            case 1:
                printf("\nUSER REGISTRATION MODULE\n");
                printf("Module will be implemented next.\n");
                break;

            case 2:
                printf("\nLOGIN AUTHENTICATION MODULE\n");
                printf("Module will be implemented next.\n");
                break;

            case 3:
                printf("\nPASSWORD CHANGE MODULE\n");
                printf("Module will be implemented next.\n");
                break;

            case 4:
                printf("\nACCOUNT RECOVERY MODULE\n");
                printf("Module will be implemented next.\n");
                break;

            case 5:
                printf("\nLOGIN ATTEMPT MONITORING MODULE\n");
                printf("Module will be implemented next.\n");
                break;

            case 6:
                printf("\nUSER ACTIVITY REPORTS MODULE\n");
                printf("Module will be implemented next.\n");
                break;

            case 7:
                printf("\nThank you for using Password Vault!\n");
                printf("Exiting system...\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}