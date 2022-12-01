# Trabalho 2 - Spatial Hashing

## Compilação

Em ambiente MSYS2:

    $ make

Não configurado em outros ambientes.

## Design e funcionamento

Pixels é a unidade de medida.

## Controles

Instruções dentro da aplicação.

## Contratos

### Contrato: distribuição de segmentos

- [X] Definir área de distribuição
- [X] Distribuir segmentos com tamanho e ângulo aleatório
- [ ] Parametrizar quantidade de segmentos em tempo de execução
- [ ] Trabalhar com segmentos maiores que 50 unidades

### Contrato: intersecção

- [X] Implementar intersecção de segmento com segmento
- [ ] Implementar detecção de segmentos colineares
- [X] Buscar intersecção de todos com todos segmentos
- [ ] Buscar intersecção dos segmentos usando spatial hash
- [ ] Execução sem congelamento da aplicação

### Contrato: spatial hash

- [ ] Hash de linha resultando em múltiplas células
- [ ] Anotar de uso e presença em células
- [ ] Distribuir da tabela crua como matriz e índices finais
- [ ] Armazenar dos objetos na tabela
- [ ] Parametrizar tamanho das células
- [ ] Estimativa exata do tamanho da tabela

### Contrato: visualização

- [X] Mostrar segmentos
- [X] Mostrar células do spatial hash
- [X] Mostrar pontos de intersecção
- [X] Mostrar resultados parcialmente
- [ ] Variar visualização parcial conforme parâmetros
- [ ] Zoom
- [ ] Variar resultados visualizados conforme zoom
- [X] Aceitar redimensionamento da janela durante execução

### Contrato: desempenho

- [ ] Medir tempo de execução
- [ ] Desconsiderar tempo de renderização
- [ ] Desconsiderar tempo de geração de segmentos
- [ ] Somar opcionalmente tempo do hash espacial

### Contrato: controles e interface

- [ ] Pausa da execução
- [ ] Interrupção da execução
- [ ] Parametrização
- [ ] Escolha de abordagem
- [ ] Responsividade

### Contrato: estatísticas 

- [ ] Histórico de execuções
- [ ] Tabela de execuções

## Restrições

- Não ultrapassar 1800x1000 pixels o tamanho da tela
- Quantidade de segmentos na escala de centenas de milhares e milhões
- Manter 200 FPS, se possível
