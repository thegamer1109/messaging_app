#pragma once
#include "net_common.h"

namespace olc {
    namespace net {
        template <typename T>
        struct message_header {
            T id{};
            uint32_t size = 0;
        };

        template <typename T>
        struct message {
            message_header<T> header{};
            std::vector<uint8_t> body;

            // returns size of message packet in bytes
            size_t size() const {
                return sizeof(message_header<T>) + body.size();
            }

            // override for std::cout compatability
            friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
                os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
                return os;
            }

            // pushes any POD data into the message buffer
            template<typename DataType>
            friend message<T>& operator << (message<T>& msg, const DataType& data) {
                // check the type of data being pushed is trivial
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector!");

                // cache current vector size, data will be pointed to here
                size_t i = msg.body.size();

                // resize vector to data size
                msg.body.resize(msg.body.size() + sizeof(DataType));

                // physically copy data into new vector space
                std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

                // recalculate message size
                msg.header.size = msg.size();

                // return the target message for chaining
                return msg;
            }

            template<typename DataType>
            friend message<T>& operator >> (message<T>& msg, DataType& data) {
                // check the type of data being pushed is trivial
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector!");

                // cache location near vector end
                size_t i = msg.body.size() - sizeof(DataType);

                // physically copy data into new vector space
                std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

                // shrink the vector
                msg.body.resize(i);

                // recalculate message size
                msg.header.size = msg.size();

                // return the target message for chaining
                return msg;
            }

        };
    }
}