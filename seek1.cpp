#include <fstream>
#include <iostream>

typedef struct registry
{
    int  qntNo;
    int  parent;
    int  chave[499];
    int  ponteiros[499];
} registry;

typedef struct bregistry
{
    int  id;
    char titulo[300];
    int  ano;
    char autores[150];
    int  citacoes;
    char atualizacao[20];
    char snippet[1024];
} bregistry;

unsigned int read(unsigned int address){
    if(address == -1){
        std::cout << "REGISTRO NAO ENCONTRADO\n";
        return -1;
    }
    std::fstream ifd("students.data", std::fstream::in |std::fstream::out | std::fstream::binary);
    ifd.seekg(address, std::ios::beg);
    bregistry regist;
    ifd.read((char*)& regist, sizeof(bregistry));

    std::cout << "ID: " << regist.id << '\n' <<
    "TITULO: " << regist.titulo << '\n' << 
    "ANO: " << regist.ano << '\n' << 
    "AUTORES: " << regist.autores << '\n' << 
    "CITACOES: " << regist.citacoes << '\n' << 
    "ATUALIZACAO: " << regist.atualizacao << '\n' << 
    "SNIPPET: " << regist.snippet << std::endl;
}

int blocos = 0;

unsigned int recBTree(int id, int bl, int level){
    //std::cout << "\n\nBL " << bl << std::endl;
    registry reg;
    std::ifstream ifd("blocos.data", std::ifstream::ate | std::fstream::in |std::fstream::out | std::fstream::binary);

    int size = ifd.tellg();
    ifd.seekg((sizeof(registry)*(bl)), std::ios::beg);
    ifd.read((char*)& reg, sizeof(reg));
    //std::cout << "qntNo " << reg.qntNo << "\nehFolha " << reg.parent << " " << level <<  std::endl;
    int b;
    for(b = 0; b < reg.qntNo; b++){
        if(reg.parent == 0){
            if(id < reg.chave[b] ){
                return recBTree(id, reg.ponteiros[b], level+1);
            } 
        } else{
            if(id == reg.chave[b]){
                blocos = level;
                return reg.ponteiros[b];
            }
        }
        //std::cout << reg.chave[b] << " ";
    }

    if(reg.parent == 0){
        return recBTree(id, reg.ponteiros[b], level+1);
    } else{
        if(bl != (size/sizeof(reg))-1){
            return recBTree(id, bl+1, level+1);
        }
        blocos = level;
        return -1;
    }

}

int main(){
    registry reg;
    std::ifstream ifd("blocos.data", std::ifstream::ate | std::fstream::in |std::fstream::out | std::fstream::binary);

    int size = ifd.tellg();
    ifd.seekg(0, std::ios::beg);
    
    std::cout << "Insira o id (Busca Arvore b+): ";
    int id;
    std::cin >> id;

    std::cout << read(recBTree(id, 0, 1)) << "\nBlocos Lidos: " << blocos << " de " << size/sizeof(registry) << " blocos" << std::endl;
    //;
    //ifd.seekg(sizeof(registry), std::ios::beg);
}