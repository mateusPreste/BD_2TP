#include "csvtest.h"

#include <fstream>
#include <iostream>

#include <iomanip>
#include <sstream>
#include <string>

#include <vector>

#include "struct.h"
#include "btreeplus.cpp"

// função responsável por inserir o registro no arquivo hash
unsigned int enterBucket(unsigned int id, registry reg){
    // abre o arquivo hash (students.data)
    std::fstream ifd("students.data", std::fstream::in |std::fstream::out | std::fstream::binary);
    // DEBUG std::ofstream output_file("students.data", std::ios::binary | std::ios_base::beg);

    // FUNÇÂO HASH
    // indica o bucket correspondente do registro atual 
    int bucketN = ((id-1) % BUCKETSAMOUNT);
    
    int ofssetArea = BUCKETSAMOUNT*(BUCKETSIZE/4);

    // representações temporárias dos elementos fisicos da nossa estrutura
    // usamos por exemplo para carregar um bloco dentro do struct block 
    block  bloquinho;
    bucket buck;
    intBlock intB;
    offsetBlock oblock;
    auto offsetAddress = sizeof(buck)*(BUCKETSAMOUNT);

        
    int size = ifd.tellg();
    ifd.seekg(0, std::ios::beg);
    ifd.seekg(sizeof(buck)*bucketN, std::ios::beg);

    // procuramos um bloco vazio (disponivel) dentro do bucket atual 
    int count = 0;
    for(int a = 0; a < BUCKETSIZE; a++){
        if(a != BUCKETSIZE-1){
            ifd.read((char*)& bloquinho, sizeof(bloquinho));
            //std::cout << "id" << id <<" bucket " << ((id-1) % BUCKETSAMOUNT) << " block " << a << std::endl;
            //std::cout << bloquinho.head << " " << count++ << " " << sizeof(bloquinho) << std::endl;
            //std::cout << bloquinho.head << "\n" << bloquinho.address1 << "\n" << bloquinho.address2 << "\n\n";
            
            //em caso de um bloco disponível
            if(bloquinho.address1 == 0 || bloquinho.address2 == 0){
                block newBlock = insertBlock(bloquinho, reg);

                ifd.seekp((sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a)));
                ifd.seekg((sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a)));

                // grava efetivamente o bloco no disco
                ifd.write((char*)&newBlock, sizeof(block));
                ifd.close();

                // retorne o endereço onde o registro foi gravado
                // é usado para gerar o índice 
                if(bloquinho.address1 == 0 && newBlock.address1 == 1){
                    // retorna o endereço do registro que acabamos de gravar dentro do bloco
                    // o valor 984 é correspondente ao offset do campo *reg1* de (struct block)
                    return (sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a))+984;
                } else if(bloquinho.address2 == 0 && newBlock.address2 == 1){
                    // retorna o endereço do registro que acabamos de gravar dentro do bloco
                    // o valor 2492 é correspondente ao offset do campo *reg1* de (struct block)
                    return (sizeof(buck)*bucketN)+(sizeof(bloquinho)*(a))+2492;
                }
                break;
            }

            ifd.seekg(sizeof(buck)*bucketN+(sizeof(bloquinho)*(a+1)), std::ios::beg);
        } else{
            // caso contrário retorne o endereço da área de offset

            // se não tiver recordpointer
            // se tiver recordpointer
            ifd.read((char*)& intB, sizeof(intB));
            //std::cout << intB.head << " " << count++ << " " << sizeof(intB) << std::endl;

            //std::cout << intB.head << "\n" << intB.address1 << "\n" << intB.address2 << "\n" << intB.recordPointer << "\n\n"; 
            ifd.seekg(sizeof(buck)*bucketN+(sizeof(intB)*(a+1)), std::ios::beg);
        }
    }

    /** DEBUG
     * ifd.seekg(offsetAddress, std::ios::beg);

    overRegistry ov;

    for(int b = 0; b < ofssetArea; b++){
        ifd.read((char*)& oblock, sizeof(oblock));
        std::cout << oblock.head << " " << count++ << " " << sizeof(intB) << std::endl;

        std::cout << oblock.head << "\n" << oblock.address1 << "\n" << oblock.address2 << "\n\n"; 
        if(oblock.address1 == 0 | oblock.address2 == 0){
            break;
        }
            
        ifd.seekg(offsetAddress+(sizeof(oblock)*(b+1)), std::ios::beg);        
    }
    **/
}

// Le a linha do csv e cria um registro *reg* onde os dados serão armazenados no arquivo hash e no indice primário
int readLine(std::string texto, btreeplus *tree, int type){
    // Lê a linha atual do csv
    std::stringstream ss;
    ss << texto;
    std::vector<std::string> v;

    //conta a quantidade de campos da linha (um campo é um elemento separado por ";")
    /* Adicionamos cada campo ao vetor temporário v, como estamos lendo do arquivo csv, a ordem dos campos no vetor
    *  é a mesma do arquivo csv 
    * v[0] - id
    * v[1] - título
    * v[2] - ano
    * v[3] - autores
    * v[4] - citação
    * v[5] - atualização
    * v[6] - snippet
    * v[7] - campo ignorado
    */
    int fields = 0;

    while (ss >> std::ws) {
        std::string csvElement;

        if (ss.peek() == '"') {
            ss >> std::quoted(csvElement);
            std::string discard;
            std::getline(ss, discard, ';');
        }
        else {
            std::getline(ss, csvElement, ';');
        }
        fields += 1;

        v.push_back(csvElement);
    }

    // se a quantidade de campos for diferente de 8, então a linha não representa um registro válido
    if(fields != 8){
        //std::cout << fields << std::endl << std::endl;

        for(std::string value: v) {
            //std::cout << value << std::endl;
        }

        //std::cout << std::endl;
        return 0;
    } else{
        /*Atribuimos os campos as suas variaveis correspondentes
          os nomes das variaves já descrevem o seu conteudo
        */
        char titulo[300];
        strncpy(titulo, v[1].c_str(), 300);
        char autores[150];
        strncpy(autores, v[3].c_str(), 150);
        char atualizacao[16];
        strncpy(atualizacao, v[5].c_str(), 16);
        char snippet[1024];
        strncpy(snippet, v[6].c_str(), 1024);

        unsigned int key;

        if (type == 0){
            key = stoi(v[0]);
        } else{
            key = fnv_hash_1a_32(titulo, 300);
            //std::cout << key << " |" << titulo << "\n";
        }

        // criamos o registro do dado atual usando a função writeRegistry, ela vai usar o padrao do (struct registry)
        // para armazenar o dado
        registry reg = writeRegistry(key, titulo, stoi(v[2]), autores, stoi(v[4]), atualizacao, snippet);
        
        // inserimos o registro atual no arquivo hash usando a função enterBucket
        // essa função vai retornar o endereço correspondente do registro no ARQUIVO HASH
        unsigned int address = enterBucket(key, reg);
        
        // DEBUG 
        //std::cout << address << std::endl;

        // criamos o par (id_registro, endereço_registro_arquivo_hash) e inserimos na estrutura do nosso indice primário
        tree->inserir(key, address);


        // DEBUG std::cout << "-|" << v[0] << std::endl;
        // limpamos o vetor v para receber os dados da próxima linha do csv
        v.clear();
    }
    return 1;
}
