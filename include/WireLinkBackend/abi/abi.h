#ifndef WIRELINK_BACKEND_ABI_H
#define WIRELINK_BACKEND_ABI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <WireLinkBackend/export_macro.h>
#include <netinet/in.h>

typedef struct DataPacket {
    // ip and port of the sender
    uint8_t sender_addr_data[28]; // Max size for sockaddr_in6
    sa_family_t addr_family; // AF_INET or AF_INET6
    
    int size;
    char* data;
} DataPacket;

WIRELINKBACKEND_EXPORT DataPacket* WaitForData();

#ifdef __cplusplus
}
#endif

#endif // WIRELINK_BACKEND_ABI_H