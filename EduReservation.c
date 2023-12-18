#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROOMS 6
#define MAX_SCHEDULES 6
#define MAX_BOOKINGS 10
#define MAX_NIM_LENGTH 15
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_CUSTOM_SCHEDULE_LENGTH 30
#define JadwalMaksimal 20
#define MaksimalPengisian 20



struct Booking {
    char room[20];
    char schedule[30];
};

struct User {
    char username[MAX_USERNAME_LENGTH];
    char nim[MAX_NIM_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct Booking bookings[MAX_BOOKINGS];
    int bookingCount;
};
int tahunKabisat(int tahun) {
    return ((tahun % 4 == 0 && tahun % 40 != 0) || (tahun % 400 == 0));
}

int dataTanggal(int hari, int bulan, int tahun) {
    if (tahun < 1 || bulan < 1 || bulan > 12 || hari < 1) {
        return 0;
    }

    int maksimalHari = 31;

    switch (bulan) {

            maksimalHari = 30;
            break;
        case 1:
            maksimalHari = tahunKabisat(tahun) ? 29 : 28;
            break;
    }

    return (hari <= maksimalHari);
}

typedef struct {
    char waktuPemakaianawal[MaksimalPengisian];
    char selesaiPemakaian[MaksimalPengisian];
    char layanan[MaksimalPengisian];
} Booking;

int waktuYangtersedia(Booking bookings[], int nomerBookings, const char *waktuPemakaianawal, const char *selesaiPemakaian) {
    for (int i = 0; i < nomerBookings; i++) {
        if ((strcmp(bookings[i].waktuPemakaianawal, waktuPemakaianawal) < 0 && strcmp(bookings[i].selesaiPemakaian, waktuPemakaianawal) > 0) ||
            (strcmp(bookings[i].waktuPemakaianawal, waktuPemakaianawal) < 0 && strcmp(bookings[i].selesaiPemakaian, selesaiPemakaian) > 0) ||
            (strcmp(bookings[i].waktuPemakaianawal, waktuPemakaianawal) >= 0 && strcmp(bookings[i].selesaiPemakaian, selesaiPemakaian) <= 0) ||
            (strcmp(waktuPemakaianawal, bookings[i].waktuPemakaianawal) < 0 && strcmp(selesaiPemakaian, bookings[i].waktuPemakaianawal) > 0) ||
            (strcmp(waktuPemakaianawal, bookings[i].waktuPemakaianawal) < 0 && strcmp(selesaiPemakaian, bookings[i].selesaiPemakaian) > 0) ||
            (strcmp(waktuPemakaianawal, bookings[i].waktuPemakaianawal) >= 0 && strcmp(selesaiPemakaian, bookings[i].selesaiPemakaian) <= 0)) {
            return 0;
        }
    }
    return 1;
}

int formatWaktu(const char *jam) {
    if (strlen(jam) != 5 || jam[2] != ':' ||
        (jam[0] < '0' || jam[0] > '2') ||                  // jam puluhan
        ((jam[0] == '2') && (jam[1] < '0' || jam[1] > '3')) ||  // jam unit (jika puluhan adalah 2)
        (jam[1] < '0' || jam[1] > '9') ||
        (jam[3] < '0' || jam[3] > '5') ||
        (jam[4] < '0' || jam[4] > '9')) {
        return 0;
    }

    return 1;
}

void tampilanBooking(Booking bookings[], int nomerBookings) {
    printf("Jadwal Layanan:\n");
    for (int i = 0; i < nomerBookings; i++) {
        printf("%s - %s: %s\n", bookings[i].waktuPemakaianawal, bookings[i].selesaiPemakaian, bookings[i].layanan);
    }
}


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
    printf("1. 07:00-09:30\n");
    printf("2. 09:30-11:10\n");
    printf("3. 11:10-13:00\n");
    printf("4. 13:00-15:00\n");
    printf("5. 15:30-17:10\n");
    printf("6. Waktu khusus\n");
}

int main() {
    struct User users[MAX_BOOKINGS];
    int userCount = 0;

    while (1) {
        char nim[MAX_NIM_LENGTH];
        char password[MAX_PASSWORD_LENGTH];
        char username[MAX_USERNAME_LENGTH];
        // Login
        login:
        printf("=========================================================================================================\n");
        printf("\t\t\t\t        PROGRAM BOOKING KELAS \n");
        printf("\t\t\t\t  UNIVERSITAS PENDIDIKAN INDONESISA \n");
        printf("\t\t\t\t          KAMPUS PURWAKARTA \n");
        printf("=========================================================================================================\n\n");

        printf("\nLogin:\n");
        printf("Masukkan Username: ");
        scanf("%s", username);
        
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
                sprintf(newUser.nim, "%s", username);
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
            // Display menu
            menu:
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
                            sprintf(newBooking.schedule, "7:00-09:30");
                            break;
                        case 2:
                            sprintf(newBooking.schedule, "9:30-11:10");
                            break;
                        case 3:
                            sprintf(newBooking.schedule, "11:10-13:00");
                            break;
                        case 4:
                            sprintf(newBooking.schedule, "13:00-15:30");
                            break;
                        case 5:
                            sprintf(newBooking.schedule, "15:30-17:10");
                            break;
                        case 6: {
                            printf("Masukkan waktu khusus (contoh: Selasa 10:00-12:00): ");
                            scanf(" %s", newBooking.schedule);
                            break;
                        }
                    }
                    if (users[userIndex].bookingCount < MAX_BOOKINGS) {
                        users[userIndex].bookings[users[userIndex].bookingCount++] = newBooking;
                        printf("Booking berhasil!\n");
                        goto menu;
                    } else {
                        printf("Jumlah booking Anda sudah mencapai batas maksimal.\n");
                    }

                    break;
                    }
                    case 2: {
                        // Lihat jadwal kelas tersedia
                        jadwal:
                        int backToMenu;
                        displaySchedule();
                        displayAvailableSchedules();
                        printf("Apakah ingin kembali ke menu?\n");
                        printf("1. Ya\n");
                        printf("2. Tidak\n");
                        scanf("%d", &backToMenu);
                        if (backToMenu == 1)
                        {
                           goto menu;
                        }
                        else if (backToMenu ==2)
                        {
                            goto jadwal;
                        }
                    }
                    case 3: {
                        // Lihat jadwal pribadi
                        int backToMenu;
                        displayPersonalSchedule(&users[userIndex]);
                        printf("Apakah ingin kembali ke menu?\n");
                        printf("1. Ya\n");
                        printf("2. Tidak\n");
                        scanf("%d", &backToMenu);
                        if (backToMenu == 1)
                        {
                           goto menu;
                        }
                        else if (backToMenu ==2)
                        {
                            goto jadwal;
                        }
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
                                goto menu;
                            } else {
                                printf("Nomor booking tidak valid.\n");
                            }
                        } else {
                            printf("Anda belum melakukan booking.\n");
                            goto menu;
                        }
    
                        break;
                    }
                    case 5: {
                       int Logout;
                        printf("Logout berhasil");
                        printf("Apakah ingin keluar atau login?\n");
                        printf("1. Login\n");
                        printf("2. Exit\n");
                        scanf("%d",&Logout);
                        if (Logout==1)
                        {
                            goto login;
                        }
                        else if(Logout==2){
                            break;
                        }
                    }
                    default: {
                        printf("Pilihan tidak valid. Silakan pilih 1-5.\n");
                        break;
                    }
            }
        }

    return 0;
}


