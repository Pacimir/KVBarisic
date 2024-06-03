#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "product.h"

void displayMenu() {
    printf("\n1. Dodaj Produkt\n");
    printf("2. Prikazi Produkte\n");
    printf("3. Azuriraj Produkt\n");
    printf("4. Izbrisi Produkt\n");
    printf("5. Izlaz\n");
}

void addProduct(Product* product) {
    FILE* file = fopen("products.txt", "a");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke za pisanje");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d,%s,%.2f,%d,%d\n", product->id, product->name, product->price, product->quantity, product->instore);
    fclose(file);
    printf("Produkt uspjesno dodan.\n");
}

void listProducts() {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke za citanje");
        exit(EXIT_FAILURE);
    }
    printf("\nLista Produkta:\n");
    Product product;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        printf("ID: %d, Ime: %s, Cijena: %.2f Eur, Kolicina: %d, Na skladistu: %d\n", product.id, product.name, product.price, product.quantity, product.instore);
    }
    fclose(file);
}

void updateProduct(int id, Product* updatedProduct) {
    FILE* file = fopen("products.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Greska pri otvaranju datoteke");
        exit(EXIT_FAILURE);
    }

    Product product;
    int found = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        if (product.id == id) {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", id, updatedProduct->name, updatedProduct->price, updatedProduct->quantity, updatedProduct->instore);
            found = 1;
        }
        else {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", product.id, product.name, product.price, product.quantity, product.instore);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("products.txt");
        rename("temp.txt", "products.txt");
        printf("Produkt uspjesno azuriran.\n");
    }
    else {
        remove("temp.txt");
        printf("Produkt s ID-om %d nije pronaden.\n", id);
    }
}

void deleteProduct(int id) {
    FILE* file = fopen("products.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Greska pri otvaranju datoteke");
        exit(EXIT_FAILURE);
    }

    Product product;
    int found = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        if (product.id != id) {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", product.id, product.name, product.price, product.quantity, product.instore);
        }
        else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("products.txt");
        rename("temp.txt", "products.txt");
        printf("Produkt uspjesno izbrisan.\n");
    }
    else {
        remove("temp.txt");
        printf("Produkt s ID-om %d nije pronaden.\n", id);
    }
}
