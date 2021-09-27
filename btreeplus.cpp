#include <bits/stdc++.h>
#include <algorithm>
#include <iostream> 
using namespace std;

#define MAX 499

class btreeplus{
    int grau = 495;

public:
    typedef struct registry
    {
        // quantidade de nós válidos nesse bloco
        // essa variável pode ser usada para 
        int  qntNo;
        // indica se é nó folha, 0 - não é nó folha, 1 - é nó folha
        int  parent;
        // vetor de chave, contem os valores de chave armazenados nesse bloco
        // a quantidade de valores de chave válidos é indicado pela variável qntNo
        int  chave[499];
        // ponteiros para outros blocos
        // cada bloco está sequencialmente amazenado um após o outro
        // e todos os blocos tem o mesmo tamanho
        // o valor desse ponteiro é a sua ordem na lista(numero inteiro de inicio 0 (nó raiz) até x (ultimo nó folha))
        int  ponteiros[499];
    } registry;

    struct Bloco{
        // quantidade de nós
        int qntNo;
        // ponteiro para bloco pai
        Bloco *blocoPai;
        // valores
        int valores[MAX]{};
        //child Blocks
        Bloco *blocosFilhos[MAX]{};
        int ponteiros[MAX]{};
        Bloco() { // construtor do bloco
            qntNo = 0;
            blocoPai = nullptr;
            for(int i=0; i<MAX; i++){
                valores[i] = INT_MAX;
                blocosFilhos[i] = nullptr;
            }
        }
    };

    // Raiz
    Bloco *blocoRaiz = new Bloco();

    void divideFolha(Bloco *bloco){
        int x;

        x = grau / 2;

        auto *blocoDireito = new Bloco();

        bloco->qntNo = x;
        blocoDireito->qntNo = grau - x;
        blocoDireito->blocoPai = bloco->blocoPai;

        for(int i=x, j=0; i < grau; i++, j++){
            blocoDireito->valores[j] = bloco->valores[i];
            blocoDireito->ponteiros[j] = bloco->ponteiros[i];
            bloco->valores[i] = INT_MAX;
            bloco->ponteiros[i] = 0;
        }
        int val = blocoDireito->valores[0];
        int ptn = blocoDireito->ponteiros[0];

        if(bloco->blocoPai == nullptr){
            auto *blocoPai = new Bloco();
            blocoPai->blocoPai = nullptr;
            blocoPai->qntNo = 1;
            blocoPai->valores[0] = val;
            blocoPai->ponteiros[0] = ptn;
            blocoPai->blocosFilhos[0] = bloco;
            blocoPai->blocosFilhos[1] = blocoDireito;
            bloco->blocoPai = blocoDireito->blocoPai = blocoPai;
            blocoRaiz = blocoPai;
            return;
        }
        else{
            bloco = bloco->blocoPai;

            auto *novoBlocoFilho = blocoDireito;

            for(int i=0; i <= bloco->qntNo; i++){
                if(val < bloco->valores[i]){
                    swap(bloco->valores[i], val);
                    swap(bloco->ponteiros[i], ptn);
                }
            }

            bloco->qntNo++;

            for(int i=0; i < bloco->qntNo; i++){
                if(novoBlocoFilho->valores[0] < bloco->blocosFilhos[i]->valores[0]){
                    swap(bloco->blocosFilhos[i], novoBlocoFilho);
                }
            }
            bloco->blocosFilhos[bloco->qntNo] = novoBlocoFilho;

            for(int i=0; bloco->blocosFilhos[i] != nullptr; i++){
                bloco->blocosFilhos[i]->blocoPai = bloco;
            }
        }

    }

    void divideNaoFolha(Bloco *bloco){
        int x, i, j;

        x = grau / 2;

        auto *blocoDireito = new Bloco();

        bloco->qntNo = x;
        blocoDireito->qntNo = grau - x - 1;
        blocoDireito->blocoPai = bloco->blocoPai;


        for(i=x, j=0; i <= grau; i++, j++){
            blocoDireito->valores[j] = bloco->valores[i];
            blocoDireito->blocosFilhos[j] = bloco->blocosFilhos[i];
            bloco->valores[i] = INT_MAX;
            if(i!=x) bloco->blocosFilhos[i] = nullptr;
        }

        int valor = blocoDireito->valores[0];
        memcpy(&blocoDireito->valores, &blocoDireito->valores[1], sizeof(int) * (blocoDireito->qntNo + 1));
        memcpy(&blocoDireito->blocosFilhos, &blocoDireito->blocosFilhos[1], sizeof(blocoRaiz) * (blocoDireito->qntNo + 1));

        for(i=0; bloco->blocosFilhos[i] != nullptr; i++){
            bloco->blocosFilhos[i]->blocoPai = bloco;
        }
        for(i=0; blocoDireito->blocosFilhos[i] != nullptr; i++){
            blocoDireito->blocosFilhos[i]->blocoPai = blocoDireito;
        }

        if(bloco->blocoPai == nullptr){
            auto *blocoPai = new Bloco();
            blocoPai->blocoPai = nullptr;
            blocoPai->qntNo=1;
            blocoPai->valores[0] = valor;
            blocoPai->blocosFilhos[0] = bloco;
            blocoPai->blocosFilhos[1] = blocoDireito;

            bloco->blocoPai = blocoDireito->blocoPai = blocoPai;

            blocoRaiz = blocoPai;
            return;
        }
        else{
            bloco = bloco->blocoPai;

            auto *newChildBlock = blocoDireito;

            for(i=0; i <= bloco->qntNo; i++){
                if(valor < bloco->valores[i]){
                    swap(bloco->valores[i], valor);
                }
            }

            bloco->qntNo++;

            for(i=0; i < bloco->qntNo; i++){
                if(newChildBlock->valores[0] < bloco->blocosFilhos[i]->valores[0]){
                    swap(bloco->blocosFilhos[i], newChildBlock);
                }
            }
            bloco->blocosFilhos[i] = newChildBlock;

            for(i=0; bloco->blocosFilhos[i] != nullptr; i++){
                bloco->blocosFilhos[i]->blocoPai = bloco;
            }
        }

    }

