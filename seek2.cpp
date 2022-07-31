#include <fstream>
#include <iostream>
#include <string.h>
#include <bitset>

#include "struct.h"

typedef struct lregistry
{
    int  qntNo;
    int  parent;
    unsigned int chave[499];
    unsigned int ponteiros[499];
} lregistry;

typedef struct bregistry
{
    unsigned int  id;
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

unsigned int recBTree(unsigned int id, int bl, int level){
    //if(level > 10) return -1;
    //std::cout << "\n\nBL " << bl << std::endl;
 
    lregistry reg;
    std::ifstream ifd("blocos1.data", std::ifstream::ate | std::fstream::in |std::fstream::out | std::fstream::binary);

    int size = ifd.tellg();
    ifd.seekg((sizeof(lregistry)*(bl)), std::ios::beg);
    ifd.read((char*)& reg, sizeof(reg));
    //std::cout << "qntNo " << reg.qntNo << "\nehFolha " << reg.parent << " " << level <<  std::endl;
    int b;
    for(b = 0; b < reg.qntNo; b++){
        //std::cout << (unsigned int) reg.chave[b] << " |";

        if(reg.parent == 0){
            if(id < (unsigned int) reg.chave[b] ){
                return recBTree(id, reg.ponteiros[b], level+1);
            } 
        } else{
            if(id == (unsigned int) reg.chave[b]){
                blocos = level;
                return reg.ponteiros[b];
            }
        }
        
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
    lregistry reg;
    std::ifstream ifd("blocos1.data", std::ifstream::ate | std::fstream::in |std::fstream::out | std::fstream::binary);

    int size = ifd.tellg();
    ifd.seekg(0, std::ios::beg);
    
    std::cout << "Insira o titulo (Busca Arvore b+): ";
    char inp[1024];
    std::cin.getline(inp, 300);

    //std::cout << inp << std::endl; 

    unsigned int id = fnv_hash_1a_32(inp, 300);

    unsigned int address = recBTree(id, 0, 1);

    //std::cout << "id " << id << std::endl;
    //std::cout << "address " << address << " " << (signed int) address << std::endl;

    read(address);
    std::cout  << "\nBlocos Lidos: " << blocos << " de " << size/sizeof(lregistry) << " blocos" << std::endl;
    //;
    //ifd.seekg(sizeof(registry), std::ios::beg);
}