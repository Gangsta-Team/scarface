#ifndef SF_MOD_NET_EVENT_H
#define SF_MOD_NET_EVENT_H


namespace gangsta::net {

    enum NetEventType : int {
        CharacterSpawn = 0,
    };

    struct NetEvent {
        NetEventType type;
    };

}

#endif