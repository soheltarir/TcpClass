#ifndef __tcpstream_h__
#define __tcpstream_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include "macros.h"

using namespace std;

class TCPStream
{
    private:
    int     m_sd;
    string  m_peerIP;
    int     m_peerPort;

    public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    virtual ~TCPStream();

    ssize_t send(const char* buffer, size_t len);
    ssize_t receive(char* buffer, size_t len, int timeout=0);
    bool isConnected();

    string getPeerIP();
    int    getPeerPort();

    enum {
        connectionClosed = 0,
        connectionReset = -1,
        connectionTimedOut = -2
    };

    private:
    bool waitForReadEvent(int timeout);
    bool waitForSendEvent(int timeout);

    TCPStream(int sd, struct sockaddr_in* address);
    TCPStream();
    TCPStream(const TCPStream& stream);
};

#endif
