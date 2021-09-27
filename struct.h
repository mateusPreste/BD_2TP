#ifndef STRUCT_H   /* Include guard */
#define STRUCT_H

#include <cstring>


#define BUCKETSIZE 18
#define BUCKETSAMOUNT 45000

typedef struct registry
{
    int  id;
    char titulo[300];
    int  ano;
    char autores[150];
    int  citacoes;
    char atualizacao[20];
    char snippet[1024];
} registry;

typedef struct overRegistry
{
    int  id;
    char titulo[300];
    int  ano;
    char autores[150];
    int  citacoes;
    char atualizacao[20];
    char snippet[1024];
    int recordPointer;
} overRegistry;

typedef struct block
{
    int head;
    int address1;
    int address2;

    char free[972]; // Espcaço livre necessário para que cada bloco tenha 4kb

    registry reg1; // offset 984
    registry reg2; // offset 2492
} block;

typedef struct intBlock
{
    int head;
    int address1;
    int address2;

    char free[968]; // Espcaço livre necessário para que cada bloco tenha 4kb

    registry reg1;
    registry reg2;

    int recordPointer;
} intBlock;


typedef struct offsetBlock
{
    int head;
    int address1;
    int address2;

    char free[964]; // Espcaço livre necessário para que cada bloco tenha 4kb

    overRegistry reg1;
    overRegistry reg2;
} offsetBlock;

typedef struct bucket
{
    block blocks[BUCKETSIZE-1];
    intBlock intBlocks;
} bucket;

typedef struct bucketArea
{
    bucket buckets[BUCKETSAMOUNT];
} bucketArea;

registry writeRegistry(int id, const char titulo[300], int ano, const char autores[150], int citacoes, const char atualizacao[16], const char snippet[1024]){
    registry r = {};
        r.id = id;
        strncpy(r.titulo, titulo, 300);
        r.ano = ano;
        strncpy(r.autores, autores, 150);
        r.citacoes = citacoes;
        strncpy(r.atualizacao, atualizacao, 16);
        strncpy(r.snippet, snippet, 1024);
    return r;
}

overRegistry writeOverRegistry(int id, const char titulo[300], int ano, const char autores[150], 
                                int citacoes, const char atualizacao[16], const char snippet[1024], int rPointer){
    overRegistry r = {};
        r.id = id;
        strncpy(r.titulo, titulo, 300);
        r.ano = ano;
        strncpy(r.autores, autores, 150);
        r.citacoes = citacoes;
        strncpy(r.atualizacao, atualizacao, 16);
        strncpy(r.snippet, snippet, 1024);
        r.recordPointer = rPointer;
    return r;
}

block writeBlock(int head, int address1, int address2, registry reg1, registry reg2){
    block bl = {};
        bl.head = head;
        bl.address1 = address1;
        bl.address2 = address2;
        bl.reg1 = reg1;
        bl.reg2 = reg2;
    return bl;
}

intBlock writeIntBlock(int head, int address1, int address2, registry reg1, registry reg2, int rPointer){
    intBlock bl = {};
        bl.head = head;
        bl.address1 = address1;
        bl.address2 = address2;
        bl.reg1 = reg1;
        bl.reg2 = reg2;
        bl.recordPointer = rPointer;
    return bl;
}

offsetBlock writeOffsetBlock(int head, int address1, int address2, overRegistry reg1, overRegistry reg2, int rPointer){
    offsetBlock bl = {};
        bl.head = head;
        bl.address1 = address1;
        bl.address2 = address2;
        bl.reg1 = reg1;
        bl.reg2 = reg2;
    return bl;
}

block insertBlock(block block, registry reg){
    if(block.address1 == 0){
        return writeBlock(block.head, 1, block.address2, reg, block.reg2);
    } else{
        return writeBlock(block.head, block.address1, 1, block.reg1, reg);
    }
}

intBlock insertIntBlock(intBlock block, registry reg, int rPoint){
    if(block.address1 == 0){
        return writeIntBlock(block.head, 1, block.address2, reg, block.reg2, rPoint);
    } else{
        return writeIntBlock(block.head, block.address1, 1, block.reg1, reg, rPoint);
    }
}

offsetBlock insertOffsetBlock(offsetBlock block, overRegistry reg, int rPoint){
    if(block.address1 == 0){
        return writeOffsetBlock(block.head, 1, block.address2, reg, block.reg2, rPoint);
    } else{
        return writeOffsetBlock(block.head, block.address1, 1, block.reg1, reg, rPoint);
    }
}

#endif // FOO_H_