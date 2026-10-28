#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_is_empty, types, 16)

UTEST_I(vmnl_net_buffer_is_empty, returns_true_when_new, NTYPES)
{
    EXPECT_TRUE(vmnl_net_buffer_is_empty(utest_fixture->buffer));
}

UTEST_I(vmnl_net_buffer_is_empty, returns_false_with_stored_bytes, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_FALSE(vmnl_net_buffer_is_empty(utest_fixture->buffer));
}

UTEST_I(vmnl_net_buffer_is_empty, returns_true_after_consuming_every_byte, NTYPES)
{
    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    EXPECT_TRUE(vmnl_net_buffer_is_empty(utest_fixture->buffer));
}
