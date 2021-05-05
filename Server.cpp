/*
 * Server.cpp
 * Author: Sapir David.
 * ID: 318574712.
 */

#include "Server.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

using namespace std;


Server::Server(int port)throw (const char*) {
    ifStop= false;
    //open the server socket
    fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd<0){
        throw "socket failed";
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(::bind(fd,(struct sockaddr*)&server,sizeof(server))<0){
        throw "bind failure";
    }

    if(listen(fd,3)<0){
        throw "listen failure";
    }
}

void Server::start(ClientHandler& ch)throw(const char*) {
    t = new thread([&ch,this]() {
        while (ifStop==0) {
            socklen_t clientSize=sizeof(client);
            cout<<"wait"<<endl;
            //alarm(5);
            int aclient = accept(fd,(struct sockaddr*)&client,&clientSize);
            if(aclient<0){
                throw "accept failure";
            }
            ch.handle(aclient);
            close(aclient);
        }
        close(fd);
    });
}

void Server::stop() {
	t->join(); // do not delete this!
	this->ifStop = true;
}

Server::~Server() {
}

