#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

int resolvehelper(const char* hostname, int family, const char* service, sockaddr_storage* pAddr)
{
    int result;
    addrinfo* result_list = NULL;
    addrinfo hints = {};
    hints.ai_family = family;
    hints.ai_socktype = SOCK_DGRAM; // without this flag, getaddrinfo will return 3x the number of addresses (one for each socket type).
    result = getaddrinfo(hostname, service, &hints, &result_list);
    if (result == 0)
    {
        //ASSERT(result_list->ai_addrlen <= sizeof(sockaddr_in));
        memcpy(pAddr, result_list->ai_addr, result_list->ai_addrlen);
        freeaddrinfo(result_list);
    }

    return result;
}

std::string Int2Str(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str(); 
    return s;
}

std::string Doub2Str(double i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str(); 
    return s;
}

std::string createPacket(int pressures[], double temperatures[], int voltage, int motors[], std::string stateString[], int loopTime) {
	healthPacket = "";

	healthPacket += "p:";
	for (int i = 0; i <=6; i++) {
		healthPacket += Int2Str(pressures[i])
		if (i != 6) {
			healthPacket += ",";
		}
	}

	healthPacket += ";t:";
	for (int j = 0; j<=5; j++) {
		healthPacket += Doub2Str(temperatures[j])
		if (j!=5) {
			healthPacket += ",";
		}
	}

	healthPacket += ";v:";
	healthPacket += Int2Str(voltage);

	//where Alex started
	healthPacket += ";m;"
	for(int i = 0; i < 3; i++){
		healthPacket+= Int2Str(motors[i]);
		if(i!=3)
			healthPacket+= ",";
	}

	healthPacket += ";h:";
	for(int i = 0; i < sizeof(stateString); i++){
		healthPacket+= stateString[i];
		if(i != sizeof(stateString))
			healthPacket += ",";
	}
	healthPacket+= ";s:";
	healthPacket+= Int2Str(loopTime);
	healthPacket+= ";?";
	int packetLength = healthPacket.length() - 1;
	healthPacket+=Int2Str(packetLength);
	healthPacket+= "|";
	return healthPacket;
}


void sendPacket()
{	
    int result = 0;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    char szIP[100];

    sockaddr_in addrListen = {}; // zero-int, sin_port is 0, which picks a random port for bind.
    addrListen.sin_family = AF_INET;
    result = bind(sock, (sockaddr*)&addrListen, sizeof(addrListen));
    if (result == -1)
    {
       int lasterror = errno;
       std::cout << "error: " << lasterror;
       exit(1);
    }


    sockaddr_storage addrDest = {};
    result = resolvehelper("192.168.0.4", AF_INET, "9000", &addrDest);
    if (result != 0)
    {
       int lasterror = errno;
       std::cout << "error: " << lasterror;
       exit(1);
    }

    std:string healthPacket = createPacket(pressures, temperatures, voltage, motors, stateString, loopTime);
    const char* msg = healthPacket.c_str();
    size_t msg_length = strlen(msg);

    result = sendto(sock, msg, msg_length, 0, (sockaddr*)&addrDest, sizeof(addrDest));

    //std::cout << result << " bytes sent" << std::endl;



}