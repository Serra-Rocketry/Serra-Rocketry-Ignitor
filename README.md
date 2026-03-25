# Serra Rocketry Ignitor

![Status](https://img.shields.io/badge/status-em%20desenvolvimento-yellow)
![Versão](https://img.shields.io/badge/versão-1.0.0-blue)

## Sobre
Sistema de ignição remota para foguetes experimentais da Serra Rocketry.

A solução é formada por duas estações independentes que se comunicam via LoRa 433 MHz:

1. Estação de Comando (operada pelo operador em distância segura)
2. Estação de Ignição (conectada fisicamente ao ignitor)

O objetivo é permitir ignição com redundância de segurança, confirmação de comunicação e feedback visual/sonoro durante toda a sequência.

## Arquitetura

### Estação de Comando
- Inicia a sequência de ignição por botão dedicado
- Exige ação mantida por 5 segundos
- Envia comando via LoRa e aguarda ACK
- Exibe status por LEDs e buzzer

### Estação de Ignição
- Recebe comando da estação de comando
- Executa contagem regressiva de 5 segundos
- Aborta se o comando for interrompido
- Aciona o ignitor ao fim da contagem

Para detalhes completos de pinagem, BOM, consumo, segurança e esquemáticos, consulte a documentação de hardware.

## Sequência de Ignição (Resumo)
1. Operador pressiona e mantém o botão de ignição por 5 s
2. Estação de comando envia `ARM` via LoRa
3. Estação de ignição confirma recebimento e inicia contagem
4. Se o botão for solto antes do fim, procedimento é abortado
5. Ao final de 5 s, a estação de ignição aciona o ignitor

## Componentes Principais
- 2x Raspberry Pi Pico
- 2x Módulos LoRa SX1278 (433 MHz)
- 2x LEDs amarelos e 2x LEDs vermelhos
- 2x Buzzers ativos
- 2x Botões liga/desliga
- 1x Botão de ignição momentâneo
- 1x Relé para acionamento do ignitor
- 2x Módulos TP4056 com proteção
- Baterias Li-ion/LiPo (capacidade em definição)
- Cases impressos em 3D

## Estrutura do Repositório
- `docs/`: documentação técnica e operacional
- `firmware/`: código embarcado e interface web local
- `hardware/`: esquemáticos, modelos, imagens e arquivos de fabricação
- `software/`: ferramentas de apoio de software
- `test/`: materiais de teste e validação

## Documentação
- [Hardware: arquitetura, BOM e pinagem](./hardware/README.md)
- [Instalação](./docs/INSTALACAO.md)
- [API e protocolos](./docs/API.md)
- [Testes](./docs/TESTES.md)
- [Troubleshooting](./docs/TROUBLESHOOTING.md)

## Segurança
- O botão de ignição deve ser momentâneo (sem trava)
- O comando de ignição deve permanecer ativo por toda a janela de 5 s
- Em perda de comunicação/heartbeat, o sistema deve abortar
- Testes devem ser feitos com carga dummy antes de ignição real
- Manter distância operacional segura entre operador e foguete

## Status do Projeto
- [x] Arquitetura macro de duas estações definida
- [x] BOM e pinagem iniciais documentados
- [ ] Firmware de comando e ignição finalizado
- [ ] Validação completa de comunicação LoRa
- [ ] Validação elétrica em bancada e campo
- [ ] Cases 3D finalizados para produção

## Contribuição
Consulte as diretrizes em [Boas Práticas Serra Rocketry](https://github.com/Serra-Rocketry/best-practices).

## Equipe
Projeto desenvolvido pela equipe Serra Rocketry.
