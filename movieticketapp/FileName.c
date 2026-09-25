#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MOVIES 5
#define MAX_TICKETS 20

typedef struct {
    int id;
    char name[50];
    int available_seats;
    float price;
} Movie;

typedef struct {
    int ticket_id;
    int movie_id;
    char movie_name[50];
    int status;
} Ticket;

typedef struct {
    char username[20];
    char password[20];
} User;


void loadMovies(Movie movies[]) {
    FILE* file;
    if (fopen_s(&file, "movies.txt", "r") == 0) {
        for (int i = 0; i < MAX_MOVIES; i++) {
            
            if (fscanf_s(file, "%d %d %f %[^\n]", &movies[i].id, &movies[i].available_seats, &movies[i].price, movies[i].name, (unsigned)sizeof(movies[i].name)) != 4) {
                break;
            }
        }
        fclose(file);
    }
    else {
        Movie defaults[MAX_MOVIES] = {
            {1, "Dune: Part Two", 45, 150.0},
            {2, "Oppenheimer", 15, 120.0},
            {3, "Barbie", 12, 100.0},
            {4, "The Batman", 30, 110.0},
            {5, "Interstellar (Tekrar)", 5, 130.0}
        };
        for (int i = 0; i < MAX_MOVIES; i++) {
            movies[i] = defaults[i];
        }

        // Ýlk kez oluþturulan filmleri dosyaya kaydet
        if (fopen_s(&file, "movies.txt", "w") == 0) {
            for (int i = 0; i < MAX_MOVIES; i++) {
                fprintf(file, "%d %d %.2f %s\n", movies[i].id, movies[i].available_seats, movies[i].price, movies[i].name);
            }
            fclose(file);
        }
    }
}

void saveMovies(Movie movies[]) {
    FILE* file;
    if (fopen_s(&file, "movies.txt", "w") == 0) {
        for (int i = 0; i < MAX_MOVIES; i++) {
            fprintf(file, "%d %d %.2f %s\n", movies[i].id, movies[i].available_seats, movies[i].price, movies[i].name);
        }
        fclose(file);
    }
}
void loadTickets(Ticket tickets[], int* ticket_counter) {
    FILE* file;
    *ticket_counter = 0;
    if (fopen_s(&file, "tickets.txt", "r") == 0) {
        while (*ticket_counter < MAX_TICKETS &&
            fscanf_s(file, "%d %d %d %[^\n]", &tickets[*ticket_counter].ticket_id,
                &tickets[*ticket_counter].movie_id,
                &tickets[*ticket_counter].status,
                tickets[*ticket_counter].movie_name,
                (unsigned)sizeof(tickets[*ticket_counter].movie_name)) == 4) {
            (*ticket_counter)++;
        }
        fclose(file);
    }
}
void saveTickets(Ticket tickets[], int ticket_counter) {
    FILE* file;
    if (fopen_s(&file, "tickets.txt", "w") == 0) {
        for (int i = 0; i < ticket_counter; i++) {
            fprintf(file, "%d %d %d %s\n", tickets[i].ticket_id, tickets[i].movie_id, tickets[i].status, tickets[i].movie_name);
        }
        fclose(file);
    }
}


void registerUser() {
    User newUser;
    FILE* file;

    printf("\n--- Yeni Kayit ---\n");
    printf("Kullanici Adi belirleyin: ");
    scanf_s("%s", newUser.username, (unsigned)sizeof(newUser.username));
    printf("Sifre belirleyin: ");
    scanf_s("%s", newUser.password, (unsigned)sizeof(newUser.password));

    if (fopen_s(&file, "users.txt", "a") == 0) {
        fprintf(file, "%s %s\n", newUser.username, newUser.password);
        fclose(file);
        printf(">> Kayit basarili! Artik giris yapabilirsiniz.\n");
    }
    else {
        printf(">> HATA: Dosya acilamadi veya olusturulamadi!\n");
    }
}

int loginSystem() {
    char input_user[20], input_pass[20];
    char file_user[20], file_pass[20];
    FILE* file;
    int is_found = 0;

    printf("\n--- Giris Yap ---\n");
    printf("Kullanici Adi: ");
    scanf_s("%s", input_user, (unsigned)sizeof(input_user));
    printf("Sifre: ");
    scanf_s("%s", input_pass, (unsigned)sizeof(input_pass));

    if (fopen_s(&file, "users.txt", "r") == 0) {
        while (fscanf_s(file, "%s %s", file_user, (unsigned)sizeof(file_user), file_pass, (unsigned)sizeof(file_pass)) != EOF) {
            if (strcmp(input_user, file_user) == 0 && strcmp(input_pass, file_pass) == 0) {
                is_found = 1;
                break;
            }
        }
        fclose(file);
    }

    if (is_found) {
        printf(">> Giris basarili! Sisteme yonlendiriliyorsunuz...\n");
        return 1;
    }
    else {
        printf(">> HATA: Kullanici adi veya sifre yanlis!\n");
        return 0;
    }
}


void selectMovie(Movie movies[]) {
    printf("\n--- Vizyondaki Filmler ---\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        if (movies[i].available_seats > 0) {
            printf("[%d] %s | Bos Koltuk: %d | Fiyat: %.2f TL\n",
                movies[i].id, movies[i].name, movies[i].available_seats, movies[i].price);
        }
    }
}

