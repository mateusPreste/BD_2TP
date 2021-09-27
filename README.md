INSTRUÇÕES PARA GERAR OS ARQUIVOS DE DADOS E O INDICE PRIMÁRIO

1. extraia o arquivo artigo.csv.gz com o nome artigo.csv e extraia para a raiz do projeto
2. compile o arquivo.cpp com o g++ com o comando "g++ upload.cpp -o upload"
3. execute o executável upload com o comando "./upload"
4. acompanhe as mensagens de progresso na tela até o fim da execução do programa
5. ao fim vc terá os arquivos bloco.data (representação fisica da b+tree) e students.data (representação fisica da estrutura hash)

DESCRIÇÃO DAS ESTRUTURAS
todos os structs estão definidos em struct.h

Arquivo hashing
É organizado por buckets (struct.h struct bucket), cada bucket é formado por 18 blocos
cada bloco é um bloco fisico de 4kb (struct.h struct block) e cada bloco possui 2 registros (struct.h struct registry)
que efetivamente guardam os dados.
basicamente para encontrar o bloco certo, basta encontrar o bucket correspondente usando a função hash (csvTest.cpp:23:5) definida no codigo e fazer uma busca sequencial dentro do bucket até encontrar o registro desejado.

Indice Primário (Árvore B+)
Possui grau 495
É composto por blocos de 4kb, com a estrutura definida em btreepluss.cpp (struct registry)
Todas as referencias aos outros blocos em (struct registry->ponteiros) são referencias virtuais, não são endereços físicos mas apenas um indice
do bloco seguinte na lista
A variável qntNo indica a quantidade de nós validos presente naquele bloco, use como condição final do seu laço de repetição para iterar os vetores chave e ponteiros.


