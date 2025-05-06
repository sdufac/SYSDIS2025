#include <stdio.h>       // pour perror(), printf(), etc.
#include <stdlib.h>      // pour exit(), atoi(), etc.
#include <string.h>      // pour memset(), memcpy(), etc.
#include <unistd.h>      // pour close(), read(), write(), etc.
#include <arpa/inet.h>   // pour inet_pton(), inet_ntoa(), htons(), htonl(), etc.
#include <sys/socket.h>  // pour socket(), bind(), connect(), recv(), send(), etc.
#include <netinet/in.h>  // pour struct sockaddr_in
#include <netdb.h> 

#define TAILLEBUF 40

int main(int argc, char *argv[]){
	// identification socket d'écoute du serveur
	static struct sockaddr_in addr_serveur;
	// identifiants de la machine où tourne le serveur
	struct hostent *host_serveur;
	// socket locale coté client
	int sock;
	// message à envoyer au serveur
	char *message = "bonjour";
	// chaîne où sera écrit le message reçu
	char reponse[TAILLEBUF];
	// nombre d'octets envoyés/reçus
	int nb_octets;
	// création socket TCP
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("creation socket");
		exit(1);
	}
	// récupération identifiants de la machine serveur
	host_serveur = gethostbyname("localhost");
	if (host_serveur==NULL) {
		perror("erreur récupération adresse serveur\n");
		exit(1);
	}
	// création de l'identifiant de la socket d'écoute du serveur
	bzero((char *) &addr_serveur,sizeof(addr_serveur));
	addr_serveur.sin_family = AF_INET;
	addr_serveur.sin_port = htons(4000);
	memcpy(&addr_serveur.sin_addr.s_addr,
	host_serveur->h_addr,
	host_serveur->h_length);
	// connexion de la socket client locale à la socket coté serveur
	if (connect(sock, (struct sockaddr *)&addr_serveur,
		sizeof(struct sockaddr_in)) == -1) {
		perror("erreur connexion serveur");
		exit(1);
	}
	// connexion etablie, on envoie le message
	nb_octets = write(sock, message, strlen(message)+1);
	// on attend la réponse du serveur
	nb_octets = read(sock, reponse, TAILLEBUF);
	printf(" reponse recue : %s\n", reponse);
	// on ferme la socket
	close(sock); 
}
