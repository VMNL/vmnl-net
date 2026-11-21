/**
 * \file buffer_benchmarks.hpp
 * \brief Buffers and sizes shared by the buffer benchmarks.
 */

#ifndef VMNL_NET_BUFFER_BENCHMARKS_HPP
#define VMNL_NET_BUFFER_BENCHMARKS_HPP

#include <vmnl/net/buffer.h>

#include <benchmark/benchmark.h>

#include <cstddef>
#include <vector>

#define BUFFER_CAPACITY ((size_t) 65536) /*!< Capacity of the buffers, as many bytes as the largest size of the benchmarks. */
#define BUFFER_BATCH    100 /*!< Buffers that an iteration creates, destroys, clears, commits to or consumes from. */
#define BUFFER_VIEWS    8 /*!< Views given to the buffer functions, more than #BUFFER_CAPACITY bytes of chunks need. */

/**
 * \brief Buffers of a benchmark, all of one type, destroyed with the object.
 */
class Buffers {
  public:
    /**
     * \brief Creates the buffers.
     * \param state State of the benchmark, skipped with an error if a creation fails.
     * \param type Type of the buffers.
     * \param capacity Capacity of each buffer.
     * \param nbuffers Number of buffers to create.
     */
    Buffers(benchmark::State &state, VmnlNetBufferType type, size_t capacity, size_t nbuffers) : views(BUFFER_VIEWS)
    {
        for (size_t it = 0; it < nbuffers; it++) {
            VmnlNetBuffer *buffer = vmnl_net_buffer_create(type, capacity, NULL);

            if (buffer == NULL) {
                state.SkipWithError("vmnl_net_buffer_create() failed");
                break;
            }
            buffers.push_back(buffer);
        }
    }

    /**
     * \brief Destroys the buffers.
     */
    ~Buffers()
    {
        for (const auto &buffer : buffers) {
            vmnl_net_buffer_destroy(buffer);
        }
    }

    /**
     * \name Copies and moves
     * Not copyable nor movable, as two objects would destroy the buffers twice.
     * \{
     */
    Buffers(const Buffers &) = delete;
    Buffers &operator=(const Buffers &) = delete;
    Buffers(Buffers &&) = delete;
    Buffers &operator=(Buffers &&) = delete;
    /** \} */

    /**
     * \brief Prepares free space in each buffer, so that a commit can follow.
     * \param state State of the benchmark, skipped with an error if a preparation fails.
     * \param nbytes Free bytes needed in each buffer.
     * \return true on success, false otherwise.
     */
    bool prepare(benchmark::State &state, size_t nbytes)
    {
        for (const auto &buffer : buffers) {
            size_t nviews = views.size();

            if (vmnl_net_buffer_prepare(buffer, nbytes, views.data(), &nviews, NULL) == false) {
                state.SkipWithError("vmnl_net_buffer_prepare() failed");
                return false;
            }
        }
        return true;
    }

    /**
     * \brief Stores bytes in each buffer, prepared then committed without being written.
     * \param state State of the benchmark, skipped with an error if a preparation fails.
     * \param nbytes Bytes to store in each buffer.
     * \return true on success, false otherwise.
     */
    bool commit(benchmark::State &state, size_t nbytes)
    {
        if (prepare(state, nbytes) == false) {
            return false;
        }
        for (const auto &buffer : buffers) {
            vmnl_net_buffer_commit(buffer, nbytes, NULL);
        }
        return true;
    }

    /**
     * \brief Drops the stored bytes of each buffer.
     */
    void clear() const
    {
        for (const auto &buffer : buffers) {
            vmnl_net_buffer_clear(buffer);
        }
    }

    /**
     * \brief Gets a buffer.
     * \param index Index of the buffer, lower than size().
     * \return Buffer at \p index.
     */
    VmnlNetBuffer *operator[](size_t index) const
    {
        return buffers[index];
    }

    /**
     * \brief Counts the buffers.
     * \return Number of created buffers, fewer than asked if a creation failed.
     */
    size_t size() const
    {
        return buffers.size();
    }

  private:
    std::vector<VmnlNetBuffer *> buffers; /*!< Created buffers. */
    std::vector<VmnlNetBufferView> views; /*!< Views filled by the preparations, never read. */
};

#endif
