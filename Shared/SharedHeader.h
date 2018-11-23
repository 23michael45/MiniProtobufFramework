#ifndef SHARED_HEADER
#define SHARED_HEADER

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)
#include "code/utility/Singleton.h"
#include "code/utility/BaseSocketConnection.h"
#include "code/protobuf/BaseCmd.pb.h"
#include "asio/asio/include/asio.hpp"


#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>


#endif