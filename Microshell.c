#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

#define GREEN "\x1B[32m"
#define MAGENTA "\x1B[35m"
#define RESET "\x1B[0m" 
#define WHITE "\x1B[37m"
#define RED "\x1B[31m"

void type_prompt();
void check_command(char *s);
void cd(char *dir);
void rm(char *file);

int main(){
char polecenie[100];
char history[500][500];
int licznik=0;
int j;

printf("\n\n *** Projekt Microshell ***\n Autor:Marcin Krupinski \n\n");



	while(1){
	type_prompt();
	fgets(polecenie,sizeof(polecenie),stdin);
	if(polecenie[strlen(polecenie)-1]=='\n')
		polecenie[strlen(polecenie)-1] = 0;
	for(j=0;j<=strlen(polecenie);j++){
	    history[licznik][j]=polecenie[j];
	}
	licznik = licznik+1;
	
	if(strcmp(polecenie,"exit")==0)
		return 0;
	else if(strcmp(polecenie,"help")==0){
		printf("\n\n**************************************\n");
		printf("Autorem programu jest Marcin Krupinski\n");
		printf("Jego obecne funkcje to:\n");
		printf("1.exit - powoduje zakonczenie programu\n");
		printf("2.cd - zmiana obecnego katalogu\n");
		printf("3.rm - usuwa dany plik\n");
		printf("4.history - wyswietla historie wpisanych polecen\n");
		printf("**************************************\n\n");
		}
	else if(strcmp(polecenie,"history")==0){
		int i;
		int hist_count = 1;
		for(i=0;i<licznik;i++){
			printf("%d  ",hist_count);
			for(j=0;j<500;j++)
				printf("%c",history[i][j]);
	 		printf("\n");
			hist_count = hist_count+1;
		}
	}

	else 
	   check_command(polecenie);
	}
return 0;
}


void type_prompt(){
char directory[200];
getcwd(directory,sizeof(directory));
printf(WHITE "[" MAGENTA "%s:" GREEN "%s" WHITE "]" GREEN "\n$ " RESET,getlogin(),directory);
}

void check_command(char *s){
char korektor[] = " ";
char *schowek;
schowek = strtok(s,korektor);
	while(1){
		if(strcmp(schowek,"cd")==0){
		schowek = strtok(NULL,korektor);
		if(schowek==NULL){
			chdir("/root");
			break;
		}
		else{
			cd(schowek);
			break;
		}
	     }
		else if(strcmp(schowek,"rm")==0){
		schowek = strtok(NULL,korektor);
		rm(schowek);
		break;
		}
		else{
		   char *args[10];
		   int licznik=0;
		   while(schowek!=NULL){
			args[licznik] = schowek;
			licznik = licznik+1;
			schowek = strtok(NULL,korektor);
			}
		   args[licznik] = NULL;
		   int pid;
		   pid = fork();

		   if(pid==0){

			if(execvp(args[0],args)==-1){
				perror(RED "Blad: Nieobslugiwane polecenie!" RESET);
				exit(1);
			}
		   }
		   else{
		      wait(NULL);
			break;
		   }
		}
	}
}
void cd(char *dir){
if(strcmp(dir,"~")==0)
	chdir("/root");
else if(chdir(dir)!=0)
	perror(RED "Blad: Nie udalo znalezc sie podanej sciezki!" RESET);
}
void rm(char *file){
int status;
status = remove(file);
if(status == 0)
	printf(GREEN "Pomyslne usuniecie pliku!\n" RESET);
else
	perror(RED "Blad: Nie udalo sie usunac pliku!" RESET);
}




