#include "entity.h"

/*
Function give information about entity
 */
int info_entity(entity* ent) {
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n++Info_entity++ : Id de l'entité %s\n", (*ent).id);
    printf("++Info_entity++ : Port TCP l'entité %d\n", (*ent).tcp_port);
    printf("++Info_entity++ : Ip de l'entité %s\n", (*ent).my_ip);
    printf("++Info_entity++ : Port UDP de l'entité %d\n", (*ent).my_uport);
    printf("++Info_entity++ : Adresse IP de l'entité suivante sur l'anneau 1 %s\n", (*ent).next_ip1);
    printf("++Info_entity++ : Port UDP de l'entité suivante sur l'anneau 1 %d\n", (*ent).next_uport1);
    printf("++Info_entity++ : Adresse IP MULTI de l'entité sur l'anneau 1 %s\n", (*ent).cast_ip1);
    printf("++Info_entity++ : Port UDP MULTI de l'entité  sur l'anneau 1 %d\n", (*ent).cast_port1);
    if ((*ent).cast_ip2 != NULL) {
        printf("++Info_entity++ : Adresse IP de l'entité suivante sur l'anneau 2 %s\n", (*ent).next_ip2);
        printf("++Info_entity++ : Port UDP de l'entité suivante sur l'anneau 2 %d\n", (*ent).next_uport2);
        printf("++Info_entity++ : Adresse IP MULTI de l'entité sur l'anneau 2 %s\n", (*ent).cast_ip2);
        printf("++Info_entity++ : Port UDP MULTI de l'entité  sur l'anneau 2 %d\n", (*ent).cast_port2);
    }
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    return 0;
}

/*
Function which initialize an entity, and prepare it for insertion
 */
int init_entity(entity* ent) {
    char* ip = get_ip();
    int uport = free_uport(ip_removeZero(ip));
    int tport = free_tport(ip_removeZero(ip));
    if (uport != -1 && ip != NULL && tport != -1) {
        (*ent).id = gen_code();
        printf("\n################Entity####################################\n\ninit_entity : Id de l'entité %s\n", (*ent).id);
        (*ent).nb_insert = 0;
        (*ent).tcp_port = tport;
        printf("init_entity : Port TCP l'entité %d\n", (*ent).tcp_port);
        (*ent).my_ip = ip;
        printf("init_entity : Ip de l'entité %s\n", (*ent).my_ip);
        (*ent).my_uport = uport;
        printf("init_entity : Port UDP de l'entité %d\n", (*ent).my_uport);
        (*ent).next_ip1 = ip;
        (*ent).next_uport1 = uport;
        (*ent).cast_ip1 = ip_libre_multi();
        printf("init_entity : Adresse IP MULTI de l'entité %s\n", (*ent).cast_ip1);
        (*ent).cast_port1 = port_libre_multi((*ent).cast_ip1);
        printf("init_entity : Port UDP MULTI de l'entité %d\n", (*ent).cast_port1);
        printf("\n##########################################################\n\n");
        return 0;
    }
    return -1;
}

/*
 Function that handle the insertion
 */
