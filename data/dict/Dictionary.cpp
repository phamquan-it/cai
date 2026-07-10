#include "Dictionary.hpp"
#include <iostream>
#include <iomanip>

// ============================================
// CREATE
// ============================================

bool Dictionary::add(const std::string& key, const std::string& value) {
    if (exists(key)) {
        return false;
    }
    
    entries.push_back(Entry(key, value));
    indexMap[key] = entries.size() - 1;
    return true;
}

bool Dictionary::addOrUpdate(const std::string& key, const std::string& value) {
    if (exists(key)) {
        return update(key, value);
    }
    return add(key, value);
}

// ============================================
// READ
// ============================================

std::string Dictionary::get(const std::string& key) const {
    auto it = indexMap.find(key);
    if (it == indexMap.end()) {
        throw std::runtime_error("Key not found: " + key);
    }
    return entries[it->second].value;
}

bool Dictionary::exists(const std::string& key) const {
    return indexMap.find(key) != indexMap.end();
}

int Dictionary::size() const {
    return entries.size();
}

bool Dictionary::isEmpty() const {
    return entries.empty();
}

std::vector<Dictionary::Entry> Dictionary::getAll() const {
    return entries;
}

// ============================================
// UPDATE
// ============================================

bool Dictionary::update(const std::string& key, const std::string& newValue) {
    auto it = indexMap.find(key);
    if (it == indexMap.end()) {
        return false;
    }
    entries[it->second].value = newValue;
    return true;
}

// ============================================
// DELETE
// ============================================

bool Dictionary::remove(const std::string& key) {
    auto it = indexMap.find(key);
    if (it == indexMap.end()) {
        return false;
    }
    
    int position = it->second;
    entries.erase(entries.begin() + position);
    
    indexMap.clear();
    for (int i = 0; i < entries.size(); i++) {
        indexMap[entries[i].key] = i;
    }
    
    return true;
}

void Dictionary::clear() {
    entries.clear();
    indexMap.clear();
}

// ============================================
// SEARCH
// ============================================

std::vector<std::string> Dictionary::searchByKey(const std::string& keyword) const {
    std::vector<std::string> results;
    
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), 
                   lowerKeyword.begin(), ::tolower);
    
    for (const auto& entry : entries) {
        std::string lowerKey = entry.key;
        std::transform(lowerKey.begin(), lowerKey.end(), 
                       lowerKey.begin(), ::tolower);
        
        if (lowerKey.find(lowerKeyword) != std::string::npos) {
            results.push_back(entry.key);
        }
    }
    return results;
}

std::vector<std::string> Dictionary::searchByValue(const std::string& keyword) const {
    std::vector<std::string> results;
    
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), 
                   lowerKeyword.begin(), ::tolower);
    
    for (const auto& entry : entries) {
        std::string lowerValue = entry.value;
        std::transform(lowerValue.begin(), lowerValue.end(), 
                       lowerValue.begin(), ::tolower);
        
        if (lowerValue.find(lowerKeyword) != std::string::npos) {
            results.push_back(entry.key);
        }
    }
    return results;
}

std::vector<Dictionary::Entry> Dictionary::search(const std::string& keyword) const {
    std::vector<Entry> results;
    
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), 
                   lowerKeyword.begin(), ::tolower);
    
    for (const auto& entry : entries) {
        std::string lowerKey = entry.key;
        std::string lowerValue = entry.value;
        std::transform(lowerKey.begin(), lowerKey.end(), 
                       lowerKey.begin(), ::tolower);
        std::transform(lowerValue.begin(), lowerValue.end(), 
                       lowerValue.begin(), ::tolower);
        
        if (lowerKey.find(lowerKeyword) != std::string::npos ||
            lowerValue.find(lowerKeyword) != std::string::npos) {
            results.push_back(entry);
        }
    }
    return results;
}

// ============================================
// UTILITY
// ============================================

void Dictionary::printAll() const {
    if (entries.empty()) {
        std::cout << "Dictionary is empty!" << std::endl;
        return;
    }
    
    std::cout << "\n========== DICTIONARY ==========" << std::endl;
    std::cout << std::left << std::setw(20) << "KEY" 
              << std::setw(30) << "VALUE" << std::endl;
    std::cout << "---------------------------------" << std::endl;
    
    for (const auto& entry : entries) {
        std::cout << std::left << std::setw(20) << entry.key
                  << std::setw(30) << entry.value << std::endl;
    }
    std::cout << "=================================" << std::endl;
    std::cout << "Total: " << entries.size() << " entries" << std::endl;
}

std::vector<Dictionary::Entry> Dictionary::getEntries() const {
    return entries;
}
