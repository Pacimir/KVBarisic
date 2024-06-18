#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "product.h"

void addProduct(Product* product) {
    FILE* file = fopen("products.txt", "a");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke za pridodavanje");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d,%s,%.2f,%d,%d\n", product->id, product->name, product->price, product->quantity, product->instore);
    fclose(file);
}

void listProducts() {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        perror("Greska pri otvaranju datoteke za citanje");
        exit(EXIT_FAILURE);
    }
    static Product product;
    printf("\nLista Produkta:\n");
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        printf("ID: %d, Ime: %s, Cijena: %.2f, Kolicina: %d, Na skladištu: %d\n", product.id, product.name, product.price, product.quantity, product.instore);
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
    static Product product;
    static int found = 0;
    while (fscanf(file, "%d,%49[^,],%f,%d,%d\n", &product.id, product.name, &product.price, &product.quantity, &product.instore) != EOF) {
        if (product.id == id) {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", updatedProduct->id, updatedProduct->name, updatedProduct->price, updatedProduct->quantity, updatedProduct->instore);
            found = 1;
        }
        else {
            fprintf(tempFile, "%d,%s,%.2f,%d,%d\n", product.id, product.name, product.price, product.quantity, product.instore);
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("products.txt");
    rename("temp.txt", "products.txt");
    if (!found) {
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
    static Product product;
    static int found = 0;
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
    remove("products.txt");
    rename("temp.txt", "products.txt");
    if (!found) {
        printf("Produkt s ID-om %d nije pronaden.\n", id);
    }
}

void displayMenu() {
    printf("\n1. Dodaj Produkt\n");
    printf("2. Lista Produkta\n");
    printf("3. Azuriraj Produkt\n");
    printf("4. Izbrisi Produkt\n");
    printf("5. Exit\n");
}

static int compareProductsByID(const void* a, const void* b) {
    const Product* productA = (const Product*)a;
    const Product* productB = (const Product*)b;
    return productA->id - productB->id;
}

static void sortProductsByID(Product* products, size_t count) {
    qsort(products, count, sizeof(Product), compareProductsByID);
}
