//
//  main.cpp
//  RaftImp
//
//  Created by BaiJinyan on 6/5/2016.
//  Copyright © 2016 BaiJinyan. All rights reserved.
//

#include <iostream>
#include <unistd.h>

#include <msgpack.hpp>
#include <zmq.hpp>

int main(int argc, const char * argv[]) {
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");
    
    while (true) {
        zmq::message_t request;
        
        //  Wait for next request from client
        socket.recv (&request);
        std::cout << "Received Hello" << std::endl;
        
        //  Do some 'work'
        sleep(1);
        
        //  Send reply back to client
        zmq::message_t reply (5);
        memcpy (reply.data (), "World", 5);
        socket.send (reply);
    }
    return 0;
}
