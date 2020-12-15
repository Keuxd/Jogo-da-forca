#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

char objeto[30] = {};
char objeto_dica[30] = {};
int controle = 0;
int dificuldade = 2;// 1 = facil | 2 = normal | 3 = dificil
const int chances = 6;

void derrota();
void menu();
int resetar_jogo();
void preencher_objeto_comp(char v[]);
void tela(int *erros, char *palavra, char *letras);
int checar(char L);
void ler_letra(char *L, char *P);
void palavras(int numero);

int main(){
  int erros = 0;
  char letra;
  char objeto_comp[30] = {}, letras_colocadas[26] = {};

srand(time(0));
setlocale(LC_ALL, "Portuguese");
reset:;
  menu();
  palavras(21);
  system("clear || cls");
  preencher_objeto_comp(objeto_comp);  

while(1){

  tela(&erros, objeto_comp, letras_colocadas);
  ler_letra(&letra, letras_colocadas);

  switch(checar(letra)){
    case 0:{ //letra está errada
      if(dificuldade == 1 || dificuldade == 2){
        erros += 1;
      }
      else if(dificuldade == 3){
        erros += 2;
      }
      break;
    }
    case 1:{ //letra está certa
      for(int i = 0; i < strlen(objeto); i++){
        if(tolower(letra) == objeto[i] || toupper(letra) == objeto[i]){
          objeto_comp[i] = objeto[i];
        }
      }
      break;
    }
  }
  system("clear || cls");

  if(!strcmp(objeto_comp,objeto)){ //ganhar
    system("clear || cls");
    printf("Palavra: %s\n", objeto);
    printf("\nParabéns, você ganhou!\n");
    break;
  }
  else if(erros == chances){ //perder
    derrota();
    break;
  }
}
  
  switch (resetar_jogo()){
    case 0:{ //caso não, encerrar o programa
      printf("\nEncerrando o jogo...\n");
      sleep(1);
      break;
    }
    case 1:{ //caso sim, reiniciar as variaveis e voltar ao começo do programa
      printf("\n\nReiniciando o jogo...\n");
      sleep(1);
      erros = 0;
      for(int i = 0; i < 26; i++){
        letras_colocadas[i] = 0;
      }
      for(int i = 0; i < 30; i++){//reiniciando todos os valores estaticos.
        objeto[i] = 0;
        objeto_dica[i] = 0;
        objeto_comp[i] = 0;
        controle = 0;
      }
      goto reset;
    }
  }

  return 0;
}

//lê uma letra
void ler_letra(char *L, char *P)
{
  int esta = 0;
  printf("\nDigite uma letra: ");
  scanf(" %c", L);
//checagem para caso de repetição de letras
do{
  esta = 0;
  for(int i = 0; i < 26; i++){
    if(tolower(*L) == P[i] || toupper(*L) == P[i]){
      esta = 1;
      break;
    }
  }

  if(esta){
    printf("Esta letra já foi colocada, digite outra: ");
    scanf(" %c", L);

  }
  else if(!esta){
    P[controle] = toupper(*L);
    controle++;
  }
}while(esta);
}

//checa se o caractere passado está no objeto retornando 1 para sim e 0 para não
int checar(char L)
{
  int tamanho = strlen(objeto);
  for(int i = 0; i < tamanho; i++){
    if(tolower(L) == objeto[i] || toupper(L) == objeto[i]){
      return 1;
    }
  }
  return 0;
}

//abre e printa os arquivos respectivos ao nº de erros
void tela(int *erros, char *palavra, char *letras)
{
  FILE *forca = fopen("forca.txt", "r");
  char linha[15];

  printf("Palavra: ");
  for(int i = 0; i < strlen(palavra); i++){
    printf("%c ", palavra[i]);
  }
  
  printf("\n\nDica da palavra: %s \n\n", objeto_dica);
  switch (*erros){
    case 0:{
      fseek(forca,0,SEEK_SET);
      break;
    }
    case 1:{
      fseek(forca,41,SEEK_SET);
      break;
    }
    case 2:{
      fseek(forca,95,SEEK_SET);
      break;
    }
    case 3:{
      fseek(forca,155,SEEK_SET);
      break;
    }
    case 4:{
      fseek(forca,215,SEEK_SET);
      break;
    }
    case 5:{
      fseek(forca,276,SEEK_SET);
      break;
    }
    case 6:{
      fseek(forca,342,SEEK_SET);
      break;
    }
  }

  for(int i = 0; i < 8; i++){//printa 8 linhas do .txt
    fscanf(forca, " %[^\n]s ", linha);
    printf("%s\n", linha);
  }
  printf("\nLetras colocadas: ");
  for(int i = 0; i < 26; i++){//printa as letras existentes no vetor
    if(letras[i]){
      printf("%c ", letras[i]);
    }
  }
  printf("\nErros: %i\n\n", *erros);
  
  fclose(forca);
}