    void inserir(int valor, int pt){
        inserirNo(blocoRaiz, valor, pt);
    }

    void inserirNo(Bloco *bloco, int valor, int pt){
        for(int i=0; i <= bloco->qntNo; i++){
            if(valor < bloco->valores[i] && bloco->blocosFilhos[i] != nullptr){
                inserirNo(bloco->blocosFilhos[i], valor, pt);
                if(bloco->qntNo == grau)
                    divideNaoFolha(bloco);
                return;
            }
            else if(valor < bloco->valores[i] && bloco->blocosFilhos[i] == nullptr){
                swap(bloco->valores[i], valor);
                swap(bloco->ponteiros[i], pt);
                if(i == bloco->qntNo){
                    bloco->qntNo++;
                    break;
                }
            }
        }

        if(bloco->qntNo == grau){
            divideFolha(bloco);
        }
    }

    vector<registry> regs;

    std::unordered_map<Bloco*, int> m;
    int counter = 0;

    void salvar(){
        save(vector<Bloco*>{blocoRaiz});
    }

    void save(vector < Bloco* > blocos){
        vector < Bloco* > novosBlocos;
        for(auto curBlock : blocos){
            int j;
            for(j=0; j<curBlock->qntNo; j++){
                if(curBlock->blocosFilhos[j] != nullptr)
                    novosBlocos.push_back(curBlock->blocosFilhos[j]);
            }
            if(curBlock->valores[j] == INT_MAX && curBlock->blocosFilhos[j] != nullptr) {
                novosBlocos.push_back(curBlock->blocosFilhos[j]);
            }

            registry reg = {};
            bool isLeaf = true;

            if(curBlock->blocosFilhos[0] == 0){
                std::copy(curBlock->ponteiros, curBlock->ponteiros + MAX, reg.ponteiros);
            } else{
                isLeaf = false;
                for(int a=0; a<MAX; a++){
                    if(curBlock->blocosFilhos[a] != 0){
                        reg.ponteiros[a] = m[curBlock->blocosFilhos[a]];
                    } else{
                        reg.ponteiros[a] = 0;
                    }
                }
            }

            reg.qntNo = curBlock->qntNo;
            reg.parent = (isLeaf)?1:0;
            std::copy(curBlock->valores, curBlock->valores + MAX, reg.chave);

            regs.push_back(reg);

        }


        if(novosBlocos.empty()){
            std::ofstream output_file("blocos.data", std::ios::binary);
            for(registry re : regs){
                output_file.write((char*)&re, sizeof(registry));
            }
            output_file.close();

            blocos.clear();
        }
        else {
            blocos.clear();
            save(novosBlocos);
        }
    }

    void printar(){
        print(vector<Bloco*>{blocoRaiz});
    }

    void print(vector < Bloco* > blocos){
        vector < Bloco* > novosBlocos;
        for(auto curBlock : blocos){
            //cout <<"[|";
            int j;
            for(j=0; j<curBlock->qntNo; j++){
                //cout << curBlock->valores[j] << "|";
                if(curBlock->blocosFilhos[j] != nullptr)
                    novosBlocos.push_back(curBlock->blocosFilhos[j]);
            }
            if(curBlock->valores[j] == INT_MAX && curBlock->blocosFilhos[j] != nullptr)
                novosBlocos.push_back(curBlock->blocosFilhos[j]);

            m[curBlock] = counter++;
            //cout << "]  ";// << curBlock->blocoPai << "|" << curBlock;
        }


        if(novosBlocos.empty()){

            //puts("");
            //puts("");
            blocos.clear();
        }
        else {
            //puts("");
            //puts("");
            blocos.clear();
            print(novosBlocos);
        }
    }

};

