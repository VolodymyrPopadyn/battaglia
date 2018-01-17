#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <unistd.h>		//per poter sospendere per tot secondi
#define close closesocket

//inizializzazione delle costanti
const int DIM = 10;
const char NAVE = '*';
const char COLPITO = 'x';
const char ACQUA = 'o';
int PORTA = 2620;			//modificare questa in base alla rete
//color
const int black 	  = 0;
const int dark_blue   = 1;
const int dark_green  = 2;
const int dark_aqua   = 3;
const int dark_red 	  = 4;
const int dark_purple = 5;
const int dark_yellow = 6;
const int dark_white  = 7;
const int gray 		  = 8;
const int blue 		  = 9;
const int green 	  = 10;
const int aqua 		  = 11;
const int red 		  = 12;
const int purple 	  = 13;
const int yellow 	  = 14;
const int white 	  = 15;

//sezione del server
init_winsock(void) {
	WSADATA wsadata;
	int err;
	err=WSAStartup(MAKEWORD(2,2),&wsadata);
	if(err!=0)
	return 0;
	else
	return 1;
}
//fine sezione server

void setTextColor(int backcolor, int forecolor) {
	HANDLE hConsole;
	int colorattribute;
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	colorattribute = forecolor + backcolor * 4;
	SetConsoleTextAttribute(hConsole, colorattribute);
}

//stampa di due griglie con anche intestazione di righe e colonne e il "nome" delle matrici
void StampaGriglia(char G1[DIM][DIM], char G2[DIM][DIM]) {
	int i, j;
	setTextColor(black, white);
	//se modifico la visualizzazione devo modificare sia la riga successiva, sia quella dopo ancora
	printf("   Tabella avversaria                                 Tabella mia\n");
	printf("   | A | B | C | D | E | F | G | H | I | J            | A | B | C | D | E | F | G | H | I | J \n");
	
		for(i=0;i<DIM;i++) {
			printf("---+---+---+---+---+---+---+---+---+---+---        ---+---+---+---+---+---+---+---+---+---+---\n");
			printf("%2d ",i);
			for(j=0;j<DIM;j++) {
				setTextColor(black, white);
				printf("|");
				if(G1[i][j]==NAVE) {
					setTextColor(black, purple);
					printf(" * ", G1[i][j]);
				} else if(G1[i][j]==COLPITO) {
					setTextColor(black, red);
					printf(" x ", G1[i][j]);
				} else if(G1[i][j]==ACQUA) {
					setTextColor(black, aqua);
					printf(" o ", G1[i][j]);
				} else {
					setTextColor(black, white);
					printf("%2c ", G1[i][j]);
				}
			}
			setTextColor(black, white);
			//sia questa prossima
			printf("        %2d ",i);
			for(j=0;j<DIM;j++) {
				setTextColor(black, white);
				printf("|");
				if(G2[i][j]==NAVE) {
					setTextColor(black, purple);
					printf(" * ", G2[i][j]);
				} else if(G2[i][j]==COLPITO) {
					setTextColor(black, red);
					printf(" x ", G2[i][j]);
				} else if(G2[i][j]==ACQUA) {
					setTextColor(black, aqua);
					printf(" o ", G2[i][j]);
				} else {
					setTextColor(black, white);
					printf("%2c ", G2[i][j]);
				}
			}
			printf("\n");
		}
	printf("\n");
}

//crea una matrice vuota
void GrigliaVuota(char G[DIM][DIM]) {
	int i, j;
	for(i=0; i!=DIM; i++) {
		for(j=0; j!=DIM; j++) {
			G[i][j]= ' ';
		}
	}
}

//controlla la sinistra della posizione passata con colonna e verifica se ? all'interno della matrice
void ControlloOrizzontaleSinistra(char matrice[DIM][DIM], int colonna, int riga, int x) {
	int i;
	if(colonna-x>=-1){
		for(i=1; i<x; i++) {
			colonna--;
			matrice[riga][colonna] = NAVE;
		}
	}
}

