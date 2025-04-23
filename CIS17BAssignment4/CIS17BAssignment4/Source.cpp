#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>

using namespace std;

//Classs for DuplicateItemException
class DuplicateItemException : public std::runtime_error {
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

//Class for ItemNotFoundException
class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

//Class for StoredItem
class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(id), description(desc), location(loc) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

//Class for StorageManger
class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;

public:
    void addItem(const std::shared_ptr<StoredItem>& item) {
        string id;
        string description;
        id = item->getId();
        description = item->getDescription();
        
        if (itemById.find(id) != itemById.end()) {
            throw DuplicateItemException("Item with ID " + id + " already exists");
        }
        
        itemById[id] = item;
        itemByDescription[description] = item;
    }

    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        auto items = itemById.find(id);
        if (items == itemById.end())
        {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }

        return items->second;
    }

    void removeItem(const std::string& id) {
        auto items = itemById.find(id);
        if (items == itemById.end()) {
            throw ItemNotFoundException("Item with ID " + id + " not found!");
        }
        string description = items->second->getDescription();
        itemById.erase(items);
        itemByDescription.erase(description);
    }


    void listItemsByDescription() const {
        cout << "Items in Description Order:" << endl;
        for (const auto& pair : itemByDescription) {
            cout << "- " << pair.second->getDescription() << ": " << pair.second->getLocation() << endl;
        }
    }
};

//Adds duplicate items 
void testDuplicateAddition(StorageManager& position)
{
    cout << "Attempting to add ITEM001 again..." << endl;
    try {
        auto duplicateItem = make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 9, Shelf 9");
        position.addItem(duplicateItem);
    }
    catch (const DuplicateItemException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
//If an item does not exist, throws an error
void testItemNotFound(StorageManager& position)
{
    cout << "removing ITEM003..." << endl;
    try {
        position.removeItem("ITEM003");
    }
    catch (const ItemNotFoundException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

int main() {

    StorageManager position;

    //Creates and add sample items
    cout << "Addint item: ITEM001 - LED Light" << endl;
    auto item1 = make_shared<StoredItem>("ITEM001", "LED Light", "Aisle 3, Shelf 1");
    position.addItem(item1);

    cout << "Adding item: ITEM002 - Fan Motor" << endl;
    auto item2 = make_shared<StoredItem>("ITEM002", "Fan Motor", "Aisle 2, Shelf 5");
    position.addItem(item2);

    //Checks for duplicates that is inputted
    testDuplicateAddition(position);

    //Prints existing items inputted
    cout << "Retrieving ITEM002..." << endl;
    try {
        auto found = position.findById("ITEM002");
        cout << "found: " << found->getDescription() << " at " << found->getLocation() << endl;
    }
    catch (const ItemNotFoundException& e) {
        cout << "Error: " << e.what() << endl;
    }

    //Checks to see if item can be found. Run tests for removing non-existent item
    testItemNotFound(position);

    position.listItemsByDescription();

    return 0;
}
