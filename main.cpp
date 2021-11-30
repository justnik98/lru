#include <iostream>
#include "src/lru.hpp"
int main() {
    lru<int> l(2);
    l.put(3);
    return 0;
}