//controlla la destra della posizione passata con colonna e verifica se ? all'interno della matrice
void ControlloOrizzontaleDestra(char matrice[DIM][DIM], int colonna, int riga, int x) {
	int i;
    if(colonna+x<=10) {
		for(i=1; i<x; i++) {
			colonna++;
			matrice[riga][colonna] = NAVE;
		}
	}
}

//controlla il sopra della posizione passata con colonna e verifica se ? all'interno della matrice
void ControlloVerticaleSu(char matrice[DIM][DIM], int colonna, int riga, int x) {
	int i;
	if(riga-x>=-1){
		for(i=1; i<x; i++) {
			riga--;
			matrice[riga][colonna] = NAVE;
		}
	}
}

//controlla il sotto della posizione passata con colonna e verifica se ? all'interno della matrice
void ControlloVerticaleGiu(char matrice[DIM][DIM], int colonna, int riga, int x) {
	int i;
    if(riga+x<=10) {
		for(i=1; i<x; i++) {
			riga++;
			matrice[riga][colonna] = NAVE;
		}
	}
}

//prende una lettera e restituisce il numero equivalente per le colonne
char prendiLettera() {
	char cl = -1;
	int entrato = 0;
	while(cl<0 || cl>10 && entrato!=0) {
		printf("\rInserisci la colonna: ");
		scanf("%c",& cl);
		cl -=65;
		entrato = 1;
	} 
	return cl;
}

//prende un numero (in carattere) e restituisce il relativo valore in intero
char prendiNumero() {
	char rg;
	do {
		printf("\rInserisci la riga: ");
		scanf("%c",& rg);
		rg -= 48;
	} while(rg<0 || rg>9);
	return rg;
}

//chiede la direzione per la nave
int selezioneDirezione() {
	int take;
	do {
		printf("1) La nave va in su\n");
		printf("2) La nave va in gi%c\n", 151);
		printf("3) La nave va a destra\n");
		printf("4) La nave va a sinistra\n");
		printf("Inserisci la tua decisione: ");
		scanf("%d", &take);
	}while(take<1 || take>4);
	return take;
}

//posiziona una nave con i parametri passati
void posizionaNave(char Griglia1[DIM][DIM], char Griglia2[DIM][DIM], int cl, int rg, int x, int dir) {
	switch(dir) {
		case 1:		//la nave va in su
			ControlloVerticaleSu(Griglia2, cl, rg, x);
			break;
		
		case 2:		//la nave va in giù\n
			ControlloVerticaleGiu(Griglia2, cl, rg, x);
			break;
			
		case 3:		//la nave va a destra
			ControlloOrizzontaleDestra(Griglia2, cl, rg, x);
			break;
			
		case 4:		//la nave va a sinistra
			ControlloOrizzontaleSinistra(Griglia2, cl, rg, x);
			break;
	}
}

void prendiNave(char Griglia1[DIM][DIM], char Griglia2[DIM][DIM], char cl, char rg, int x, int dir) {
	Griglia2[rg][cl] = NAVE;
	posizionaNave(Griglia1, Griglia2, cl, rg, x, dir);
}

//funzione per determinare se si è presa una nave oppure no
int naveColpita(char G1[DIM][DIM], int col, int rig) {
	if(G1[rig][col] == NAVE) {
		return 1;
	} else {
		return -1;
	}
}

//riempie nella posizione indicata col messaggio passato
void riempiGriglia(char G[DIM][DIM], int col, int rig, char msg) {
	G[rig][col] = msg;
}

