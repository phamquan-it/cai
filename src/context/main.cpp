#include "context_manager.hpp"

int main() {
    ContextManager cm;
    
    cm.addContext("user", {"Alice", "admin", "alice@email.com"});
    cm.addContext("session", {"12345", "active", "2026-07-11"});
    cm.appendToContext("user", "last_login");
    
    cm.setActiveContext("user");
    
    auto active = cm.getActiveContext();
    if (active) {
        std::cout << "Active context: ";
        for (const auto& val : *active) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    
    cm.printContexts();
    
    return 0;
}
