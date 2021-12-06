#include <iostream>
#include <chrono>
#include "src/lru.hpp"
#include "src/lru_base.hpp"

using namespace std;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

int main() {
    auto num = 1000000000;
    lru_base<int> l1(num);
    lru<int> l2(num);

    cout << "insert into not full container" << endl;
    auto start = std::chrono::system_clock::now();
    for (auto i = 0; i < num; ++i) {
        l1.put(num);
    }
    auto finish = std::chrono::system_clock::now();

    cout << std::chrono::duration_cast<milliseconds>(finish - start).count() << endl;
    start = std::chrono::system_clock::now();
    for (auto i = 0; i < num; ++i) {
        l2.put(num);
    }
    finish = std::chrono::system_clock::now();
    cout << std::chrono::duration_cast<milliseconds>(finish - start).count() << endl;

    cout << "insert into full container" << endl;
    start = std::chrono::system_clock::now();
    for (auto i = num; i < num*2; ++i) {
        l1.put(num);
    }
    finish = std::chrono::system_clock::now();

    cout << std::chrono::duration_cast<milliseconds>(finish - start).count() << endl;
    start = std::chrono::system_clock::now();
    for (auto i = num; i < num*2; ++i) {
        l2.put(num);
    }
    finish = std::chrono::system_clock::now();
    cout << std::chrono::duration_cast<milliseconds>(finish - start).count() << endl;

    cout << "update full container" << endl;
    start = std::chrono::system_clock::now();
    for (auto i = 0; i < num; ++i) {
        l1.has(num);
    }
    finish = std::chrono::system_clock::now();

    cout << std::chrono::duration_cast<milliseconds>(finish - start).count() << endl;
    start = std::chrono::system_clock::now();
    for (auto i = 0; i < num; ++i) {
        l2.has(num);
    }
    finish = std::chrono::system_clock::now();
    cout << std::chrono::duration_cast<milliseconds>(finish - start).count() << endl;
    return 0;
}
