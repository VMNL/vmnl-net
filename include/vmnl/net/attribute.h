/**
 * \file attribute.h
 *
 * \defgroup Attribute Attribute
 * \addtogroup Attribute
 * \brief Additional compiler checks on the use of the API.
 *
 * \par Example
 * \code
 * VMNL_NET_API VMNL_NET_NODISCARD bool vmnl_net_buffer_prepare(VmnlNetBuffer *, size_t, VmnlNetBufferView *, size_t *, VmnlNetError *);
 *
 * VmnlNetBufferView prepare_buffer(VmnlNetBuffer *buffer)
 * {
 *     VmnlNetBufferView view;
 *
 *     vmnl_net_buffer_prepare(buffer, 42, &view, &(size_t) {1}, NULL); // warning: ignoring return value, does it succeed?
 *     return view;
 * }
 * \endcode
 * @{
 */

#ifndef VMNL_NET_ATTRIBUTE_H
#define VMNL_NET_ATTRIBUTE_H

/**
 * \def VMNL_NET_NODISCARD
 * \brief Warns when the result of a function is ignored.
 * \remark `warn_unused_result` attribute on GCC and clang, `_Check_return_` on MSVC, which checks it with /analyze only, nothing elsewhere.
 */
#if defined(_MSC_VER) && !defined(__clang__)
#include <sal.h>

#define VMNL_NET_NODISCARD _Check_return_
#elif defined(__GNUC__) || defined(__clang__)
#define VMNL_NET_NODISCARD __attribute__((warn_unused_result))
#else
#define VMNL_NET_NODISCARD
#endif

#endif /* !VMNL_NET_ATTRIBUTE_H */

/** @} */
