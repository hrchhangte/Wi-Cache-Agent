/*
 * CacheAgentMain.h
 *
 *  Created on: 17-Mar-2017
 *      Author: zela
 *
 * This defines the functions that will
 * be implemented by CacheAgentMain.cpp
 */

#ifndef CACHEAGENTMAIN_H_
#define CACHEAGENTMAIN_H_

#include <sstream>
#include <iostream>
#include <cstddef>
#include <cstring> 	// used for memset.
#include <arpa/inet.h> 	// for inet_ntop function
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include <pthread.h>
#include <cstdlib>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <unordered_map>

#include <tins/tins.h>
#include<tins/dns.h>
#include<tins/sniffer.h>
#include<thread>

using namespace std;
using namespace Tins;

/*
 * this is the structure that defines the
 * parameters that are to be passed to the
 * thread that handles the sending of segment
 * to a client
 */
struct threadParam{
	int threadId; // thread id
	string clntIp; // the client IP address to which segment will be sent
	string fileName; //the segment name that is to be sent
	long fileSize; //the size of segment
	long skipBytes; //the number of bytes to be skipped, i.e., byte index
	long bytesSent; //the number of bytes sent already
	string finTrans; //whether the sending process is completed
	string handOver; //whether there is a handover or not
	int ctrSockHndle; //the socket handle for the connection to controller for control
	const char *clntPort; //the port number where client is expecting data of segment
	int clntSockHndle; //the socket handle for the connection to the client
	string cachedStat; //whether the segment is cached or not
	int ctrSockHndleDat; //the socket handle for the connection to controller for data
	string neighborAP; //the IP address of a neighbor AP from where data will be fetched
};

/*
 * this is the structure that defines
 * the connection between the agent and
 * the client to which the segment
 * is to be delivered
 *
 */
struct contReqParam{

	int threadId; //thread id
	int connKey; //connection identifier between the controller and agent/AP
	string segName; //the egment name
	long segSize; //the size of the segment
	long skipBytes; //the number of bytes to be skipped, i.e, byte index
	int clientKey; //the key of the client to which segment will be sent
	long segAck; //the ack for the segment from the client
	long wifiDeliverTime; //current not used
	const char *clientPort; //the port where client is expecting data
	int clntSockHndle; //the socket handle for the connection to the client
	int ctrSockHndle; //the socket handle for the connection to the controller
	string neighborAP; //the IP address of a neighbor AP from where data will be fetched

};

/*
 * this is the structure to keep track
 * of the wifi clients
 */
struct wifiConnParam{
	uint16_t assocID; //association id
	string clientIP; //client IP
	string status; //the status whether it is associated/disassociated
};

static unordered_map <string, struct threadParam> threadMap; //map of client IP address and the thread serving the client

static unordered_map <string, struct wifiConnParam> wifiConnMap; //map of client IP address and the wifiConnParam object

static unordered_map <string, struct contReqParam> contReqMap; //map of connection id+segment name and the thread object

static unordered_map<string, long> fileMap; //map of the file/segment and size

class CacheAgentMain {
private:

	int ctrSockHndle; //socket handle for connection to controller (for control)

	int ctrSockHndleDat; //socket handle for connection to controller (for data)

	const char *ctrIp; //controller IP address

	const char *ctrPort; //controller port (for control)

	const char *ctrPortDat; //controller port (for data)

	struct addrinfo *ctrAddrInfoList; //this is used in controller socket connection

	struct addrinfo ctrAddrInfo; //this is used in controller socket connection

	int threadId; //thread w.r.t controller socket connection

	long totalCacheSize; //total size of cache (in bytes) available for caching

	long remCacheSize; //remaining cache size (in bytes) available for caching

	string dataTransProto; //protocol used for data transfer between AP and client

	const char *ctrUdpPort; //controller port (for UDP update)

	const char *clntDataPort; //port used by client for data transfer/download

public:
	CacheAgentMain();
	virtual ~CacheAgentMain();

	/*
	 * start the execution of the WiCache agent
	 */
	void startAgnt();

	/*
	 * read the configuration file
	 * contains server IP, port, etc.
	 */

	void readConfigFile();

	/*
	 * initiate socket connection with the controller
	 */
	void connCtr();

	/*
	 * listens, wait for incoming control message
	 * from the controller
	 */

	void getCtrMesg();

	/*
	 * function called for every thread created
	 * to handle incoming control message from the controller
	 */

	static void *clntThrd(void *);

	/*
	 * setup TCP socket for sending data to client
	 */

	void setupSockTcp(string);

	/*
	 * send file/segment data to the client
	 */

	void sndFileTcp(string);

	/*
	 * send file/segment data to the client
	 * and handles mobility (which may result in
	 * handoff)
	 */

	void sndFileTcpMobility(string);

	/*
	 * update the controller of the data transfer
	 * completion to the client
	 */

	void sndFinishUpdate(string);

	/*
	 * keep updating the data transfer status to
	 * the controller
	 */
	void sndUpdate(string);

	/*
	 * send update message using
	 * UDP protocol
	 */
	void udpUpdate(string,int);

	/*
	 * setup sctp socket that has
	 * multiple streams
	 */

	int setupSockSctpMany(int, const char *, unsigned short int, string, string);

	/*
	 * send data over sctp socket that
	 * has multiple streams
	 */
	void sndFileSctpMany(int, string, string, string, int, struct sockaddr_in);

	/*
	 * setup sctp socket that has one stream
	 */
	int setupSockSctpOne(int, const char *, unsigned short int, string, string);

	/*
	 * send data over sctp socket that has
	 * one stream
	 */
	void sndFileSctpOne(int, string, string, string, int, struct sockaddr_in);

	int startAssocSniff();

	bool dot11AssocResp(PDU&);

	int startDisAssocSniff();

	bool dot11DisAssocResp(PDU&);

	int startDhcpSniff();

	bool dhcpAck(PDU&);

	void printConnMap(string key);
};

#endif /* CACHEAGENTMAIN_H_ */
