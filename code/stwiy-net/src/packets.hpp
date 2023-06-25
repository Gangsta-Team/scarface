#ifndef _PACKETS_HPP
#define _PACKETS_HPP

struct OnFootSyncPacket {
    float posX;
    float posY;
    float posZ;
    float rotationPitch;
    float rotationYaw;
    float rotationRoll;
    int animationState;
    int equipmentState;
    int skinId;
};

struct VehicleSyncPacket {
    float posX;
    float posY;
    float posZ;
    float rotationPitch;
    float rotationYaw;
    float rotationRoll;
    float velocity;
    bool isOccupied;
};

enum class RPCAction {
    Walk,
    Shoot,
    Jump,
    Interact
};

struct RPCPacket {
    RPCAction action;
    union {
        OnFootSyncPacket onFootSync;
        VehicleSyncPacket vehicleSync;
    };
    int playerId;
};

#endif