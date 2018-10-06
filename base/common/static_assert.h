
#ifndef PC_STATIC_ASSERT_H
#define PC_STATIC_ASSERT_H

// taken from BOOST_STATIC_ASSERT
#define PC_STATIC_ASSERT(...)  static_assert(__VA_ARGS__, #__VA_ARGS__)

#endif