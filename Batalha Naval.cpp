#include <iostream>
#include <limits>
#include <unistd.h>


using namespace std;

#define YELLOW  "\033[33m"      //Defenir a cor amarelo
#define RESET   "\033[0m"       //Comando para dar reset na cor e voltar ao padrão

int tamanhoBarcos[] = {5,4,3,3,2};
char CampoJogador1[10][10], CampoJogador2[10][10],CampoJogador1Atacar[10][10],CampoJogador2Atacar[10][10];//Campo do jogador 1; Campo do jogador 2; Campo que o joagdor 1 ataca; Campo que o jogador 2 ataca
int listaBarcosJogador2[17][3]; //Lista dos barcos do jogador 2
int listaBarcosJogador1[17][3];     //Lista dos barcos do jogador 1
char orientacao;        //Guarda o v ou h
int opcao,linha,coluna,linhaSimulacao,colunaSimulacao;
int a = 1;
int contBarcos=0;
int barcosDerrotados=0;
bool acertou=false;

int l,c;
int linhaBarco = 0;


void menu() {// Função menu
    cout << "=== Batalha Naval ===" << endl;
    cout << "1-Iniciar Jogo" << endl;
    cout << "2-Sair" << endl;
}

void PrintCampo(char Campo[10][10], string jogador) {//Função para dar print nos campos de batalha
    cout << "Campo do " << jogador << ":" << endl;

    cout << "  0 1 2 3 4 5 6 7 8 9" << endl; // Adiciona a linha de índices

    for (int i = 0; i < 10; i++) {
        cout << i <<" "; // Adiciona o índice da linha no início de cada linha

        for (int j = 0; j < 10; j++) {   
            cout << Campo[i][j] << ' ';
        }

        cout << endl;
    }
    cout << endl;
}

void CampoDaBatalha(char Campo[10][10]) {//Função para inicializar os campos de batalha
    // Inicializa o campo com 'O'
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            Campo[i][j] = '~';
        }
    }
}

void GuardarBarcos(int Campo[17][3]) {//Função para guardar os barcos numa matriz iniciada com valores a -1
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 3; j++) {
            Campo[i][j] = -1;
        }
    }
}

