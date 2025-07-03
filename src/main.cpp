#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>

// Base Directory
std::string base_directory = "";

// Function
std::vector<std::string> trim(const std::string &str);
void handleClient(const int client);
void post_request(std::string path, std::string request, int client);


int main(int argc, char **argv) {

  for (int i = 1; i < argc - 1; ++i) {
    if (std::string(argv[i]) == "--directory") {
        base_directory += argv[i + 1];
    }
  }

  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "Server: Logs from server will appear here!\n";


  // Create a socket
  // AF_INET is IPv4, SOCK_STREAM is TCP
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
   std::cerr << "Server: Failed to create server socket\n";
   return 1;
  }
  
  // setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "Server: setsockopt failed\n";
    return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);
  
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Server: Failed to bind to port 4221\n";
    return 1;
  }
  
  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "Server: listen failed\n";
    return 1;
  }
  
  while(true){
  // client addr
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
    
  std::cout << "Server: Waiting for a client to connect...\n";
  
  int client = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
  if(client<0){
    std::cout << "Server: Failed to accept client connection\n";
  }
  std::cout << "Server: Client connected\n";

  std::thread(handleClient, client).detach();
  }

  


  return 0;
}


/*
  * Function to trim the string by splitting it into lines
*/
std::vector<std::string> trim(const std::string &str) {
  std::vector<std::string> lines;
  size_t start = 0, end = 0;
  while ((end = str.find("\r\n", start)) != std::string::npos) {
    lines.push_back(str.substr(start, end - start));
    start = end + 2;
  }
  lines.push_back(str.substr(start)); 
  return lines;
}

/*
  * Function handle each client seprately
*/
// Get the client's IP address and path
void handleClient(const int client){
  char recv_buffer[1024];
  ssize_t bytes_received = recv(client, recv_buffer, sizeof(recv_buffer) - 1, 0);

  if (bytes_received < 0) {
    std::cerr << "Failed to receive data from client\n";
    close(client);
  }

  recv_buffer[bytes_received] = '\0'; // Null-terminate
  std::cout << "\nServer: Received data" <<"\n";
  std::vector<std::string> lines;
  lines = trim(std::string(recv_buffer));

  for(auto line : lines){
    std::cout<<"Server: "<<line<<"\n";
  }

  // Parse the request to get the path
  std::string request(recv_buffer);
  size_t start = request.find(" ");
  size_t end = request.find(" ", start + 1);
  std::string path = request.substr(start + 1, end - start - 1);
  std::cout << "Server: Requested path {" << path << "}\n";

  // Send a response to the client
  std::string message;
  if(path == "/"||path == "/index.html"){
    message = "HTTP/1.1 200 OK\r\n";
    send(client, message.c_str(), message.length(), 0);
    close(client);
    return;
  }
  else if(path.find("/echo") == 0){
    // Extract the echo request from the path
    std::string echo_req = path.substr(6);
    std::cout << "Echo request: " << echo_req << "\n";
    int echo_len = echo_req.length();
    message = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length:"+std::to_string(echo_len)+"\r\n\r\n";
    message += echo_req;
    send(client, message.c_str(), message.length(), 0);
    close(client);
    return;
  }
  else if(path.find("/user-agent") == 0){
    // Extract the User-Agent from the request headers
    std::string user_agent_value = request.substr(request.find("User-Agent: ") + 12, request.find("\r\n", request.find("User-Agent: ")) - (request.find("User-Agent: ") + 12));
    std::cout << "User-Agent: " << user_agent_value << "\n";
    message = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length:"+std::to_string(user_agent_value.length())+"\r\n\r\n";
    message += user_agent_value;
    send(client, message.c_str(), message.length(), 0);
    close(client);
    return;
  }
  else if(path.find("/files") == 0){
    // Extract file name from the request header
    std::string file_path =base_directory+path.substr(6);
    std::cout << "File request: " << file_path << "\n";

    // Open the requested file
    FILE *file = fopen(file_path.c_str(), "r");
    if (file == nullptr) {
      std::cerr << "File not exist" << file_path << "\n";
      std::string message = "HTTP/1.1 404 Not Found\r\n\r\n";
      send(client, message.c_str(), message.length(), 0);
      close(client);
      return;
    }

    // Read the file content
    std::ostringstream file_content;
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
      file_content.write(buffer, bytes_read);
    }
    fclose(file);

    // Send the file content as the response
    message = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length:" + std::to_string(file_content.str().length()) + "\r\n\r\n" + file_content.str();
    send(client, message.c_str(), message.length(), 0);
    close(client);
    return;
  }
  else if(method == "POST") {
    size_t path_start = request.find(' ') + 1;
    size_t path_end = request.find(' ', path_start);
    path = request.substr(path_start, path_end - path_start);

    post_request(path, request, client);
    close(client);
    return;
  }
  else{
    message = "HTTP/1.1 404 Not Found\r\n\r\n";
    send(client, message.c_str(), message.length(), 0);
    close(client);
    return;
  }
}

/*
  * Function handle Post Request
*/
void post_request(std::string path, std::string request, int client){
  if(path.find("/files")==0){
    std::string file_path = base_directory + path.substr(6); 
    std::cout << "File request: " << file_path << "\n";

    // Extract the file content from the request
    size_t content_start = request.find("\r\n\r\n") + 4;
    std::string file_content = request.substr(content_start);

    // Open the file for writing
    FILE *file = fopen(file_path.c_str(), "w");
    if (file == nullptr) {
      std::cerr << "Failed to open file for writing: " << file_path << "\n";
      std::string message = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
      send(client, message.c_str(), message.length(), 0);
      close(client);
      return;
    }

    // Write the content to the file
    fwrite(file_content.c_str(), 1, file_content.length(), file);
    fclose(file);

    // Send a response indicating success
    std::string message = "HTTP/1.1 201 Created\r\n\r\n";
    send(client, message.c_str(), message.length(), 0);
  } else {
    std::string message = "HTTP/1.1 404 Not Found\r\n\r\n";
    send(client, message.c_str(), message.length(), 0);
  }
}