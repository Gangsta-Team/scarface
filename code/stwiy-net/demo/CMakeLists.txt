set(SF_NET_DEMO_MAIN sf_net_demo)
set(SF_NET_DEMO_FILES main.cpp)

add_executable(${SF_NET_DEMO_MAIN} ${SF_NET_DEMO_FILES})
target_link_libraries (${SF_NET_DEMO_MAIN} PRIVATE ${SF_NET_LIB_MAIN} Ws2_32.lib winmm.lib enet)