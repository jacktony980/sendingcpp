
#include "debug.hpp"

namespace Kazv
{
    namespace detail
    {
        boost::iostreams::stream<boost::iostreams::null_sink> voidOutputHelper{boost::iostreams::null_sink()};
    }
}
