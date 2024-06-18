#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Dodano za EXIT_FAILURE
#include "product.h"

#define ADMIN_PASSWORD "patrikb123"

extern int getIntInput(const char* prompt);
extern float getFloatInput(const char* prompt);
extern void getStringInput(const char* prompt, char* input, int minLen);
extern void userMenu();
extern void adminMenu();
extern void addToCart(Product* cart, int* cartSize);
extern void viewCart(Product* cart, int cartSize);
extern Product* recursiveProductSearch(FILE* file, int targetID);

static int choice;  // Globalna varijabla označena kao static
static Product cart[100];
static int cartSize = 0;

int main() {
    do {
        printf("\n1. Korisnik\n");
        printf("2. Administrator\n");
        printf("3. Exit\n");
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  // Oèisti ulazni meðuspremnik

        switch (choice) {
        case 1:
            userMenu();
            break;
        case 2: {
            static char password[50];
            getStringInput("Unesite administratorsku lozinku: ", password, 1);
            if (strcmp(password, ADMIN_PASSWORD) == 0) {
                adminMenu();
            }
            else {
                printf("Neispravna lozinka!\n");
            }
            break;
        }
        case 3:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

void userMenu() {
    static int choice;

    do {
        printf("\n1. Pregledaj Produkte\n");
        printf("2. Dodaj Produkt u Kosaricu\n");
        printf("3. Pregledaj Kosaricu\n");
        printf("4. Povratak na Glavni Meni\n");
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  // Oèisti ulazni meðuspremnik

        switch (choice) {
        case 1:
            listProducts();
            break;
        case 2:
            addToCart(cart, &cartSize);
            break;
        case 3:
            viewCart(cart, cartSize);
            break;
        case 4:
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);
}

void adminMenu() {
    static int choice;
    static Product product;
    static int id;

    do {
        displayMenu();
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  // Oèisti ulazni meðuspremnik

        switch (choice) {
        case 1:
            product.id = getIntInput("Unesite ID produkta: ");
            getStringInput("Unesite ime produkta: ", product.name, 3);
            product.price = getFloatInput("Unesite cijenu produkta: ");
            product.quantity = getIntInput("Unesite kolicinu produkta: ");
            product.instore = getIntInput("Unesite dostupan broj primjeraka produkta: ");
            addProduct(&product);
            break;
        case 2:
            listProducts();
            break;
        case 3:
            id = getIntInput("Unesite ID produkta kojeg zelite azurirati: ");
            getStringInput("Unesite novo ime produkta: ", product.name, 3);
            product.price = getFloatInput("Unesite novu cijenu produkta: ");
            product.quantity = getIntInput("Unesite novu kolicinu produkta: ");
            product.instore = getIntInput("Unesite novu dostupnu kolicinu primjeraka produkta: ");
            updateProduct(id, &product);
            break;
        case 4:
            id = getIntInput("Unesite ID produkta kojeg zelite izbrisati: ");
            deleteProduct(id);
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);
}

void addToCart(Product* cart, int* cartSize) {
    static int id = getIntInput("Unesite ID produkta kojeg zelite dodati u kosaricu: ");
    static Product product;
    FILE* file = fopen("products.txt", "r");

    if (file == NULL) {
        perror("Greska pri otvaranju datoteke za citanje");
        exit(EXIT_FAILURE);
    }

    Product* foundProduct = recursiveProductSearch(file, id);

    if (foundProduct != NULL) {
        cart[*cartSize] = *foundProduct;
        (*cartSize)++;
        printf("Produkt dodan u kosaricu.\n");
        free(foundProduct); // Sigurno brisanje memorije
    }
    else {
        printf("Produkt s ID-om %d nije pronaden.\n", id);
    }

    fclose(file);
}

Product* recursiveProductSearch(FILE* file, int targetID) {
    Product* product = (Product*)malloc(sizeof(Product));
    if (product == NULL) {
        perror("Greska pri alokaciji memorije");
        exit(EXIT_FAILURE);
    }

    long currentPos = ftell(file); // Spremljena trenutna pozicija u datoteci

    if (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product->id, product->name, &product->price, &product->quantity, &product->instore) == EOF) {
        free(product);
        return NULL;
    }

    if (product->id == targetID) {
        return product;
    }
    else {
        Product* result = recursiveProductSearch(file, targetID);
        fseek(file, currentPos, SEEK_SET); // Vraćanje na prethodnu poziciju u datoteci
        free(product); // Sigurno brisanje memorije
        return result;
    }
}

void viewCart(Product* cart, int cartSize) {
    static float total = 0;
    printf("\nKosarica:\n");
    for (int i = 0; i < cartSize; i++) {
        printf("ID: %d, Ime: %s, Cijena: %.2f Eur\n", cart[i].id, cart[i].name, cart[i].price);
        total += cart[i].price;
    }
    printf("Ukupna cijena: %.2f Eur\n", total);
}

inline int getIntInput(const char* prompt) {
    static int value;
    static char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                return value;
            }
            else {
                printf("Unos nije validan. Molimo unesite broj.\n");
            }
        }
    }
}

inline float getFloatInput(const char* prompt) {
    static float value;
    static char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%f", &value) == 1) {
                return value;
            }
            else {
                printf("Unos nije validan. Molimo unesite broj.\n");
            }
        }
    }
}

inline void getStringInput(const char* prompt, char* input, int minLen) {
    static char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';  // Ukloni novi red na kraju
            if (strlen(buffer) >= minLen && strspn(buffer, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") >= minLen) {
                strcpy(input, buffer);
                return;
            }
            else {
                printf("Unos nije validan. Molimo unesite barem %d slova.\n", minLen);
            }
        }
    }
}
