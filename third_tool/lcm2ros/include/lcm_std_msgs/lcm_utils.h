#ifndef __lcm_utils_h__
#define __lcm_utils_h__

#include <stdio.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <iostream>

inline std::string getUdpmAddr(bool bMulticast) {
    char* url;
    url = getenv ("LCM_DEFAULT_URL");
    if (url!=NULL && strlen(url)) {
        if (strstr(url, "udpm") != NULL) {
            std::cout << "LCM UDPM Address: " << url << std::endl;
            return url;
        }
    }

#if defined(__arm__) || defined(__aarch64__)
    return "udpm://239.255.76.67:7666";
#endif

    std::string addr = "udpm://239.255.76.67:7";
    struct ifaddrs * ifAddrStruct = NULL;
    struct ifaddrs * ifa = NULL;
    void * tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    bool valid = false;
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            // printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);

            char *tokenPtr = strtok(addressBuffer, ".");
            if(!strcmp(tokenPtr, "192")) {
                strtok(NULL,".");
                strtok(NULL,".");
                tokenPtr = strtok(NULL,".");
                if(strlen(tokenPtr)==1) addr+="00";
                if(strlen(tokenPtr)==2) addr+="0";
                addr += tokenPtr;
                valid = true;
                break;
            }
        }
    }
    if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);

    if(valid) {
        if(bMulticast)
            addr += "?ttl=1";
    }
    else
        addr.clear();
    std::cout << "LCM UDPM Address: " << addr << std::endl;

    return addr;
}

#endif
