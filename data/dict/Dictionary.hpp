#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

class Dictionary {
private:
    struct Entry {
        std::string key;
        std::string value;
        
        Entry(const std::string& k = "", const std::string& v = "") 
            : key(k), value(v) {}
    };
    
    std::vector<Entry> entries;
    std::unordered_map<std::string, int> indexMap;
    
public:
    // CREATE
    bool add(const std::string& key, const std::string& value);
    bool addOrUpdate(const std::string& key, const std::string& value);
    
    // READ
    std::string get(const std::string& key) const;
    bool exists(const std::string& key) const;
    int size() const;
    bool isEmpty() const;
    std::vector<Entry> getAll() const;
    
    // UPDATE
    bool update(const std::string& key, const std::string& newValue);
    
    // DELETE
    bool remove(const std::string& key);
    void clear();
    
    // SEARCH
    std::vector<std::string> searchByKey(const std::string& keyword) const;
    std::vector<std::string> searchByValue(const std::string& keyword) const;
    std::vector<Entry> search(const std::string& keyword) const;
    
    // UTILITY
    void printAll() const;
    std::vector<Entry> getEntries() const;
};

#endif // DICTIONARY_HPP
