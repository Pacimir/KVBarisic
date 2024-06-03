#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "product.h"

#define ADMIN_PASSWORD "patrikb123"

int getIntInput(const char* prompt);
float getFloatInput(const char* prompt);
void getStringInput(const char* prompt, char* input, int minLen);
void userMenu();
void adminMenu();
void addToCart(Product** cart, int* cartSize);
void viewCart(Product* cart, int cartSize);
void checkout(Product** cart, int* cartSize);
int compareProductById(const void* a, const void* b);

int main() {
    int choice;

    do {
        printf("\nDobrodosli u Patrikov webshop!\n");
        printf("Odaberite vasu poziciju\n");
        printf("\n1. Korisnik\n");
        printf("2. Administrator\n");
        printf("3. Exit\n");
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  // Očisti ulazni međuspremnik

        switch (choice) {
        case 1:
            userMenu();
            break;

        case 2: {
            char password[50];
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
    int choice;
    Product* cart = NULL;
    int cartSize = 0;

    do {
        printf("\n1. Pregledaj Produkte\n");
        printf("2. Dodaj Produkt u Kosaricu\n");
        printf("3. Pregledaj Kosaricu\n");
        printf("4. Kupi\n");
        printf("5. Povratak na Glavni Meni\n");
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  // Očisti ulazni međuspremnik

        switch (choice) {
        case 1:
            listProducts();
            break;

        case 2:
            addToCart(&cart, &cartSize);
            break;

        case 3:
            viewCart(cart, cartSize);
            break;

        case 4:
            checkout(&cart, &cartSize);
            break;

        case 5:
            free(cart);
            break;

        default:
            printf("Nije dobar izbor, molimo pokusajte ponovno.\n");
        }
    } while (choice != 5);
}

void adminMenu() {
    int choice;
    Product product;
    int id;

    do {
        displayMenu();
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  // Očisti ulazni međuspremnik

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

void addToCart(Product** cart, int* cartSize) {
    int id = getIntInput("Unesite ID produkta kojeg zelite dodati u kosaricu: ");
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke za citanje");
        exit(EXIT_FAILURE);
    }

    Product products[100];
    int count = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &products[count].id, products[count].name, &products[count].price, &products[count].quantity, &products[count].instore) != EOF) {
        count++;
    }
    fclose(file);

    Product* foundProduct = (Product*)bsearch(&id, products, count, sizeof(Product), compareProductById);
    if (foundProduct != NULL && foundProduct->instore > 0) {
        *cart = realloc(*cart, (*cartSize + 1) * sizeof(Product));
        (*cart)[*cartSize] = *foundProduct;
        (*cartSize)++;
        foundProduct->instore--;
        printf("Produkt dodan u kosaricu.\n");

        // Ažuriranje datoteke s novom količinom
        file = fopen("products.txt", "w");
        for (int i = 0; i < count; i++) {
            fprintf(file, "%d,%s,%.2f,%d,%d\n", products[i].id, products[i].name, products[i].price, products[i].quantity, products[i].instore);
        }
        fclose(file);
    }
    else {
        printf("Produkt s ID-om %d nije pronaden ili nije dostupan.\n", id);
    }
}

void viewCart(Product* cart, int cartSize) {
    float total = 0;
    printf("\nKosarica:\n");
    for (int i = 0; i < cartSize; i++) {
        printf("ID: %d, Ime: %s, Cijena: %.2f Eur\n", cart[i].id, cart[i].name, cart[i].price);
        total += cart[i].price;
    }
    printf("Ukupna cijena: %.2f Eur\n", total);
}

void checkout(Product** cart, int* cartSize) {
    float total = 0;
    printf("\nRacun:\n");
    for (int i = 0; i < *cartSize; i++) {
        printf("ID: %d, Ime: %s, Cijena: %.2f Eur\n", (*cart)[i].id, (*cart)[i].name, (*cart)[i].price);
        total += (*cart)[i].price;
    }
    printf("Ukupna cijena: %.2f Eur\n", total);

    // Ažuriranje dostupnih količina u datoteci
    FILE* file = fopen("products.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Greska pri otvaranju datoteke");
        exit(EXIT_FAILURE);
    }

    Product product;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        for (int i = 0; i < *cartSize; i++) {
            if (product.id == (*cart)[i].id) {
                product.instore--;
            }
        }
        fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", product.id, product.name, product.price, product.quantity, product.instore);
    }

    fclose(file);
    fclose(tempFile);
    remove("products.txt");
    rename("temp.txt", "products.txt");

    // Resetiranje kosarice nakon kupnje
    free(*cart);
    *cart = NULL;
    *cartSize = 0;
    printf("Kupnja uspjesna! Hvala vam na kupnji.\n");
}

int getIntInput(const char* prompt) {
    int value;
    char buffer[100];
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

float getFloatInput(const char* prompt) {
    float value;
    char buffer[100];
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

void getStringInput(const char* prompt, char* input, int minLen) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
                len--;
            }
            if (len >= minLen) {
                strcpy(input, buffer);
                return;
            }
            else {
                printf("Unos mora imati najmanje %d znakova.\n", minLen);
            }
        }
    }
}

int compareProductById(const void* a, const void* b) {
    int idA = ((Product*)a)->id;
    int idB = ((Product*)b)->id;
    return (idA - idB);
}