//preencher o vetor da palavra a ser completada com underlines '_' antes de começar o jogo
void preencher_objeto_comp(char v[])
{
  int tamanho = strlen(objeto);

  for(int i = 0 ; i < tamanho; i++){
    v[i] = '_';
  }
}

int resetar_jogo()
{
  char entrada[6];
  
  printf("\nDeseja jogar novamente ?\n");
  scanf("%s", entrada);

reset:;
  for(int i = 0; i < strlen(entrada); i++){
    entrada[i] = tolower(entrada[i]);
  }

  if(!strcmp(entrada, "sim")){
    return 1;
  }
  else if(!strcmp(entrada, "não") || !strcmp(entrada, "nÃo")){
    return 0;
  }
  else{
    printf("Entrada inválida, responda com 'Sim' ou 'Não': ");
    scanf("%s", entrada);
    goto reset;
  }
}

//printa e executa todas as funções do menu
void menu()
{
  FILE *F = fopen("menu.txt", "r");
  char linha[150];
  int entrada;

reset:;
  fseek(F,0,SEEK_SET);
  system("clear || cls");
  for(int i = 0; i < 8; i++){//printa 8 linhas do .txt
    fscanf(F, " %[^\n]s ", linha);
    printf("%s\n", linha);
  }

  //usando ANSI escape code para printar com o efeito 1(bold) cor 34(azul)
  printf("\033[1;34m                  0 = Iniciar | 1 = Info\033[0m\n\a");
  printf("\033[1;34m                        2 = Sair\033[0m\n\n\n\n");
  
  printf("\033[1;36m                   Fácil\033[0m");
  printf("\033[1;33m Normal\033[0m");
  printf("\033[1;31m Difícil\033[0m\n");

  switch(dificuldade){//visual da dificuldade
    case 1:{
      printf("\033[1;36m                    [x]\033[0m");
      break;
    }
    case 2:{
      printf("\033[1;33m                           [x]\033[0m");
      break;
    }
    case 3:{
      printf("\033[1;31m                                  [x]                \033[0m");
      break;
    }
  }
  printf("\n\n");
  scanf("%i", &entrada);

  switch(entrada){//caminhos para o botão digitado pelo usuário
    case 0:{//sai da função menu e incia o jogo na main
      break;
    }
    case 1:{//info
      fseek(F,453,SEEK_SET);
      system("clear || cls");
      for(int i = 0; i < 7; i++){
        fscanf(F," %[^\n]s ", linha);
        if(i <= 3){
          printf("\033[1;32m%s\033[0m\n\n\a", linha);
        }
        else if(i == 4){
          printf("\033[1;36m%s\033[0m\n\n\a", linha);
        }
        else if(i == 5){
          printf("\033[1;33m%s\033[0m\n\n\a", linha);
        }
        else if(i == 6){
          printf("\033[1;31m%s\033[0m\n\n\a", linha);
        }
      }
      sleep(5);
      printf("\nDigite um número para voltar ao menu: ");
      scanf("%i", &entrada);
      goto reset;
    }
    case 2:{//encerra o programa
      sleep(2);
      exit(1);
    }
    case 3:{//escolhe a dificuldade facil
      dificuldade = 1;
      goto reset;
    }
    case 4:{//escolhe a dificuldade normal
      dificuldade = 2;
      goto reset;
    }
    case 5:{//escolhe a dificuldade dificil
      dificuldade = 3;
      goto reset;
    }
    default:{
      goto reset;
    }
  }
  fclose(F);
}

//printa a derrota :(
void derrota()
{
  FILE *P = fopen("forca.txt", "r");
  char linha[15];

  system("clear || cls");
  printf("Você perdeu X_X\n\n");

  fseek(P,342,SEEK_SET);

  for(int i = 0; i < 8; i++){
    fscanf(P," %[^\n]s ", linha);
    printf("%s\n", linha);
  }
  fclose(P);
}

void palavras(int numero)
{
  FILE *pont_arq = fopen ("ditado.txt", "r");
  char linha[2][30];
  int i;

  for(i=0; i<numero; i++){//lendo a o objeto e a dica
    fscanf(pont_arq,"%s %[^\n]s", linha[0], linha[1]); 
  }

  for(i = 0; i<30; i++){//limpando o vetor dos objetos
    objeto[i] = 0;
    objeto_dica[i] = 0;
  }

  strcpy(objeto,linha[0]);
  strcpy(objeto_dica,linha[1]);

  fclose(pont_arq);
}
