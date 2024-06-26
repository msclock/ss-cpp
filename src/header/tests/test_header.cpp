#include <gtest/gtest.h>
#include "header/header.hpp"

TEST(header, version) {
    const auto version = git_ProjectVersion();
    EXPECT_STRNE(version, "");
}

TEST(header, distribution) {
    const auto is_debug = header::distribution::is_debug();
#ifdef _DEBUG
    EXPECT_TRUE(is_debug);
#else
    EXPECT_FALSE(is_debug);
#endif
}
