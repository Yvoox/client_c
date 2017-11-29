#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SOCKETNAME "Une_Socket"

void exit_error(const char *chaine){
	perror(chaine);
	exit(EXIT_FAILURE);
}

char message[100];
char buffer[1024];
int sock;

void *thread_1(void *arg)
{
    
fgets(message, 100, stdin);
send(sock,message,sizeof(message),0);
    (void) arg;
    pthread_exit(NULL);

}

void *thread_2(void *arg)

{
recv(sock,buffer,1024,0);
printf("%s\n",buffer);
    (void) arg;
    pthread_exit(NULL);

}


int main(void){


pthread_t thread1;
pthread_t thread2;


/*Declaration de socket*/
sock = socket(AF_INET, SOCK_STREAM, 0);
if(sock == -1)
{
    perror("socket()");
    exit(errno);
}

struct hostent *hostinfo = NULL;
struct sockaddr_in sin;
const char *hostname = "10.57.110.13";

hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
if (hostinfo == NULL) /* l'hôte n'existe pas */
{
    fprintf (stderr, "Unknown host %s.\n", hostname);
    exit(EXIT_FAILURE);
}

sin.sin_addr = *(struct in_addr *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
sin.sin_port = htons(6565); /* on utilise htons pour le port */
sin.sin_family = AF_INET;

if(connect(sock,(struct sockaddr *) &sin, sizeof(struct sockaddr)) == -1)
{
    perror("connect()");
    exit(errno);
}

while(1){


    if (pthread_create(&thread1, NULL, thread_1, NULL)) {
	perror("pthread_create");
	return EXIT_FAILURE;
    }

    if (pthread_join(thread1, NULL)) {
	perror("pthread_join");
	return EXIT_FAILURE;
    }

    if (pthread_create(&thread2, NULL, thread_2, NULL)) {
	perror("pthread_create");
	return EXIT_FAILURE;
    }

    if (pthread_join(thread2, NULL)) {
	perror("pthread_join");
	return EXIT_FAILURE;
    }

/*
fgets(message_, 100, stdin);
send(sock_,message_,sizeof(message_),0);
recv(sock_,buffer_,1024,0);
printf("%s\n",buffer_);*/




}
/*
pthread_t thread1;
pthread_t thread2;

    if (pthread_create(&thread1, NULL, thread_1, NULL)) {
	perror("pthread_create");
	return EXIT_FAILURE;
    }

    if (pthread_join(thread1, NULL)) {
	perror("pthread_join");
	return EXIT_FAILURE;
    }

    if (pthread_create(&thread2, NULL, thread_2, NULL)) {
	perror("pthread_create");
	return EXIT_FAILURE;
    }

    if (pthread_join(thread2, NULL)) {
	perror("pthread_join");
	return EXIT_FAILURE;
    }
*/



}
