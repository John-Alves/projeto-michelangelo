# Michelangelo Drawing Machine
  
O projeto Michelangelo é uma CNC de dois eixos (2D), completamente open source, feita usando o microcontrolador da ST Microeletronics. 

Ficou confuso? Calma, a intenção desse repositório é descomplicar sua vida. Trocando em miudos, o _Projeto Michelangelo_ trata-se de uma máquina desenhista que "receberá uma imagem" e irá desenha-la em uma superficie.

  ![](https://github.com/John-Alves/projeto-michelangelo/blob/master/Imagens/michelangelo-logo.png?raw=true)
  
## Motivação

O _Michelangelo Drawing Machine_ foi desenvolvido como projeto final da disciplina de Sistemas Microprocessados, ministrada pelo professor Jardel na Universidade Federal do Ceará.

Tá, mas e ai?! Atualmente, existem muitos projetos de impressoras 3D e máquinas CNC circulando pela internet. O que há de novo? Fazendo uma análise um pouco mais minunciosa desses projetos, vemos que grande parte deles, utilizam o arduino como placa de prototipagem (o que faz consumir bastante energia) e o código que é executado no Microcontrolador é cheio de bibliotecas e funções que nunca sequer são utilizadas, fazendo com que muita memoria seja usada.

_Michelangelo_ vem com intuito de sanar esses problemas evitando o uso desnecessário de memória e otimizando o código ao máximo (além de ser mais vantajoso energicamente, uma vez que trocou-se o arduino pelo Cortex M0). 

Outra motivação para o projeto foi: "Deve ser bem legal construir uma máquina que desenha sozinha". :D

## Materiais e Ferramentas Utilizados

Os materiais e ferramentas utilizados nesse projeto estão listados abaixo:
* 1x STM32F030F4 (Cortex M0): Também chamado de "Green Pill", esse é microcontrolador usado no projeto.
* 2x Motor de Passo + eixo: Foi retirado de um DVD antigo. Usou-se o eixo que o proprio aparelho usa para movimentar a lente.  
* 2x Driver p/ Motor de Passo A4988: Usando para estabelecer a comunicação entre o Microcontrolados e os motores de passo.
* Jumpers.
* 2x Protoboard 760 pontos.
* STM32CubeMX: Usando para configurar os pinos adequadamente e devido a implementação da Hardware Abstract Language (HAL).
* Attolic TrueSTUDIO: IDE de desemvolvimento.
