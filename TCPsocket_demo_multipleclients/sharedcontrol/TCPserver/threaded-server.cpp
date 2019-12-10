/***********************************************************************
 threaded-server.cpp - Implements a simple Winsock server that accepts
    connections and spins each one off into its own thread, where it's
    treated as a blocking socket.

    Each connection thread reads data off the socket and echoes it
    back verbatim.

 Compiling:
    VC++: cl -GX threaded-server.cpp main.cpp ws-util.cpp wsock32.lib
    BC++: bcc32 threaded-server.cpp main.cpp ws-util.cpp

 This program is hereby released into the public domain.  There is
 ABSOLUTELY NO WARRANTY WHATSOEVER for this product.  Caveat hacker.
***********************************************************************/

#include "ws-util.h"

#include <winsock.h>

#include <iostream>

using namespace std;


//// Constants /////////////////////////////////////////////////////////

const int kBufferSize = 1024;

// For reading from file
using namespace std;
#include <string>
#include <iostream>
#include <fstream>
// Set up file for reading - if not working, maybe try replacing the '/' with '\\'
string docPath_touch = "C:/Users/numur/Downloads/touch_locations.txt";
string docPath_person = "C:/Users/numur/Desktop/darpa/darpahaptics/HapticDisplay/person_position.txt";
int filefetch_wait = 100; // Milliseconds between trying to open file

#define DEFAULT_BUFLEN 100 // Max length of string to send over TCP socket
#define MAX_STRING 10000 // Maximum length of text file string 
 //The number of characters to save to determine if a new string has been opened
#define MAX_STRING_SAVE_PREV 100  

//// Prototypes ////////////////////////////////////////////////////////

SOCKET SetUpListener(const char* pcAddress, int nPort);
void AcceptConnections(SOCKET ListeningSocket);
bool EchoIncomingPackets(SOCKET sd);

//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.

int DoWinsock(const char* pcAddress, int nPort)
{
    cout << "Establishing the listener..." << endl;
    SOCKET ListeningSocket = SetUpListener(pcAddress, htons(nPort));
    if (ListeningSocket == INVALID_SOCKET) {
        cout << endl << WSAGetLastErrorMessage("establish listener") <<
                endl;
        return 3;
    }

    cout << "Waiting for connections..." << flush;
    while (1) {
        AcceptConnections(ListeningSocket);
        cout << "Acceptor restarting..." << endl;
    }

#if defined(_MSC_VER)
    return 0;   // warning eater
#endif
}


//// SetUpListener /////////////////////////////////////////////////////
// Sets up a listener on the given interface and port, returning the
// listening socket if successful; if not, returns INVALID_SOCKET.

SOCKET SetUpListener(const char* pcAddress, int nPort)
{
    u_long nInterfaceAddr = inet_addr(pcAddress);
    if (nInterfaceAddr != INADDR_NONE) {
        SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
        if (sd != INVALID_SOCKET) {
            sockaddr_in sinInterface;
            sinInterface.sin_family = AF_INET;
            sinInterface.sin_addr.s_addr = nInterfaceAddr;
            sinInterface.sin_port = nPort;
            if (bind(sd, (sockaddr*)&sinInterface,
                    sizeof(sockaddr_in)) != SOCKET_ERROR) {
                listen(sd, SOMAXCONN);
                return sd;
            }
            else {
                cerr << WSAGetLastErrorMessage("bind() failed") <<
                        endl;
            }
        }
    }

    return INVALID_SOCKET;
}


//// EchoHandler ///////////////////////////////////////////////////////
// Handles the incoming data by reflecting it back to the sender.

DWORD WINAPI EchoHandler(void* sd_)
{
    int nRetval = 0;
    SOCKET sd = (SOCKET)sd_;

    if (!EchoIncomingPackets(sd)) {
        cerr << endl << WSAGetLastErrorMessage(
                "Echo incoming packets failed") << endl;
        nRetval = 3;
    }

    cout << "Shutting connection down..." << flush;
    if (ShutdownConnection(sd)) {
        cout << "Connection is down." << endl;
    }
    else {
        cerr << endl << WSAGetLastErrorMessage(
                "Connection shutdown failed") << endl;
        nRetval = 3;
    }

    return nRetval;
}


//// AcceptConnections /////////////////////////////////////////////////
// Spins forever waiting for connections.  For each one that comes in,
// we create a thread to handle it and go back to waiting for
// connections.  If an error occurs, we return.

void AcceptConnections(SOCKET ListeningSocket)
{
    sockaddr_in sinRemote;
    int nAddrSize = sizeof(sinRemote);

    while (1) {
        SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote,
                &nAddrSize);
        if (sd != INVALID_SOCKET) {
            cout << "Accepted connection from " <<
                    inet_ntoa(sinRemote.sin_addr) << ":" <<
                    ntohs(sinRemote.sin_port) << "." <<
                    endl;

            DWORD nThreadID;
            CreateThread(0, 0, EchoHandler, (void*)sd, 0, &nThreadID);
        }
        else {
            cerr << WSAGetLastErrorMessage("accept() failed") <<
                    endl;
            return;
        }
    }
}


//// EchoIncomingPackets ///////////////////////////////////////////////
// Bounces any incoming packets back to the client.  We return false
// on errors, or true if the client closed the socket normally.

