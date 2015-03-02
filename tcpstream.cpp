#include <arpa/inet.h>
#include "tcpstream.h"
#include <exception>
#include <signal.h>

TCPStream::TCPStream(int sd, struct sockaddr_in* address) : m_sd(sd) {
    char ip[50];
    inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
    m_peerIP = ip;
    m_peerPort = ntohs(address->sin_port);
    signal(SIGPIPE, SIG_IGN);
}
TCPStream::~TCPStream()
{
    close(m_sd);
}

ssize_t TCPStream::send(const char* buffer, size_t len) 
{
    ssize_t ret;
    /*if (isConnected()){
        ret = write(m_sd, buffer, len);
    }
    else{
        //PRINT_ERROR << "Peer closed connection for " << m_peerIP << ":" << m_peerPort << endl;
        ret = -1;
    }*/
    ret = write(m_sd, buffer, len);
    return ret;
}

ssize_t TCPStream::receive(char* buffer, size_t len, int timeout) 
{
    if (timeout <= 0) return read(m_sd, buffer, len);

    if (waitForReadEvent(timeout) == true)
    {
        return read(m_sd, buffer, len);
    }
    return connectionTimedOut;

}

string TCPStream::getPeerIP() 
{
    return m_peerIP;
}

int TCPStream::getPeerPort() 
{
    return m_peerPort;
}

bool TCPStream::waitForReadEvent(int timeout)
{
    fd_set sdset;
    struct timeval tv;

    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    FD_SET(m_sd, &sdset);
    if (select(m_sd+1, &sdset, NULL, NULL, &tv) > 0)
    {
        return true;
    }
    return false;
}
bool TCPStream::waitForSendEvent(int timeout)
{
    fd_set sdset;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;
    FD_SET(m_sd, &sdset);
    if (select(m_sd+1, NULL, &sdset, NULL, &tv) > 0)
    {
        return true;
    }
    return false;
}

bool TCPStream::isConnected()
{
    int err = 0;
    socklen_t optlen = sizeof err;
    if (getsockopt(m_sd, SOL_SOCKET, SO_ACCEPTCONN, &err, &optlen) < 0 ){
        return false;
    }
    return true;
}
