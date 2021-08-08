/*
 * CacheAgentMain.cpp
 *
 *  Created on: 17-Mar-2017
 *      Author: zela
 *
 * This is the main file that implements the
 * WiCache Agent functions
 */

#include "CacheAgentMain.h"

#define MAX_NO_THREADS 100
#define AGENT_FILE_PATH "/mnt/app/"
#define CACHE_PATH "/mnt/cache/"
#define NEIGHBOR_CACHE_PATH "/mnt/rcache"
#define AGENT_CONFIG_FILE "agentConfigFile"
#define MAX_OSTREAMS_SCTP 5
#define MAX_INSTREAMS_SCTP 5
#define MAX_ATTEMPTS 3

using namespace std;
//using namespace Tins;

/*
 * initializes the data structures
 */

CacheAgentMain::CacheAgentMain() {
	// TODO Auto-generated constructor stub

	ctrSockHndle = 0;
	ctrSockHndleDat = 0; //not used
	//ctrIp = "192.168.1.145"; //can be set here or obtained from the config. file
	//ctrPort = "8000"; //can be set here or obtained from the config. file
	ctrPortDat = "0"; //not used
	ctrAddrInfoList = NULL;
	memset(&ctrAddrInfo, 0, sizeof(ctrAddrInfo));
	threadId = 0;
	remCacheSize = totalCacheSize;

}

CacheAgentMain::~CacheAgentMain() {
	// TODO Auto-generated destructor stubs
}

/*
 * start the execution of the WiCache agent
*/
void CacheAgentMain::startAgnt(){

	readConfigFile();
	connCtr();
	getCtrMesg();

}

/*
 * read the configuration file
 * contains server IP, port, etc.
 */

void CacheAgentMain::readConfigFile(){

	string fileName = AGENT_CONFIG_FILE;

	//read from the cache/file
	string filePath = AGENT_FILE_PATH;
	filePath.append(fileName);
	std::fstream fs;

	fs.open(filePath.c_str());

	if(fs.is_open()){
		string line;
		while (getline(fs, line )) {
		    // output the line
		   // cout<<line<<endl;

		    //split message to tokens (tokens of strings)
			vector<string> tokens;
			stringstream str(line);
			while(!str.eof()) {
				string tmp;
				str >> tmp;
				tokens.push_back(tmp);
			}

			//ctrIp = "192.168.1.145";
			//ctrPort = "8000";

			if(tokens[0].compare("ctrAddr")==0){
				//obtain the controller's IP address
				ctrIp = tokens[1].c_str();
			}
			else if(tokens[0].compare("ctrPort")==0){
				//obtain the controller's port no.
				ctrPort = tokens[1].c_str();
			}
			else if(tokens[0].compare("ctrUdpPort")==0){
				//obtain the controller's port no. used for status update
				ctrUdpPort = tokens[1].c_str();
			}
			else if(tokens[0].compare("dataTransProto")==0){
				//obtain the data transport protocol used for sending
				//file/segment to client
				dataTransProto = tokens[1];
			}
			else if(tokens[0].compare("clntDataPort")==0){
				clntDataPort = tokens[1].c_str();
			}
		}//while

		fs.close();

	}//if (fs.is_open)

	else{
		cout<<"Could not open file: agentConfigFile"<<endl;
	}

}

/*
 * initiate socket connection with the controller
 */

