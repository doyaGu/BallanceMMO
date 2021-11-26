#ifndef BALLANCEMMOSERVER_PACKET_HPP
#define BALLANCEMMOSERVER_PACKET_HPP

#include "../entity/entity.hpp"
#include <steam/steamnetworkingtypes.h>
#include <vector>
#include <unordered_map>
#include <sstream>

namespace bmmo {
    enum opcode : uint32_t {
        None,
        LoginRequest,
        LoginAccepted,
        LoginDenied,
        PlayerDisconnected,
        PlayerConnected,

        Ping,
        BallState,
        OwnedBallState,
        KeyboardInput
    };

    template<typename T, opcode C = None>
    struct message {
        opcode code = C;
        T content;
    };

    // DO NOT ALLOCATE THIS! Only for typing pointers
    typedef struct message<uint8_t[k_cbMaxSteamNetworkingSocketsMessageSizeSend - sizeof(opcode)]> general_message;

    struct serializable_message {
        explicit serializable_message(opcode code): code(code) {}

        opcode code;
        std::stringstream raw;

        size_t size() const {
            return raw.str().size();
        }

        // entity -> raw
        virtual void serialize() {
            raw.write(reinterpret_cast<const char*>(&code), sizeof(opcode));
        }

        // raw -> entity
        virtual void deserialize() {
            opcode c;
            raw.read(reinterpret_cast<char*>(&c), sizeof(opcode));
            assert(c == code);
        }
    };
}

#endif //BALLANCEMMOSERVER_PACKET_HPP