int insertion(entity* e, char* host, int e1_tcp) {
    if ((*e).nb_insert == 0) {
        //Settings structures needed later
        int sock = socket(PF_INET, SOCK_STREAM, 0);
        struct sockaddr_in adressin;

        adressin.sin_family = AF_INET;
        adressin.sin_port = htons(e1_tcp);
        int inet = inet_aton(host, &(adressin.sin_addr));
        if (inet != 0) {
            int con = connect(sock, (struct sockaddr*) &adressin, (socklen_t)sizeof (struct sockaddr_in));
            if (con == 0) {
                char buff[100];
                int recu = recv(sock, buff, 99 * sizeof (char), 0);
                if (recu > 0) {
                    //printf("C1\n");
                    //Managing the message received from the previous entity [WELC␣ip␣port␣ip-diff␣port-diff\n]
                    buff[recu] = '\0';
                    printf("insertion : Message received from entity of the ring %s\n", buff);
                    char** tab = split(buff, ' ');
                    if (str_arrsize(tab) == 5) {
                        char* ip_e2 = tab[1];
                        int port_e2 = atoi(tab[2]);
                        char* ip_cast = tab[3];
                        int port_cast = atoi(tab[4]);
                        //Allocation to current entity of the information received from the previous entity
                        if (strcmp("WELC", tab[0]) == 0
                                //&& check_ip(ip_e2) != -1 
                                //&& check_ip(ip_cast) != -1
                                && port_e2 <= 9999 && port_e2 > 0
                                && port_cast <= 9999 && port_cast > 0
                                //&& port_cast != port_e2
                                ) {
                            (*e).next_ip1 = ip_e2;
                            (*e).next_uport1 = port_e2;
                            (*e).cast_ip1 = ip_cast;
                            (*e).cast_port1 = port_cast;
                            //Preparation of the current entity answer [NEWC␣ip␣port\n]
                            char* mess = malloc(sizeof (char)*120);
                            strcpy(mess, "NEWC ");
                            strcat(mess, ip_addZero((*e).my_ip));
                            strcat(mess, " ");
                            char* my_port = intchar((*e).my_uport, 4);
                            if (my_port != NULL) {
                                strcat(mess, my_port);
                                strcat(mess, "\n");
                                //printf("insertion : Message envoyé %s\n",mess);
                                //Sending to the previous entity the message about current entity
                                send(sock, mess, strlen(mess) * sizeof (char), 0);
                                printf("insertion : Message sent %s\n", mess);

                                free(my_port);
                                free(mess);
                                char buff2[100];
                                int recu2 = recv(sock, buff2, 99 * sizeof (char), 0);
                                if (recu2 > 0) {

                                    buff2[recu2] = '\0';
                                    printf("insertion : Message received from the entity of the ring %s\n", buff2);
                                    if (strcmp(buff2, "ACKC\n") == 0) {
                                        close(sock);
                                        free(tab);
                                        (*e).nb_insert = 1;
                                        (*e).tcp_port = free_tport((*e).my_ip);
                                        printf("********************************************\n");
                                        printf("insertion : This my tcp_port %d\n", (*e).tcp_port);
                                        printf("********************************************\n");
                                        //serv_tcp(e);
                                        return 0;
                                    } else {
                                        fprintf(stderr, "insertion : Problem with message received from the entity of ring %s\n", buff2);
                                        return -1;
                                    }
                                } else {
                                    fprintf(stderr, "recv : Problem with the second message received from the entity of ring %d\n", recu2);
                                    return -1;
                                }
                            } else {
                                fprintf(stderr, "insertion : Problem with entity udp port %d\n", (*e).my_uport);
                                return -1;
                            }
                        } else {
                            fprintf(stderr, "insertion : Problem with the ip and ports received\n");
                            return -1;
                        }
                    } else {
                        fprintf(stderr, "insertion : Problem wrong message form, lack of arguments\n");
                        return -1;
                    }
                } else {
                    fprintf(stderr, "insertion : recv : Problem the number of char received for the message%d\n", recu);
                    perror("insertion : recv ");
                    return -1;
                }
            } else {
                perror("insertion : connect : Problem with the connection  ");
                return -1;
            }
        } else {
            perror("insertion : Problem for inet_aton with ip ");
        }
    } else {
        fprintf(stderr, "insertion : Number of insertions authorized overpassed\n");
        return -1;
    }
    return -1;
}

/*Function for using thread for the insertion
 */
void* pth_insertion(void* arg) {
    entity* e = (entity*) arg;
    char* host = (*e).my_ip;
    char* ip = get_ip();

    int tcp_port = (*e).tcp_port;
    int port = free_tport(ip);
    if (ip != NULL && port != -1) {
        (*e).my_ip = ip;
        (*e).tcp_port = port;
        printf("Start of insertion\n");
        insertion(e, host, tcp_port);
    }
    return NULL;
}

