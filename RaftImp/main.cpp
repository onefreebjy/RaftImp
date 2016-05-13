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

using namespace std;

#define MAX_RUNTIME 10
#define FOLLOWER 0
#define CANDIDATE 1
#define LEADER 2

int state;

int leaderElection(zmq::context_t *context);
int followerRoutine(zmq::context_t *context);

int main(int argc, const char * argv[]) {
    zmq::context_t context(1);
    while(1){
        leaderElection(&context);
    }
        return 0;
}

static void watchdog(int signro){
    cout<<"alarm"<<endl;
    //exit(signro);
}

int leaderElection(zmq::context_t *context){
    //  Set state
    state = FOLLOWER;
    
    //  Set timeout
    
    //  Receive from candidate
    
    
    
    followerRoutine(context);
    return 0;
}

int followerRoutine(zmq::context_t *context){
    //  Bind to a port to wait for request from client
    zmq::socket_t receiver (*context, ZMQ_REP);
    receiver.bind ("tcp://127.0.0.1:5555");
    
    //  Connect to publisher
    zmq::socket_t subscriber(*context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "10001", 5);
    
    //  Initialize poll set
    zmq::pollitem_t items[] = {
        {(void *)receiver, 0, ZMQ_POLLIN, 0},
        {(void *)subscriber, 0, ZMQ_POLLIN, 0}
    };
    cout<<"1"<<endl;
    
    if (signal(SIGALRM, watchdog) == SIG_ERR)
        exit(2);
    alarm(MAX_RUNTIME);

    while (1){
        zmq::message_t message;
        zmq::poll(items, 2, -1);
        
        if (items[0].revents & ZMQ_POLLIN){
            //  Wait for next request from client
            receiver.recv (&message);
            char buf[15];
            memcpy(buf, message.data(), message.size());
            cout << "Received " << buf << endl;
            
            //  Send reply back to client
            strcpy(buf, "World");
            zmq::message_t reply (strlen(buf));
            memcpy (reply.data (), buf, strlen(buf));
            receiver.send (reply);
        }
        
        if (items[1].revents & ZMQ_POLLIN){
            subscriber.recv(&message);
            char buf[15];
            memcpy(buf, message.data(), message.size());
            cout << "Received " << buf << endl;
        }
    }
    return 0;
}
