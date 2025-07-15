#pragma once
#include <introspect/Config.hpp>

#include <boost/uuid/uuid.hpp>

namespace introspect
{

// UUID is the most trivial and well-suited concept for this task. It is fast, thanks to
// the guys who invented SSE, and also easy-to-use and parse.
using ModuleId = boost::uuids::uuid;

} // namespace introspect