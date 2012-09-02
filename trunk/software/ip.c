///////////////////////////////////////////////////////////
//
// Get IP address
//
// 24.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#include "ip.h"

int IP_Get(char* buffer) {
    struct ifaddrs *ifaddr, *ifa;
    int family, s, found;
    char host[NI_MAXHOST];

    found = 0;
    
    if (getifaddrs(&ifaddr) == -1) {
	return 0;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
	if (ifa->ifa_addr == NULL)
	    continue;

	family = ifa->ifa_addr->sa_family;

	// skip non-internet addresses
	if(family != AF_INET /*&& family != AF_INET6*/) continue;
	// skip localhost
	if(strncmp(ifa->ifa_name, "lo", 2) == 0) continue;

	s = getnameinfo(ifa->ifa_addr,
			(family == AF_INET) ? sizeof(struct sockaddr_in) :
				      sizeof(struct sockaddr_in6),
		host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
	if (s != 0) {
	    return 0;
	}
	strcpy(buffer, host);
	found = 1;
	break;
    }

    freeifaddrs(ifaddr);
    if(found) return 1; else return 0;
}

