#include <iostream>
#include "src/lru.hpp"
int main() {
    lru<int> l(2);
    l.put(3);
    l.put(2);
    l.has(3);
    l.put(1);
    return 0;
}
