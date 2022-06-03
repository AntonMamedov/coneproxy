#include <gtest/gtest.h>
#include <array>

extern "C" {
#include "details/strings_hash_table.h"
}

namespace {
    std::array<const char*, 6> sCStrings {"12sadasasd", "sdgfdfdsc56", "kdknkjnbcxzhbhdczjxx<", ",,mndhjls", "aslmkouywey7", "sad32jus"};
}



TEST(StringsHashTableInsertTest, StringsHashTableInsertTest__Test) {
    auto sStringsHashMap = make_strings_hash_table(13, 4);

    for (const char* sStr : sCStrings) {
        string_t key;
        string_init(&key);
        string_copy_c_str(&key, sStr);
        string_t value;
        string_init(&value);
        string_copy_c_str(&value, sStr);
        size_t index = strings_hash_table_insert(&sStringsHashMap, &key, &value);
        ASSERT_EQ(std::string(string_data(&sStringsHashMap.table[index].key)), std::string(sStr));
        ASSERT_EQ(std::string(string_data(&sStringsHashMap.table[index].value)), std::string(sStr));
    }

}


TEST(StringsHashTableSearchTest, StringsHashTableSearchTest__Test) {
    auto sStringsHashMap = make_strings_hash_table(53, 4);

    for (const char* sStr : sCStrings) {
        string_t key;
        string_init(&key);
        string_copy_c_str(&key, sStr);
        string_t value;
        string_init(&value);
        string_copy_c_str(&value, sStr);
        strings_hash_table_insert(&sStringsHashMap, &key, &value);
    }

    for (const char* sStr : sCStrings) {
        string_t key;
        string_init(&key);
        string_copy_c_str(&key, sStr);
        auto val = strings_hash_table_search(&key, &sStringsHashMap);
        ASSERT_EQ(std::string(string_data(val)), std::string(sStr));
    }
}