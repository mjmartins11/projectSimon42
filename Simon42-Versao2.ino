#define LED_VERMELHO 12
#define LED_AMARELO 8
#define LED_VERDE 10

#define LED_PERDEU 6
#define LED_GANHOU 7

#define BOTAO_VERMELHO 13
#define BOTAO_AMARELO 9
#define BOTAO_VERDE 11

#define CODIGO_VERMELHO 0
#define CODIGO_AMARELO 1 
#define CODIGO_VERDE 2

#define QUARTO_SEGUNDO 250
#define MEIO_SEGUNDO 500
#define DOIS_SEGUNDOS 2000

#define QUANTIDADE_DE_RODADAS 7
#define CONTAGEM_TENTATIVAS 120

void setup() {  
  Serial.begin(9600);
  
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_PERDEU, OUTPUT);
  pinMode(LED_GANHOU, OUTPUT);
  
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERDE, INPUT_PULLUP);

  tornar_aleatorio();
}

void tornar_aleatorio() {
  randomSeed(analogRead(A0));
}

void gerar_sequencia(int sequencia[QUANTIDADE_DE_RODADAS]){
    int i;
    for(i = 0; i < QUANTIDADE_DE_RODADAS; i++) {
      sequencia[i] = random(CODIGO_VERMELHO, CODIGO_VERDE + 1);
    }   
}

int selecionar_led(int led){
  switch(led) {
    case CODIGO_VERMELHO:
      return LED_VERMELHO;
    break;  
    case CODIGO_AMARELO:
      return LED_AMARELO;
    break;
    case CODIGO_VERDE:
      return LED_VERDE;
    break;
  }
}


int selecionar_botao(int led){
  switch(led) {
    case CODIGO_VERMELHO:
      return BOTAO_VERMELHO;
    break;  
    case CODIGO_AMARELO:
      return BOTAO_AMARELO;
    break;
    case CODIGO_VERDE:
      return BOTAO_VERDE;
    break;
  }
}

void piscar_led(int led, int indice, int quantidade_de_vezes, int intervalo) {
  
  digitalWrite(led, HIGH);
  delay (intervalo);
  digitalWrite(led, LOW);
  delay (intervalo);
    
  if(indice == quantidade_de_vezes) {
    return;
  }

  indice++;
  piscar_led(led, indice, quantidade_de_vezes, intervalo);

}

void imprimir_sequencia(int sequencia[QUANTIDADE_DE_RODADAS], int contador){ 
  int i;
  int led;
  
  for(i = 0; i < contador; i++) {
    led = selecionar_led(sequencia[i]);
    piscar_led(led, 1, 1, MEIO_SEGUNDO);
  }
  
}

int ler_botao() {
  int botao = -1, tentativas = 0;
  
    do {
      Serial.println("esperando botao");
      if (digitalRead(BOTAO_VERMELHO) == 0){
        botao = BOTAO_VERMELHO;
        break;
      } 
      
      if (digitalRead(BOTAO_AMARELO) == 0){
        botao = BOTAO_AMARELO;
        break;
      }
      
      if (digitalRead(BOTAO_VERDE) == 0){
        botao = BOTAO_VERDE;
        break;
      }
      
      tentativas++;
  } while(botao == -1 && tentativas < CONTAGEM_TENTATIVAS);

  return botao;
}

int ler_sequencia(int sequencia[QUANTIDADE_DE_RODADAS], int contador) {
  int i;

  for(i = 0; i < contador; i++) {
    Serial.print(i);
    Serial.print(" quer botao ");
    Serial.println(selecionar_botao(sequencia[i]));
    if(ler_botao() != selecionar_botao(sequencia[i])) {
      Serial.println("errou");
      return 0; 
    }
    delay(MEIO_SEGUNDO);
  }

  Serial.println("acertou");
  return 1;
  
}

void loop() {
  Serial.println("**********inicio de jogo************");
  delay(DOIS_SEGUNDOS);
  
  int continuar = 1, contador = 0, ganhou;
  int sequencia[QUANTIDADE_DE_RODADAS];
  
  gerar_sequencia(sequencia);

  while(contador < QUANTIDADE_DE_RODADAS) {
    imprimir_sequencia(sequencia, contador + 1);
    ganhou = ler_sequencia(sequencia, contador + 1);
   
    if(!ganhou) {
      Serial.println("perdeu");
      break;
    }
    
    contador++;
  }

  if(ganhou) {
    piscar_led(LED_GANHOU, 1, 3, QUARTO_SEGUNDO);
  } else {
    piscar_led(LED_PERDEU, 1, 3, QUARTO_SEGUNDO);
  }

  Serial.println("**********fim de jogo************");
}
