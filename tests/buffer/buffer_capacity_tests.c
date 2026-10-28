#include "buffer_tests.h"

#include "utest.h"

BUFFER_FIXTURE_I(vmnl_net_buffer_capacity, types, 16)

UTEST_I(vmnl_net_buffer_capacity, covers_requested_capacity, NTYPES)
{
    EXPECT_GE(vmnl_net_buffer_capacity(utest_fixture->buffer), (size_t) 16);
}

UTEST_I(vmnl_net_buffer_capacity, keeps_value_while_writing_and_reading, NTYPES)
{
    const size_t capacity = vmnl_net_buffer_capacity(utest_fixture->buffer);

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), capacity);
    ASSERT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 5, NULL));
    EXPECT_EQ(vmnl_net_buffer_capacity(utest_fixture->buffer), capacity);
}
