# Descrição
Implementação da árvore PATRICIA para roteamento de endereços IP.

# Objetivo
Demonstrar como a estrutura de dados em questão pode ser utilizada para a busca na tabela de repasse, prestando atenção ao uso da memória.

# Inspiração
O método para determinar o skip foi inspirado na proposta feita para o sistema operacional de código aberto [NetBSD](https://wiki.netbsd.org/projects/project/atomic_radix_patricia_trees/), que utiliza operações bit-a-bit AND para calcular o número de zeros antes do primeiro 1 no resultado da comparação entre prefixos.