bool EchoIncomingPackets(SOCKET sd)
{
	// Set up file for reading 
	ifstream file_touchlocations;
	string string_of_coordinates; // Variable to read from file
	string string_of_coordinates_prev; // Previous read to check to see if they are the same
	bool strings_equal; // boolean to record whether string_of_coordinates==string_of_coordinates_prev
	int len_string_of_coordinates;

	// Initialize character arrays for handling and sending strings
	char recvbuf[MAX_STRING];
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf_prev[MAX_STRING_SAVE_PREV];
	char recvbuf_section[DEFAULT_BUFLEN];
	char recvbuf_person[DEFAULT_BUFLEN];

	// Main loop
	// Variables for seperating and sending string in sections
	int flag = 0;
	int flag2;
	int i, i_temp, i_start, i_end = 0, comma_index = 0, end_loop_here;
	bool endhere = false;
	int recvbuf_1len;
	bool done_message_sent = true;
	bool first_iter = true;
	int nReadBytes;

	// Send string messages to linux machine
	flag = 0; // For while loop
	do {
		// Read file
		flag2 = 0; // For determining if the file has been opened properly
		do {
			file_touchlocations.open(docPath_touch);
			// Check is file has been opened properly, if not wait 
			// (important because other program is opening and writing to this file)
			if (file_touchlocations.is_open())
			{
				flag2 = 1;
				//printf("File opened \n");
			}
			else {
				Sleep(filefetch_wait);
				printf("failed to open \n");
			}
		} while (flag2 == 0);
		getline(file_touchlocations, string_of_coordinates);
		file_touchlocations.close();

		// Format to char for 
		string_of_coordinates.copy(recvbuf, string_of_coordinates.size() + 1);
		recvbuf[string_of_coordinates.size()] = '\0'; // terminate char
		recvbuflen = string_of_coordinates.size();

		i = 0;
		if (recvbuflen == 0) {
			strings_equal = 1;
		}
		else {
			strings_equal = (recvbuf_prev[i] == recvbuf[i]);
			while (strings_equal && (i < MAX_STRING_SAVE_PREV)) {
				strings_equal = (strings_equal && (recvbuf_prev[i] == recvbuf[i]));
				i++;
				//printf("%d \n ", strings_equal);
			}
		}

		// If string is new, 
		if (strings_equal) {
			// Don't do anything
		}
		else {
			printf("New string \n");

			// Save as new previous string
			for (i = 0; i < MAX_STRING_SAVE_PREV; i++) {
				recvbuf_prev[i] = recvbuf[i];
			}

			// At each iteration, send the next section of string
			i_start = 0; // Starting index in string for next section
			do {
				// Copy the coordinates to recvbuf_section by iterating through characters
				if ((DEFAULT_BUFLEN - 2) < recvbuflen)
					end_loop_here = (DEFAULT_BUFLEN - 2);
				else
					end_loop_here = recvbuflen;
				comma_index = 0;
				for (i = 0; i < end_loop_here; i++) {
					recvbuf_section[i] = recvbuf[i_start + i];
					if ((int)recvbuf_section[i] == 44)
						comma_index = comma_index + 1;
					if (comma_index == 2) {
						comma_index = 0;
						// Only update a possible i_end if both x and y coordinates have been added
						i_end = i;
					}
					//}
				}
				// End string with '!' followed by '\0'
				// printf("End index: %d \n", i_end);
				recvbuf_section[i_end + 1] = '!';
				recvbuf_section[i_end + 2] = '\0';
				i_start = i_start + i_end + 1; // update i_start for next iteration

				printf("Send string: %s\n", recvbuf_section);

				// Send string to client
				recvbuf_1len = i_end + 2;
				int nTemp = send(sd, recvbuf_section, recvbuf_1len, 0);
				if (nTemp == SOCKET_ERROR) {
					return false;
				}
				// Get reply
				nReadBytes = recv(sd, recvbuf_person, DEFAULT_BUFLEN, 0);
			// Continue as long as there are still characters in string that need to be send 
			} while (i_start < recvbuflen);
		}
		printf("Send done message \n");
		int nTemp = send(sd, "Done", 5, 0);
		if (nTemp == SOCKET_ERROR) {
			return false;
		}

		// Get reply
		nReadBytes = recv(sd, recvbuf_person, DEFAULT_BUFLEN, 0);
	} while (flag == 0);

	// Original code
    //// Read data from client
    //char acReadBuffer[kBufferSize];
    //int nReadBytes;
    //do {
    //    nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
    //    if (nReadBytes > 0) {
    //        cout << "Received " << nReadBytes <<
    //                " bytes from client." << endl;

    //        int nSentBytes = 0;
    //        while (nSentBytes < nReadBytes) {
    //            int nTemp = send(sd, acReadBuffer + nSentBytes,
    //                    nReadBytes - nSentBytes, 0);
    //            if (nTemp > 0) {
    //                cout << "Sent " << nTemp <<
    //                        " bytes back to client." << endl;
    //                nSentBytes += nTemp;
    //            }
    //            else if (nTemp == SOCKET_ERROR) {
    //                return false;
    //            }
    //            else {
    //                // Client closed connection before we could reply to
    //                // all the data it sent, so bomb out early.
    //                cout << "Peer unexpectedly dropped connection!" <<
    //                        endl;
    //                return true;
    //            }
    //        }
    //    }
    //    else if (nReadBytes == SOCKET_ERROR) {
    //        return false;
    //    }
    //} while (nReadBytes != 0);

    //cout << "Connection closed by peer." << endl;
    return true;
}
