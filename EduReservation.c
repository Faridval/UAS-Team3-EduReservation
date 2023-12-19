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

typedef struct pesan{
    char waktuPemakaianawal[MaksimalPengisian];
    char selesaiPemakaian[MaksimalPengisian];
    char layanan[MaksimalPengisian];
    char room[MAX_BOOKINGS];
}Booking;

struct User {
    char username[MAX_USERNAME_LENGTH];
    char nim[MAX_NIM_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct  pesan bookings[MAX_BOOKINGS];
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



bool isValidNIM(const char nim[]) {
    if (strlen(nim) < 2) {
        return false;
    }

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

void displayPersonalSchedule(const struct User *user) {
    printf("\nJadwal kelas Anda:\n");
    for (int i = 0; i < user->bookingCount; ++i) {
        printf("Ruangan: %s, %s\n", user->bookings[i].room);
    }
}

void bookingKelas(struct User *user) {
    if (user->bookingCount < MAX_BOOKINGS) {
        displaySchedule();
        printf("Pilih ruangan (1-6): ");
        int roomChoice;
        scanf("%d", &roomChoice);

        if (roomChoice < 1 || roomChoice > MAX_ROOMS) {
            printf("Pilihan ruangan tidak valid.\n");
            return;
        }

         int hari, bulan, tahun;

    do {
        printf("Masukkan tanggal (hari bulan tahun) dalam format 'DD MM YYYY': ");
        int hasil = scanf("%d %d %d", &hari, &bulan, &tahun);

        // Mengecek apakah input sesuai dengan format yang benar
        if (hasil != 3 || getchar() != '\n' || !dataTanggal(hari, bulan, tahun)) {
            printf("Format tanggal tidak sesuai. Silakan coba lagi.\n");

            // Membersihkan input buffer
            while (getchar() != '\n');
        } else {
            break; // Format tanggal benar, keluar dari loop
        }
    } while (1);

    printf("Tanggal yang dimasukkan: %02d/%02d/%04d\n", hari, bulan, tahun);

    Booking bookings[JadwalMaksimal];
    int nomerBookings = 0;

    while (1) {
        char masukkanWaktuawal[MaksimalPengisian];
        char masukkanWaktuakhir[MaksimalPengisian];
        char masukkanLayanan[MaksimalPengisian];

        printf("Masukkan waktu awal (HH:MM): ");
        scanf("%s", masukkanWaktuawal);

        if (!formatWaktu(masukkanWaktuawal)) {
            printf("Format waktu tidak sesuai. Silakan masukkan waktu dalam format HH:MM.\n");
            continue;
        }

        printf("Masukkan waktu akhir (HH:MM): ");
        scanf("%s", masukkanWaktuakhir);

        if (!formatWaktu(masukkanWaktuakhir)) {
            printf("Format waktu tidak sesuai. Silakan masukkan waktu dalam format HH:MM.\n");
            continue;
        } else if (strcmp(masukkanWaktuawal, masukkanWaktuakhir) >= 0) {
            printf("Waktu akhir harus setelah waktu awal.\n");
            continue;
        } else if (!waktuYangtersedia(bookings, nomerBookings, masukkanWaktuawal, masukkanWaktuakhir)) {
            printf("Jadwal pada waktu tersebut sudah ter Booking.\n");
            continue;
        }

        printf("Keperluan Pemakaian Kelas : ");
        scanf("%s", masukkanLayanan);

        if (nomerBookings < JadwalMaksimal) {
            strcpy(bookings[nomerBookings].waktuPemakaianawal, masukkanWaktuawal);
            strcpy(bookings[nomerBookings].selesaiPemakaian, masukkanWaktuakhir);
            strcpy(bookings[nomerBookings].layanan, masukkanLayanan);
            nomerBookings++;
        } else {
            printf("Maksimum jadwal layanan telah tercapai.\n");
            break;
        }

        tampilanBooking(bookings, nomerBookings);

        char anotherBooking;
        printf("Lakukan pemesanan layanan lagi? (y/n): ");
        scanf(" %c", &anotherBooking);

        if (anotherBooking != 'y' && anotherBooking != 'Y') {
            break;
        }
    }
    


        struct pesan newBooking;
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
                sprintf(newBooking.room, "MKB 29");
                break;
            case 6:
                sprintf(newBooking.room, "MKB 30");
                break;
        }

        

        user->bookings[user->bookingCount++] = newBooking;
        printf("Booking berhasil!\n");
    } else {
        printf("Jumlah booking Anda sudah mencapai batas maksimal.\n");
    }
}

void displayAvailableSchedules(struct User *user, Booking *booking) {

    printf("|----------------------------------------------------------------|\n");
    printf("| %-10s | %-10s | %-10s | %-10s | %-10s |\n", " Ruang ", " Kelas ", " waktu " , " tanggal ", "Penggunaan");
    printf("|------------|------------|------------|------------|------------|\n");
    
    for (int i = 0; i < user->bookingCount; ++i) {
    printf("| %-10s | %s - %s | %d | %-10s |\n", user->bookings[i].room, user->bookings[i].waktuPemakaianawal, user->bookings[i].selesaiPemakaian, user->bookings[i].layanan );
    printf("|------------|------------|------------|------------|------------|\n");
    }
    
    
}



void pembatalanBooking(struct User *user) {
    if (user->bookingCount > 0) {
        printf("Jadwal kelas yang sudah dibooking:\n");
        displayPersonalSchedule(user);

        printf("Pilih nomor booking yang ingin dibatalkan (1-%d): ", user->bookingCount);
        int cancelChoice;
        scanf("%d", &cancelChoice);

        if (cancelChoice >= 1 && cancelChoice <= user->bookingCount) {
            for (int i = cancelChoice - 1; i < user->bookingCount - 1; ++i) {
                user->bookings[i] = user->bookings[i + 1];
            }
            user->bookingCount--;
            printf("Booking berhasil dibatalkan.\n");
        } else {
            printf("Nomor booking tidak valid.\n");
        }
    } else {
        printf("Anda belum melakukan booking.\n");
    }
}

int logout() {
    printf("Logout berhasil\n");
    return 0; // Kembali ke tahap login
}

int main() {
    struct User users[MAX_BOOKINGS];
    int userCount = 0;

    int loggedIn = 1;
    while (loggedIn) {
        char nim[MAX_NIM_LENGTH];
        char password[MAX_PASSWORD_LENGTH];
        char username[MAX_USERNAME_LENGTH];

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

                printf("Login berhasil! Selamat datang, %s.\n", username);
                userIndex = userCount - 1;
            } else {
                printf("Jumlah pengguna sudah mencapai batas maksimal.\n");
                continue;
            }
        } else if (strcmp(users[userIndex].password, password) != 0) {
            printf("Login gagal. Password salah.\n");
            continue;
        }

        int choice;
        while (1) {
            printf("\nMenu:\n");
            printf("1. Booking kelas\n");
            printf("2. Lihat jadwal kelas tersedia\n");
            printf("3. Lihat jadwal pribadi\n");
            printf("4. Pembatalan booking kelas\n");
            printf("5. Logout\n");
            printf("Pilih menu (1-5): ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    bookingKelas(&users[userIndex]);
                    break;
                case 2:
                    displayAvailableSchedules(&users[userIndex],&users);
                    tampilanBooking;
                    break;
                case 3:
                    displayAvailableSchedules(&users[userIndex],&users);;
                    break;
                case 4:
                    pembatalanBooking(&users[userIndex]);
                    break;
                case 5:
                    loggedIn = logout();
                    break;
                default:
                    printf("Pilihan tidak valid. Silakan pilih 1-5.\n");
                    break;
            }

            if (!loggedIn) {
                break; // Keluar dari loop menu jika sudah logout
            }
        }
    }

    return 0;
}
