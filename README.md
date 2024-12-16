# Socket-based Chat Application

This project implements a simple client-server-based chat application that facilitates communication between multiple clients. The server acts as a central hub, relaying messages between connected clients. Multithreading has been used to facilitate parallel communication, ensuring efficient handling of multiple clients simultaneously.

## Features
- Real-time communication between clients
- Lightweight and efficient
- Easy to set up and run
- Supports parallel communication using multithreading

## Prerequisites
Make sure you have the following installed:
- **g++** (GNU C++ Compiler)
- A terminal/command prompt to execute the commands

## Steps to Run the Application

### 1. Setting Up the Server
1. Compile the server code using the following command:
   ```bash
   g++ server.cpp -o s
   ```
2. Run the server with a specified port number (e.g., 5000):
   ```bash
   ./s 5000
   ```

### 2. Setting Up the Client
1. Compile the client code using the following command:
   ```bash
   g++ client.cpp -o c
   ```
2. Run the client with the server's IP address (e.g., `0.0.0.0`) and the port number:
   ```bash
   ./c 0.0.0.0 5000
   ```

### 3. Connecting Multiple Clients
Repeat the steps for setting up the client on multiple terminals to connect more clients to the server.

## Screenshots

Server Interface
![image](https://github.com/user-attachments/assets/c5fc8f31-13f0-4bc3-9f64-4ad7d94ad735)

Client 1 Interface 
![image](https://github.com/user-attachments/assets/d7e852f6-02d2-43d3-92a6-aec7c5506b22)

Client 2 Interface
![image](https://github.com/user-attachments/assets/aa1f869e-80e3-4f66-b50e-fcf341d5b38a)

