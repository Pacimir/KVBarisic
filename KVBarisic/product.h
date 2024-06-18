#ifndef PRODUCT_H
#define PRODUCT_H

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
    int instore;
} Product;

void addProduct(Product* product);
void listProducts();
void updateProduct(int id, Product* updatedProduct);
void deleteProduct(int id);
void displayMenu();
void sortProductsByID(Product* products, size_t count);
Product* recursiveProductSearch(FILE* file, int targetID);

#endif // PRODUCT_H
