#include <bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctime>    
#include<signal.h>
using namespace std;

const int MSG_LEN = 1024;
char msg[MSG_LEN];
vector<int> clientSockets;
bool running;

void handleClient(int clientSocket) {

    // Notify about successful connection
    cout << "\x1b[092m" << "Client " << clientSocket << " connected\n" << "\x1b[0m";

    memset((char *)&msg, 0, sizeof(msg));  
    strcpy(msg, to_string(clientSocket).c_str());
    send(clientSocket, msg , strlen(msg)+1, 0);

    
    for(auto x:clientSockets){
        memset((char *)&msg, 0, sizeof(msg));  
        string temp =  "Client " + to_string(clientSocket) + " Connected";
        strcpy(msg, temp.c_str());
        send(x, msg , strlen(msg)+1, 0);
    }


    string temp = "-";
    for(auto x:clientSockets){
        temp += "-" + to_string(x);
    }
    memset((char *)&msg, 0, sizeof(msg));   
    strcpy(msg, temp.c_str());
    send(clientSocket, msg , strlen(msg)+1, 0);

    clientSockets.push_back(clientSocket);

    while (true) {
        // Receive data from the client
        memset((char *)&msg, 0, sizeof(msg));
        int bytesread = recv(clientSocket, msg, sizeof(msg), 0);

        if (bytesread == 0 || strcmp(msg, "/exit") == 0) {
            break;
        }

        int i = 0;
        int id = 0;

        while(msg[i] != ' ' && i < 1024){
            if(msg[i] - '0' >= 0 && msg[i] - '0' <= 9){
                id = id*10 + (msg[i] - '0');
            }
            else{
                break;
            }
            i++;
        }

        if(id == 1){
            char newmsg[1024];
            memset((char *)&newmsg, 0, sizeof(newmsg));
            i++;
            int j = 0;
            string temp = to_string(clientSocket);
            for(int k = 0; k < temp.size(); k++){
                newmsg[j++] = temp[k];
            }
            newmsg[j++] = ' ';
            while(j < 1024){
                newmsg[j++] = msg[i++]; 
            }
            for(auto x:clientSockets){
                if(x != clientSocket){
                    send(x, newmsg , strlen(newmsg)+1, 0);
                }
            }
        }

        else{
            bool valid = false;
            for(auto x:clientSockets){
                if(x != clientSocket && x == id){
                    valid = true;
                }
            }
            if(valid){
                char newmsg[1024];
                memset((char *)&newmsg, 0, sizeof(newmsg));
                i++;
                int j = 0;
                string temp = to_string(clientSocket);
                for(int k = 0; k < temp.size(); k++){
                    newmsg[j++] = temp[k];
                }
                newmsg[j++] = ' ';
                while(j < 1024){
                    newmsg[j++] = msg[i++]; 
                }
                send(id, newmsg , strlen(newmsg)+1, 0);
            }
            else{
                char newmsg[1024];
                memset((char *)&newmsg, 0, sizeof(newmsg));
                strcpy(newmsg,"Invalid client id");
                send(clientSocket, newmsg,strlen(newmsg) + 1, 0);
            }
        }



    }

    // Close client socket

    for(auto x:clientSockets){
        if(x != clientSocket){
            ((char *)&msg, 0, sizeof(msg));  
            string temp =  "Client " + to_string(clientSocket) + " Disconnected";
            strcpy(msg,temp.c_str());
            send(x, msg , strlen(msg)+1, 0);       
        }
    }

    clientSockets.erase(find(clientSockets.begin(),clientSockets.end(),clientSocket));
    close(clientSocket);

    if(running){
        cout << "\x1b[091m" << "Client " << clientSocket << " disconnected\n" << "\x1b[0m";
    }
}


void takeInput(){
    while(true){
        memset((char *)&msg, 0, sizeof(msg));
        cin.getline(msg, MSG_LEN);
        if(strlen(msg) == 0 || strcmp(msg,"/exit") == 0 || strcmp(msg,"^C")){
            for(auto x:clientSockets){
                memset((char *)&msg, 0, sizeof(msg));  
                strcpy(msg,"Server Disconnected");
                send(x, msg, strlen(msg)+1, 0);
            }
            exit(0);
        }
    }
}


void sighandler(int sig){
    running = false;
    for(auto x:clientSockets){
        memset((char *)&msg, 0, sizeof(msg));  
        strcpy(msg,"Server Disconnected");
        send(x, msg, strlen(msg)+1, 0);
    }
    exit(0);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Invalid arguments\n" ;
        return 1;
    }

    int serverPort = stoi(argv[1]);
    running = true;
    signal(SIGINT, &sighandler);

    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cout << "Socket creation failed\n";
        return 1;
    }

    // Bind
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(serverPort);

    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cout << "Binding failed\n";
        return 1;
    }

    // Listen
    if (listen(serverSocket, 5) == -1) {
        cout << "Listening\n";
        return 1;
    }

    cout << "\x1b[032m" << "Server listening on port " << serverPort << "\n" << "\x1b[0m";

    thread(takeInput).detach();


    while (true) {
        // Accept incoming connection
        int clientAddrLen = sizeof(serverAddress);
        int clientSocket = accept(serverSocket, (sockaddr*)&serverAddress,(socklen_t*) &clientAddrLen);

        if (clientSocket == -1) {
            cout << "Accept failed\n";
            continue;
        }

        // Handle the client in a new thread
        thread(handleClient, clientSocket).detach();
    }

    // Close server socket
    close(serverSocket);

    return 0;
}
