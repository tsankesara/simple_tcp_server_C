/*
 * Simple TCP Server that listens on port 8181
 * Accepts one connection, reads data, and sends a response
 */

// Include necessary header files for networking and system operations
#include <netinet/in.h>      // For internet address family structures (like sockaddr_in)
#include <stdio.h>           // Standard I/O functions
#include <sys/socket.h>      // For socket-related functions and structures
#include <arpa/inet.h>       // For IP address manipulation functions
#include <sys/types.h>       // For data types used in system calls
#include <unistd.h>          // For POSIX operating system API (close(), etc.)
#include <stdlib.h>          // Standard library functions (exit(), etc.)
#include <string.h>          // String manipulation functions

#define PORT 8181            // Define a constant for the port number (8181)

int main()
{
    // Socket variables
    int s, c;                // 's' = server socket descriptor, 'c' = client socket descriptor
    socklen_t addrlen;       // Variable to store address length (used in accept())
    
    // Network address structures
    struct sockaddr_in srv, cli;  // 'srv' for server address, 'cli' for client address
    
    // Data buffers
    char buf[512];           // Buffer to store received data
    char *data;              // Pointer for response data

    /********************** INITIALIZATION **********************/
    
    // Initialize address length
    addrlen = 0;             // Will be properly set before accept() call
    
    // Clear server and client address structures
    memset(&srv, 0, sizeof(srv));  // Zero out server address structure
    memset(&cli, 0, sizeof(cli));  // Zero out client address structure

    /********************** SOCKET CREATION **********************/
    
    // Create TCP socket (IPv4, stream-based, default protocol)
    s = socket(AF_INET, SOCK_STREAM, 0);
    
    // Check if socket creation failed
    if(s < 0){
        printf("socket() failed\n");  // Error message
        return -1;                    // Exit with error code
    }

    /********************** SERVER CONFIGURATION **********************/
    
    // Configure server address structure
    srv.sin_family = AF_INET;          // Use IPv4 address family
    srv.sin_addr.s_addr = 0;           // INADDR_ANY (0) = accept connections on all interfaces
    srv.sin_port = htons(PORT);        // Set port number (convert to network byte order)

    /********************** SOCKET BINDING **********************/
    
    // Bind socket to the server address
    if(bind(s, (struct sockaddr *)&srv, sizeof(srv)))
    {
        printf("bind() failed\n");      // Binding failed
        close(s);                      // Close socket before exiting
        return -1;                     // Exit with error code
    }

    /********************** LISTEN FOR CONNECTIONS **********************/
    
    // Start listening for incoming connections
    // 5 = maximum length of the queue of pending connections
    if(listen(s, 5))
    {
        printf("listen() failed\n");    // Listen failed
        close(s);                      // Close socket before exiting
        return -1;                     // Exit with error code
    }
    
    // Server is now ready
    printf("Listening on 0.0.0.0:%d\n", PORT);  // Show listening address

    /********************** CLIENT CONNECTION **********************/
    
    // Accept an incoming connection
    // Note: This uses srv instead of cli - this is incorrect but left as in original
    c = accept(s, (struct sockaddr *)&srv, &addrlen);
    if (c < 0){
        printf("accept() failed\n");    // Connection accept failed
        close(s);                      // Close server socket
        return -1;                     // Exit with error code
    }

    // Connection established
    printf("Client Connected\n");

    /********************** DATA EXCHANGE **********************/
    
    // Read data from client (up to 511 bytes to leave room for null terminator)
    read(c, buf, 511);
    
    // Prepare response data
    data = "httpd v1.0 \n";            // Simple response message
    
    // Send response to client
    write(c, data, strlen(data));       // Send exact length of the data

    /********************** CLEANUP **********************/
    
    // Close client connection
    close(c);
    
    // Close server socket
    close(s);
    
    // Exit successfully
    return 0;
}