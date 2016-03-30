#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define BROADCASTVREQUEST "DISCOVER_CEGATEWAY_REQUEST"
#define BROADCASTVRESPONSE "DISCOVER_CEGATEWAY_RESPONSE"

int main()
{
	daemon(1,1);
	setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 

	// 绑定地址
	struct sockaddr addrto;
	bzero(&addrto, sizeof(struct sockaddr));
	((struct sockaddr_in *)&addrto)->sin_family = AF_INET;
	((struct sockaddr_in *)&addrto)->sin_addr.s_addr = htonl(INADDR_ANY);
	((struct sockaddr_in *)&addrto)->sin_port = htons(8888);

	struct sockaddr from;
	bzero(&from, sizeof(struct sockaddr));
	//	((struct sockaddr_in *)&from)->sin_family = AF_INET;
	//	((struct sockaddr_in *)&from)->sin_addr.s_addr = htonl(INADDR_ANY);
	//	((struct sockaddr_in *)&from)->sin_port = htons(8888);

	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{   
		fprintf(stdout, "socket error\n");
		return -1;
	}   

	const int opt = 1;
	//设置该套接字为广播类型，
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		fprintf(stdout, "set socket error...\n");
		return -1;
	}

	if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr)) == -1) 
	{   
		fprintf(stdout, "bind error...\n");
		return -1;
	}

	int len = sizeof(struct sockaddr);

	while(1)
	{
		//从广播地址接受消息
		char smsg[128] = {0};
		int ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&from,(socklen_t*)&len);
		if(ret<=0)
		{
			fprintf(stdout, "read error....\n");
			continue;
		}
		else
		{		
			char *recip = inet_ntoa(((struct sockaddr_in *)&from)->sin_addr);
			printf("from:%s>>>%s\n",recip, smsg);	
			if(0 == strcmp(BROADCASTVREQUEST, (const char *)smsg)){
				if (sendto(sock, BROADCASTVRESPONSE, sizeof(BROADCASTVREQUEST), 0,  
							(struct sockaddr *) &from,  
							sizeof(struct sockaddr)) < 0) {  
					fprintf(stdout, "sendto error!\n");  
					continue;
				}
			}
			//back res
			fprintf(stdout, "send ok\n");
		}
		fprintf(stdout, "hhh\n");
	}

	return 0;
}

