#include "../lib/common.hpp"
#include "../lib/MemcachedCluster.hpp"

TEST_CASE("memcached exist") {
  pair<string, MemcachedCluster> tests[]{
    {"bin_mixed", MemcachedCluster::mixed()},
    {"network", MemcachedCluster::network()},
    {"socket", MemcachedCluster::socket()}
  };

  tests[0].second.enableBinaryProto();

  LOOPED_SECTION(tests) {
    auto memc = &test.memc;

    SECTION("initial not found") {
      REQUIRE_RC(MEMCACHED_NOTFOUND,memcached_exist(memc, S("frog")));
    }

    SECTION("set found") {
        REQUIRE_SUCCESS(memcached_set(memc, S("frog"), S("frog"), 0, 0));
        REQUIRE_SUCCESS(memcached_exist(memc, S("frog")));

        SECTION("deleted not found") {
          REQUIRE_SUCCESS(memcached_delete(memc, S("frog"), 0));
          REQUIRE_RC(MEMCACHED_NOTFOUND, memcached_exist(memc, S("frog")));
        }
    }

    SECTION("by key") {
      SECTION("initial not found") {
        REQUIRE_RC(MEMCACHED_NOTFOUND, memcached_exist_by_key(memc, S("master"), S("frog")));
      }

      SECTION("set found") {
        REQUIRE_SUCCESS(memcached_set_by_key(memc, S("master"), S("frog"), S("frog"), 0, 0));
        REQUIRE_SUCCESS(memcached_exist_by_key(memc, S("master"), S("frog")));

        SECTION("deleted not found") {
          REQUIRE_SUCCESS(memcached_delete_by_key(memc, S("master"), S("frog"), 0));
          REQUIRE_RC(MEMCACHED_NOTFOUND, memcached_exist_by_key(memc, S("master"), S("frog")));
        }
      }
    }
  }

}