//verifica se le posizioni in cui dovrebbe andare la nave non sono occupate
int accettabile( char matrice[DIM][DIM], int x, int colonna, int riga, int dir) {
	int i;
	switch(dir) {
		case 1:
			if(riga-x>=0) {
				for(i=1; i<=x; i++) {
					riga--;
					if(matrice[riga][colonna] == NAVE || matrice[riga][colonna-1]==NAVE || matrice[riga][colonna+1]==NAVE) {
						return -1;
					}
				}
			} else {
				return -1;
			}
			return 1;
			break;
		case 2:
			if(riga+x<=10) {
				for(i=1; i<=x; i++) {
					riga++;
					if(matrice[riga][colonna] == NAVE || matrice[riga][colonna-1]==NAVE || matrice[riga][colonna+1]==NAVE) {
						return -1;
					}
				}
			}  else {
				return -1;
			}
			return 1;
			break;
		case 3:
			if(colonna+x<=10) {
				for(i=1; i<=x; i++) {
					colonna++;
					if(matrice[riga][colonna] == NAVE || matrice[riga-1][colonna]==NAVE || matrice[riga+1][colonna]==NAVE) {
						return -1;
					}
				}
			}  else {
				return -1;
			}
			return 1;
			break;
		case 4:
			if(colonna-x>=0) {
				for(i=1; i<=x; i++) {
					colonna--;
					if(matrice[riga][colonna] == NAVE || matrice[riga-1][colonna]==NAVE || matrice[riga+1][colonna]==NAVE) {
						return -1;
					}
				}
			}  else {
				return -1;
			}
			return 1;
			break;
	}
}

void menShip(int n2, int n3, int n4, int n6) {
	printf("Ti rimangono ancora queste navi:\n");
	printf("%d navi da 2\n", n2);
	printf("%d navi da 3\n", n3);
	printf("%d navi da 4\n", n4);
	printf("%d navi da 6\n", n6);
}

//controllo le posizioni nelle vicinanze di quella passata tramite riga e colonna, una di quelle posizioni è occupata da una nave restituisco 1
int controlloInput(char matrice[DIM][DIM], int riga, int colonna) {
	int i, j;
	int xi[9] = {-1, +0, +1, +1, +1, +0, -1, -1, +0};
	int xj[9] = {+1, +1, +1, +0, -1, -1, -1, +0, +0};
	for(i=0; i!=9; i++) {
		for(j=0; j!=9; j++) {
			if(matrice[riga+xi[i]][colonna+xj[i]]==NAVE) {
				return 1;
			}
		}
	}
	return 0;
}

void pulisciSchermo(char m1[DIM][DIM], char m2[DIM][DIM]) {
	system("cls");
	StampaGriglia(m1, m2);
}