/*
Function that manage insertion(server part)
 */
int serv_tcp(entity* e) {
    if ((*e).nb_insert < 2) {
        int sock = socket(PF_INET, SOCK_STREAM, 0);
        struct sockaddr_in address_sock;

        address_sock.sin_family = AF_INET;
        if ((*e).tcp_port <= 9999 && (*e).tcp_port >= 1024) {

            address_sock.sin_port = htons((*e).tcp_port);
            int inet = inet_aton((*e).my_ip, &address_sock.sin_addr);

            if (inet != 0) {
                int r = bind(sock, (struct sockaddr*) &address_sock, sizeof (struct sockaddr_in));

                if (r == 0) {
                    r = listen(sock, 0);
                    if (r == 0) {
                        while (1) {
                            struct sockaddr_in caller;
                            socklen_t size = sizeof (caller);
                            int sock2 = accept(sock, (struct sockaddr *) &caller, &size);

                            if (sock2 >= 0) {
                                //Preparation of the current entity answer [NEWC␣ip␣port\n]
                                char* mess = malloc(sizeof (char)*120);
                                strcpy(mess, "WELC ");
                                //Establishing ip(next_ip)
                                strcat(mess, ip_addZero((*e).next_ip1));
                                strcat(mess, " ");
                                //Establishing port(next_uport)
                                char* next_uport = intchar((*e).next_uport1, 4);
                                if (next_uport != NULL) {
                                    strcat(mess, next_uport);
                                    strcat(mess, " ");
                                    //Establishing ip_diff
                                    char* cast_ip = ip_addZero((*e).cast_ip1);
                                    if (cast_ip != NULL) {
                                        strcat(mess, cast_ip);
                                        strcat(mess, " ");
                                        //Establishing port_diff
                                        char* cast_port = intchar((*e).cast_port1, 4);
                                        if (cast_port != NULL) {

                                            strcat(mess, cast_port);
                                            strcat(mess, "\n");

                                            //Sending to the previous entity the message about current entity
                                            send(sock2, mess, strlen(mess) * sizeof (char), 0);
                                            printf("serv_tcp : Message sent %s\n", mess);
                                            free(next_uport);
                                            free(cast_port);
                                            //Manage message reception
                                            char buff[100];
                                            int recu = recv(sock2, buff, 99 * sizeof (char), 0);
                                            buff[recu] = '\0';
                                            if (recu > 0) {
                                                char** tab = split(buff, ' ');
                                                //Managing the insertion
                                                if (str_arrsize(tab) == 3) {
                                                    if ((*e).nb_insert < 2) {
                                                        if (strcmp(tab[0], "NEWC") == 0) {
                                                            int next_uport = atoi(tab[2]);
                                                            if (check_ip(tab[1]) != -1
                                                                    && next_uport <= 9999 && next_uport > 0) {
                                                                (*e).next_ip1 = tab[1];
                                                                (*e).next_uport1 = next_uport;
                                                                printf("serv_tcp : Message recu : %s\n", buff);
                                                                mess = "ACKC\n";

                                                                send(sock2, mess, strlen(mess) * sizeof (char), 0);
                                                                (*e).nb_insert = 1;
                                                                printf("serv_tcp : Insertion succeed\n");
                                                                close(sock2);
                                                            }
                                                        }
                                                    }
                                                    //Managing the duplication 
                                                } else if (str_arrsize(tab) == 5) {
                                                    if ((*e).nb_insert == 1) {
                                                        if (strcmp(tab[0], "DUPL") == 0) {
                                                            int next_port2 = atoi(tab[2]);
                                                            int cast_port2 = atoi(tab[4]);
                                                            if (next_port2 <= 9999 && next_port2 > 0
                                                                    && cast_port2 <= 9999 && cast_port2 > 0
                                                                    //&& cast_port2 != next_port2 && strcmp(tab[1],tab[3])!=0
                                                                    ) {
                                                                (*e).next_ip2 = tab[1];
                                                                (*e).next_uport2 = next_port2;
                                                                (*e).cast_ip2 = tab[3];
                                                                (*e).cast_port2 = cast_port2;
                                                                printf("serv_tcp : Message recu : %s\n", buff);

                                                                char msg[120] = "ACKD ";
                                                                char* my_uport = intchar((*e).my_uport, 4);
                                                                //printf("Im here");
                                                                if (strlen(my_uport) > 0) {
                                                                    strcat(msg, my_uport);
                                                                    strcat(msg, "\n");
                                                                    //printf("message %s\n",msg);
                                                                    send(sock2, msg, sizeof (char)*strlen(msg), 0);
                                                                    //printf("serv_tcp : Message sent %s\n",msg);
                                                                    printf("serv_tcp : Duplication succeed\n");
                                                                    free(my_uport);
                                                                    (*e).nb_insert = 2;
                                                                    close(sock2);
                                                                }
                                                            } else {
                                                                fprintf(stderr, "serv_tcp : Wrong arguments given for duplication\n");
                                                                return -1;
                                                            }
                                                        } else {
                                                            fprintf(stderr, "serv_tcp : Problem with the message received\n");
                                                            return -1;
                                                        }
                                                    } else {
                                                        fprintf(stderr, "serv_tcp : Number of duplications authorized is overpassed\n");
                                                        return -1;
                                                    }

                                                } else {
                                                    fprintf(stderr, "serv_tcp : Problem with the message received wrong arguments\n");
                                                    close(sock2);
                                                }
                                            } else {
                                                fprintf(stderr, "serv_tcp : Problem with the message received wrong form serv %s\n", buff);
                                                close(sock2);
                                            }
                                        } else {
                                            fprintf(stderr, "serv_tcp : Problem with entity cast port1 %d\n", (*e).cast_port1);
                                        }
                                    } else {
                                        fprintf(stderr, "serv_tcp : Problem with cast_ip1 %s\n", (*e).cast_ip1);
                                    }
                                } else {
                                    fprintf(stderr, "serv_tcp : Problem with next udp port1 %d\n", (*e).next_uport1);
                                }
                            } else {
                                perror("serv_tcp : Problem with the connection to the server ");
                                close(sock2);
                            }
                        }
                    } else {
                        perror("serv_tcp : ");
                    }
                } else {
                    perror("serv_tcp : Problem with the bind of the server socket ");
                    return -1;
                }
            } else {
                perror("serv_tcp : Problem for inet_aton with ip ");
            }
        } else {
            fprintf(stderr, "serv_tcp : Wrong tcp port given %d\n", (*e).tcp_port);
        }
    }
    return -1;
}

