/**
 * Reid Harry
 * Computer Project #6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <stdexcept>
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
    void SetQuantity(int q) { quantity = q; }
    string GetDescription() { return description; }
};

class Order
{
private:
    int CID = -1;
    int PID = -1;
    int quantity = -1;
public:
    Order(int cid, int pid, int q) : CID(cid), PID(pid), quantity(q) {}
    ~Order() {}
    int GetCID() { return CID; }
    int GetPID() { return PID; }
    int GetQuantity() { return quantity; }
};

void* producer(void* arg);
void* consumer(void* arg);

vector<Product> inventory;
vector<Order> orders;
int numProducers = 1;
int bufferSize = 10;

int main(int argc, char **argv)
{
    // Get command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'b')
            {
                // Buffer size argument

                // Try to get a number from the following arguemnt
                try 
                {
                    bufferSize = stoi(argv[i+1]);
                    i++;
                    if (bufferSize > 30)
                    {
                        printf("Given buffer size is too large. Setting to max capacity (30).\n");
                        bufferSize = 30;
                    }
                }
                catch (std::invalid_argument& e)
                {
                    printf("-b was not followed by a valid argument! Using default value.\n");
                }
                catch (std::out_of_range& e)
                {
                    printf("-b was not followed by any argument! Using default value.\n");
                }
                catch (std::logic_error& e)
                {
                    printf("-b was not followed by any argument! Using default value.\n");
                }
            }
            else if (argv[i][1] == 'p')
            {
                // Number of producers argument

                // Try to get a number from the following arguemnt
                try 
                {
                    numProducers = stoi(argv[i+1]);
                    i++;
                    if (numProducers > 9)
                    {
                        printf("Given number of producers is too large. Setting to max capacity (9).\n");
                        numProducers = 9;
                    }
                }
                catch (std::invalid_argument& e)
                {
                    printf("-p was not followed by a valid argument! Using default value.\n");
                }
                catch (std::out_of_range& e)
                {
                    printf("-p was not followed by any argument! Using default value.\n");
                }
                catch (std::logic_error& e)
                {
                    printf("-p was not followed by any argument! Using default value.\n");
                }
            }
        }
    }

    // Open inventory.old
    FILE* oldInventory = fopen("inventory.old", "r");
    if (oldInventory == NULL)
    {
        // inventory.old not found
        printf("ERROR: inventory.old was not found in the current directory\n");
        exit(-1);
    }

    // Populate the inventory vector
    int PID = -1;
    float price = -1.0;
    int quantity = -1;
    char description[31] = "";
    while (fscanf(oldInventory, "%d %f %d %[^\n]", 
        &PID, &price, &quantity, description) == 4)
    {
        inventory.push_back(Product(PID, price, quantity, description));
    }

    // Create a producer thread
    pthread_t producer_t;
    if (pthread_create(&producer_t, NULL, producer, NULL))
    {
        printf("Error creating thread.\n");
        exit(-1);
    }
    if (pthread_join(producer_t, NULL))
    {
        printf("Error joining thread.\n");
        exit(-1);
    }

    // Create a consumer thread
    pthread_t consumer_t;
    if (pthread_create(&consumer_t, NULL, consumer, NULL))
    {
        printf("Error creating thread.\n");
        exit(-1);
    }
    if (pthread_join(consumer_t, NULL))
    {
        printf("Error joing thread.\n");
        exit(-1);
    }

    // Create and populate inventory.new
    FILE* newInventory = fopen("inventory.new", "w");
    if (newInventory == NULL)
    {
        // inventory.new creation failure
        printf("ERROR: inventory.new failed to be created\n");
        exit(-1);
    }
    for (auto item : inventory)
    {
        fprintf(newInventory, "%6d %5.2f %5d %s\n", 
            item.GetPID(), item.GetPrice(), 
            item.GetQuantity(), item.GetDescription().c_str());
    }

    // Close inventory.old
    fclose(oldInventory);
}

void* producer(void* arg)
{
    // Open orders
    FILE* ordersFile = fopen("orders", "r");
    if (ordersFile == NULL)
    {
        // orders not found
        printf("ERROR: orders was not found in the current directory\n");
        exit(-1);
    }

    // Populate the orders list
    int CID = -1;
    int PID = -1;
    int quantity = -1;
    while (fscanf(ordersFile, "%d %d %d", 
        &CID, &PID, &quantity) == 3)
    {
        orders.push_back(Order(CID, PID, quantity));
    }

    // Exit thread
    pthread_exit(NULL);
}

void* consumer(void* arg)
{
    // Open log
    FILE* log = fopen("log", "w");
    if (log == NULL)
    {
        // log creation failure
        printf("ERROR: log failed to be created\n");
        exit(-1);
    }

    // Process orders and populate log file
    int foundIndex = -1;
    string result = "Rejected";
    for (auto order : orders)
    {
        result = "Rejected";
        foundIndex = -1;
        // Try to find the PID of the order in inventory
        for (size_t i = 0; i < inventory.size(); i++)
        {
            if (order.GetPID() == inventory[i].GetPID())
            {
                // Found
                foundIndex = i;
                // Check if enough quantity is in inventory
                if (inventory[i].GetQuantity() >= order.GetQuantity())
                {
                    // The order can be fufilled
                    inventory[i].SetQuantity(inventory[i].GetQuantity() - order.GetQuantity());
                    result = "Filled";
                }
                break;
            }
        }
        if (foundIndex != -1)
        {
            fprintf(log, "%07d %06d %30s %5d $%-10.2f %10s\n", 
                    order.GetCID(), order.GetPID(), inventory[foundIndex].GetDescription().c_str(),
                    order.GetQuantity(), order.GetQuantity()*inventory[foundIndex].GetPrice(), result.c_str());
        }
        else
        {
            string errorMessage = "Item not found.";
            fprintf(log, "%07d %06d %30s %5d $%-10.2f %10s\n", 
                    order.GetCID(), order.GetPID(), errorMessage.c_str(),
                    order.GetQuantity(), -1.0, result.c_str());
        }
    }

    // Exit thread
    pthread_exit(NULL);
}