int main() {
	char Griglia1[DIM][DIM];         //matrice per l'avversario
	char Griglia2[DIM][DIM];         //matrice utilizzata da me
	int i, j, dir;
	char cooIns[4];
	char cl, rg, x;
	int nave2 = 4;
	int nave3 = 3;
	int nave4 = 2;
	int nave6 = 1;
	int contNavi = 0;
	int vittoria = -1;
	
	GrigliaVuota(Griglia1);
	GrigliaVuota(Griglia2);
	
	do {
		pulisciSchermo(Griglia1, Griglia2);
		do {
			do {
				pulisciSchermo(Griglia1, Griglia2);
				menShip(nave2, nave3, nave4, nave6);
				printf("\rScegli la grandezza della nave che vuoi inserire (2,3,4,6): ");
				scanf("%c",&x);
				switch(x) {
					case 50: if(nave2>0) { } else { x = 1; }; break;
					case 51: if(nave3>0) { } else { x = 1; }; break;
					case 52: if(nave4>0) { } else { x = 1; }; break;
					case 54: if(nave6>0) { } else { x = 1; }; break;
				}
			}while( x!=50 && x!=51 && x!=52 && x!=54 );
			x -= 48;
			pulisciSchermo(Griglia1, Griglia2);
			printf("Hai scelto la nave da %d\n", x);
			do {
				printf("Inserisci le coordinate iniziali della nave (es. A9): ");
				scanf("%s", cooIns);
				cl = cooIns[0] - 65;
				rg = cooIns[1] - 48;
			}while( controlloInput(Griglia2, rg, cl) != 0 || ((cl<0 || cl>9) || (rg<0 || rg>9)) || cooIns[2]!=0);
			pulisciSchermo(Griglia1, Griglia2);
			printf("Le coordinate iniziali [%c,%d]\n", (cl+65), rg);
			dir = selezioneDirezione();
		}while(accettabile(Griglia2, x, cl, rg, dir) != 1);
		
		switch(x) {
			case 2: 
					prendiNave(Griglia1, Griglia2, cl, rg, x, dir);
					nave2--;
					contNavi++; 
				break;
			case 3: 
					prendiNave(Griglia1, Griglia2, cl, rg, x, dir);
					nave3--;
					contNavi++;
				break;
			case 4: 
					prendiNave(Griglia1, Griglia2, cl, rg, x, dir);
					nave4--;
					contNavi++;
				break;
			case 6: 
					prendiNave(Griglia1, Griglia2, cl, rg, x, dir);
					nave6--;
					contNavi++;
				break;		
		}
	} while(contNavi!=10);
	
	int sock, clientsd, port = PORTA;
	char buffer[256];
	char buffer2[256];
	int flag;
	struct sockaddr_in saddr;

	if(!init_winsock()) {
	  printf("Errore durante l'inizializzazione di winsock2.\n");
	  return -1;
	}

	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	  printf("Errore durante la creazione del socket.\n");
	  return -1;
	}

	saddr.sin_family = PF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(port);

	if((bind(sock, (struct sockaddr *) &saddr, sizeof(saddr))) < 0) {
	  printf("Errore durante il binding del socket.\n");
	  return -1;
	}

	if((listen(sock, 1)) < 0) {
	  printf("Errore durante il settaggio del socket in listening.\n");
	  return -1;
	}

	printf("In attesa che un client si connetta...\n");
	clientsd = accept(sock, NULL, NULL);
	printf("Si e' connesso un client.\n\n");

	char colIns, rigIns, colRic, rigRic, preso;
	int controlloInserimento = 0;
	int naviColpite = 0;
	do {
		system("cls");
		StampaGriglia(Griglia1, Griglia2);
		//invia le coordinate
		do {
			
			printf("\rInserisci le coordinate che desideri colpire: ");
			scanf("%s", &buffer);
			colIns = buffer[0] - 65;
			rigIns = buffer[1] - 48;
		}while( (colIns<0 || colIns>=9) && (rigIns<0 || rigIns>=9) ||buffer[2]!=0);
		send(clientsd, buffer, sizeof(buffer), 0);
		flag = 1;
		fflush(stdin);

		//riceve colpito?
		while(controlloInserimento != 1){
			while (recv(clientsd, buffer2, 255, 0) > 0 && flag ==1) {
				flag = 0;
				preso = buffer2[0];	//prendo il primo carattere
			}
			fflush(stdin);
			flag = 1;
			controlloInserimento = 1;
		}
		controlloInserimento = 0;

		//posizionamento sulla propria matrice colpito o acqua
		switch(preso) {
			case 110: Griglia1[rigIns][colIns] = ACQUA; break;
			case 121: Griglia1[rigIns][colIns] = COLPITO; break;
		}
		system("cls");
		StampaGriglia(Griglia1, Griglia2);
	
		//riceve coordinate
		while(controlloInserimento != 1){
			while (recv(clientsd, buffer2, 255, 0) > 0 && flag ==1) {
				printf("Lettera inserita: %s\n", buffer2);
				flag = 0;
				colRic = buffer2[0] - 65;
				rigRic = buffer2[1] - 48;
			}
			fflush(stdin);
			flag = 1;
			controlloInserimento = 1;
		}
		controlloInserimento = 0;
		
		//verifica nave colpita
		
		if(naveColpita(Griglia2, colRic, rigRic) == 1) {
			strcpy(buffer, "y");
			Griglia2[rigRic][colRic] = COLPITO;
			naviColpite++;
		} else {
			strcpy(buffer, "n");
			Griglia2[rigRic][colRic] = ACQUA;
		}
		if(naviColpite==31) {
			strcpy(buffer, "e");	//end of game
		}
		//invia messaggio colpito?
		send(clientsd, buffer, sizeof(buffer), 0);
		StampaGriglia(Griglia1, Griglia2);
		
	}while(naviColpite!=31 || vittoria!=1);
	
	if(naviColpite==31) {
		printf("Mi spiace, hai perso!");
	} else {
		printf("Congratulazioni!, Hai vinto!");
	}

	printf("Connessione persa.\n");
	close(sock);
	WSACleanup();
	
	system("pause");
}
