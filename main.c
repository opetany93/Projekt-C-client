/*
    Simple udp client
*/
#include<stdio.h> 				//printf
#include<string.h> 				//memset
#include<stdlib.h> 				//exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>


#define BUFLEN 64  				//Max length of buffer
#define PORT 666    			//The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}


int main(int argc, char **argv)
{
	//>>moje zmienne
	char *server;
	//<<

    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    server = argv[4];

    //Wyswietlanie pobranych danych
    printf("\n\rWczytane parametry: \n\r");
    printf("\n\rZmienna A: %s" 		, argv[1]);
    printf("\n\rZmienna B: %s" 		, argv[2]);
    printf("\n\rZadanie  : %s"  	, argv[3]);
    printf("\n\rADR SERW : %s \n\r" , argv[4]);

    //Umieszczenie zmiennych w payloadzie
    sprintf(message , "%s %s %s" , argv[1] , argv[2] , argv[3]);

    //Tworzenie gniazda
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(server , &si_other.sin_addr) == 0)			//inte_aton konwertuje adres IP na binerne dane i laduje do struktury in_adr
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    //send the message
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }

    //receive a reply and print it
    //clear the buffer by filling null, it might have previously received data
    memset(buf,'\0', BUFLEN);

    //try to receive some data, this is a blocking call
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
    {
        die("recvfrom()");
    }

    printf("\n\r\n\r>>>WYNIK: %s", buf);

    close(s);
    return 0;
}
