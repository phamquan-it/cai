#ifndef CONTEXT_MANAGER_HPP
#define CONTEXT_MANAGER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>

class ContextManager {
private:
    std::map<std::string, std::vector<std::string>> contexts;
    std::vector<std::string>* activeContext;
    
public:
    ContextManager() : activeContext(nullptr) {}
    
    void addContext(const std::string& name, const std::vector<std::string>& data) {
        contexts[name] = data;
    }
    
    void appendToContext(const std::string& name, const std::string& value) {
        auto it = contexts.find(name);
        if (it != contexts.end()) {
            it->second.push_back(value);
        } else {
            contexts[name] = {value};
        }
    }
    
    bool setActiveContext(const std::string& name) {
        auto it = contexts.find(name);
        if (it != contexts.end()) {
            activeContext = &(it->second);
            return true;
        }
        activeContext = nullptr;
        return false;
    }
    
    const std::vector<std::string>* getActiveContext() const {
        return activeContext;
    }
    
    const std::vector<std::string>* getContext(const std::string& name) const {
        auto it = contexts.find(name);
        if (it != contexts.end()) {
            return &(it->second);
        }
        return nullptr;
    }
    
    bool removeContext(const std::string& name) {
        auto it = contexts.find(name);
        if (it != contexts.end()) {
            if (activeContext == &(it->second)) {
                activeContext = nullptr;
            }
            contexts.erase(it);
            return true;
        }
        return false;
    }
    
    bool hasContext(const std::string& name) const {
        return contexts.find(name) != contexts.end();
    }
    
    std::vector<std::string> getContextNames() const {
        std::vector<std::string> names;
        for (const auto& pair : contexts) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    void printContexts() const {
        std::cout << "===== Contexts =====" << std::endl;
        for (const auto& pair : contexts) {
            std::cout << "[" << pair.first << "]: ";
            for (const auto& val : pair.second) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Active: " << (activeContext ? "Yes" : "None") << std::endl;
        std::cout << "===================" << std::endl;
    }
};

#endif // CONTEXT_MANAGER_HPP
