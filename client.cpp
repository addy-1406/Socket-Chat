#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
using namespace std;

const int MSG_LEN = 1024;
char msg[MSG_LEN];
int clientSocket;

void handleInput(){
    while(true){
        // take input
        memset((char *)&msg, 0, sizeof(msg));
        cin.getline(msg, MSG_LEN);
        
        // Check if the user wants to exit
        if (strlen(msg) == 0 || strcmp(msg, "/exit") == 0) {
            break;
        }

        // Send message to the server
        send(clientSocket, msg, strlen(msg)+1, 0);
    }

    memset((char *)&msg, 0, sizeof(msg));
    strcpy(msg,"/exit");
    send(clientSocket, msg, strlen(msg)+1, 0);
    close(clientSocket);
}

void messageRecd(){
    while(true){
        // Receive message from server
        memset((char *)&msg, 0, sizeof(msg));
        int bytesread = recv(clientSocket, msg, sizeof(msg), 0);

        if (strcmp(msg, "/exit") == 0) {
            close(clientSocket);
            exit(0);
        }


        if(bytesread > 0){
            if(strcmp(msg,"Server Disconnected") == 0){
                cout << "\x1b[095m" << "Server Disconnected" << "\x1b[0m" << "\n" ;
                close(clientSocket);
                exit(0);
            }
            else if(strcmp(msg, "Invalid client id") == 0){
                cout << "\x1b[095m" << "Server : Invalid Client ID" << "\x1b[0m" << "\n" ;

            }

            else if(msg[0] == '-'){
                int i = 0;
                int id = 0;
                while(i < 1024){
                    if(msg[i] - '0' >= 0 && msg[i] - '0' <= 9){
                        id = id*10 + (msg[i] - '0');
                    }
                    else{
                        if(id != 0){
                            cout << "\x1b[095m" << "Server : Client" << id  << " Connected" << "\x1b[0m" << "\n" ;
                        }
                        id = 0;
                    }
                    i++;
                }
            }

            else{
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
                if(id != 0){
                    string out = "";
                    i++;
                    while(i < 1024){
                        out += msg[i++];
                    }
                    cout << "\x1b[095m" << "Client " << id << " : " << out << "\x1b[0m" << "\n" ;
                }
                else{
                    cout << "\x1b[095m" << "Server: " << msg << "\x1b[0m" << "\n" ;
                }
            }

        }

    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Invalid arguments\n" ;
        return 1;
    }

    char* serverIP = argv[1];
    int serverPort = stoi(argv[2]);

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cout << "Socket creation failed\n";
        return 1;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);
    serverAddress.sin_port = htons(serverPort);

    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cout << "Connection failed\n";
        return 1;
    }

    // Notify about successful connection
     cout << "Connected to server!" <<  "\n";
    // cout << "Enter /exit to exit\n" << "\x1b[0m";

    memset((char *)&msg, 0, sizeof(msg));
    int bytesread = recv(clientSocket, msg, sizeof(msg), 0);
    cout << "\x1b[094m" <<  "CLIENT ID : " << msg << "\n";

    thread(messageRecd).detach();
    thread(handleInput).join();

    cout << "\x1b[032m" << "***************************Connection closed!****************************\n" << "\x1b[0m";

    return 0;
}
