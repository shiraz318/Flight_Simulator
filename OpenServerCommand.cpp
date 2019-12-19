//
// Created by shiraz and nili on 12.12.2019.
//
#ifndef ex3__OPENDATASERVER_CPP_
#define ex3__OPENDATASERVER_CPP_
#include <bits/basic_string.tcc>
#include <future>
#include "Command.h"
#include "Var.cpp"
#include "ex1.h"
static int client_socket_in;

  class OpenServerCommand : public Command {
   public:
    virtual void execute(queue<string> &token) {
      Singleton* singleton = Singleton::getSingleton();
      cout << "I am executing in Open Data Server" << endl;

      //openSocketOut(port);

      //closing the listening socket
      //thread thread_in(readFromSim, client_socket);
      //thread_in.join();
      return;
    }
     void openSocketOut(float port)  {
      int socketfd;
      sockaddr_in address;

      socketfd = socket(AF_INET, SOCK_STREAM, 0);//create socket
      if (socketfd == -1) {
        //error
        cerr << "Could not create a socket" << endl;
      }
      //bind socket to IP address
// we first need to create the sockaddr obj.
//in means IP4
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
      address.sin_port = htons(port);
      //we need to convert our number
// to a number that the network understands.
//the actual bind command
      if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        cerr << "Could not bind the socket to an IP" << endl;
      }

      //making socket listen to the port
      if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        cerr << "Error during listening command" << endl;
      } else {
        cout << "Server is now listening ..." << endl;
      }
      //acceptAndRead(socketfd, address);
       client_socket_in = accept(socketfd, (struct sockaddr *) &address,
                                 (socklen_t *) &address);
      if (client_socket_in == -1) {
        cerr << "Error accepting client" << endl;
      }
      close(socketfd);
    }

//    void acceptAndRead(int socketfd, sockaddr_in &address) const {// accepting a client
//      int client_socket = accept(socketfd, (struct sockaddr *) &address,
//                                 (socklen_t *) &address);
//      if (client_socket == -1) {
//        cerr << "Error accepting client" << endl;
//        exit(1);
//      }
//      close(socketfd); //closing the listening socket
//      //future<int> f1 = async(launch::async, readFromSim, client_socket);
//      thread thread_in(readFromSim, client_socket);
//      cout<<"im out"<<endl;
//      thread_in.join();
//      return;
//
//    }
//    void createAndBindSocket(int port, int &socketfd, sockaddr_in &address) const {
//      socketfd = socket(AF_INET, SOCK_STREAM, 0);//create socket
//      if (socketfd == -1) {
//        //error
//        cerr << "Could not create a socket" << endl;
//        exit(1);
//      }
//      //bind socket to IP address
//      // we first need to create the sockaddr obj.
//      //in means IP4
//      address.sin_family = AF_INET;
//      address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
//      address.sin_port = htons(port);
//      //we need to convert our number
//      // to a number that the network understands.
//      //the actual bind command
//      if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
//        cerr << "Could not bind the socket to an IP" << endl;
//        exit(1);
//      }
//    }

     void separateByComma(char *buffer) {
      mutex_lock.lock();
      Singleton* singleton = Singleton::getSingleton();
      string pusher, current_var_name;
      int i = 0, index_comma = 0, npos = (int) std::string::npos;
      string str = string(buffer);
      while ((index_comma = str.find(COMMA)) != npos) {
        pusher = str.substr(0, index_comma);
        //update the value in symbol-table-simulator
        current_var_name = singleton->index.at(i);
        Var* v = (Var*)singleton->symbol_table_simulator.at(current_var_name);
        if (v->getDirection().compare("<-") == 0) {
          v->setValue(atof(pusher.c_str()));
        }

        i++;
        str.erase(0, index_comma + 1);
        mutex_lock.unlock();
      }
    }int readFromSim(int client_socket_in) {//reading from client
      while (true) {
        char buffer[LINE_SIZE] = {0};
        int valread = read(client_socket_in, buffer, LINE_SIZE);
        cout << buffer << endl;
        separateByComma(buffer);
      }
      return client_socket_in;
    }
  };
#endif