void EscolherPosicao(int variavelFor){//Função para escolher a posição dos barcos a colocar no campo
    cout << "Introduza as coordenadas para o barco tamanho " << tamanhoBarcos[variavelFor] << endl;//Pede as coordenadas dos barcos que estao guardados numa lista chamada tamanhoBarcos
    cout << "Linha(0-9): ";                                     
    while (!(cin >> linha) || linha < 0 || linha > 9)
    {
        cout << "Coordenada inválida. Por favor, digite um valor entre 0 e 9." << endl;  
        cin.clear();                                        //Limpa o estado de erro caso seja introduzido algum caracter inválido, o que permite que a entrada continue                                                  
        cin.ignore(numeric_limits<streamsize>::max(), '\n');//Garante que nao há caracteres inválidos deixaods no buffer de entrada após a leitura
    }

    cout << "Coluna(0-9): ";
    while (!(cin >> coluna) || coluna < 0 || coluna > 9)
    {
        cout << "Coordenada inválida. Por favor, digite um valor entre 0 e 9." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Introduza a orientação v(Vertical) ou h(Horizontal): ";
    while (!(cin >> orientacao) || (orientacao != 'h' && orientacao != 'v'))
    {
        cout << "Orientacao inválida. Por favor, digite 'h' ou 'v'." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (orientacao == 'h' && coluna + tamanhoBarcos[variavelFor] - 1 > 9)  //Se escolher opção h, na coluna que o utilizador introduziu é somado o tamanho do barco
    {
        a = 0;   //Se for possivel adicionar o a==0
    }

    if (orientacao == 'v' && linha + tamanhoBarcos[variavelFor] - 1 > 9) //Se escolher a opção v, na linha que o utilizador introduziu é somado o tamanho do barco
    {
        a = 0;   //Se for possivel adicionar o a==0
    }
}

void IntroduzirBarcos(int variavelFor, int listaBarcos[17][3], char jogador[10][10]){//Função que coloca os barcos no campo consoante a posição e guarda-os numa lista
    
    for (int y = 0; y < tamanhoBarcos[variavelFor]; y++)
    {
        listaBarcos[linhaBarco][0] = tamanhoBarcos[variavelFor]; //Substitui na lista listaBarcos o tamanho na 1 posição, e as coordenadas de cada ponto do barco
        listaBarcos[linhaBarco][1] = linha;                    
        listaBarcos[linhaBarco][2] = coluna;

        linhaBarco++;                        //Por cada linha preenche o tamanho, coluna e linha. Depois passa a proxima linha e volta  apreencher

        if (orientacao == 'h')
        {
            coluna++;                                   
        }
        if (orientacao == 'v')
        {
            linha++;
        }
    }

    for (int abc = 0; abc < 17; abc++)   //Vai a cada posição de cada ponto do barco e subsititui pela letra B      
    {
        l = listaBarcos[abc][1];
        c = listaBarcos[abc][2];

        jogador[l][c] = 'B';
    }
}

void SimularIntroducaoBarcos(int variavelFor,int listaBarcos[17][3]){//Função para evitar sobreposições. Simula antes de colcar na matriz a posição do barco

    linhaSimulacao = linha;
    colunaSimulacao = coluna;                    //Verifica se a coluna e a linha introduzida pelo jogador já esta na lista dos barcos

    for (int e = 0; e < tamanhoBarcos[variavelFor]; e++)
    {

        for (int d = 0; d < 17; d++)
        {

            if (listaBarcos[d][1] == linhaSimulacao && listaBarcos[d][2] == colunaSimulacao) 
            {
                a = 0;
            }
        }

        if (orientacao == 'h')
        {
            colunaSimulacao++;
        }

        if (orientacao == 'v')
        {
            linhaSimulacao++;
        }
    }
}

void EscolherAtaque(){  //Função para escolher a coordenada para efetuar o ataque
    cout << "Linha(0-9): ";
    while (!(cin >> linha) || linha < 0 || linha > 9)    
    {
        cout << "Coordenada inválida. Por favor, digite um valor entre 0 e 9." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Coluna(0-9): ";
    while (!(cin >> coluna) || coluna < 0 || coluna > 9)
    {
        cout << "Coordenada inválida. Por favor, digite um valor entre 0 e 9." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void PrintMatrizesAtacar(char Campo[10][10],char CampoAtacar[10][10]){   //Dá print no campo em que o jogador vai atacar e o seu próprio campo

    cout << "  0 1 2 3 4 5 6 7 8 9 " << endl;

    for (int i = 0; i < 10; ++i) {
        cout << i << " "; // Adiciona o índice da linha

        // Imprime elementos do campo em que se ataca
        for (int j = 0; j < 10; ++j) {
            cout << Campo[i][j] << " ";
        }

        cout << "  |   ";

        // Imprime elementos do campo do jogador
        for (int j = 0; j < 10; ++j) {
            cout << CampoAtacar[i][j] << " ";
        }

        cout << endl;
    }
}

void Jogar(char Campo1[10][10],char Campo2[10][10],char CampoAtacante[10][10],int listaDosBarcosAtacar[17][3],string jogador){//Função do jogo
    do{
        cout << endl;

        EscolherAtaque();//Se o jogador escolher uma posição em que ja atacou, o programa volta a pedir
        while (CampoAtacante[linha][coluna] == 'A' || CampoAtacante[linha][coluna] == ' ')
        {
            cout << "Posição Inválida. Tente novamente." << endl;
            EscolherAtaque();
        }

        CampoAtacante[linha][coluna] = ' '; // Fica marcados onde se manda tiros na água

        if (Campo2[linha][coluna] == 'B')  //Se no campo do jogador oponente, na posição em que o jogador escolheu, estiver a letra 'B', no campo em que se ataca vai ficar a letra 'A' e no campo do jogador que foi atingido vai ficar a letra 'X'
        {
            acertou = true;                

            Campo2[linha][coluna] = 'X';
            CampoAtacante[linha][coluna] = 'A';

            for (int w = 0; w < 17; w++)//Os pontos dos barcos antigidos são marcados a -1
            {
                if (listaDosBarcosAtacar[w][1] == linha && listaDosBarcosAtacar[w][2] == coluna)
                {
                    listaDosBarcosAtacar[w][1] = -1;
                    listaDosBarcosAtacar[w][2] = -1;
                }
            }

            for (int num = 0; num < 5; num++)//verifica se todos os pontos de um determinado barco foram afundados e adiciona á variavel contBarcos
            {
                contBarcos = 0;
                for (int w = 0; w < 17; w++)
                {
                    if (listaDosBarcosAtacar[w][0] == tamanhoBarcos[num])
                    {
                        if (listaDosBarcosAtacar[w][1] == -1)
                        {
                            contBarcos++;
                        }
                    }
                }

                if (contBarcos == tamanhoBarcos[num])//Se todos os pontos de um barco forem afundados é é imprimida uma mensagem a informar que afundou um barco
                {
                    cout<<endl;
                    cout<<YELLOW<<"Afundou o barco de tamanho " << tamanhoBarcos[num]<<RESET<< endl;
                    cout<<endl;

                    for (int w = 0; w < 17; w++)                  //Se o barco for afundado o tamanho passa também a -1
                    {
                        if (listaDosBarcosAtacar[w][1] == -1)
                        {
                            listaDosBarcosAtacar[w][0] = -1;
                        }
                    }
                }
            }
        }
        
        if (Campo2[linha][coluna] == '~')//Se acertar em água é substituido por um espaço em branco
        {
            acertou = false;
            Campo2[linha][coluna] = ' ';
        }

        PrintMatrizesAtacar(CampoAtacante, Campo1);
        cout << endl;
        
        barcosDerrotados = 0;
        for (int w = 0; w < 17; w++)
        {
            if (listaDosBarcosAtacar[w][0] == -1)//Se o barco for derrotado incrementa a variavel barcosDerrotados
            {
                barcosDerrotados++;
            }
        }

        if (barcosDerrotados == 17)//Quando 17 pontos de barco forem afundados o jogador ganha
        {
            cout<<"Parabéns "<<jogador<<", ganhaste 🏆!!!" << endl;
            break;
        }

    } while (acertou == true);//So muda de jogador quando esse jogador falahr um ataque
}

int main() {

    CampoDaBatalha(CampoJogador1);  // Inicializa o campo para o jogador 1
    CampoDaBatalha(CampoJogador2);  // Inicializa o campo para o jogador 2
    CampoDaBatalha(CampoJogador1Atacar);  // Inicializa o campo do jogador 2 com os barcos para o jogador 1 mandar abaixo
    CampoDaBatalha(CampoJogador2Atacar);  // Inicializa o campo do jogador 1 com os barcos para o jogador 2 mandar abaixo
    GuardarBarcos(listaBarcosJogador1); //inicializa a lista onde se guarda os barcos do jogador 1 em que todas as posições tem valor -1
    GuardarBarcos(listaBarcosJogador2); //inicializa a lista onde se guarda os barcos do jogador 2 em que todas as posições tem valor -1
    do {
        menu();
        
        cout << "Escolha a opção:";
        if (!(cin >> opcao)) {
            cout << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opção inválida. Por favor, insira um número válido." << endl;
            cout << endl;
            continue;
        }

        cout << endl;

        switch (opcao) {
            case 1:
                linhaBarco = 0;
                PrintCampo(CampoJogador1,"Jogador 1");//Print no campo do jogador 1
                
                for (int i = 0; i < 5; i++)
                {
                    do
                    {
                        a = 1;                                      //A variável é iniciada a 1 até prova em contrário(se a entrada for inválida)

                        EscolherPosicao(i);                         

                        SimularIntroducaoBarcos(i,listaBarcosJogador1);//Simula a posição dos barcos sem modificar no campo do jogador 1

                        if(a==1){
                            IntroduzirBarcos(i,listaBarcosJogador1,CampoJogador1);//Se for possível introduzir o barco, adiciona-se na matriz e na lista do jogador 1
                        }

                        if(a==0){
                            cout<<"Posição inválida. Tente novamente"<<endl;//Se não for possível volta a perguntar
                        }

                        PrintCampo(CampoJogador1,"Jogador 1");//Volta a mostar o campo com o barco adicionado

                    } while (a != 1 );
                
                }
                
    
                for(int limpar=0;limpar<20;limpar++){//Da um espaço em branco de 20 linhas para "limpar" o terminal
                    cout<<" "<<endl;
                }
                


                linhaBarco=0;
                PrintCampo(CampoJogador2,"Jogador 2");//Print no campo do jogador 2
                
                for (int i = 0; i < 5; i++)
                {
                    do
                    {
                        a = 1;          //A variável é iniciada a 1 até prova em contrário(se a entrada for inválida)

                        EscolherPosicao(i);

                        SimularIntroducaoBarcos(i,listaBarcosJogador2);//Simula a posição dos barcos sem modificar no campo do jogador 2

                        if(a==1){
                            IntroduzirBarcos(i,listaBarcosJogador2,CampoJogador2);//Se for possível introduzir o barco, adiciona-se na matriz e na lista do jogador 2
                        }

                        if(a==0){
                            cout<<"Posição inválida. Tente novamente"<<endl;//Se não for possível volta a perguntar
                        }
                        PrintCampo(CampoJogador2,"Jogador 2");//Volta a mostar o campo com o barco adicionado
                        
                        
                    } while (a != 1 );
                
                }
                                                                                                                                                                                                                                                                                       
                //Limpar terminal
                for(int limpar=0;limpar<20;limpar++){
                    cout<<" "<<endl;
                }

                //Pausa de 5 segundos para os jogadores trocarem de posições
                for(int pausa=5;pausa>0;pausa--){
                    cout<<pausa<<endl;
                    sleep(1);
                }

                while (barcosDerrotados != 17)
                {
                    //Começa o jogo
                    cout << "Vez do Jogador 1 atacar: "<< endl;

                    PrintMatrizesAtacar(CampoJogador1Atacar, CampoJogador1);//Mostra o campo que o jogador 1 tem que atacar e o seu campo
                    Jogar(CampoJogador1, CampoJogador2, CampoJogador1Atacar, listaBarcosJogador2, "Jogador 1");//Jogador 1 ataca

                    // Limpar terminal
                    for (int limpar = 0; limpar < 20; limpar++)
                    {
                        cout << " " << endl;
                    }
                    //Pausa de 5 segundos para os jogadores trocarem de posições
                    for (int pausa = 5; pausa > 0; pausa--)
                    {
                        cout << pausa << endl;
                        sleep(1);
                    }

                    cout << "Vez do Jogador 2 atacar: "<< endl;

                    PrintMatrizesAtacar(CampoJogador2Atacar, CampoJogador2);//Mostra o campo que o jogador 2 tem que atacar e o seu campo
                    Jogar(CampoJogador2, CampoJogador1, CampoJogador2Atacar, listaBarcosJogador1,"Jogador 2");//Jogador 2 ataca

                    // Limpar terminal
                    for (int limpar = 0; limpar < 20; limpar++)
                    {
                        cout << " " << endl;
                    }
                    //Pausa de 5 segundos para os jogadores trocarem de posições
                    for (int pausa = 5; pausa > 0; pausa--)
                    {
                        cout << pausa << endl;
                        sleep(1);
                    }
                }

                break;
            
            case 2:
                cout << "A sair do jogo...  Adeus!" << endl;
                break;

            default:
                cout << "Opção inválida. Por favor, insira um número válido." << endl;
                cout << endl;
                break;
        }

    } while (opcao != 2 && opcao != 1);//Volta a perguntar novamente se a opção nao for 1 ou 2

    return 0;
}


