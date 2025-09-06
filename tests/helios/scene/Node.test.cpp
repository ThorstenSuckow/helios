#include <gtest/gtest.h>


import helios.scene.Node;

namespace scene = helios::scene;

struct test_data {

};

static test_data setup() {
    return test_data{};
}


TEST(NodeTest, Constructor) {

    // init data
    test_data data = setup();
    const auto n = scene::Node();
    const auto guid = n.guid();
    EXPECT_GT(guid.value(), 0);
    /**
     * @todo set_mesh
     */
    EXPECT_NE(n.mesh(), nullptr);

}


