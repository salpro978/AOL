#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>
#include <errno.h>

#define CLR_SCREEN "\e[1;1H\e[2J"
#define SCREEN_DELAY 500000

enum room_type
{
    single_room = 1,
    double_room,
    twin_room,
    suite_room
};

enum room_level
{
    normal = 1,
    VIP,
    VVIP
};

struct guest
{
    char name[100];
    int day, month, years;
    int long_day;
    int price;
    enum room_type room;
    enum room_level level;
};

const char *type_output(enum room_type room)
{
    switch (room)
    {
    case 1:
        return "Single room";

    case 2:
        return "Double room";

    case 3:
        return "Twin room";

    case 4:
        return "Suite room";

    default:
        return "Unknown";
    }
}

const char *class_output(enum room_level level)
{
    switch (level)
    {
    case 1:
        return "Normal";

    case 2:
        return "VIP";

    case 3:
        return "VVIP";

    default:
        return "Unknown";
    }
}

void booking_room();
void booking_history();
void type_menu();
void bank_menu();
void sort_booking();
void price_room();
void enableAnsiEscapeCodes();
void CLEAR_SCREEN();
int menu();
int choice_price_single(struct guest *user);
int choice_price_double(struct guest *user);
int choice_price_twin(struct guest *user);\
int choice_price_suite(struct guest *user);
int check_date(int day, int month, int years);
void update_file(struct guest *user);

int main()
{
    enableAnsiEscapeCodes();
    CLEAR_SCREEN();

    int choice = menu();
    getchar();

    switch (choice)
    {
    case 1:
        booking_room();
        break;

    case 2:
        booking_history();
        break;

    case 3:
        return 0;
    }

    return 0;
}

void enableAnsiEscapeCodes()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void booking_room()
{
    int date_condition;
    int room_condition;
    int room_type_choice;
    struct guest user;

    system("cls");
    printf("Input your name : ");
    scanf("%[^\n]", &user.name);
    getchar();
    do
    {
        printf("Input the reservation date (dd mm yyyy): ");
        scanf("%d %d %d", &user.day, &user.month,
              &user.years);
        getchar();
        date_condition = check_date(user.day, user.month,
                                    user.years);
    } while (date_condition);

    printf("Input the length of stay (days): ");
    scanf("%d", &user.long_day);
    
    do
    {
        type_menu();
        printf("Input your choice: ");
        scanf("%d", &room_type_choice);
        switch (room_type_choice)
        {
        case 1:
            room_condition = choice_price_single(&user);
            break;

        case 2:
            room_condition = choice_price_double(&user);
            break;

        case 3:
            room_condition = choice_price_twin(&user);
            break;

        case 4:
            room_condition = choice_price_suite(&user);
            break;

        default:
            break;
        }
    } while (room_condition);
    puts("Congratulations, you have registered.");
    puts("Please make payment by transfer to the number provided.");
    printf("Total payment: %d\n", user.price);
    bank_menu();
    puts("your room has been booked");
    getch();
    update_file(&user);

    main();
}

int menu()
{
    int choice;
    static int count = 0;
    if(count == 0)
    {
        puts("Welcome to reservation program");
        count++;
    }
    else
    {
        puts("Welcome back to reservation program");   
    }
    puts("\t1. Booking bedroom");
    puts("\t2. Booking history");
    puts("\t3. Exit");
    printf("Input your choice: ");

    scanf("%d", &choice);
    if(choice < 1 || choice > 3)
    {
        puts("Enter the available numbers...");
        usleep(SCREEN_DELAY);
        menu();
    }

    return choice;
}

void booking_history()
{
    CLEAR_SCREEN();
    sort_booking();

    FILE *file = fopen("booking.csv", "r");
    if (file == NULL)
    {
        printf("File couldn't open : %s\n", strerror(errno));
        sleep(2);
    }

    puts("Booking history");
    puts(" ------------------------------------------------------------------------");
    printf("| %-10s | %-15s | %-14s | %-10s | %-9s |\n", "Name", "Date", "Room", "Class", "Price");
    puts(" ------------------------------------------------------------------------");

    struct guest temp;
    char date[100];
    while(fscanf(file, "%[^,],%[^,],%d,%d,%d,%d\n", temp.name,
                  &date, &temp.long_day, &temp.room, &temp.level, &temp.price) == 6)
    {
        printf("| %-10s | %-15s | %-14s | %-10s | %-9d |\n", temp.name,
               date, type_output(temp.room), class_output(temp.level), temp.price);
    }
    puts(" ------------------------------------------------------------------------");
    fclose(file);

    printf("Press any key to back...");
    getch();
    main();
}

void update_file(struct guest *user)
{
    FILE *file = fopen("booking.csv", "a");
    if (file == NULL)
    {
        puts("File couldn't open");
        sleep(2);
    }

    fprintf(file, "%s,%d-%d-%d,%d,%d,%d,%d\n", user->name,
            user->day, user->month, user->years,
            user->long_day, user->room, user->level,
            user->price);

    fclose(file);
}