void CacheAgentMain::connCtr(){

	//this part is for initiating tcp connection to the controller
	ctrAddrInfo.ai_family = AF_UNSPEC;
	ctrAddrInfo.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(ctrIp, ctrPort, &ctrAddrInfo, &ctrAddrInfoList)!=0)
		cerr<<"[Thread - "<<threadId<<"] "<<"Getaddrinfo error : tcp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Getaddrinfo success : tcp"<<endl;

	if((ctrSockHndle = socket(ctrAddrInfoList->ai_family, ctrAddrInfoList->ai_socktype, ctrAddrInfoList->ai_protocol))==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"Socket error : tcp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Socket created : tcp"<<endl;

	if(connect(ctrSockHndle, ctrAddrInfoList->ai_addr, ctrAddrInfoList->ai_addrlen)==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"Connect error : tcp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Socket connection to controller : "<<ctrIp
		<<":"<<ctrPort<<" successful"<<endl;

	/*
	if(getaddrinfo(ctrIp, ctrPortDat, &ctrAddrInfo, &ctrAddrInfoList)!=0)
		cerr<<"[Thread - "<<threadId<<"] "<<"Getaddrinfo error : tcp data"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Getaddrinfo success : tcp data"<<endl;

	if((ctrSockHndleDat = socket(ctrAddrInfoList->ai_family, ctrAddrInfoList->ai_socktype, ctrAddrInfoList->ai_protocol))==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"Socket error : tcp data"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Socket created : tcp data"<<endl;

	if(connect(ctrSockHndleDat, ctrAddrInfoList->ai_addr, ctrAddrInfoList->ai_addrlen)==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"Connect error : tcp data"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Socket connection to controller : "<<ctrIp
		<<":"<<ctrPort<<" successful data"<<endl;

	*/
}

/*
void CacheAgentMain::getCtrMesg(){

	//setup thread attributes
	int maxThrdNo = MAX_NO_THREADS;
	pthread_t threadAttr[maxThrdNo];
	struct contReqParam  param[maxThrdNo];

	static char recvChar[BUFSIZ];
	int i = 1;
	string transfrStat;

	while(true){

		//read data/message from socket as char array
		if((read(ctrSockHndle, recvChar, sizeof(recvChar)))==-1){
			cerr<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: Socket read error"<<endl;
			//break;
		}
		else
			cout<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: Message received from controller!"<<endl;

		cout<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: "<<recvChar<<endl;
		//convert char array to string
		string recvMesg = string(recvChar);

		//split message to tokens (tokens of strings)
		vector<string> tokens;
		stringstream str(recvMesg);
		while(!str.eof()) {
			string tmp;
			str >> tmp;
			tokens.push_back(tmp);
		}

		//for setting cache size
		if(tokens[0].compare("SET_CACHE")==0){
			//Message format : SET_CACHE <cache size>
			long size = atol((char *)tokens[3].c_str());

			totalCacheSize = size;
			remCacheSize = totalCacheSize;
		}

		//for adding a new file/segment
		else if(tokens[0].compare("ADD_FILE")==0){
			//Message format : ADD_FILE <FILE_NAME/SEG_NAME> <FILE_SIZE/SEG_SIZE>

			string fileName = tokens[2];
			long fileSize = atol((char *)tokens[3].c_str());

			//This is already taken care by the controller
			//the controller checks already, this is another level of checking done
			if(remCacheSize >= fileSize){
				fileMap.emplace(fileName, fileSize);
				remCacheSize = remCacheSize - fileSize;

				cout<<"ADD_FILE"<<"::"<<"New file added, fileName="<<fileName<<","<<"fileSize="<<fileSize<<endl;
			}
			else{
				remCacheSize = 0;

				cerr<<"ADD_FILE"<<"::"<<"Error, no more cache space, fileName="<<fileName<<","<<"fileSize="<<fileSize<<endl;
			}
			//the actual file transfer is performed by the controller
			//to the mounted cache storage of the AP at the controller
		}

		//for deleting/removing a file/segment
		else if(tokens[0].compare("DEL_FILE")){
			//Message format : DEL_FILE <FILE_NAME/SEG_NAME> <FILE_SIZE/SEG_SIZE>

			string fileName = tokens[2];
			string fileSize = atol((char *)tokens[3].c_str());

			//if file/segment exists
			if(fileMap.find(fileName) != filemap.end()){
				fileMap.erase(fileName);

				remCacheSize = remCacheSize + fileSize;
				if(remCacheSize > totalCacheSize){
					remCacheSize = totalCacheSize;
				}
			}
			else{
				cerr<<"DEL_FILE"<<"::"<<"Error, file does not exist"<<endl;
			}
			//the actual file deletion is performed by the controller
			//from the mounted cache storage of the AP at the controller
		}

		else if(tokens[0].compare("SEND_SEG")==0){

			//Message format : SEND_SEG <SEG_NAME> <SEG_SIZE> <SKIP_BYTES> <CLIENT_KEY> <CLIENT_PORT>
													//<NEIGHBOR_IP = NULL IF NOT NEEDED>

			param[i].threadId = i;
			param[i].connKey =  atoi((char *)tokens[1].c_str());
			param[i].segName = tokens[2];
			param[i].segSize = atol((char *)tokens[3].c_str());
			param[i].skipBytes = atol((char *)tokens[4].c_str());
			param[i].clientKey = atoi((char *)tokens[5].c_str());
			param[i].clientPort =  tokens[6].c_str();
			param[i].clntSockHndle = 0;
			param[i].ctrSockHndle = ctrSockHndle;
			param[i].neighborAP = tokens[7];

			//to identify the thread handling the segment transfer
			string key = tokens[1]+tokens[2];
			contReqMap.emplace(key,param[i]);

			if(pthread_create(&threadAttr[i], NULL, &CacheAgentMain::clntThrd, (void *)&key)!=0)
				cerr<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: Error creating thread"<<endl;

			if(i==maxThrdNo)
				i=1;
			else
				i++;
		}

		else if(tokens[0].compare("REQ_CLNT_STAT")==0){

			//message format : REQ_CLNT_STAT <client ip>
			//response format : CLNT_STAT <client ip> <finish status> <file name> <file size> <bytes sent>

			stringstream fileSizess, bytesSentss;
			fileSizess<<threadMap[tokens[1]].fileSize;
			bytesSentss<<threadMap[tokens[1]].bytesSent;

			transfrStat = "CLNT_STAT";
			transfrStat += " " + threadMap[tokens[1]].clntIp + " " + threadMap[tokens[1]].finTrans + " " + threadMap[tokens[1]].fileName
					+ " " + fileSizess.str() + " " + bytesSentss.str();
			if((send(ctrSockHndle, (void *)transfrStat.c_str(), (size_t)strlen(transfrStat.c_str()), 0))==-1){
				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : tcp update"<<endl;
			}
			else
				cout<<"[Thread - "<<threadId<<"] "<<"Update sent successfully : tcp"<<endl;

		}

		else if(tokens[0].compare("REQ_WIRELESS_STAT")==0){
			//message format : REQ_WIRELESS_STAT <client ip>
			//response format : WIRELESS_START <client ip> <RSSI> <Noise>

			//currently this is obtained from the Odin agent
			//the controller fetches these stats from the Odin agent periodically
			//additional information is to provided from here
		}

		else if(tokens[0].compare("CLNT_HANDOVER")==0){

			//message format : CLNT_HANDOVER <client ip>

			if(threadMap[tokens[1]].finTrans.compare("false")==0){
				threadMap[tokens[1]].handOver = "true";
			}
		}

	}//while
}

*/

/*
 * listens, wait for incoming control message
 * from the controller
 */

void CacheAgentMain::getCtrMesg(){

	//setup thread attributes
	int maxThrdNo = MAX_NO_THREADS;
	pthread_t threadAttr[maxThrdNo];
	struct threadParam param[maxThrdNo];

	static char recvChar[BUFSIZ];
	int i = 1;
	//string transfrStat;

	while(true){

		//read data/message from socket as char array
		if((read(ctrSockHndle, recvChar, sizeof(recvChar)))==-1){
			cerr<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: Socket read error"<<endl;
			//break;
		}
		else
			cout<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: Message received from controller!"<<endl;

		cout<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: "<<recvChar<<endl;
		//convert char array to string
		string recvMesg = string(recvChar);

		//split message to tokens (tokens of strings)
		vector<string> tokens;
		stringstream str(recvMesg);
		while(!str.eof()) {
			string tmp;
			str >> tmp;
			tokens.push_back(tmp);
		}

		//for setting cache size
		if(tokens[0].compare("SET_CACHE")==0){
			//Message format : SET_CACHE <cache size>
			long size = atol((char *)tokens[3].c_str());

			totalCacheSize = size;
			remCacheSize = totalCacheSize;
		}

		//for adding a new file/segment
		else if(tokens[0].compare("ADD_FILE")==0){
			//Message format : ADD_FILE <FILE_NAME/SEG_NAME> <FILE_SIZE/SEG_SIZE>

			string fileName = tokens[2];
			long fileSize = atol((char *)tokens[3].c_str());

			//This is already taken care by the controller
			//the controller checks already, this is another level of checking done
			if(remCacheSize >= fileSize){
				fileMap.emplace(fileName, fileSize);
				remCacheSize = remCacheSize - fileSize;

				cout<<"ADD_FILE"<<"::"<<"New file added, fileName="<<fileName<<","<<"fileSize="<<fileSize<<endl;
			}
			else{
				remCacheSize = 0;

				cerr<<"ADD_FILE"<<"::"<<"Error, no more cache space, fileName="<<fileName<<","<<"fileSize="<<fileSize<<endl;
			}
			//the actual file transfer is performed by the controller
			//to the mounted cache storage of the AP at the controller
		}

		//for deleting/removing a file/segment
		else if(tokens[0].compare("DEL_FILE")){
			//Message format : DEL_FILE <FILE_NAME/SEG_NAME> <FILE_SIZE/SEG_SIZE>

			string fileName = tokens[2];
			string fileSize = atol((char *)tokens[3].c_str());

			//if file/segment exists
			if(fileMap.find(fileName) != filemap.end()){
				fileMap.erase(fileName);

				remCacheSize = remCacheSize + fileSize;
				if(remCacheSize > totalCacheSize){
					remCacheSize = totalCacheSize;
				}
			}
			else{
				cerr<<"DEL_FILE"<<"::"<<"Error, file does not exist"<<endl;
			}
			//the actual file deletion is performed by the controller
			//from the mounted cache storage of the AP at the controller
		}

		else if(tokens[0].compare("SEND_SEG")==0){

			//new message format : SEND_SEG <file name> <file size> <skip bytes> <client ip>  <cached stat> <neighbor AP>
			param[i].threadId = i;
			param[i].fileName = tokens[1];
			param[i].fileSize = atol((char *)tokens[2].c_str());
			param[i].skipBytes = atol((char *)tokens[3].c_str()); //to skip initially
			param[i].bytesSent = atol((char *)tokens[3].c_str());//to keep track of bytes sent //not used
			param[i].clntIp = tokens[4];
			param[i].finTrans = "false";
			param[i].handOver = "false";
			param[i].ctrSockHndle = ctrSockHndle; //for writing back to controller
			//param[i].clntPort = "9001";
			param[i].clntPort = clntDataPort;
			param[i].clntSockHndle = 0; //initialized to 0
			param[i].cachedStat = tokens[5];
			param[i].ctrSockHndleDat = ctrSockHndleDat; //not used
			param[i].neighborAP = tokens[6];

			threadMap.emplace(tokens[4],param[i]);//client ip (tokens[4] is the key

			if(pthread_create(&threadAttr[i], NULL, &CacheAgentMain::clntThrd, (void *)&tokens[4])!=0)
				cerr<<"[Thread - "<<threadId<<"] "<<"getCtrMesg: Error creating thread"<<endl;

			if(i==maxThrdNo)
				i=1;
			else
				i++;
		}

		else if(tokens[0].compare("REQ_CLNT_STAT")==0){

			//message format : REQ_CLNT_STAT <client ip>
			//response format : CLNT_STAT <client ip> <finish status> <file name> <file size> <bytes sent>

			stringstream fileSizess, bytesSentss;
			fileSizess<<threadMap[tokens[1]].fileSize;
			bytesSentss<<threadMap[tokens[1]].bytesSent;

			transfrStat = "CLNT_STAT";
			transfrStat += " " + threadMap[tokens[1]].clntIp + " " + threadMap[tokens[1]].finTrans + " " + threadMap[tokens[1]].fileName
					+ " " + fileSizess.str() + " " + bytesSentss.str();
			if((send(ctrSockHndle, (void *)transfrStat.c_str(), (size_t)strlen(transfrStat.c_str()), 0))==-1){
				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : tcp update"<<endl;
			}
			else
				cout<<"[Thread - "<<threadId<<"] "<<"Update sent successfully : tcp"<<endl;

		}

		else if(tokens[0].compare("CLNT_HANDOVER")==0){

			//message format : CLNT_HANDOVER <client ip>

			if(threadMap[tokens[1]].finTrans.compare("false")==0){
				threadMap[tokens[1]].handOver = "true";
			}
		}

	}//while
}


/*
 * function called for every thread created
 * to handle incoming control message (SEND_SEG) from the controller
 */

void * CacheAgentMain::clntThrd(void *param){

	string mapKey = *((string *)param);
	int threadId = threadMap[mapKey].threadId;
	string clntIp = threadMap[mapKey].clntIp;
	string fileName = contReqMap[mapKey].fileName;
	long fileSize = threadMap[mapKey].fileSize;

	cout<<"[Thread - "<<threadId<<"] "<<"clntThrd: New thread created successfully!"<<endl;

	//assign transport layer protocol
	string transProto("tcp");
	if(transProto.compare("tcp")==0){

		CacheAgentMain obj;
		obj.setupSockTcp(mapKey);
		obj.sndFileTcp(mapKey);
		//obj.sndFileTcpMobility(mapKey);
	}


	else if(transProto.compare("sctpOne")==0){
		unsigned short int clntPort = 9001;
		const char *clntIpAddr = clntIp.c_str();
		CacheAgentMain obj;
		clntSockHndle = obj.setupSockSctpOne(threadId, clntIpAddr, clntPort, fileSize, fileName);
	}

	else if(transProto.compare("sctpMany")==0){
		unsigned short int clntPort = 9001;
		const char *clntIpAddr = clntIp.c_str();
		CacheAgentMain obj;
		clntSockHndle = obj.setupSockSctpMany(threadId, clntIpAddr, clntPort, fileSize, fileName);
	}

	else
		cout<<"[Thread - "<<threadId<<"] "<<"clntThrd: Error: Check transport protocol option!"<<endl;

	//terminate thread
	return 0;
}

//void * CacheAgentMain::clntThrd(void *param){
//
//	string mapKey = *((string *)param);
//	int threadId = contReqMap[mapKey].threadId;
//
//	cout<<"[Thread - "<<threadId<<"] "<<"clntThrd: New thread created successfully!"<<endl;
//
//	//assign transport layer protocol
//	//string transProto("tcp");
//	if(dataTransProto.compare("tcp")==0){
//
//		CacheAgentMain obj;
//		obj.setupSockTcp(mapKey);
//		//obj.sndFileTcp(mapKey);
//		obj.sndFileTcpMobility(mapKey);
//	}
//
//	/*
//	else if(dataTransProto.compare("sctpOne")==0){
//		unsigned short int clntPort = 9001;
//		const char *clntIpAddr = clntIp.c_str();
//		CacheAgentMain obj;
//		clntSockHndle = obj.setupSockSctpOne(threadId, clntIpAddr, clntPort, fileSize, fileName);
//	}
//
//	else if(dataTransProto.compare("sctpMany")==0){
//		unsigned short int clntPort = 9001;
//		const char *clntIpAddr = clntIp.c_str();
//		CacheAgentMain obj;
//		clntSockHndle = obj.setupSockSctpMany(threadId, clntIpAddr, clntPort, fileSize, fileName);
//	}
//	*/
//	else
//		cout<<"[Thread - "<<threadId<<"] "<<"clntThrd: Error: Check transport protocol option!"<<endl;
//
//	//terminate thread
//	return 0;
//}

/*
 * setup TCP socket for sending data to client
 */

void CacheAgentMain::setupSockTcp(string mapKey){
	int threadId = threadMap[mapKey].threadId;
	string clntIp = threadMap[mapKey].clientIP; //get clientKey

	//stringstream clientKeyss;
	//clientKeyss<<clientKey;

	//string clntIp = wifiConnMap[clientKeyss.str()].clientIP; //get IP using the clientKey
	const char *clntPort = contReqMap[mapKey].clientPort;

	int clntSockHndle;
	//setup socket for connecting with client
	struct addrinfo *clntAddrInfoList = NULL;
	struct addrinfo clntAddrInfo;
	memset(&clntAddrInfo, 0, sizeof(clntAddrInfo));

	clntAddrInfo.ai_family = AF_UNSPEC;
	clntAddrInfo.ai_socktype = SOCK_STREAM;

	//setup data structures for client control socket
	cout<<clntIp.c_str()<<" "<<clntPort<<endl;
	if(getaddrinfo(clntIp.c_str(), clntPort, &clntAddrInfo, &clntAddrInfoList)!=0)
		cerr<<"[Thread - "<<threadId<<"] "<<"setupSockTcp: Getaddrinfo error"<<endl;

	//create socket
	if((clntSockHndle = socket(clntAddrInfoList->ai_family, clntAddrInfoList->ai_socktype, clntAddrInfoList->ai_protocol))==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"setupSockTcp: Socket error"<<endl;

	//connect to client control socket (and data)
	if(connect(clntSockHndle, clntAddrInfoList->ai_addr, clntAddrInfoList->ai_addrlen)==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"setupSockTcp: Connect error"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"setupSockTcp: Socket connection to client : "<<clntIp<<":"<<clntPort<<" successful"<<endl;

	//add the client socket handle in the hash table
	contReqMap[mapKey].clntSockHndle = clntSockHndle;
}

/*
 * send file/segment data to the client
 */

//void CacheAgentMain::sndFileTcp(string mapKey){
//
//	int threadId = contReqMap[mapKey].threadId;
//	int connKey = contReqMap[mapKey].connKey;
//	int clientKey = contReqMap[mapKey].clientKey; //get clientKey
//
//	stringstream clientKeyss;
//	clientKeyss<<clientKey;
//
//	string clntIp = wifiConnMap[clientKeyss.str()].clientIP; //get IP using the clientKey
//	string segName = contReqMap[mapKey].segName;
//	long segSize = contReqMap[mapKey].segSize;
//	long skipBytes = contReqMap[mapKey].skipBytes;
//	int ctrSockHndle = contReqMap[mapKey].ctrSockHndle;
//
//	int clntSockHndle = contReqMap[mapKey].clntSockHndle;
//	string neighborAP = contReqMap[mapKey].neighborAP;
//
//	string cachedStat;
//	if(neighborAP.compare("NULL")){
//		cachedStat="CACHED";
//	}
//
//	string::size_type sz;
//	stringstream segSizess, skipBytesss, connKeyss;
//	segSizess<<segSize;
//	skipBytesss<<skipBytes;
//	connKeyss<<connKey;
//
//
//	/*---------------------------------------------------------------------------------------------------------------------------
//	 *
//	 * ---------------------------------------------------------------------------------------------------------------------------
//	*/
//
//	//send wifi deliver time to controller
//	string mesg = "WIFI_DELIVER_TIME";
//	mesg=mesg+" "+connKeyss.str()+" "+segName.c_str()+" "+"NULL";
//	if((send(ctrSockHndle, (char *)mesg.c_str(), mesg.length(), 0))==-1){
//		cerr<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Socket send error: mesg"<<endl;
//	}
//	else
//		cout<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Mesg sent successfully"<<endl;
//
//	//send seg head to client
//	mesg = "SEG_HEAD";
//	mesg=mesg+" "+connKeyss.str()+" "+segName+" "+segSizess.str()+" "+skipBytesss.str();
//	mesg += '\n';
//
//	//print and send message to client
//	cout<<"[Thread - "<<threadId<<"] "<<mesg;
//	if((send(clntSockHndle, (char *)mesg.c_str(), mesg.length(), 0))==-1){
//		cerr<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Socket send error: mesg"<<endl;
//	}
//	else
//		cout<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Mesg sent successfully"<<endl;
//
//	/*---------------------------------------------------------------------------------------------------------------------------
//	 *
//	 * ---------------------------------------------------------------------------------------------------------------------------
//	 */
//
//	//read segAck from client
//	static char recvChar[BUFSIZ];
//	if((read(clntSockHndle, recvChar, sizeof(recvChar)))==-1){
//		cerr<<"[Thread - "<<threadId<<"] "<<"sendFileTcp: Socket read error"<<endl;
//		//break;
//	}
//	else
//		cout<<"[Thread - "<<threadId<<"] "<<"sendFileTcp: Message received from client!"<<endl;
//
//	cout<<"[Thread - "<<threadId<<"] "<<"sendFileTcp: "<<recvChar<<endl;
//	//convert char array to string
//	string recvMesg = string(recvChar);
//
//	//split message to tokens (tokens of strings)
//	vector<string> tokens;
//	stringstream str(recvMesg);
//	while(!str.eof()) {
//		string tmp;
//		str >> tmp;
//		tokens.push_back(tmp);
//	}
//
//	if(tokens[0].compare("SEG_ACK")==0){
//
//		long segAckValue = atol((char *)tokens[3].c_str());
//
//		contReqMap[mapKey].skipBytes = segAckValue;
//		skipBytes = segAckValue; //update skipBytes using data from client
//
//	}
//
//	/*---------------------------------------------------------------------------------------------------------------------------
//	 *
//	 * ---------------------------------------------------------------------------------------------------------------------------
//	*/
//
//	char readData[BUFSIZ];
//	long totalRead = 0, totalSent = skipBytes;
//	long sentBytes = 0;
//	streamsize readBytes = 0;
//
//	std::fstream fs;
//	string filePath;
//
//	if(cachedStat.compare("CACHED")==0){
//
//		cout<<"[Thread - "<<threadId<<"] "<<"CACHED"<<endl;
//
//		//read from the cache/file
//		filePath = CACHE_PATH;
//		filePath.append(segName);
//
//	}
//	else{ //if (cachedStat.compare("NOT_CACHED")==0){
//
//		cout<<"[Thread - "<<threadId<<"] "<<"NOT_CACHED"<<endl;
//		cout<<"Fetching file from: "<<neighborAP<<endl;
//
//		//read from the remote cache/file
//		filePath = NEIGHBOR_CACHE_PATH;
//		filePath.append(neighborAP);
//		filePath.append("/");
//		filePath.append(segName);
//
//	}
//	fs.open(filePath.c_str());
//
//	if(fs.is_open()){
//
//		cout<<"[Thread - "<<threadId<<"] "<<"File opened/created successfully : "<<filePath<<endl;
//
//		cout<<"[Thread - "<<threadId<<"] "<<"Sending file to client : "<<clntIp<<endl;
//
//		//ignore sent bytes
//		fs.ignore(skipBytes);
//
//		//threadMap[clntIp].handOver will be updated by the main thread
//		while(totalSent < segSize){
//
//			fs.read((char *)readData, BUFSIZ);
//
//			readBytes = fs.gcount();
//			totalRead += readBytes;
//			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes read from file : "<<readBytes<<endl;
//			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes from file : "<<totalRead<<endl;
//
//			if((sentBytes = send(clntSockHndle, readData, BUFSIZ, 0))==-1){
//				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
//			}
//
//			totalSent += sentBytes;
//			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes sent to client : "<<sentBytes<<endl;
//			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client :"<<totalSent<<endl;
//
//			//update total bytes sent to client
//			threadMap[clntIp].bytesSent = totalSent;
//
//			if(totalSent >= segSize){
//				break;
//			}
//		}
//	}//if file is open
//
//	else{
//		cout<<"[Thread - "<<threadId<<"] "<<"Fail to open file : "<<filePath<<endl;
//	}//if file is not open
//
//	//close the file
//	fs.close();
//
//
//	/*---------------------------------------------------------------------------------------------------------------------------
//	 *
//	 * ---------------------------------------------------------------------------------------------------------------------------
//	*/
//
//	//static char recvChar[BUFSIZ];
//
//	if((read(clntSockHndle, recvChar, sizeof(recvChar)))==-1){
//		cerr<<"[Thread - "<<threadId<<"] "<<"sendFileTcp: Socket read error"<<endl;
//		//break;
//	}
//	else
//		cout<<"[Thread - "<<threadId<<"] "<<"sendFileTcp: Message received from client!"<<endl;
//
//	cout<<"[Thread - "<<threadId<<"] "<<"sendFileTcp: "<<recvChar<<endl;
//	//convert char array to string
//	recvMesg = string(recvChar);
//
//	//split message to tokens (tokens of strings)
//	stringstream str1(recvMesg);
//	while(!str1.eof()) {
//		string tmp;
//		str >> tmp;
//		tokens.push_back(tmp);
//
//	}
//
//	if(tokens[0].compare("SEG_ACK")==0){
//
//		long segAckValue = atol((char *)tokens[3].c_str());
//
//		contReqMap[mapKey].skipBytes = segAckValue;
//	}
//
//	//send segAck to controller
//	stringstream segAckss;
//	segAckss<<contReqMap[mapKey].segAck;
//	mesg = "SEG_ACK";
//	mesg=mesg+" "+connKeyss.str()+" "+segName+" "+segAckss.str();
//
//	if((send(ctrSockHndle, (char *)mesg.c_str(), mesg.length(), 0))==-1){
//		cerr<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Socket send error: mesg"<<endl;
//	}
//	else
//		cout<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Mesg sent successfully"<<endl;
//
//
//	cout<<"[Thread - "<<threadId<<"] "<<"Total bytes read from cache/controller :"<<totalRead<<endl;
//	cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client "<<clntIp<<" : "<<totalSent<<endl;
//
//	//close the socket
//	shutdown(clntSockHndle, 2);
//}

/*
 * send file/segment data to the client
 */

void CacheAgentMain::sndFileTcp(string mapKey){

	int threadId = threadMap[mapKey].threadId;
	string clntIp = threadMap[clientKey].clntIp; //get IP using the clientKey
	string fileName = contReqMap[mapKey].fileName;
	long fileSize = threadMap[mapKey].fileSize;
	long skipBytes = threadMap[mapKey].skipBytes;
	//long bytesSent = threadMap[mapKey].bytesSent; //not used
	//string finTrans = threadMap[mapKey].finTrans; //not used
	//string handOver = threadMap[mapKey].handOver; //not used
	int ctrSockHndle = threadMap[mapKey].ctrSockHndle;
	//const char *clntPort = threadMap[mapKey].clntPort; //already used, not needed here
	int clntSockHndle = threadMap[mapKey].clntSockHndle;
	string cachedStat = threadMap[mapKey].cachedStat;
	//int ctrSockHndleDat = threadMap[mapKey].ctrSockHndleDat; //not used
	string neighborAP = threadMap[mapKey].neighborAP;

	string::size_type sz;

	stringstream fileSizess, skipBytesss;
	fileSizess<<fileSize;
	skipBytesss<<skipBytes;
	string mesg = fileName+" "+fileSizess.str()+" "+skipBytesss.str();
	mesg += '\n';

	//print and send message to client
	//mesg format : <file name> <file size> <skip bytes>
	cout<<"[Thread - "<<threadId<<"] "<<mesg;
	if((send(clntSockHndle, (char *)mesg.c_str(), mesg.length(), 0))==-1){
		cerr<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Socket send error: mesg"<<endl;
	}
	else
		cout<<"[Thread - "<<threadId<<"] "<<"sndFileTcp: Mesg sent successfully"<<endl;

	char readData[BUFSIZ];
	long totalRead = 0, totalSent = skipBytes;
	long sentBytes = 0;
	streamsize readBytes = 0;

	std::fstream fs;
	string filePath;

	if(cachedStat.compare("CACHED")==0){

		cout<<"[Thread - "<<threadId<<"] "<<"CACHED"<<endl;

		//read from the cache/file
		filePath = CACHE_PATH;
		filePath.append(fileName);


	}
	else if (cachedStat.compare("NOT_CACHED")==0){

		cout<<"[Thread - "<<threadId<<"] "<<"NOT_CACHED"<<endl;
		cout<<"Fetching file from: "<<neighborAP<<endl;

		//read from the remote cache/file
		filePath = NEIGHBOR_CACHE_PATH;
		filePath.append(neighborAP);
		filePath.append("/");
		filePath.append(fileName);

	}

	fs.open(filePath.c_str());

	if(fs.is_open()){

		cout<<"[Thread - "<<threadId<<"] "<<"File opened/created successfully : "<<filePath<<endl;

		cout<<"[Thread - "<<threadId<<"] "<<"Sending file to client : "<<clntIp<<endl;

		//ignore sent bytes
		fs.ignore(skipBytes);

		//threadMap[clntIp].handOver will be updated by the main thread
		while(totalSent < fileSize){

			fs.read((char *)readData, BUFSIZ);

			readBytes = fs.gcount();
			totalRead += readBytes;
			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes read from file : "<<readBytes<<endl;
			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes from file : "<<totalRead<<endl;

			if((sentBytes = send(clntSockHndle, readData, BUFSIZ, 0))==-1){
				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
			}

			totalSent += sentBytes;
			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes sent to client : "<<sentBytes<<endl;
			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client :"<<totalSent<<endl;

			//update total bytes sent to client
			threadMap[clntIp].bytesSent = totalSent;

			if(totalSent >= fileSize){
				break;
			}
		}
	}//if file is open

	else{
		cout<<"[Thread - "<<threadId<<"] "<<"Fail to open file : "<<filePath<<endl;
	}//if file is not open

	//close the file
	fs.close();

	sndFinishUpdate(clntIp);

	cout<<"[Thread - "<<threadId<<"] "<<"Total bytes read from file/controller :"<<totalRead<<endl;
	cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client "<<clntIp<<" : "<<totalSent<<endl;

	//delete file transfer entry
	threadMap.erase(clntIp);

	//close the socket
	shutdown(clntSockHndle, 2);
}

/*
 * send update message using
 * UDP protocol
 */

void CacheAgentMain::udpUpdate(string mapKey, int updateCode){

	int threadId = threadMap[mapKey].threadId;

	int ctrUpdateSock;
	const char *ctrAddr =  ctrAddr;
	const char *ctrUdpPort = ctrUdpPort;
	struct addrinfo ctrUpdateAddrInfo;
	struct addrinfo *ctrUpdateAddrInfoList;

	ctrUpdateAddrInfoList = NULL;
	memset(&ctrUpdateAddrInfo, 0, sizeof(ctrUpdateAddrInfo));

	ctrUpdateAddrInfo.ai_family = AF_UNSPEC;
	ctrUpdateAddrInfo.ai_socktype = SOCK_DGRAM;

	if(getaddrinfo(ctrAddr, ctrUdpPort, &ctrUpdateAddrInfo, &ctrUpdateAddrInfoList)!=0)
		cerr<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Getaddrinfo error: udp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Getaddrinfo success: udp"<<endl;

	if((ctrUpdateSock=socket(ctrUpdateAddrInfoList->ai_family,ctrUpdateAddrInfoList->ai_socktype,
			ctrUpdateAddrInfoList->ai_protocol))==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Socket error: udp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Socket created: udp"<<endl;

	string transfrStat = "";

	if(updateCode==1){

		stringstream clientKeyss;
		clientKeyss<<contReqMap[mapKey].clientKey;

		NetworkInterface iface = NetworkInterface::default_interface();
		NetworkInterface::Info info = iface.addresses();
		string agentKey =  info.hw_addr.to_string();

		transfrStat  = "WIFI_CLIENT_ADD";
		transfrStat += " "+clientKeyss.str()+" "+agentKey;
		transfrStat += '\n';
	}

	else if(updateCode==2){

			stringstream clientKeyss;
			clientKeyss<<contReqMap[mapKey].clientKey;

			NetworkInterface iface = NetworkInterface::default_interface();
			NetworkInterface::Info info = iface.addresses();
			string agentKey =  info.hw_addr.to_string();

			 transfrStat  = "WIFI_CLIENT_REMOVE";
			transfrStat += " "+clientKeyss.str()+" "+agentKey;
			transfrStat += '\n';
		}


	//print update and send to controller
	cout<<"[Thread - "<<threadId<<"] "<<transfrStat;
	if((sendto(ctrUpdateSock, transfrStat.c_str(), transfrStat.length(), 0, ctrUpdateAddrInfoList->ai_addr,
			ctrUpdateAddrInfoList->ai_addrlen))==-1){
		cerr<<"[Thread - "<<threadId<<"] "<<"updUpdate: Socket send error: udp update"<<endl;
	}
	else
		cout<<"[Thread - "<<threadId<<"] "<<"udpUpdate: Update sent successfully: udp"<<endl;

}

/*
 * update the controller of the data transfer
 * completion to the client
 */

void CacheAgentMain::sndFinishUpdate(string mapKey){

	int threadId = threadMap[mapKey].threadId;

	int ctrUpdateSock;
	const char *ctrAddr =  ctrAddr;
	const char *ctrUdpPort = ctrUdpPort;
	struct addrinfo ctrUpdateAddrInfo;
	struct addrinfo *ctrUpdateAddrInfoList;

	ctrUpdateAddrInfoList = NULL;
	memset(&ctrUpdateAddrInfo, 0, sizeof(ctrUpdateAddrInfo));

	ctrUpdateAddrInfo.ai_family = AF_UNSPEC;
	ctrUpdateAddrInfo.ai_socktype = SOCK_DGRAM;

	if(getaddrinfo(ctrAddr, ctrUdpPort, &ctrUpdateAddrInfo, &ctrUpdateAddrInfoList)!=0)
		cerr<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Getaddrinfo error: udp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Getaddrinfo success: udp"<<endl;

	if((ctrUpdateSock=socket(ctrUpdateAddrInfoList->ai_family,ctrUpdateAddrInfoList->ai_socktype,
			ctrUpdateAddrInfoList->ai_protocol))==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Socket error: udp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Socket created: udp"<<endl;

	string transfrStat = "FILE_SENT";
	transfrStat += " " + threadMap[mapKey].fileName+" "+threadMap[mapKey].clntIp;
	transfrStat += '\n';

	//print update and send to controller
	cout<<"[Thread - "<<threadId<<"] "<<transfrStat;
	if((sendto(ctrUpdateSock, transfrStat.c_str(), transfrStat.length(), 0, ctrUpdateAddrInfoList->ai_addr,
			ctrUpdateAddrInfoList->ai_addrlen))==-1){
		cerr<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Socket send error: udp update"<<endl;
	}
	else
		cout<<"[Thread - "<<threadId<<"] "<<"sndFinishUpdate: Update sent successfully: udp"<<endl;

}

/*
 * keep updating the data transfer status to
 * the controller
 */

void CacheAgentMain::sndUpdate(string mapKey){

	int threadId = threadMap[mapKey].threadId;

	int ctrUpdateSock;
	const char *ctrAddr =  ctrAddr;
	const char *ctrUdpPort = ctrUdpPort;
	struct addrinfo ctrUpdateAddrInfo;
	struct addrinfo *ctrUpdateAddrInfoList;

	ctrUpdateAddrInfoList = NULL;
	memset(&ctrUpdateAddrInfo, 0, sizeof(ctrUpdateAddrInfo));

	ctrUpdateAddrInfo.ai_family = AF_UNSPEC;
	ctrUpdateAddrInfo.ai_socktype = SOCK_DGRAM;

	if(getaddrinfo(ctrAddr, ctrUdpPort, &ctrUpdateAddrInfo, &ctrUpdateAddrInfoList)!=0)
		cerr<<"[Thread - "<<threadId<<"] "<<"Getaddrinfo error : udp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Getaddrinfo success : udp"<<endl;

	if((ctrUpdateSock=socket(ctrUpdateAddrInfoList->ai_family,ctrUpdateAddrInfoList->ai_socktype,
			ctrUpdateAddrInfoList->ai_protocol))==-1)
		cerr<<"[Thread - "<<threadId<<"] "<<"Socket error : udp"<<endl;
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Socket created : udp"<<endl;

	stringstream fileSizess, bytesSentss;
	fileSizess<<threadMap[mapKey].fileSize;
	bytesSentss<<threadMap[mapKey].bytesSent;

	string transfrStat = "CLNT_STAT";
	transfrStat += " " + threadMap[mapKey].clntIp + " " + threadMap[mapKey].finTrans + " " + threadMap[mapKey].fileName
					+ " " + fileSizess.str() + " " + bytesSentss.str();
	transfrStat += '\n';

	//print update and send to controller
	cout<<"[Thread - "<<threadId<<"] "<<transfrStat;
	if((sendto(ctrUpdateSock, transfrStat.c_str(), transfrStat.length(), 0, ctrUpdateAddrInfoList->ai_addr,
			ctrUpdateAddrInfoList->ai_addrlen))==-1){
		cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : udp update"<<endl;
	}
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Update sent successfully : udp"<<endl;

}

/*
 * send file/segment data to the client
 * and handles mobility (which may result in
 * handoff)
 */

void CacheAgentMain::sndFileTcpMobility(string mapKey){

	int threadId = threadMap[mapKey].threadId;
	string clntIp = threadMap[mapKey].clntIp;
	string fileName = threadMap[mapKey].fileName;
	long fileSize = threadMap[mapKey].fileSize;
	long skipBytes = threadMap[mapKey].skipBytes;
	long bytesSent = threadMap[mapKey].bytesSent;
	//string finTrans = threadMap[mapKey].finTrans; //not used
	//string handOver = threadMap[mapKey].handOver; //not used
	int ctrSockHndle = threadMap[mapKey].ctrSockHndle;
	//const char *clntPort = threadMap[mapKey].clntPort; //already used, not needed here
	int clntSockHndle = threadMap[mapKey].clntSockHndle;
	string cachedStat = threadMap[mapKey].cachedStat;
	//int ctrSockHndleDat = threadMap[mapKey].ctrSockHndleDat; //not used

	//if some bytes has been sent to the client already
	//get the number of bytes received the client so far
	if(bytesSent>0){

		char recvChar[BUFSIZ];

		//cout<<"here"<<endl;
		//read update from client
		if((read(clntSockHndle, recvChar, sizeof(recvChar)))==-1){
			cerr<<"[Thread - "<<threadId<<"] "<<"Socket read error"<<endl;
			//break;
		}
		else
			cout<<"[Thread - "<<threadId<<"] "<<"Update received from client!"<<endl;

		cout<<"[Thread - "<<threadId<<"] "<<recvChar<<endl;

		string recvMesg = string(recvChar);

		//split message to tokens (tokens of strings)
		vector<string> tokens;
		stringstream str(recvMesg);
		while(!str.eof()) {
			string tmp;
			str >> tmp;
			tokens.push_back(tmp);
		}

		long bytesSentRecv = atol((char *)tokens[2].c_str());

		//update bytes sent with info received from client
		if((tokens[0].compare("SEG_UPDATE")==0) && (tokens[1].compare(fileName.c_str())==0)
				&& (bytesSent!=bytesSentRecv)){
			threadMap[clntIp].bytesSent = atol((char *)tokens[2].c_str());

			//get the updated bytesSent again
			bytesSent = threadMap[clntIp].bytesSent;
		}

		//update the controller with latest info
		sndUpdate(clntIp);
	}

	string::size_type sz;

	stringstream fileSizess, bytesSentss;
	fileSizess<<fileSize;
	bytesSentss<<bytesSent;
	string mesg = fileName+" "+fileSizess.str()+" "+bytesSentss.str();
	mesg += '\n';

	//print and send message to client
	//mesg format : <file name> <file size> <bytes sent already>
	cout<<"[Thread - "<<threadId<<"] "<<mesg;
	if((send(clntSockHndle, (char *)mesg.c_str(), mesg.length(), 0))==-1){
		cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : mesg"<<endl;
	}
	else
		cout<<"[Thread - "<<threadId<<"] "<<"Mesg sent successfully"<<endl;

	//read from the cache/file
	string filePath = CACHE_PATH;
	filePath.append(fileName);
	std::fstream fs;

	char readData[BUFSIZ];
	long totalRead = 0, totalSent = bytesSent;
	long sentBytes = 0;
	streamsize readBytes = 0;

	if(cachedStat.compare("CACHED")==0){

		cout<<"[Thread - "<<threadId<<"] "<<"CACHED"<<endl;

		fs.open(filePath.c_str());

			if(fs.is_open()){

			cout<<"[Thread - "<<threadId<<"] "<<"File opened/created successfully : "<<filePath<<endl;


			cout<<"[Thread - "<<threadId<<"] "<<"Sending file to client : "<<clntIp<<endl;

			//ignore sent bytes
			fs.ignore(skipBytes);

			//threadMap[clntIp].handOver will be updated by the main thread
			while((totalSent < fileSize) && (threadMap[clntIp].handOver.compare("false")==0)){

				fs.read((char *)readData, BUFSIZ);

				readBytes = fs.gcount();
				totalRead += readBytes;
				//cout<<"[Thread - "<<threadId<<"] "<<"Bytes read from file : "<<readBytes<<endl;
				//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes from file : "<<totalRead<<endl;

				if((sentBytes = send(clntSockHndle, readData, BUFSIZ, 0))==-1){
					cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
				}

				totalSent += sentBytes;
				//cout<<"[Thread - "<<threadId<<"] "<<"Bytes sent to client : "<<sentBytes<<endl;
				//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client :"<<totalSent<<endl;

				//update total bytes sent to client
				threadMap[clntIp].bytesSent = totalSent;

				if(totalSent >= fileSize){
					break;
				}
			}
		}

		else{
				cout<<"[Thread - "<<threadId<<"] "<<"Fail to open file : "<<filePath<<endl;
		}
	}//cached

	else if(cachedStat.compare("NOT_CACHED")==0){

		cout<<"[Thread - "<<threadId<<"] "<<"NOT_CACHED"<<endl;

		//if cache is full, delete file here
		//create file if it does not exist
		fs.open(filePath.c_str(), ios::out | ios::binary | ios::trunc);

		if(fs.is_open()){

			cout<<"[Thread - "<<threadId<<"] "<<"File opened/created successfully : "<<filePath<<endl;

			//threadMap[clntIp].handOver will be updated by the main thread
			while((totalSent < fileSize) && (threadMap[clntIp].handOver.compare("false")==0)){

				//read data/message from socket as char array
				if((readBytes = recv(ctrSockHndleDat, (void *) readData, sizeof(readData), 0))==-1){
					cerr<<"[Thread - "<<threadId<<"] "<<"Socket read error : data from controller"<<endl;
					//break;
				}

				totalRead += readBytes;
				//cout<<"[Thread - "<<threadId<<"] "<<"Bytes read from controller : "<<readBytes<<endl;
				//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes from controller : "<<totalRead<<endl;

				if((sentBytes = send(clntSockHndle, readData, readBytes, 0))==-1){
					cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
				}

				//write to file/cache
				fs<<(void *)readData;

				totalSent += sentBytes;
				//cout<<"[Thread - "<<threadId<<"] "<<"Bytes sent to client : "<<sentBytes<<endl;
				//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client :"<<totalSent<<endl;

				//update total bytes sent to client
				threadMap[clntIp].bytesSent = totalSent;

				if(totalSent >= fileSize){
					break;
				}
			}
		}

		else{
				cout<<"[Thread - "<<threadId<<"] "<<"Fail to open file : "<<filePath<<endl;
		}
	}//not cached

	//close the file
	fs.close();

	//file sent complete
	if(totalSent>=fileSize){
		/*
		//send the last block of bytes again
		if(cachedStat.compare("CACHED")==0){
			if((sentBytes = send(clntSockHndle, readData, BUFSIZ, 0))==-1){
				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
			}
		}
		else if(cachedStat.compare("NOT_CACHED")==0){
			if((sentBytes = send(clntSockHndle, readData, readBytes, 0))==-1){
				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
			}
		}
		*/
		cout<<"[Thread - "<<threadId<<"] "<<"File : "<<fileName<<" sent complete!"<<endl;

		//update the controller with latest info
		threadMap[clntIp].finTrans = "true";

		sndFinishUpdate(mapKey);
		//sndUpdate(mapKey);
	}

	//hand-over message received from the controller
	else if(threadMap[clntIp].handOver.compare("true")==0){

		cout<<"[Thread - "<<threadId<<"] "<<"HANDOVER!!"<<endl;

		/*
		//update mobility manager
		stringstream fileSizess, bytesSentss;
		fileSizess<<threadMap[clntIp].fileSize;
		bytesSentss<<threadMap[clntIp].bytesSent;

		string transfrStat = "TRANSFR_STAT";
		transfrStat += " " + threadMap[clntIp].clntIp + " " + threadMap[clntIp].finTrans + " " + threadMap[clntIp].fileName
				+ " " + fileSizess.str() + " " + bytesSentss.str();

		transfrStat += '\n';

		//print update and send to controller
		cout<<"[Thread - "<<threadId<<"] "<<transfrStat;

		if((send(ctrSockHndle, (void *)transfrStat.c_str(), (size_t)strlen(transfrStat.c_str()), 0))==-1){
			cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : tcp update"<<endl;
		}
		else
			cout<<"[Thread - "<<threadId<<"] "<<"Update sent successfully : tcp"<<endl;
		*/
		//sndFinishUpdate(mapKey);
		sndUpdate(mapKey);
	}

	cout<<"[Thread - "<<threadId<<"] "<<"Total bytes read from file/controller :"<<totalRead<<endl;
	cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client "<<clntIp<<" : "<<totalSent<<endl;

	//delete file transfer entry
	threadMap.erase(clntIp);

	//close the socket
	shutdown(clntSockHndle, 2);
}

/*
 * setup sctp socket that has one stream
 */

int CacheAgentMain::setupSockSctpOne(int threadId, const char *clntIp, unsigned short int clntPort, string fileSize,
		string fileName){

	int connSock, in;
	struct sockaddr_in servAddr;
	struct sctp_status status;
	struct sctp_event_subscribe events;
	struct sctp_initmsg initMsg;

	/* Create an SCTP TCP-Style Socket */
	connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

	/* Specify that a maximum of 5 streams will be available per socket */
	memset(&initMsg, 0, sizeof(initMsg));
	initMsg.sinit_num_ostreams = MAX_OSTREAMS_SCTP;
	initMsg.sinit_max_instreams = MAX_INSTREAMS_SCTP;
	initMsg.sinit_max_attempts = MAX_ATTEMPTS;
	setsockopt(connSock, IPPROTO_SCTP, SCTP_INITMSG, &initMsg, sizeof(initMsg));

	/* Specify the peer endpoint to which we'll connect */
	bzero((void *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(clntPort);
	servAddr.sin_addr.s_addr = inet_addr(clntIp);

	/* Connect to the server */
	connect(connSock, (struct sockaddr *)&servAddr, sizeof(servAddr));

	/* Enable receipt of SCTP Snd/Rcv Data via sctp_recvmsg */
	memset( (void *)&events, 0, sizeof(events) );
	events.sctp_data_io_event = 1;
	setsockopt( connSock, SOL_SCTP, SCTP_EVENTS,(const void *)&events, sizeof(events) );

	/* Read and emit the status of the Socket (optional step) */
	in = sizeof(status);
	getsockopt( connSock, SOL_SCTP, SCTP_STATUS,
	(void *)&status, (socklen_t *)&in );

	cout<<"[Thread - "<<threadId<<"] "<<"association id : "<<status.sstat_assoc_id<<endl;
	cout<<"[Thread - "<<threadId<<"] "<<"association state : "<<status.sstat_state<<endl;
	cout<<"[Thread - "<<threadId<<"] "<<"instreams : "<<status.sstat_instrms<<endl;
	cout<<"[Thread - "<<threadId<<"] "<<"outstreams : "<<status.sstat_outstrms<<endl;

	sndFileSctpOne(threadId, clntIp, fileName, fileSize, connSock, servAddr);

	return connSock;
}

/*
 * send data over sctp socket that has
 * one stream
 */

void CacheAgentMain::sndFileSctpOne(int threadId, string clntIp, string fileName, string fileSize, int connSock,
		struct sockaddr_in servAddr){

	//struct sctp_sndrcvinfo sndrcvinfo;
	string::size_type sz;
	long fileSizeLong = atol((char *)fileSize.c_str());//for loop exit only

	//send file size to client
	//fileSize += '\n';
	if((send(connSock, (void *)fileSize.c_str(), (size_t)strlen(fileSize.c_str()), 0))==-1){
		cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : file size"<<endl;
	}
	else
		cout<<"[Thread - "<<threadId<<"] "<<"File size sent successfully"<<endl;

	//read from the cache/file
	string filePath = CACHE_PATH;
	filePath.append(fileName);
	//cout<<"[Thread - "<<threadId<<"] "<<filePath<<endl;
	std::fstream fs;
	fs.open(filePath.c_str());

	if(fs.is_open())
	{
		cout<<"[Thread - "<<threadId<<"] "<<"File : "<<filePath<<" opened successfully : "<<filePath<<endl;
		unsigned char sendData[BUFSIZ];
		long totalRecv = 0, totalSent = 0;
		int sentBytes = 0;
		streamsize recvBytes = 0;

		cout<<"[Thread - "<<threadId<<"] "<<"Sending file to client : "<<clntIp<<endl;
		while(totalSent < fileSizeLong){
			fs.read((char *)sendData, BUFSIZ);

			recvBytes = fs.gcount();
			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes read : "<<recvBytes<<endl;
			totalRecv += recvBytes;
			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes read : "<<totalRecv<<endl;

			if((sentBytes = send(connSock, (void *)sendData, recvBytes, 0))==-1){
				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
			}

			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes sent : "<<sentBytes<<endl;
			totalSent += sentBytes;
			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent :"<<totalSent<<endl;

			if(totalSent >= fileSizeLong){
				break;
			}
		}

		cout<<"[Thread - "<<threadId<<"] "<<"Total bytes read from file :"<<totalRecv<<endl;
		cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client "<<clntIp<<" : "<<totalSent<<endl;
		fs.close();
	}
	else{
		cout<<"[Thread - "<<threadId<<"] "<<"Fail to open file : "<<filePath<<endl;
	}

	//close the socket
	shutdown(connSock, 2);

}

/*
 * setup sctp socket that has
 * multiple streams
 */

int CacheAgentMain::setupSockSctpMany(int threadId, const char *clntIp, unsigned short int clntPort, string fileSize,
		string fileName){

	int connSock;// in;
	struct sockaddr_in servAddr;
	//struct sctp_status status;
	//struct sctp_event_subscribe events;
	//struct sctp_initmsg initMsg;

	/* Create an SCTP UDP-Style Socket */
	connSock = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

	/* Specify that a maximum of 5 streams will be available per socket */
	//memset(&initMsg, 0, sizeof(initMsg));
	//initMsg.sinit_num_ostreams = 5;
	//initMsg.sinit_max_instreams = 5;
	//initMsg.sinit_max_attempts = 4;
	//setsockopt(connSock, IPPROTO_SCTP, SCTP_INITMSG, &initMsg, sizeof(initMsg));

	/* Specify the peer endpoint to which we'll connect */
	bzero((void *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(clntPort);
	servAddr.sin_addr.s_addr = inet_addr(clntIp);

	sndFileSctpMany(threadId, clntIp, fileName, fileSize, connSock, servAddr);

	return connSock;
}

/*
 * send data over sctp socket that
 * has multiple streams
 */

void CacheAgentMain::sndFileSctpMany(int threadId, string clntIp, string fileName, string fileSize, int connSock,
		struct sockaddr_in servAddr){

	//struct sctp_sndrcvinfo sndrcvinfo;
	string::size_type sz;
	long fileSizeLong = atol((char *)fileSize.c_str());//for loop exit only

	//send file size to client
	//fileSize += '\n';
	if((sctp_sendmsg(connSock, (void *)fileSize.c_str(), (size_t)strlen(fileSize.c_str()),
			(struct sockaddr *)&servAddr, sizeof(servAddr), 0, 0, 0, 0, 0))==-1){
		cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : file size"<<endl;
	}
	else
		cout<<"[Thread - "<<threadId<<"] "<<"File size sent successfully"<<endl;

	//read from the cache/file
	string filePath = CACHE_PATH;
	filePath.append(fileName);
	//cout<<"[Thread - "<<threadId<<"] "<<filePath<<endl;
	std::fstream fs;
	fs.open(filePath.c_str());

	if(fs.is_open())
	{
		cout<<"[Thread - "<<threadId<<"] "<<"File : "<<filePath<<" opened successfully : "<<filePath<<endl;
		unsigned char sendData[BUFSIZ];
		long totalRecv = 0, totalSent = 0;
		int sentBytes = 0;
		streamsize recvBytes = 0;

		cout<<"[Thread - "<<threadId<<"] "<<"Sending file to client : "<<clntIp<<endl;
		while(totalSent < fileSizeLong){
			fs.read((char *)sendData, BUFSIZ);

			recvBytes = fs.gcount();
			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes read : "<<recvBytes<<endl;
			totalRecv += recvBytes;
			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes read : "<<totalRecv<<endl;

			if((sentBytes = sendto(connSock, (void *)sendData, recvBytes, 0,
					(struct sockaddr *)&servAddr, sizeof(servAddr)))==-1){
				cerr<<"[Thread - "<<threadId<<"] "<<"Socket send error : data"<<endl;
			}

			//cout<<"[Thread - "<<threadId<<"] "<<"Bytes sent : "<<sentBytes<<endl;
			totalSent += sentBytes;
			//cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent :"<<totalSent<<endl;

			if(totalSent >= fileSizeLong){
				break;
			}
		}

		cout<<"[Thread - "<<threadId<<"] "<<"Total bytes read from file :"<<totalRecv<<endl;
		cout<<"[Thread - "<<threadId<<"] "<<"Total bytes sent to client "<<clntIp<<" : "<<totalSent<<endl;
		fs.close();
	}
	else{
		cout<<"[Thread - "<<threadId<<"] "<<"Fail to open file : "<<filePath<<endl;
	}

	//close the socket
	shutdown(connSock, 2);

}

bool CacheAgentMain::dot11AssocResp(PDU &some_pdu){

	Dot11AssocResponse &assocResp = some_pdu.rfind_pdu<Dot11AssocResponse>();

	string clientMAC = assocResp.addr1().to_string();//dest, client
	if(wifiConnMap.find(clientMAC)!=wifiConnMap.end()){

		if(wifiConnMap[clientMAC].status.compare("disassociated")==0){

			cout<<"Assoc: Client entry exists and disassociated, will be replaced!"<<endl;

			wifiConnParam param;
			param.assocID = assocResp.aid();
			param.clientIP = ""; //to be filled by DHCP thread
			param.status = "associated";
			//add the new entry
			wifiConnMap.erase(clientMAC);
			wifiConnMap.emplace(clientMAC,param);

			CacheAgentMain temp;
			temp.printConnMap(clientMAC);

		}
	}

	else{

		cout<<"Assoc: Client entry does not exists, associated for first time!"<<endl;

		wifiConnParam param;
		param.assocID = assocResp.aid();
		param.clientIP = "";
		param.status = "associated";
		//add the new entry
		wifiConnMap.emplace(clientMAC,param);

		CacheAgentMain temp;
		temp.printConnMap(clientMAC);
	}

	return true;
}


int CacheAgentMain::startAssocSniff(){

	cout<<"Association thread started"<<endl;
	SnifferConfiguration config;
	config.set_promisc_mode(true);
	// config.set_filter("ip src 192.168.1.133");
	Sniffer sniffer("mon0", config);
	sniffer.sniff_loop(make_sniffer_handler(this, &CacheAgentMain::dot11AssocResp));

	return 0;

}

bool CacheAgentMain::dot11DisAssocResp(PDU &some_pdu){

	Dot11Deauthentication &deAuth = some_pdu.rfind_pdu<Dot11Deauthentication>();

	cout<<"Disassocation/Deauthentication code: "<<deAuth.reason_code()<<endl;

	string clientMAC = deAuth.addr2().to_string(); //src, client
	if(wifiConnMap.find(clientMAC)!=wifiConnMap.end()){

		if(wifiConnMap[clientMAC].status.compare("associated")==0){
			cout<<"DisAssoc: Client entry exists and associated, will be disassociated!"<<endl;

			wifiConnMap[clientMAC].status="disassociated";

			CacheAgentMain temp;
			temp.printConnMap(clientMAC);

		}

	}
	else{
		cout<<"DisAssoc: Client entry does not exist!!"<<endl;
	}



	return true;
}

int CacheAgentMain::startDisAssocSniff(){

	cout<<"Disassociation thread started!"<<endl;
	SnifferConfiguration config;
	config.set_promisc_mode(true);
	// config.set_filter("ip src 192.168.1.133");
	Sniffer sniffer("mon0", config);
	sniffer.sniff_loop(make_sniffer_handler(this, &CacheAgentMain::dot11DisAssocResp));

	return 0;

}


bool CacheAgentMain::dhcpAck(PDU &some_pdu){

	UDP &udp = some_pdu.rfind_pdu<UDP>();
	IP &ip = some_pdu.rfind_pdu<IP>();

	//DHCPACK
	if(udp.sport()==67 &&  udp.dport()==68 && (ip.dst_addr().to_string().compare("255.255.255.255")!=0)){

		DHCP dhcp = some_pdu.rfind_pdu<RawPDU>().to<DHCP>();
		Dot11Data &dot11 = some_pdu.rfind_pdu<Dot11Data>();

		string clientMAC = dot11.addr1().to_string(); //dest, client
		string clientIP = dhcp.yiaddr().to_string();// IP assigned

		if(wifiConnMap.find(clientMAC)!=wifiConnMap.end()){

			if(wifiConnMap[clientMAC].status.compare("associated")==0){
				cout<<"DHCP: Client entry exists and associated, IP assigned now!"<<endl;
				wifiConnMap[clientMAC].clientIP = clientIP;

				CacheAgentMain temp;
				temp.printConnMap(clientMAC);

			}

		}
		else{
			cerr<<"DHCP: Client entry does not exist!"<<endl;
		}

	}

	return true;
}

int CacheAgentMain::startDhcpSniff(){

	cout<<"DHCP thread started!"<<endl;
	SnifferConfiguration config;
	config.set_promisc_mode(true);
	// config.set_filter("ip src 192.168.1.133");
	Sniffer sniffer("mon0", config);
	sniffer.sniff_loop(make_sniffer_handler(this, &CacheAgentMain::dhcpAck));

	return 0;

}

void CacheAgentMain::printConnMap(string key){

	cout<<"\t"<<"ClientMAC: "<<key<<endl;
	cout<<"\t"<<"Assoc ID: "<<wifiConnMap[key].assocID<<endl;
	cout<<"\t"<<"ClientIP: "<<wifiConnMap[key].clientIP<<endl;
	cout<<"\t"<<"Status: "<<wifiConnMap[key].status<<endl;

}


int main(){
	CacheAgentMain cacheAgent;

//	thread t1(&CacheAgentMain::startAssocSniff, &cacheAgent);
//	thread t2(&CacheAgentMain::startDisAssocSniff, &cacheAgent);
//	thread t3(&CacheAgentMain::startDhcpSniff, &cacheAgent);
//
//	t1.join();
//	t2.join();
//	t3.join();

	cacheAgent.startAgnt();

	cout<<"Exiting main thread"<<endl;

	return 0;
}
