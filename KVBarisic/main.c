#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
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

static int choice;  
static Product cart[100];
static int cartSize = 0;

int main() {
    do {
        printf("--------------------------------------------");
        printf("\n1. Korisnik\n");
        printf("2. Administrator\n");
        printf("3. Exit\n");
        printf("--------------------------------------------");
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  

        switch (choice) {
        case Korisnik:
            userMenu();
            break;
        case Administrator: {
            static char password[50];
            printf("--------------------------------------------");
            getStringInput("\nUnesite administratorsku lozinku: ", password, 1);
            if (strcmp(password, ADMIN_PASSWORD) == 0) {
                adminMenu();
            }
            else {
                printf("--------------------------------------------");
                printf("\nNeispravna lozinka!\n");
            }
            break;
        }
        case Exit:
            printf("\nExiting...\n");
            break;
        default:
            printf("Nepostojeća opcija, unesite ponovno!\n");
        }
    } while (choice != 3);

    return 0;
}

void userMenu() {
    static int choice;

    do {
        printf("--------------------------------------------");
        printf("\n1. Pregledaj Produkte\n");
        printf("2. Dodaj Produkt u Kosaricu\n");
        printf("3. Pregledaj Kosaricu\n");
        printf("4. Povratak na Glavni Meni\n");
        printf("--------------------------------------------");
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  

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
            printf("--------------------------------------------");
            printf("Nepostojeća opcija, unesite ponovno!\n");
            printf("--------------------------------------------");
        }
    } while (choice != 4);
}

void adminMenu() {
    static int choice;
    static Product product;
    static int id;

    do {
        displayMenu();
        printf("--------------------------------------------");
        printf("\nUnesite vas izbor: ");
        scanf("%d", &choice);
        while (getchar() != '\n');  

        switch (choice) {
        case 1:
            printf("--------------------------------------------");
            product.id = getIntInput("\nUnesite ID produkta: ");
            getStringInput("\nUnesite ime produkta: ", product.name, 3);
            product.price = getFloatInput("\nUnesite cijenu produkta: ");
            product.quantity = getIntInput("\nUnesite kolicinu produkta: ");
            product.instore = getIntInput("\nUnesite dostupan broj primjeraka produkta: ");
            printf("--------------------------------------------");
            addProduct(&product);
            break;
        case 2:
            listProducts();
            break;
        case 3:
            printf("--------------------------------------------");
            id = getIntInput("\nUnesite ID produkta kojeg zelite azurirati: ");
            getStringInput("\nUnesite novo ime produkta: ", product.name, 3);
            product.price = getFloatInput("\nUnesite novu cijenu produkta: ");
            product.quantity = getIntInput("\nUnesite novu kolicinu produkta: ");
            product.instore = getIntInput("\nUnesite novu dostupnu kolicinu primjeraka produkta: ");
            printf("--------------------------------------------");
            updateProduct(id, &product);
            break;
        case 4:
            printf("--------------------------------------------");
            id = getIntInput("\nUnesite ID produkta kojeg zelite izbrisati: ");
            printf("--------------------------------------------");
            deleteProduct(id);
            break;
        case 5:
            printf("--------------------------------------------");
            printf("\nExiting...\n");
            break;
        default:
            printf("--------------------------------------------");
            printf("\nNepostojeća opcija, unesite ponovno!\n");
        }
    } while (choice != 5);
}

void addToCart(Product* cart, int* cartSize) {
    printf("--------------------------------------------");
    int id = getIntInput("\nUnesite ID produkta kojeg zelite dodati u kosaricu: ");
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
        printf("--------------------------------------------");
        printf("\nProdukt dodan u kosaricu.\n");
        free(foundProduct);
        foundProduct = NULL;

    }
    else {
        printf("--------------------------------------------");
        printf("\nProdukt s ID-om %d nije pronaden.\n", id);
    }

    fclose(file);
}

Product* recursiveProductSearch(FILE* file, int targetID) {
    Product* product = (Product*)malloc(sizeof(Product));
    if (product == NULL) {
        perror("Greska pri alokaciji memorije");
        exit(EXIT_FAILURE);
    }

    long currentPos = ftell(file); 

    if (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product->id, product->name, &product->price, &product->quantity, &product->instore) == EOF) {
        free(product);
        product = NULL;
        return NULL;
    }

    if (product->id == targetID) {
        return product;
    }
    else {
        Product* result = recursiveProductSearch(file, targetID);
        fseek(file, currentPos, SEEK_SET); 
        free(product); 
        product = NULL;
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
                printf("--------------------------------------------");
                printf("\nUnos nije validan. Molimo unesite broj.\n");
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
            buffer[strcspn(buffer, "\n")] = '\0';  
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