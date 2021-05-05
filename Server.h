
#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"


using namespace std;

using namespace std;
class SocketIO:public DefaultIO{
    int aclient;
    char buffer[1024];
public:
    SocketIO(int aclient){
        this->aclient=aclient;
    }
    virtual string read() {
        string clientInput="";
        char c = 0;
        recv(aclient, &c, sizeof(char), 0);
        while(c!='\n') {
            clientInput+=c;
            recv(aclient, &c, sizeof(char), 0);
        }
        return clientInput;
    }
    virtual void write(string text){
        send(aclient,text.c_str(),text.length(),0);
    }

    virtual void write(float f){
        string s= to_string(f);
        send(aclient,s.c_str(),s.length(),0);
    }

    virtual void read(float* f){
        recv(aclient, &f, sizeof(float), 0);
    }
    ~SocketIO(){}
};




// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        /*char buffer[1024];
        int n = read(clientID,buffer,100);
        cout<<buffer<<endl;
        const char* hello ="hello";
        send(clientID,hello,strlen(hello),0);*/
        SocketIO sock(clientID);
        CLI cli(&sock);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
    thread* t; // the thread to run the start() method in
    sockaddr_in server;
    sockaddr_in client;
    int fd;
    int ifStop;
    // you may add data members

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */
