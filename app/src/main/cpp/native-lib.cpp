#include <jni.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define PORT 8080

extern "C" JNIEXPORT
jstring
JNICALL
Java_com_example_native_1cpp_1chat_1app_MainActivity_start(
        JNIEnv *env,
        jobject obj) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while(true) {
        int valread = read(new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , buffer , strlen(buffer) , 0);
    }
    return env->NewStringUTF("Echo server started");
}



//#include <jni.h>
//#include <string>
//#include <iostream>
//#include <cstdlib>
//#include <netinet/in.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <type_traits>
//#include <unistd.h>
//
//#define PORT_NUMBER 8080
//#define BUFFER_SIZE 1024
//
//extern "C" JNIEXPORT jstring JNICALL
//Java_com_example_native_1cpp_1chat_1app_MainActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    std::cout << hello << std::endl;
//    return env->NewStringUTF(hello.c_str());
//}
//
//jint plus(jint x, jint y) {
//    return x + y;
//}
//
//extern "C" JNIEXPORT jint JNICALL
//Java_com_example_native_1cpp_1chat_1app_MainActivity_addNumbers(
//        JNIEnv* env,
//        jobject,
//        jint x,
//        jint y
//        ) {
//    return plus(x, y);
//}
//
//// Checks if the parameter (c-style string) is an integer
//bool is_int(char *c) {
//    while( *c != '\0' ) {
//        if (*c < '0' || *c > '9') {
//            return false;
//        }
//        c ++;
//    }
//    return true;
//}
//
//extern "C" JNIEXPORT jint JNICALL
//Java_com_example_native_1cpp_1chat_1app_MainActivity_createSocket(
//        JNIEnv* env,
//        jobject,
//        jint argc,
//        jstring argv) {
//
//    if(argc != 2 || !is_int((char * )argv)) {
//        std::cerr << "[ERROR] Port is not provided via command line parameters!\n";
//        return -1;
//    }
//
//    // Creates a socket and gets the file descriptor
//    int sock_listener = socket(AF_INET, SOCK_STREAM, 0);
//    // Checks if the socket is created
//    if (sock_listener < 0) {
//        std::cerr << "[ERROR] Socket cannot be created!\n";
//        return -2;
//    }
//
//    std::cout << "[INFO] Socket has been created.\n";
//    //Address info to bind socket
//    sockaddr_in server_addr;
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_port = htons(std::atoi((char *)argv));
//    //server_addr.sin_addr.s_addr = INADDR_ANY;
//    //OR
//    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);
//
//    char buf[INET_ADDRSTRLEN];
//
//    //Binds socket
//    if (bind(sock_listener, (sockaddr *) &server_addr, sizeof(server_addr)) < 0){
//        std::cerr << "[ERROR] Created socket cannot be binded to ( "
//                  << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
//                  << ":" << ntohs(server_addr.sin_port) << ")\n";
//        return -3;
//    }
//
//    std::cout << "[INFO] Sock is binded to ("
//              << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
//              << ":" << ntohs(server_addr.sin_port) << ")\n";
//
//    // Start listening
//    if (listen(sock_listener, SOMAXCONN) < 0) {
//        std::cerr << "[ERROR] Socket cannot be switched to listen mode!\n";
//        return -4;
//    }
//    std::cout << "[INFO] Socket is listening now.\n";
//
//    // Accept a call
//    sockaddr_in client_addr;
//    socklen_t client_addr_size = sizeof(client_addr);
//    int sock_client;
//    if ((sock_client = accept(sock_listener, (sockaddr*)&client_addr, &client_addr_size)) < 0) {
//        std::cerr << "[ERROR] Connections cannot be accepted for a reason.\n";
//        return -5;
//    }
//
//    std::cout << "[INFO] A connection is accepted now.\n";
//
//    // Close main listener socket
//    close(sock_listener);
//    std::cout << "[INFO] Main listener socket is closed.\n";
//
//    // Get name info
//    char host[NI_MAXHOST];
//    char svc[NI_MAXSERV];
//    if (getnameinfo(
//            (sockaddr*)&client_addr, client_addr_size,
//            host, NI_MAXHOST,
//            svc, NI_MAXSERV, 0) != 0) {
//        std::cout << "[INFO] Client: (" << inet_ntop(AF_INET, &client_addr.sin_addr, buf, INET_ADDRSTRLEN)
//                  << ":" << ntohs(client_addr.sin_port) << ")\n";
//    } else {
//        std::cout << "[INFO] Client: (host: " << host << ", service: " << svc << ")\n";
//    }
//
//
//    char msg_buf[4096];
//    int bytes;
//    // While receiving - display & echo msg
//    while (true) {
//        bytes = recv(sock_client, &msg_buf, 4096, 0);
//        // Check how many bytes recieved
//        // If there is no data, it means server is disconnected
//        if (bytes == 0) {
//            std::cout << "[INFO] Client is disconnected.\n";
//            break;
//        }
//            // If something gone wrong
//        else if (bytes < 0) {
//            std::cerr << "[ERROR] Something went wrong while receiving data!.\n";
//            break;
//        }
//            // If there are some bytes
//        else {
//            // Print message
//            std::cout << "client> " << std::string(msg_buf, 0, bytes) << "\n";
//            // Resend the same message
//            if (send(sock_client, &msg_buf, bytes, 0) < 0) {
//                std::cerr << "[ERROR] Message cannot be send, exiting...\n";
//                break;
//            }
//        }
//
//    }
//
//    // Close client socket
//    close(sock_client);
//    std::cout << "[INFO] Client socket is closed.\n";
//
//    return 0;
//}