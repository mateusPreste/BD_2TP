#include <string.h>
#include <iostream>
#include <bitset>
#include "struct.h"

int main(){
    
    char text[300] = "Eï¬cient Symbolic Model Checking for Process Algebrasâ";
    unsigned int hash1 = fnv_hash_1a_32(text, 300);
    std::cout << text << " " << hash1 << std::endl;

    char inp[1024];
    std::cin.getline(inp, 300);

    std::cout << inp << std::endl; 

    unsigned int id = fnv_hash_1a_32(inp, 300);


    std::cout << " hash 1: " << hash1 << " " << "hash2: " << id;
}