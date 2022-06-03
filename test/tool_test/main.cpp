#include <iostream>
#include <chrono>
#include <array>
#include <cstring>
#include <map>
extern "C" {
#include "details/strings.h"
}

std::array<string_t, 6> sStrings = {string_new("test1"), string_new("test1"), string_new("test22-0test22-0"), string_new("test22-0test22-0"), string_new("sadsadsasdasdasdasasdsadasdsadsadasasdasdsadas"), string_new("sadsadsasdasdasdasasdsadasdsadsadasasdasdsadas")};

static size_t hash_impl(const string_t* str, size_t table_size, size_t key ) {
    unsigned long hash_result = 0;
    const size_t * num_str = (size_t*)string_data(str);
    for (size_t i = 0; i < str->len / sizeof(size_t) + (str->len % sizeof(size_t) > 0 ? 1 : 0); i++)
        hash_result = (key * hash_result + num_str[i]);
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

static size_t hash_impl2(const string_t* str, size_t table_size, size_t key ) {
    unsigned long hash_result = 0;
    auto c_str = string_data(str);
    for (size_t i = 0; i < str->len; i++)
        hash_result = (key * hash_result + c_str[i]);
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

int main() {
    using namespace std;
    using namespace std::chrono;

    {
        steady_clock::time_point start = steady_clock::now();
        for (size_t i = 0; i < 1000000000; i++) {
            for (size_t j = 0; j < 6; j++) {
                string_is_eq(&sStrings[i % 6], &sStrings[j]);
            }
        }
        steady_clock::time_point end = steady_clock::now();
        steady_clock::duration dur = end - start;
        cout << "size_t cmp: " << duration<double, nano>(dur).count() << " nano" << endl;
        cout << "size_t cmp: " << duration<double, milli>(dur).count() << " ms" << endl;
    }

    {
        steady_clock::time_point start = steady_clock::now();
        for (size_t i = 0; i < 1000000000; i++) {
            for (size_t j = 0; j < 6; j++) {
                string_is_eq2(&sStrings[i % 6], &sStrings[j]);
            }
        }
        steady_clock::time_point end = steady_clock::now();
        steady_clock::duration dur = end - start;
        cout << "default cmp: " << duration<double, nano>(dur).count() << " nano" << endl;
        cout << "default cmp: " << duration<double, milli>(dur).count() << " ms" << endl;
    }

//    {
//        steady_clock::time_point start = steady_clock::now();
//        for (size_t i = 0; i < 1000000000; i++) {
//            hash_impl(&sStrings[5], 13, 12);
//        }
//
//        steady_clock::time_point end = steady_clock::now();
//        steady_clock::duration dur = end - start;
//        cout << "size hash: " << duration<double, nano>(dur).count() << " nano" << endl;
//        cout << "size hash: " << duration<double, milli>(dur).count() << " ms" << endl;
//    }
//
//    {
//        steady_clock::time_point start = steady_clock::now();
//        for (size_t i = 0; i < 1000000000; i++) {
//            hash_impl2(&sStrings[5], 13, 12);
//        }
//
//        steady_clock::time_point end = steady_clock::now();
//        steady_clock::duration dur = end - start;
//        cout << "size hash: " << duration<double, nano>(dur).count() << " nano" << endl;
//        cout << "size hash: " << duration<double, milli>(dur).count() << " ms" << endl;
//    }


    return 0;
}