void purchaseTicket(Movie movies[], Ticket tickets[], int* ticket_counter) {
    int selected_id;
    int requested_tickets;

    printf("\nSatin almak istediginiz filmin ID'sini girin: ");
    scanf_s("%d", &selected_id);

    int found = 0;
    for (int i = 0; i < MAX_MOVIES; i++) {
        if (movies[i].id == selected_id) {
            found = 1;

            printf("Kac adet bilet almak istiyorsunuz? (Bos Koltuk: %d): ", movies[i].available_seats);
            scanf_s("%d", &requested_tickets);

            if (requested_tickets > 0 && movies[i].available_seats >= requested_tickets) {
                float total_price = requested_tickets * movies[i].price;
                printf("\n>> Islem Basarili! Toplam Tutar: %.2f TL\n", total_price);

                movies[i].available_seats -= requested_tickets;

                for (int j = 0; j < requested_tickets; j++) {
                    if (*ticket_counter < MAX_TICKETS) {
                        tickets[*ticket_counter].ticket_id = *ticket_counter + 100;
                        tickets[*ticket_counter].movie_id = movies[i].id;
                        strcpy_s(tickets[*ticket_counter].movie_name, sizeof(tickets[*ticket_counter].movie_name), movies[i].name);
                        tickets[*ticket_counter].status = 1;

                        printf("- Bilet Numaraniz: %d (%s)\n", tickets[*ticket_counter].ticket_id, movies[i].name);
                        (*ticket_counter)++;
                    }
                    else {
                        printf(">> HATA: Sistem maksimum bilet kapasitesine ulasti!\n");
                        break;
                    }
                }

              
                saveMovies(movies);
                saveTickets(tickets, *ticket_counter);
            }
            else if (requested_tickets <= 0) {
                printf(">> HATA: Gecerli bir bilet sayisi girmelisiniz (En az 1).\n");
            }
            else {
                printf(">> Maalesef bu film icin yeterli bos koltuk yok. Sadece %d koltuk kaldi.\n", movies[i].available_seats);
            }
            break;
        }
    }

    if (!found) {
        printf(">> HATA: Gecersiz film ID'si girdiniz!\n");
    }
}

void summaryOfTicket(Ticket tickets[], int ticket_counter) {
    printf("\n--- Bilet Ozetiniz ---\n");
    int has_active_ticket = 0;

    for (int i = 0; i < ticket_counter; i++) {
        if (tickets[i].status == 1) {
            printf("- Bilet ID: %d | Film: %s | Durum: Gecerli\n",
                tickets[i].ticket_id, tickets[i].movie_name);
            has_active_ticket = 1;
        }
    }

    if (!has_active_ticket) {
        printf(">> Su an aktif bir biletiniz bulunmamaktadir.\n");
    }
}

void cancelTicket(Movie movies[], Ticket tickets[], int ticket_counter) {
    int cancel_id;
    printf("\nIptal etmek istediginiz Biletin ID'sini girin: ");
    scanf_s("%d", &cancel_id);

    int found = 0;
    for (int i = 0; i < ticket_counter; i++) {
        if (tickets[i].ticket_id == cancel_id && tickets[i].status == 1) {
            tickets[i].status = 0;
            found = 1;

            for (int j = 0; j < MAX_MOVIES; j++) {
                if (movies[j].id == tickets[i].movie_id) {
                    movies[j].available_seats++;
                    break;
                }
            }

           
            saveMovies(movies);
            saveTickets(tickets, ticket_counter);

            printf(">> %d numarali biletiniz basariyla iptal edildi.\n", cancel_id);
            break;
        }
    }

    if (!found) {
        printf(">> HATA: Gecerli veya size ait boyle bir bilet bulunamadi!\n");
    }
}

void logout() {
    printf("\n>> Sistemden cikis yapiliyor... Iyi gunler dileriz!\n");
}

int main() {
    Movie movies[MAX_MOVIES];
    Ticket tickets[MAX_TICKETS];
    int ticket_counter = 0;
    int is_logged_in = 0;
    int choice;
    int start_choice;

    loadMovies(movies);
    loadTickets(tickets, &ticket_counter);

    while (!is_logged_in) {
        printf("\nLutfen bir islem secin:\n");
        printf("1. Giris Yap\n");
        printf("2. Kayit Ol\n");
        printf("3. Misafir Olarak Giris Yap\n");
        printf("4. Sistemi Kapat\n");
        printf("Seciminiz: ");
        scanf_s("%d", &start_choice);

        switch (start_choice) {
        case 1:
            is_logged_in = loginSystem();
            break;
        case 2:
            registerUser();
            break;
        case 3:
            printf(">> Misafir olarak giris yapildi. Hos geldiniz!\n");
            is_logged_in = 1;
            break;
        case 4:
            printf(">> Sistem kapatiliyor. Iyi gunler!\n");
            return 0;
        default:
            printf(">> HATA: Gecersiz secim! Lutfen 1, 2, 3 veya 4 giriniz.\n");
        }
    }

    do {
        printf("\n==== MENU ====\n");
        printf("1. Vizyondaki Filmleri Gor\n");
        printf("2. Bilet Satin Al\n");
        printf("3. Biletlerimi Goruntule\n");
        printf("4. Bilet Iptal Et\n");
        printf("5. Cikis Yap\n");
        printf("Seciminiz: ");
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            selectMovie(movies);
            break;
        case 2:
            selectMovie(movies);
            purchaseTicket(movies, tickets, &ticket_counter);
            break;
        case 3:
            summaryOfTicket(tickets, ticket_counter);
            break;
        case 4:
            cancelTicket(movies, tickets, ticket_counter);
            break;
        case 5:
            logout();
            break;
        default:
            printf(">> HATA: Gecersiz bir secim yaptiniz. Lutfen tekrar deneyin.\n");
        }
    } while (choice != 5);

    return 0;
}