/*!
 * \file src/utils/instanceof.hpp
 * \brief Instance of. Modified from:
 *
 *      https://github.com/SiriusNEO/CoconutJVM/blob/main/src/utils/misc.h.
 *
 * \author SiriusNEO
 */

#ifndef UTILS_INSTANCEOF_HPP_
#define UTILS_INSTANCEOF_HPP_

namespace alice {

namespace utils {

/*!
 * \brief Check a pointer of BaseClass whether it points to a SubClass. SubClass
 * must be a subclass of BaseClass.
 * \tparam SubClass The sub-class.
 * \tparam BaseClass The base class.
 * \param ptr pointer of the BaseClass.
 * \return True if the pointer indeed points to this SubClass.
 */
template <typename SubClass, typename BaseClass>
inline bool instanceof (const BaseClass* ptr) {
  return ptr != nullptr and dynamic_cast<const SubClass*>(ptr) != nullptr;
}

}  // namespace utils

}  // namespace alice

#endif  // SRC/UTILS/INSTANCEOF_HPP_
