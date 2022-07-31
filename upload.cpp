#include <fstream>
#include <iostream>

#include <iomanip>
#include <sstream>
#include <string>

#include <vector>

#include "csvTest.cpp"
//#include "btreeplus.cpp"

//Define e cria o arquivo students.data que vai armazenar os dados na forma de arquivo hash 
int mallocando(){
    bucketArea* area = new bucketArea;

    // define o tamanho da area dos buckets
    int count = 0;
    for(int a = 0; a < BUCKETSAMOUNT; a++){
        for(int b = 0; b < BUCKETSIZE - 1; b++){
            area->buckets[a].blocks[b].head = count;
            count += 1;
        }
        area->buckets[a].intBlocks.head = count;
        count += 1;
    }

    // define o tamanho do offset
    int ofssetArea = (BUCKETSAMOUNT/8)*(BUCKETSIZE);
    offsetBlock* offset = new offsetBlock[ofssetArea];
    for(int a=0; a < ofssetArea; a++){
        offset[a].head = count;
        count += 1;
    }
    
    std::cout << "Alocando espaço..." << std::endl;
    
    // cria o arquivo students.data (arquivo hash) com o tamano dos buckets e o offset
    std::ofstream output_file("students.data", std::ios::binary);
    output_file.write((char*) area, sizeof(bucketArea));
    //output_file.write((char*) offset, sizeof(offsetBlock)*ofssetArea);
    output_file.close();
}

// função principal do upload
int main(){
    // cria o arquivo hash vazio em disco
    mallocando();

    // lê o arquivo csv
    std::ifstream file("artigo.csv");
    btreeplus *tree = new btreeplus("blocos.data");
    btreeplus *tree1 = new btreeplus("blocos1.data");

    std::cout << "preenchendo hashing e arvore b+" << std::endl;

    // lê linha por linha
    if (file.is_open()) {
        std::string line;
        int count = 0;
        while(getline(file, line)) {
            // a função *redLine* é responsável por separar cada campo, criar o registro desse campo (usando o struct registry)
            // e armazena no arquivo hash e no índice primário
            if(readLine(line.c_str(), tree, 0)){
                count += 1;
            }

            readLine(line.c_str(), tree1, 1);

            // indicador visual 
            // a cada 50.000 registros ele avisa a quantidade registros salvos, apenas para indicar o progresso do programa
            if(count % 50000 == 0){
                std::cout << "foram salvos " << count << " registros" << std::endl;
            }

            //if(count == 200) break;

            //if(count >= 18-1) break;
        }

        // indica o numero total de registros salvos
        std::cout << count << " Registros Salvos\n";
        tree->printar();
        tree1->printar();

        // salva o indice primário em disco
        tree->salvar();
        tree1->salvar();

        // fecha o arquivo csv
        file.close();
    }
}