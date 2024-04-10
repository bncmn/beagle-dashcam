// Module to listen to port 12345 for all incoling UDP packets
// Each packet is one command to respond, commands including:
// "motionOn", "motionOff", "time", "stop"
#ifndef _UDP_NETWORK_H
#define _UDP_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <netdb.h>
#include <iomanip>
#include <sstream>
#include <vector>

#include "hal/cameraTrigger.h"

// Initialize socket address
// Begin/and background thread which listens and responds to incoming UDP messages
void Udp_init();
void Udp_cleanup();

#endif