#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Bus
{
    int bus_no;
    char name[50];
    int seats[50];
    char route[50];
    float fare;
};

void displayAllBuses()
{
    struct Bus b;
    FILE *fp;
    fp = fopen("buses.bin", "rb");

    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    printf("All available buses:\n");
    printf("Bus No  Name     Route    Fair");
    while (fread(&b, sizeof(b), 1, fp) == 1)
    {
        printf("\n%d\t%s\t%s\t%0.2f", b.bus_no, b.name, b.route, b.fare);
    }

    fclose(fp);
}

void displayBusByRoute()
{
    struct Bus b;
    FILE *fp;
    char route[50];
    int found = 0;
    printf("Enter route: ");
    scanf("%s", route);

    fp = fopen("buses.bin", "rb");

    while (fread(&b, sizeof(struct Bus), 1, fp) == 1)
    {
        if (strcmp(b.route, route) == 0)
        {
            printf("\n\nBuses available for route %s:\n", route);
            printf("%d\t%s\t%.2f\t", b.bus_no, b.name, b.fare);
            printf("\nAvailable seat Numbers are:\n");
            for (int i = 0; i < 20; i++)
            {
                if (b.seats[i] == 0)
                {
                    printf("%d ", i + 1);
                }
            }
            printf("\n");
            found = 1;
        }
    }
    if (!found)
    {
        printf("No buses found for route %s\n", route);
    }
    fclose(fp);
}

void addBus()
{
    struct Bus b;
    int n;
    printf("Enter bus number: ");
    scanf("%d", &b.bus_no);
    printf("Enter name: ");
    scanf("%s", b.name);
    printf("Enter bus route: ");
    scanf("%s", b.route);
    printf("Enter fare: ");
    scanf("%f", &b.fare);
    int i;
    for (i = 0; i < 50; i++)
    {
        b.seats[i] = 0;
    }

    FILE *fp;
    fp = fopen("buses.bin", "ab");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&b, sizeof(struct Bus), 1, fp);
    printf("Bus added!!");
    fclose(fp);
}

void bookSeat()
{
    struct Bus b;
    int bus_no, seatNo, found = 0;
    FILE *fp;

    printf("Enter bus number: ");
    scanf("%d", &bus_no);
    printf("Enter seat number: ");
    scanf("%d", &seatNo);

    fp = fopen("buses.bin", "r+b");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    while (fread(&b, sizeof(struct Bus), 1, fp) == 1)
    {
        if (b.bus_no == bus_no)
        {
            found = 1;
            if (b.seats[seatNo - 1] == 1)
            {
                printf("\nSeat already booked.\n");
            }
            else
            {
                b.seats[seatNo - 1] = 1;
                fseek(fp, -sizeof(struct Bus), SEEK_CUR);
                fwrite(&b, sizeof(struct Bus), 1, fp);
                printf("\nSeat booked successfully.\n");
            }
            break;
        }
    }

    if (!found)
    {
        printf("Bus not found!\n");
    }

    fclose(fp);
}

void cancelSeat()
{
    struct Bus b;
    int bus_no, seatNo, found = 0;
    FILE *fp;

    printf("Enter bus number: ");
    scanf("%d", &bus_no);
    printf("Enter seat number: ");
    scanf("%d", &seatNo);

    fp = fopen("buses.bin", "rb+");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    while (fread(&b, sizeof(struct Bus), 1, fp) == 1)
    {
        if (b.bus_no == bus_no)
        {
            found = 1;
            if (b.seats[seatNo - 1] == 0)
            {
                printf("\nSeat is not booked yet.\n");
            }
            else
            {
                b.seats[seatNo - 1] = 0;
                fseek(fp, -sizeof(struct Bus), SEEK_CUR);
                fwrite(&b, sizeof(struct Bus), 1, fp);
                printf("\nSeat cancelled successfully.\n");
            }
            break;
        }
    }

    if (!found)
    {
        printf("Bus not found!\n");
    }
    fclose(fp);
}

void updateBus()
{
    struct Bus b;
    FILE *fp;
    int found = 0, bus_no, choice;

    printf("Enter bus number: ");
    scanf("%d", &bus_no);

    fp = fopen("buses.bin", "r+b");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1)
    {
        if (b.bus_no == bus_no)
        {
            found = 1;
            printf("\nWhat do you want to update?\n");
            printf("1. Bus name\n");
            printf("2. Route\n");
            printf("3. Fare\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("Enter new bus name: ");
                scanf("%s", b.name);
                break;
            case 2:
                printf("Enter new route: ");
                scanf("%s", b.route);
                break;
            case 3:
                printf("Enter new fare: ");
                scanf("%f", &b.fare);
                break;
            default:
                printf("\nInvalid choice!\n");
                break;
            }
            fseek(fp, -sizeof(b), SEEK_CUR);
            fwrite(&b, sizeof(b), 1, fp);
            printf("\nBus details updated successfully.\n");
            break;
        }
    }
    if (!found)
    {
        printf("\nBus not found.\n");
    }
    fclose(fp);
}

void deleteBus()
{
    struct Bus b;
    FILE *fp, *temp;
    int found = 0, bus_no;

    printf("Enter bus number: ");
    scanf("%d", &bus_no);

    fp = fopen("buses.bin", "rb");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }

    temp = fopen("temp.bin", "wb");
    if (temp == NULL)
    {
        printf("Error creating temporary file!\n");
        fclose(fp);
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1)
    {
        if (b.bus_no != bus_no)
        {
            fwrite(&b, sizeof(b), 1, temp);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found)
    {
        printf("Bus not found!\n");
        remove("temp.bin");
        return;
    }

    remove("buses.bin");
    rename("temp.bin", "buses.bin");

    printf("Bus deleted successfully!\n");
}


int main()
{
    int choice, bus_no;
    char route[50];

    do
    {
        printf("\n\n");
        printf("===============================================\n");
        printf("               BUS RESERVATION SYSTEM           \n");
        printf("===============================================\n");
        printf("1. View All Available Buses\n");
        printf("2. View Available Buses For Particular Route\n");
        printf("3. Add New Bus\n");
        printf("4. Book a Seat\n");
        printf("5. Cancel seat\n");
        printf("6. Update bus details\n");
        printf("7. Delete bus details\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
        case 1:
            displayAllBuses();
            break;
        case 2:
            displayBusByRoute();
            break;
        case 3:
            addBus();
            break;
        case 4:
            bookSeat();
            break;
        case 5:
            cancelSeat();
            break;
        case 6:
            updateBus();
            break;
        case 7:
            deleteBus();
            break;
        case 8:
            printf("\nThank you for using our service!\n");
            exit(0);
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}
