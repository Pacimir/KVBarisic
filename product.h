#ifndef PRODUCT_H
#define PRODUCT_H

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
    int instore;
} Product;

void displayMenu();
void addProduct(Product* product);
void listProducts();
void updateProduct(int id, Product* updatedProduct);
void deleteProduct(int id);

#endif
