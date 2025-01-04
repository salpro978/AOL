#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#define CLR_SCREEN "\e[1;1H\e[2J"

void CaesarEncrypt(char text[], int shift)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        char ch = text[i];

        if (isalpha(ch))
        {
            char base = islower(ch) ? 'a' : 'A';
            text[i] = (ch - base + shift) % 26 + base;
        }
    }

    printf("Hasil kalimat yang sudah di enkripsi: %s\n", text);
    printf("Press any key to continue...");
    getch();
}

void CaesarDecrypt(char text[], int shift)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        char ch = text[i];

        if (isalpha(ch))
        {
            char base = islower(ch) ? 'a' : 'A';
            text[i] = (ch - base - shift + 26) % 26 + base;
        }
    }

    printf("Hasil kalimat yang sudah di dekripsi: %s\n", text);
    printf("Press any key to continue...");
    getch();
}

int main()
{

    int choice;
    int condition = 1;
    do
    {
        printf(CLR_SCREEN);
        int shift;
        char text[255];

        puts("Selamat datang di pemograman caesar chiper");
        printf("Masukkan kalimat yang ingin di enkripsi atau dekripsi: ");
        fgets(text, sizeof(text), stdin);
        text[strcspn(text, "\n")] = '\0';

        puts("Pilih program yang diinginkan");
        puts("\t1. Enkripsi");
        puts("\t2. Dekripsi");
        puts("\t0. Exit");
        printf("Masukkan angka untuk memilih program yang diinginkan: ");
        scanf("%d", &choice);

        while (getchar() != '\n');

        if(choice == 1 || choice == 2){
            printf("Masukkan nilai shift: ");
            scanf("%d", &shift);
            while (getchar() != '\n');
        }

        switch (choice)
        {
        case 1:
            CaesarEncrypt(text, shift);
            break;

        case 2:
            CaesarDecrypt(text, shift);
            break;

        case 0:
            condition = 0;
            break;

        default:
            puts("Pilihan yang anda masukkan tidak tersedia");
            break;
        }
    } while(condition);
}