/**
 * \file attribute.h
 *
 * \defgroup Attribute Attribute
 * \addtogroup Attribute
 * \brief Compiler attributes of the library.
 *
 * \par Example
 * \code
 * VMNL_NET_NODISCARD VmnlNetContext *vmnl_net_context_create(VmnlNetError *error);
 *
 * vmnl_net_context_create(NULL); // warning: ignoring return value
 * \endcode
 * @{
 */

#ifndef VMNL_NET_ATTRIBUTE_H
#define VMNL_NET_ATTRIBUTE_H

/**
 * \def VMNL_NET_NODISCARD
 * \brief Warns when the result of a function is ignored.
 * \remark `warn_unused_result` on GCC and clang, `_Check_return_` on MSVC, which checks it with /analyze only, nothing elsewhere.
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
