#include "Dictionary.hpp"
#include <iostream>

int main() {
    Dictionary dict;
    
    std::cout << "=== CREATE ===" << std::endl;
    dict.add("apple", "A red or green fruit");
    dict.add("book", "A set of written pages");
    dict.add("computer", "An electronic device");
    dict.add("dog", "A loyal animal");
    dict.add("elephant", "A large animal with trunk");
    dict.add("flower", "A colorful plant");
    dict.add("garden", "A place with plants");
    
    dict.printAll();
    
    if (!dict.add("apple", "Fruit")) {
        std::cout << "\n❌ Cannot add duplicate key: 'apple'" << std::endl;
    }
    
    std::cout << "\n=== READ ===" << std::endl;
    try {
        std::cout << "Get 'book': " << dict.get("book") << std::endl;
        std::cout << "Exists 'dog': " << (dict.exists("dog") ? "Yes" : "No") << std::endl;
        std::cout << "Exists 'cat': " << (dict.exists("cat") ? "Yes" : "No") << std::endl;
        std::cout << "Dictionary size: " << dict.size() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== UPDATE ===" << std::endl;
    dict.update("book", "A collection of written pages bound together");
    dict.update("apple", "A sweet edible fruit (red, green, or yellow)");
    std::cout << "After update:" << std::endl;
    std::cout << "  book: " << dict.get("book") << std::endl;
    std::cout << "  apple: " << dict.get("apple") << std::endl;
    
    std::cout << "\n=== SEARCH ===" << std::endl;
    auto keyResults = dict.searchByKey("ap");
    std::cout << "Search by key 'ap': ";
    for (const auto& k : keyResults) std::cout << k << " ";
    std::cout << std::endl;
    
    auto valueResults = dict.searchByValue("animal");
    std::cout << "Search by value 'animal': ";
    for (const auto& k : valueResults) std::cout << k << " ";
    std::cout << std::endl;
    
    auto results = dict.search("plant");
    std::cout << "Search 'plant' (key or value):" << std::endl;
    for (const auto& entry : results) {
        std::cout << "  " << entry.key << ": " << entry.value << std::endl;
    }
    
    std::cout << "\n=== DELETE ===" << std::endl;
    dict.remove("flower");
    dict.remove("garden");
    std::cout << "After removing 'flower' and 'garden':" << std::endl;
    dict.printAll();
    
    std::cout << "\n=== ADD OR UPDATE ===" << std::endl;
    dict.addOrUpdate("dog", "A friendly canine companion");
    dict.addOrUpdate("fish", "An aquatic animal");
    std::cout << "After addOrUpdate:" << std::endl;
    std::cout << "  dog: " << dict.get("dog") << std::endl;
    std::cout << "  fish: " << dict.get("fish") << std::endl;
    
    std::cout << "\n=== CLEAR ===" << std::endl;
    dict.clear();
    std::cout << "After clear, size: " << dict.size() << std::endl;
    dict.printAll();
    
    return 0;
}
