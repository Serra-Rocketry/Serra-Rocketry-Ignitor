# Serra Rocketry Ignitor

![Status](https://img.shields.io/badge/status-em%20desenvolvimento-yellow)
![Versão](https://img.shields.io/badge/versão-1.0.0-blue)

## 📋 Sobre
Sistema de ignição remota para foguetes experimentais Serra Rocketry. Composto por duas estações independentes: **Estação de Comando** (operada remotamente) e **Estação de Ignição** (conectada ao ignitor do foguete). Comunicação via LoRa com redundâncias de segurança e feedback audiovisual em ambas as estações.

## 🚀 Quick Start
1. Clone o repositório
2. Configure o hardware conforme esquemático
3. Carregue o firmware
4. Execute os testes

## 📁 Estrutura do Projeto
├── docs/           → Documentação detalhada
├── firmware/       → Código do microcontrolador
├── hardware/       → Esquemáticos e PCBs
├── software/       → Interfaces e análises
└── test/           → Testes e validação

## 🔧 Componentes Principais
- 2x Raspberry Pi Pico
- 2x Módulos LoRa (915 MHz)
- 6x LEDs (2 verdes, 2 amarelos, 2 vermelhos)
- 2x Buzzers ativos
- 2x Botões (comando: liga/desliga + ignição)
- 2x Baterias (a definir)
- Cases impressos em 3D

## 📖 Documentação
- [Guia de Instalação Detalhado](./docs/INSTALACAO.md)
- [Esquemático e Montagem](./hardware/README.md)
- [API e Protocolos](./docs/API.md)
- [Troubleshooting](./docs/TROUBLESHOOTING.md)

## 🤝 Contribuindo
Ver [Boas Práticas Serra Rocketry](https://github.com/Serra-Rocketry/best-practices)

## 📊 Status do Projeto
- [ ] Definição de arquitetura e componentes
- [ ] Esquemáticos e pinagem
- [ ] Firmware Estação de Comando
- [ ] Firmware Estação de Ignição
- [ ] Protocolo de comunicação LoRa
- [ ] Cases 3D
- [ ] Testes de campo

## ✨ Equipe
Projeto desenvolvido pela equipe Serra Rocketry
