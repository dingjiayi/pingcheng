
#pragma once

#include <boost/operators.hpp>

namespace pingcheng
{
// Time stamp in UTC, in microseconds resolution.
// 
// This class is immutable.
// It's recommended to pass it by value, since it's passed in register on x64.

/*

*/

class Timestamp : public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp>
{
public:
// Constructs an invalid Timestamp.
Timestamp() :

private:
    int64_t microSecondsSinceEpoch_;
}


}


https://www.cnblogs.com/Anker/p/3418792.html