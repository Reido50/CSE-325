/**
 * Reid Harry
 * Computer Project #5 
*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
using namespace std;

class Product
{
private:
    int PID = -1;
    float price = -1.0;
    int quantity = -1;
    string description = "";
public:
    Product(int id, float p, int q, char* d) : PID(id), price(p), 
        quantity(q), description(d) {}
    ~Product() {}
    int GetPID() { return PID; }
    float GetPrice() { return price; }
    int GetQuantity() { return quantity; }
    string GetDescription() { return description; }
};

int main(int argc, char **argv)
{
    list<Product> inventory;

    // Open inventory.old
    FILE* oldInventory = fopen("inventory.old", "r");
    if(oldInventory == NULL)
    {
        // inventory.old not found
        printf("ERROR: inventory.old was not found in the current directory");
        return -1;
    }

    // Populate the inventory list
    int PID = -1;
    float price = -1.0;
    int quantity = -1;
    char description[31] = "";
    while (fscanf(oldInventory, "%d %f %d %[^\n]", 
        &PID, &price, &quantity, description) == 4)
    {
        inventory.push_back(Product(PID, price, quantity, description));
    }

    

    // Close inventory.old
    fclose(oldInventory);
}