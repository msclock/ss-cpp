#include "gtest/gtest.h"

#include "_version.hpp"
#include "distribution.hpp"

TEST(uiqt, version) {
    const auto* const version = git_ProjectVersion();
    EXPECT_STRNE(version, "");
}

TEST(uiqt, distribution) {
    const auto is_debug = uiqt::distribution::is_debug();
#ifdef _DEBUG
    EXPECT_TRUE(is_debug);
#else
    EXPECT_FALSE(is_debug);
#endif
}
