#include <gtest/gtest.h>
#include <array>

extern "C" {
#include "details/string_hash_map.h"
}

namespace {
    std::array<const char*, 6> sCStrings {"12sadasasd", "sdgfdfdsc56", "kdknkjnbcxzhbhdczjxx<", ",,mndhjls", "aslmkouywey7", "sad32jus"};
}



TEST(StringsHashTableMapTest, StringsHashMapTest_Insert_Seatch_Test) {
    string_hash_map_t sTestMap;

    string_hash_map_init(&sTestMap, 53, 4);

    for (const char* sStr : sCStrings) {
        size_t index = string_hash_map_insert(&sTestMap, sStr, sStr);
        ASSERT_EQ(std::string(string_data(&sTestMap.map[index].key)), std::string(sStr));
        ASSERT_EQ(std::string(string_data(&sTestMap.map[index].value)), std::string(sStr));
    }

    for (const char* sStr : sCStrings) {
        const char* str = string_data(string_hash_map_search(&sTestMap, sStr));
        ASSERT_EQ(std::string(str), std::string(sStr));
    }
}