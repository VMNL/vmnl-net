/**
 * \file address_benchmarks.hpp
 * \brief Addresses shared by the address benchmarks.
 */

#ifndef VMNL_NET_ADDRESS_BENCHMARKS_HPP
#define VMNL_NET_ADDRESS_BENCHMARKS_HPP

#include <vmnl/net/address.h>

#include <benchmark/benchmark.h>

/**
 * \brief Makes the address of a benchmark from the text of an IP address.
 * \param state State of the benchmark, skipped with an error if the text is not an IP address.
 * \param ip_address Text of the IP address.
 * \return Address made with port 8080, zeroed if the text is not an IP address.
 */
static inline VmnlNetAddress make(benchmark::State &state, const char *ip_address)
{
    VmnlNetAddress address = {};

    if (vmnl_net_address_make(ip_address, 8080, &address, NULL) == false) {
        state.SkipWithError("vmnl_net_address_make() failed");
    }
    return address;
}

#endif
