#include "catch.hpp"

#include "_version.hpp"
#include "distribution.hpp"

TEST_CASE("uiqt_version", "[uiqt]") {
    REQUIRE(uiqt::ProjectVersion().empty() == false);
}

TEST_CASE("uiqt_distribution", "[uiqt]") {
    const auto is_debug = uiqt::distribution::is_debug();
#ifdef _DEBUG
    REQUIRE(is_debug == true);
#else
    REQUIRE(is_debug == false);
#endif
}
