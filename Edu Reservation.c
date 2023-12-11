#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROOMS 6
#define MAX_SCHEDULES 6
#define MAX_BOOKINGS 10
#define MAX_NIM_LENGTH 15
#define MAX_PASSWORD_LENGTH 20
#define MAX_CUSTOM_SCHEDULE_LENGTH 30

struct Booking {
    char room[20];
    char schedule[30];
};

struct User {
    char nim[MAX_NIM_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct Booking bookings[MAX_BOOKINGS];
    int bookingCount;
};

// Fungsi-fungsi utilitas
bool isValidNIM(const char nim[]) {
    char prefix[3];
    strncpy(prefix, nim, 2);
    prefix[2] = '\0';
    int year = atoi(prefix);

    return (year >= 20 && year <= 23);
}

int findUser(const char nim[], const struct User users[], int userCount) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].nim, nim) == 0) {
            return i; // User found
        }
    }
    return -1; // User not found
}

void displaySchedule() {
    printf("\nJadwal kelas tersedia:\n");
    printf("1. MKB 1A\n");
    printf("2. MKB 1B\n");
    printf("3. MKB 1C\n");
    printf("4. LAB\n");
    printf("5. MKB 29\n");
    printf("6. MKB 30\n");
}
void displayPersonalSchedule(const struct User* user) {
    printf("\nJadwal kelas Anda:\n");
    for (int i = 0; i < user->bookingCount; ++i) {
        printf("Ruangan: %s, %s\n", user->bookings[i].room, user->bookings[i].schedule);
    }
}

void displayAvailableSchedules() {
    printf("\nPilihan jadwal:\n");
    printf("1. Senin  07:00-09:30\n");
    printf("2. Senin  09:30-11:10\n");
    printf("3. Senin  11:10-13:00\n");
    printf("4. Senin  13:00-15:00\n");
    printf("5. Senin  15:30-17:10\n");
    printf("6. Waktu khusus\n");
}

int main() {
    struct User users[MAX_BOOKINGS];
    int userCount = 0;

    while (1) {
        char nim[MAX_NIM_LENGTH];
        char password[MAX_PASSWORD_LENGTH];

// Login
        printf("\nLogin:\n");
        printf("Masukkan NIM (20/21/22/23XXXX): ");
        scanf("%s", nim);

        if (!isValidNIM(nim)) {
            printf("Login gagal. NIM tidak valid.\n");
            continue;
        }

        printf("Masukkan password: ");
        scanf("%s", password);

        int userIndex = findUser(nim, users, userCount);

        if (userIndex == -1) {
            // User not found, create new user
            if (userCount < MAX_BOOKINGS) {
                struct User newUser;
                sprintf(newUser.nim, "%s", nim);
                sprintf(newUser.password, "%s", password);
                newUser.bookingCount = 0;

                users[userCount] = newUser;
                userCount++;

                printf("Login berhasil! Selamat datang, %s.\n", nim);
                userIndex = userCount - 1;
            } else {
                printf("Jumlah pengguna sudah mencapai batas maksimal.\n");
                continue;
            }
        } else if (strcmp(users[userIndex].password, password) != 0) {
            printf("Login gagal. Password salah.\n");
            continue;
        }