/*Function for using thread for the server tcp
 */
void* pth_tserv(void* arg) {
    entity* e = (entity*) arg;
    printf("Start of tcp server\n");
    serv_tcp(e);
    return NULL;
}

/*Function for dupliaction into a ring
 * Pas encore tester, à tester après avoir compris le sujet
 */
int duplication(entity* e, char* host, int e1_tcp) {
    if ((*e).nb_insert == 0) {

        //Settings structures needed later
        int sock = socket(PF_INET, SOCK_STREAM, 0);
        struct sockaddr_in adressin;

        adressin.sin_family = AF_INET;
        adressin.sin_port = htons(e1_tcp);

        int inet = inet_aton(ip_removeZero(host), &(adressin.sin_addr));
        if (inet != 0) {
            int con = connect(sock, (struct sockaddr*) &adressin, (socklen_t)sizeof (struct sockaddr_in));
            if (con == 0) {
                ;
                char buff[100];
                int recu = recv(sock, buff, 99 * sizeof (char), 0);
                if (recu > 0) {
                    //Managing the message received from the previous entity [WELC␣ip␣port␣ip-diff␣port-diff\n]
                    buff[recu] = '\0';
                    printf("duplication : Message received from entity of the ring %s\n", buff);
                    char** tab = split(buff, ' ');
                    if (str_arrsize(tab) == 5) {
                        char* ip_e2 = tab[1];
                        int port_e2 = atoi(tab[2]);
                        char* ip_cast = tab[3];
                        int port_cast = atoi(tab[4]);
                        //Allocation to current entity of the information received from the previous entity
                        if (strcmp("WELC", tab[0]) == 0
                                && check_ip(ip_e2) != -1
                                && check_ip(ip_cast) != -1
                                && port_e2 <= 9999 && port_e2 > 0
                                && port_cast <= 9999 && port_cast > 0
                                ) {
                            (*e).next_ip1 = ip_e2;
                            (*e).next_uport1 = port_e2;
                            //(*e).cast_ip1 = ip_cast;  
                            //(*e).cast_port1 = port_cast; 
                            //Preparation of the current entity answer [DUPL␣...\n]
                            char* mess = malloc(sizeof (char)*120);
                            strcpy(mess, "DUPL ");
                            strcat(mess, ip_addZero((*e).my_ip));
                            strcat(mess, " ");
                            strcat(mess, intchar((*e).my_uport, 4));
                            strcat(mess, " ");

                            strcat(mess, ip_addZero((*e).cast_ip1));
                            strcat(mess, " ");
                            strcat(mess, intchar((*e).cast_port1, 4));
                            strcat(mess, "\n");
                            //Sending to the previous entity the message about current entity
                            send(sock, mess, strlen(mess) * sizeof (char), 0);
                            printf("duplication : Message send %s\n", mess);
                            //free(mess);
                            char buff2[100];
                            int recu2 = recv(sock, buff2, 99 * sizeof (char), 0);
                            if (recu2 > 0) {
                                buff2[recu2] = '\0';
                                printf("duplication : Message received from the entity of the ring %s\n", buff2);
                                tab = split(buff2, ' ');
                                //printf("Value of buff %s\n",buff2);
                                //printf("Value of size %d\n",str_arrsize(tab));
                                if (str_arrsize(tab) == 2) {
                                    if (strcmp(tab[0], "ACKD") == 0) {
                                        int port = 0;
                                        port = atoi(tab[1]);
                                        if (port <= 9999 && port > 0) {
                                            (*e).next_uport1 = port;
                                            close(sock);
                                            free(tab);
                                            (*e).nb_insert = 1;
                                            printf("--------------------------------------------------\n");
                                            printf("insertion : This my tcp_port %d\n", (*e).tcp_port);
                                            printf("--------------------------------------------------\n");
                                            //serv_tcp(e);
                                            return 0;
                                        } else {
                                            fprintf(stderr, "duplication : Probleme with the port received\n");
                                        }
                                    } else {
                                        fprintf(stderr, "duplication : Wrong message received\n");
                                    }
                                } else {
                                    fprintf(stderr, "duplication : Problem with message received from the entity of ring %s\n", buff2);
                                }
                            } else {
                                perror("duplication recv : Problem with the second message received from the entity of ring  ");
                            }
                        } else {
                            fprintf(stderr, "duplication : Problem with the ip and ports received\n");
                        }
                    } else {
                        fprintf(stderr, "duplication : Problem wrong message form, lack of arguments\n");
                    }
                } else {
                    perror("duplication : recv : Problem the number of char received for the message  ");
                }
            } else {
                perror("duplication : connect : Problem with the connection  ");
            }
        } else {
            fprintf(stderr, "duplication : Problem for inet_aton with ip\n");
        }
    } else {
        fprintf(stderr, "duplication : Number of duplication authorized is not what is required\n");
    }
    return -1;
}

/*Function for using thread for the duplication
 */
void* pth_dupl(void* arg) {
    entity* e = (entity*) arg;
    char* host = (*e).my_ip;
    char* ip = get_ip();

    int tcp_port = (*e).tcp_port;
    int port = free_tport(ip);
    if (ip != NULL && port != -1) {
        (*e).my_ip = ip;
        (*e).tcp_port = port;
        printf("Start of duplication\n");
        duplication(e, host, tcp_port);
    }
    return NULL;
}
