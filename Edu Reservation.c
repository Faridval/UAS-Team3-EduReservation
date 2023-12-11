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
while (1) {
            // Display menu
            printf("\nMenu:\n");
            printf("1. Booking kelas\n");
            printf("2. Lihat jadwal kelas tersedia\n");
            printf("3. Lihat jadwal pribadi\n");
            printf("4. Pembatalan booking kelas\n");
            printf("5. Logout\n");
            printf("Pilih menu (1-5): ");

            int choice;
            scanf("%d", &choice);

            switch (choice) {
                case 1: {
                    // Booking kelas
                    displaySchedule();
                    printf("Pilih ruangan (1-6): ");
                    int roomChoice;
                    scanf("%d", &roomChoice);

                    if (roomChoice < 1 || roomChoice > MAX_ROOMS) {
                        printf("Pilihan ruangan tidak valid.\n");
                        continue;
                    }

                    printf("Pilih jadwal:\n");
                    displayAvailableSchedules();
                    printf("Pilih waktu (1-6): ");
                    int scheduleChoice;
                    scanf("%d", &scheduleChoice);

                    if (scheduleChoice < 1 || scheduleChoice > MAX_SCHEDULES) {
                        printf("Pilihan waktu tidak valid.\n");
                        continue;
                    } 
             struct Booking newBooking;
                    switch (roomChoice) {
                        case 1:
                            sprintf(newBooking.room, "MKB 1A");
                            break;
                        case 2:
                            sprintf(newBooking.room, "MKB 1B");
                            break;
                        case 3:
                            sprintf(newBooking.room, "MKB 1C");
                            break;
                        case 4:
                            sprintf(newBooking.room, "LAB");
                            break;
                        case 5:
                            sprintf(newBooking.room, "SMARTCLASS");
                            break;
                        case 6:
                            sprintf(newBooking.room, "MKB 30");
                            break;
                    }

                    switch (scheduleChoice) {
                        case 1:
                            sprintf(newBooking.schedule, "Senin 7:00-09:30");
                            break;
                        case 2:
                            sprintf(newBooking.schedule, "Senin 9:30-11:10");
                            break;
                        case 3:
                            sprintf(newBooking.schedule, "Senin 11:10-13:00");
                            break;
                        case 4:
                            sprintf(newBooking.schedule, "Senin 13:00-15:30");
                            break;
                        case 5:
                            sprintf(newBooking.schedule, "Senin 15:30-17:10");
                            break;
                        case 6: {
                            printf("Masukkan waktu khusus (contoh: Selasa 10:00-12:00): ");
                            scanf(" %[^\n]s", newBooking.schedule);
                            break;
                        }
                    }
                    if (users[userIndex].bookingCount < MAX_BOOKINGS) {
                        users[userIndex].bookings[users[userIndex].bookingCount++] = newBooking;
                        printf("Booking berhasil!\n");
                    } else {
                        printf("Jumlah booking Anda sudah mencapai batas maksimal.\n");
                    }

                    break;
                    }
                    case 2: {
                        // Lihat jadwal kelas tersedia
                        displaySchedule();
                        displayAvailableSchedules();
                        break;
                    }
                    case 3: {
                        // Lihat jadwal pribadi
                        displayPersonalSchedule(&users[userIndex]);
                        break;
                    }
                    case 4: {
                        // Pembatalan booking kelas
                        printf("Jadwal kelas yang sudah dibooking:\n");
                        displayPersonalSchedule(&users[userIndex]);
    
                        if (users[userIndex].bookingCount > 0) {
                            printf("Pilih nomor booking yang ingin dibatalkan (1-%d): ", users[userIndex].bookingCount);
                            int cancelChoice;
                            scanf("%d", &cancelChoice);
    
                            if (cancelChoice >= 1 && cancelChoice <= users[userIndex].bookingCount) {
                                for (int i = cancelChoice - 1; i < users[userIndex].bookingCount - 1; ++i) {
                                    users[userIndex].bookings[i] = users[userIndex].bookings[i + 1];
                                }
                                users[userIndex].bookingCount--;
                                printf("Booking berhasil dibatalkan.\n");
                            } else {
                                printf("Nomor booking tidak valid.\n");
                            }
                        } else {
                            printf("Anda belum melakukan booking.\n");
                        }
    
                        break;
                    }
                    case 5: {
                        printf("Logout berhasil. Sampai jumpa, %s!\n", nim);
                        break;
                    }
                    default: {
                        printf("Pilihan tidak valid. Silakan pilih 1-5.\n");
                        break;
                    }
                }
    
                if (choice == 5) {
                    break; // Logout
                }
            }
        }

    return 0;
}
