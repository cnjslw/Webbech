  // webbench.cpp : 此文件包含 "socket.c" 函数。
    /***********************************************************************
       module:       socket.c
       program:      popclient
       SCCS ID:      @(#)socket.c    1.5  4/1/94
       programmer:   Virginia Tech Computing Center
       compiler:     DEC RISC C compiler (Ultrix 4.1)
       environment:  DEC Ultrix 4.3
       description:  UNIX sockets code.
      ***********************************************************************/
    
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <sys/time.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
      /***********
      功能：通过地址和端口建立网络连接
      host:网络地址
      clientPort:端口
      Return：建立的socket连接。
    		  如果返回-1，表示建立连接失败
      ************/
      //i = Socket(proxyhost == NULL ? host : proxyhost, proxyport);
    static int Socket(const char *host, int clientPort)
    {
    	int sock;
    	unsigned long inaddr;
    	struct sockaddr_in ad;
		/*
			srtruct sockaddr_in {
				unsigned short	 sin_family; 	//地址类型
				unsigned short	 int sin_port; 	//端口号
			//struct in_addr {unsigned long s_addr;}
				struct in_addr   sin_addr; 		//IP地址
				unsigned char 	 sin_zero[8];	//填充字节
			}
		*/

		/*
			struct hostent {
				char *h_name;		//表示的是主机的规范名。例如 www.google.com 的规范名其实是 www.l.google.com 
				char **h_aliases;	//表示的是主机的别名。www.google.com 就是 google 他自己的别名。有的时候，有的主机可能有好几个别名，这些，其实都是为了易于用户记忆而为自己的网站多取的名字。
				int h_addrtype;		//表示的是主机 ip 地址的类型，到底是 ipv4(AF_INET)，还是 ipv6(AF_INET6)
				int h_length;		//表示的是主机 ip 地址的长度
				char **h_addr_list;	//表示的是主机的 ip 地址，注意，这个是以网络字节序存储的。千万不要直接用 printf 带 %s 参数来打这个东西，会有问题的哇。所以到真正需要打印出这个 IP 的话，需要调用 inet_ntop()。
			};

		*/
    	struct hostent *hp;
    	
    	memset(&ad, 0, sizeof(ad));
    	ad.sin_family = AF_INET;
        
		inaddr = inet_addr(host);/*此inet_addr函数将点分的十进制的IP转为无符号长整形，host可能是www。baifu。com也可能是IP地址数字的格式，所以下面的if判断。
*/
     if (inaddr != INADDR_NONE)
	    // when输入为正确的IP地址时候
     memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
    	 else
	   //如果host是域名，输入的是网址，结果就是错误
     {
     hp = gethostbyname(host);//用域名获取IP
    		if (hp == NULL)
    			return -1;
    		memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    	}
    	ad.sin_port = htons(clientPort);
    
    	sock = socket(AF_INET, SOCK_STREAM, 0);
    	if (sock < 0)
    		return sock;
    	//连接
    	if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)
    		return -1;
    	return sock;
    }


