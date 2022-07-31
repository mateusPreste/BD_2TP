#include <fstream>
#include <iostream>
#include "struct.h"

int read(unsigned int address){
    if(address == -1){
        std::cout << "REGISTRO "+std::to_string(address)+" NAO ENCONTRADO\n";
        return -1;
    }

    std::cout << "addr: " << address << std::endl;

    std::fstream ifd("students.data", std::fstream::in |std::fstream::out | std::fstream::binary);
    ifd.seekg((unsigned int) address, std::ios::beg);
    registry regist;
    ifd.read((char*)& regist, sizeof(registry));

    std::cout << "ID: " << regist.id << '\n' <<
    "TITULO: " << regist.titulo << '\n' << 
    "ANO: " << regist.ano << '\n' << 
    "AUTORES: " << regist.autores << '\n' << 
    "CITACOES: " << regist.citacoes << '\n' << 
    "ATUALIZACAO: " << regist.atualizacao << '\n' << 
    "SNIPPET: " << regist.snippet << std::endl;
}

int blocos = 0;

//5228EF9A

unsigned int recHash(unsigned int id){
    std::fstream ifd("students.data", std::fstream::in |std::fstream::out | std::fstream::binary);
    //std::ofstream output_file("students.data", std::ios::binary | std::ios_base::beg);

    int bucketN = (id-1) % BUCKETSAMOUNT;
    int ofssetArea = BUCKETSAMOUNT*(BUCKETSIZE/4);
    block  bloquinho;
    bucket buck;
    intBlock intB;
    offsetBlock oblock;
    auto offsetAddress = sizeof(buck)*(BUCKETSAMOUNT);

        
    int size = ifd.tellg();
    ifd.seekg(0, std::ios::beg);
    ifd.seekg(sizeof(buck)*bucketN, std::ios::beg);

    std::cout << "bucket: " << bucketN << std::endl;

    for(int a = 0; a < BUCKETSIZE; a++){
        ++blocos;
        if(a != BUCKETSIZE){
            ifd.read((char*)& bloquinho, sizeof(bloquinho));

            std::cout << "id1: " << bloquinho.reg1.id << std::endl; 
            std::cout << "id2: " << bloquinho.reg2.id << std::endl; 

            if(bloquinho.reg1.id == id){
                return (sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a))+984;
            } else if(bloquinho.reg2.id == id){
                return (sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a))+2500;
            }
           
            ifd.seekg(sizeof(buck)*bucketN+(sizeof(bloquinho)*(a+1)), std::ios::beg);
        } 
    }
}

int main(){
    //registry reg;
    //std::ifstream ifd("blocos.data", std::ifstream::ate | std::fstream::in |std::fstream::out | std::fstream::binary);

    //int size = ifd.tellg();
    //ifd.seekg(0, std::ios::beg);
    
    std::cout << "Insira o id (Busca Hash): ";
    int id;
    std::cin >> id;

    std::cout << read(recHash(id)) << "\nBlocos Lidos: " << blocos << " de " << BUCKETSAMOUNT*(BUCKETSIZE) << " blocos" << std::endl;
    //;
    //ifd.seekg(sizeof(registry), std::ios::beg);
}