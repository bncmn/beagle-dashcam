#include "udpNetwork.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include "shutdown.h"

#define MSG_MAX_LEN 1024
#define PORT_NUMBER 8088
#define REMOTE_PORT_NUMBER 54321

static struct sockaddr_in sinRemote;
static int socketDescriptor;
static void* udpThreadFunc(void*);
static pthread_t udp_threadID;

static const char* deviceUptimePath = "/proc/uptime";

// Handle each accepted command in separate function
static void Udp_commandMotionOn();
static void Udp_commandMotionOff();
static void Udp_commandTime();
static void Udp_commandStop();

// Create a UDP message to function mapping
std::unordered_map<std::string, void (*)()> udpFunctions = {
    {"motionOn",        Udp_commandMotionOn},
    {"motionOff",       Udp_commandMotionOff},
    {"time",            Udp_commandTime},
    {"stop",            Udp_commandStop},
};

static void Udp_sendMessage(const char* messageTx);

// Initialize socket address
void Udp_init() {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT_NUMBER);
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
    bind(socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
    
    sinRemote.sin_family = AF_INET;
    sinRemote.sin_port = htons(REMOTE_PORT_NUMBER);
    sinRemote.sin_addr.s_addr = inet_addr("192.168.7.1");

    // Begin/and background thread which listens and responds to incoming UDP messages
    pthread_create(&udp_threadID, NULL, udpThreadFunc, NULL);
}

void Udp_cleanup() {
    pthread_join(udp_threadID, NULL);
    close(socketDescriptor);
}

static void* udpThreadFunc(void*) {
    while (!Shutdown_isShutdown()) {  // change condition to when Shutdown is signal
        // Source: code written by Brian Fraser
		unsigned int sin_len = sizeof(sinRemote);
		char messageRx[MSG_MAX_LEN];

		// Pass buffer size - 1 for max # bytes so room for the null (string data)
		int bytesRx = recvfrom(socketDescriptor,
			messageRx, MSG_MAX_LEN - 1, 0,
			(struct sockaddr *) &sinRemote, &sin_len);

        if (bytesRx == (-1)) {
            perror("UDP: Unable to receive any message.");
            exit(1);
        }

		// Make it null terminated (so string functions work)
		// - recvfrom given max size - 1, so there is always room for the null
		messageRx[bytesRx] = 0;
        std::string messageRxStr(messageRx, bytesRx);
        
        auto it = udpFunctions.find(messageRxStr);
        if (it != udpFunctions.end()) {
            it->second();
        } else {
            std::string messageTx = "Unknown commands. Type help to see list of supported commands.\n";
            Udp_sendMessage(messageTx.c_str());
        }
    }
    return nullptr;
}

static void Udp_sendMessage(const char* messageTx) {
    unsigned int sin_len = sizeof(sinRemote);
    sendto(socketDescriptor,
        messageTx, strlen(messageTx),
        0,
        (struct sockaddr *) &sinRemote, sin_len);
}

static void Udp_commandMotionOn() {
    std::string messageTx = " Motion is turned on.\n";
    CameraTrigger_turnSensorOn();
    Udp_sendMessage(messageTx.c_str());
}
static void Udp_commandMotionOff() {
    std::string messageTx = " Motion is turned off.\n";
    CameraTrigger_turnSensorOff();
    Udp_sendMessage(messageTx.c_str());
}

static void readFile(const char* fileName, char* buff) {
    FILE *pFile = fopen(fileName, "r");
    if (pFile == nullptr) {
        printf ("ERROR: Unable to open file (%s) for rad\n", fileName);
        exit(1);
    }

    // Read string line
    const int MAX_LENGTH = 1024;
    fgets(buff, MAX_LENGTH, pFile);

    //Close
    fclose(pFile);
}

static void Udp_commandTime() {
    char buff[MSG_MAX_LEN];
    readFile(deviceUptimePath, buff);
    char* token = strtok(buff, ".");

    int uptime = atoi(token);
    char messageTx[MSG_MAX_LEN];
    snprintf(messageTx, MSG_MAX_LEN, "time %d \n", uptime);
    Udp_sendMessage(messageTx);
}

static void Udp_commandStop() {
    std::string messageTx = " Program terminating.\n";
    Udp_sendMessage(messageTx.c_str());
    Shutdown_triggerShutdown();
}