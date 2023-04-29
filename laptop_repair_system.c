#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char brand[20];
    char model[20];
    char problem[50];
    float cost;
} Laptop;

void addLaptop(Laptop laptops[], int *numLaptops)
{
    printf("\nEnter laptop brand: ");
    scanf("%s", laptops[*numLaptops].brand);
    printf("Enter laptop model: ");
    scanf("%s", laptops[*numLaptops].model);
    printf("Enter problem description: ");
    scanf("%s", laptops[*numLaptops].problem);
    printf("Enter estimated cost of repair: ");
    scanf("%f", &laptops[*numLaptops].cost);

    (*numLaptops)++;

    FILE *fp = fopen("laptops.dat", "ab");
    fwrite(&laptops[*numLaptops-1], sizeof(Laptop), 1, fp);
    fclose(fp);

    printf("Laptop added to repair list.\n");
}

void trackRepairing()
{
    FILE *fp;
    Laptop laptop;
    char brand[20], model[20];
    int found = 0;

    fp = fopen("laptops.dat", "rb");

    if (fp == NULL)
    {
        printf("Error: could not open file.\n");
        return;
    }

    printf("\nEnter laptop brand: ");
    scanf("%s", brand);
    printf("Enter laptop model: ");
    scanf("%s", model);

    while (fread(&laptop, sizeof(laptop), 1, fp) == 1)
    {
        if (strcmp(brand, laptop.brand) == 0 && strcmp(model, laptop.model) == 0)
        {
            printf("Laptop found:\n");
            printf("Brand: %s\n", laptop.brand);
            printf("Model: %s\n", laptop.model);
            printf("Problem: %s\n", laptop.problem);
            printf("Estimated cost of repair: INR %.2f\n", laptop.cost);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("Laptop not found.\n");
    }
}

void billingAndPayment()
{
    float totalCost = 0;
    printf("\nBilling and Payment Details\n");
    printf("Brand\tModel\tProblem\tCost\n");

    FILE *file = fopen("laptops.dat", "rb");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    Laptop laptop;
    while (fread(&laptop, sizeof(Laptop), 1, file))
    {
        printf("%s\t%s\t%s\t%.2f\n", laptop.brand, laptop.model, laptop.problem, laptop.cost);
        totalCost += laptop.cost;
    }

    fclose(file);

    printf("Total cost of repairs: INR %.2f\n", totalCost);
}

void viewAllLaptops(Laptop laptops[], int numLaptops)
{
    FILE *fp;
    fp = fopen("laptops.dat", "rb");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int i = 0;
    while (fread(&laptops[i], sizeof(Laptop), 1, fp) == 1)
    {
        i++;
    }
    numLaptops = i;

    fclose(fp);

    if (numLaptops == 0)
    {
        printf("There are no laptops to display.\n");
        return;
    }

    printf("Laptop Repair List\n");
    printf("----------------------------------------------------\n");
    printf("Brand\tModel\tProblem\t\tCost\n");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < numLaptops; i++)
    {
        printf("%s\t%s\t%s\t\tINR %.2f\n", laptops[i].brand, laptops[i].model,
               laptops[i].problem, laptops[i].cost);
    }
}

void removeLaptop(Laptop laptops[], int *numLaptops)
{
    char brand[20], model[20];
    int found = 0;
    scanf("%s", brand);
    printf("Enter laptop model: ");
    scanf("%s", model);

    for (int i = 0; i < *numLaptops; i++)
    {
        if (strcmp(brand, laptops[i].brand) == 0 && strcmp(model, laptops[i].model) == 0)
        {
            for (int j = i; j < (*numLaptops - 1); j++)
            {
                laptops[j] = laptops[j + 1];
            }


            FILE *fp = fopen("laptops.dat", "wb");
            fwrite(laptops, sizeof(Laptop), *numLaptops, fp);
            fclose(fp);

            printf("Laptop removed from repair list.\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Laptop not found.\n");
    }
}

int main()
{
    Laptop laptops[100];
    int numLaptops = 0;
    int choice;

    do
    {
        printf("\nLaptop Repair System\n");
        printf("1. Add laptop to repair\n");
        printf("2. View all laptops to repair\n");
        printf("3. Track Repairing Details\n");
        printf("4. Remove laptop\n");
        printf("5. Billing and payment details\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addLaptop(laptops, &numLaptops);
            break;
        case 2:
            viewAllLaptops(laptops, numLaptops);
            break;
        case 3:
            trackRepairing();
            break;
        case 4:
            removeLaptop(laptops, &numLaptops);
            break;
        case 5:
            billingAndPayment(laptops, numLaptops);
            break;
        case 6:
            printf("Exiting program...\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
    return 0;
}
