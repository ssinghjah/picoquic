#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 2002 
#define MAXLINE 65535 

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if (bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	printf("Listening to transport"); 
	fflush(stdout);

	socklen_t len;
	int n; 

	len = sizeof(cliaddr); //len is value/result 

	bool recv_from_transport = true;
	while (recv_from_transport)
	{
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
					MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
					&len); 
		buffer[n] = '\0'; 
		printf("Client : %s\n", buffer); 
		fflush(stdout);
	}
		
	return 0; 
}