void sort_booking()
{
    FILE *file = fopen("booking.csv", "r");

    if (file == NULL)
    {
        puts("File couldn't open");
        sleep(2);
        return;
        }

        struct guest *user = NULL;
        int size = 0;

        struct guest temp;
        while ((fscanf(file, "%[^,],%d-%d-%d,%d,%d,%d,%d\n", temp.name,
               &temp.day, &temp.month, &temp.years,
               &temp.long_day, &temp.room, &temp.level,
               &temp.price)) == 8)
        {
        struct guest *new_user = (struct guest *)realloc(user, (size + 1) * sizeof(struct guest));
        if (new_user == NULL)
        {
            puts("Memory allocation failed");
            fclose(file);
            return;
        }

        user = new_user;
        user[size] = temp;
        size++;
    }

    fclose(file);

    for(int i = 0; i < size; i++)
    {
        int small = i;
        for(int j = i; j < size; j++)
        {
            if(user[small].years == user[j].years)
            {
                if(user[small].month == user[j].month)
                {
                    if(user[small].day > user[j].day)
                    {
                        small = j;
                    }
                }
                else if(user[small].month > user[j].month)
                {
                    small = j;
                }
            }
            else if(user[small].years > user[j].years)
            {
                small = j;
            }
        }
        struct guest temp = user[small];
        user[small] = user[i];
        user[i] = temp;
    }

    file = fopen("booking.csv", "w");

    if(file == NULL)
    {
        puts("File couldn't open");
        fclose(file);
        sleep(2);
    }
    
    for(int i = 0; i < size; i++)
    {
        fprintf(file, "%s,%d-%d-%d,%d,%d,%d,%d\n", user[i].name,
                user[i].day, user[i].month, user[i].years,
                user[i].long_day, user[i].room, user[i].level,
                user[i].price);
    }

    fclose(file);
    free(user);
}

void type_menu()
{
    CLEAR_SCREEN();
    puts("The types of hotel rooms available are: ");
    puts("\t1. Single room");
    puts("\t2. Double room");
    puts("\t3. Twin room");
    puts("\t4. Suite room");
}

void bank_menu()
{
    CLEAR_SCREEN();
    puts("Send payment via the bank available below");
    puts("1. BRI     -> 7632-01-007520-53-0");
    puts("2. Mandiri -> 1310007374134");
    puts("3. BCA     -> 3161523756");
    printf("Press enter if you have made the payment...");
    getch();
    printf("\nChecking your payment...");
    sleep(2);
    system("cls");
}

int check_date(int day, int month, int years)
{
    if ((day <= 0 || day > 31) || (month <= 0 || month > 12) || (years <= 2024))
    {
        puts("the date you entered is incorrect");
        return 1;
    }
    else
    {
        return 0;
    }
}

int choice_price_single(struct guest *user)
{
    CLEAR_SCREEN();
    int choice;
    puts("1. Normal -> Rp.300.000,00");
    puts("2. VIP    -> Rp.500.000,00");
    puts("3. VVIP   -> Rp.800.000,00");
    puts("Enter any number to back");
    printf("Input your choice: ");
    scanf("%d", &choice);
    if (choice >= 1 && choice <= 3)
    {
        user->room = single_room;
    }
    switch (choice)
    {

    case 1:
        user->level = normal;
        user->price = 300000;
        return 0;

    case 2:
        user->level = VIP;
        user->price = 500000;
        return 0;

    case 3:
        user->level = VVIP;
        user->price = 800000;
        return 0;

    default:
        return 1;
    }

    return 0;
}

int choice_price_double(struct guest *user)
{
    CLEAR_SCREEN();
    int choice;
    puts("1. Normal -> Rp.500.000,00");
    puts("2. VIP    -> Rp.900.000,00");
    puts("3. VVIP   -> Rp.1.500.000,00");
    puts("Enter any number to back");
    printf("Input your choice: ");
    scanf("%d", &choice);
    if (choice >= 1 && choice <= 3)
    {
        user->room = double_room;
    }
    switch (choice)
    {

    case 1:
        user->level = normal;
        user->price = 500000;
        return 0;

    case 2:
        user->level = VIP;
        user->price = 900000;
        return 0;

    case 3:
        user->level = VVIP;
        user->price = 1500000;
        return 0;

    default:
        return 1;
    }
    return 0;
}

int choice_price_twin(struct guest *user)
{
    CLEAR_SCREEN();
    int choice;
    puts("1. Normal -> Rp.600.000,00");
    puts("2. VIP    -> Rp.1.000.000,00");
    puts("3. VVIP   -> Rp.1.700.000,00");
    puts("enter any number to back");
    printf("Input your choice: ");
    scanf("%d", &choice);
    if (choice >= 1 && choice <= 3)
    {
        user->room = twin_room;
    }
    switch (choice)
    {

    case 1:
        user->level = normal;
        user->price = 600000;
        return 0;

    case 2:
        user->level = VIP;
        user->price = 1000000;
        return 0;

    case 3:
        user->level = VVIP;
        user->price = 1700000;
        return 0;

    default:
        return 1;
    }
    return 0;
}

int choice_price_suite(struct guest *user)
{
    CLEAR_SCREEN();
    int choice;
    puts("1. Normal -> Rp.1.500.000,00");
    puts("2. VIP    -> Rp.2.000.000,00");
    puts("3. VVIP   -> Rp.3.000.000,00");
    puts("enter any number to back");
    printf("Input your choice: ");
    scanf("%d", &choice);
    if (choice >= 1 && choice <= 3)
    {
        user->room = suite_room;
    }
    switch (choice)
    {

    case 1:
        user->level = normal;
        user->price = 1500000;
        return 0;

    case 2:
        user->level = VIP;
        user->price = 2000000;
        return 0;

    case 3:
        user->level = VVIP;
        user->price = 3000000;
        return 0;

    default:
        return 1; 
    }
    return 0;
}

void CLEAR_SCREEN()
{
    printf(CLR_SCREEN);
}