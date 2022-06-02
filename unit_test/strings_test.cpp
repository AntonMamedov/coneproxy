#include <gtest/gtest.h>

extern "C" {
#include "details/strings.h"
}

TEST(StringCopyTests, StringCopy_CopyLittleString_Test) {
    const char* str = "1234567";
    string_t testStr;
    init_string(&testStr);
    string_copy(&testStr, str);
    ASSERT_EQ(std::string(str), std::string(string_data(&testStr)));
}

TEST(StringCopyTests, StringCopy_CopyBigString_Test) {
    const char* str = "1234567sfdasad<dxxzdx<zxd<zxdzx<dz<xd<z<";
    string_t testStr;
    init_string(&testStr);
    string_copy(&testStr, str);
    ASSERT_EQ(std::string(str), std::string(string_data(&testStr)));
}

TEST(StringCopyTests, StringCopy_CopyBigStringDoubleAllocated_Test) {
    const char* str = "1234567sfdasad<dxxzdx<zxd<zxdzx<dz<xd<z<1234567sfdasad<dxxzdx<zxd<zxdzx<dz<xd<z<1234567sfdasad<dxxzdx<zxd<zxdzx<dz<xd<z<1234567sfdasad<dxxzdx<zxd<zxdzx<dz<xd<z<";
    string_t testStr;
    init_string(&testStr);
    string_copy(&testStr, str);
    ASSERT_EQ(std::string(str), std::string(string_data(&testStr)));
}