# Hardware - Serra Rocketry Ignitor

## Diagrama Esquemático

![Esquemático da Estação de Comando](images/schematics/ESTA%C3%87AO%20DE%20COMANDO_bb.png)

## Arquitetura do Sistema

O sistema é composto por **duas estações independentes** que se comunicam via rádio LoRa:

### 1. Estação de Comando
Operada remotamente pelo operador (distância segura do foguete).

**Função:**
- Iniciar sequência de ignição via botão dedicado
- Monitorar status da Estação de Ignição
- Fornecer feedback visual e sonoro ao operador

**Componentes:**
- 1x Raspberry Pi Pico
- 1x Módulo LoRa SX1268 433 MHz
- 1x Botão liga/desliga
- 1x Botão de ignição (ação mantida por 5 s)
- 3x LEDs: verde (ligado), amarelo (conectado), vermelho (ignição iminente)
- 1x Buzzer ativo
- 1x Bateria (a definir capacidade)
- Case impresso em 3D

### 2. Estação de Ignição
Conectada fisicamente ao ignitor do foguete.

**Função:**
- Receber comandos de ignição da Estação de Comando
- Executar contagem regressiva (5 s) com avisos sonoros
- Acionar o ignitor ao final da contagem
- Abortar procedimento se comando for interrompido

**Componentes:**
- 1x Raspberry Pi Pico
- 1x Módulo LoRa SX1268 433 MHz
- 3x LEDs: verde (ligado), amarelo (conectado), vermelho (ignição iminente)
- 1x Buzzer ativo
- 1x Relé ou MOSFET para acionamento do ignitor
- 1x Bateria (a definir capacidade)
- Case impresso em 3D

## Lista de Componentes (BOM)

| Componente | Quantidade | Referência | Especificação | Link |
|------------|------------|------------|---------------|------|
| Raspberry Pi Pico | 2 | MCU1, MCU2 | RP2040 | [Raspberry Pi](https://www.raspberrypi.com/products/raspberry-pi-pico/) |
| Módulo LoRa SX1268 | 2 | LORA1, LORA2 | 433 MHz | [Hoperf](https://www.hoperf.com/) |
| LED Verde 5mm | 2 | LED_G1, LED_G2 | 20 mA | - |
| LED Amarelo 5mm | 2 | LED_Y1, LED_Y2 | 20 mA | - |
| LED Vermelho 5mm | 2 | LED_R1, LED_R2 | 20 mA | - |
| Buzzer ativo 5V | 2 | BZ1, BZ2 | Piezo | - |
| Botão Liga/Desliga | 2 | BTN_PWR1, BTN_PWR2 | 12V 20A | - |
| Botão de Ignição | 1 | BTN_IGN | 22mm, 3-9V (5V), Momentary Reset, Vermelho | - |
| MOSFET/Relé | 1 | Q1/K1 | Para ignitor (corrente a definir) | - |
| Bateria | 2 | BAT1, BAT2 | A definir (LiPo/18650) | - |
| Resistores 220Ω | 6 | R1-R6 | Para LEDs | - |
| Antena LoRa | 2 | ANT1, ANT2 | 433 MHz | - |

## Pinagem Raspberry Pi Pico

### Estação de Comando
| GPIO Pico | Conexão | Descrição |
|-----------|---------|-----------|
| GP0 | LoRa SPI RX | SPI MISO |
| GP1 | LoRa SPI CS | Chip Select |
| GP2 | LoRa SPI SCK | SPI Clock |
| GP3 | LoRa SPI TX | SPI MOSI |
| GP4 | LoRa RESET | Reset |
| GP10 | LED Verde | Status: ligado |
| GP11 | LED Amarelo | Status: conectado |
| GP12 | LED Vermelho | Status: ignição iminente |
| GP13 | Buzzer | Alerta sonoro |
| GP14 | Botão Power | Liga/desliga |
| GP15 | Botão Ignição | Comando de ignição (segurar 5 s) |

### Estação de Ignição
| GPIO Pico | Conexão | Descrição |
|-----------|---------|-----------|
| GP0 | LoRa SPI RX | SPI MISO |
| GP1 | LoRa SPI CS | Chip Select |
| GP2 | LoRa SPI SCK | SPI Clock |
| GP3 | LoRa SPI TX | SPI MOSI |
| GP4 | LoRa RESET | Reset |
| GP10 | LED Verde | Status: ligado |
| GP11 | LED Amarelo | Status: conectado com comando |
| GP12 | LED Vermelho | Status: ignição iminente |
| GP13 | Buzzer | Contagem regressiva |
| GP16 | Gate Ignitor | Comando MOSFET/Relé |
| GP17 | Sensor Continuidade | Detecta circuito ignitor (opcional) |

## Sequência de Operação

### Estados dos LEDs
| Estado | LED Verde | LED Amarelo | LED Vermelho |
|--------|-----------|-------------|--------------|
| Desligado | OFF | OFF | OFF |
| Ligado (idle) | ON | OFF | OFF |
| Conectado | ON | ON | OFF |
| Ignição iminente | ON | ON | PISCA |
| Ignição ativa | ON | ON | ON |
| Erro | OFF | OFF | PISCA |

### Sequência de Ignição

1. **Operador pressiona botão de ignição na Estação de Comando**
   - Botão deve ser **mantido pressionado por 5 segundos**
   - LED vermelho começa a piscar
   - Buzzer emite tom contínuo

2. **Estação de Comando envia sinal LoRa para Estação de Ignição**
   - Pacote com comando `ARM`
   - Requer confirmação (ACK) da Estação de Ignição

3. **Estação de Ignição recebe comando e inicia contagem**
   - LED vermelho pisca rapidamente
   - Buzzer emite **5 apitos** (1 por segundo) = contagem regressiva
   - A cada segundo, verifica se comando ainda está ativo

4. **Durante a contagem (5 s):**
   - Se botão na Estação de Comando for **solto**: abortado
   - Estação de Ignição envia mensagem de abort
   - Ambas as estações voltam ao estado "Conectado"

5. **Ao final dos 5 segundos:**
   - LED vermelho fica **sólido** em ambas estações
   - Estação de Ignição aciona o ignitor (GPIO16 HIGH)
   - Buzzer emite tom longo de confirmação

6. **Após ignição:**
   - Sistema aguarda 3 s e retorna ao estado "Conectado"

## Segurança

- Botão de ignição deve ser do tipo **momentâneo** (não trava)
- Ignição só ocorre se comando for mantido por toda a sequência (5 s)
- Comunicação LoRa bidirecional: Estação de Ignição confirma recebimento
- Timeout: se Estação de Ignição não receber heartbeat por 2 s, aborta
- Sensor de continuidade (opcional): verifica se ignitor está conectado antes de armar

## Consumo de Energia

**Estimativas iniciais (a validar):**
- Operação normal (conectado): ~50 mA @ 3.3 V
- Transmissão LoRa: ~120 mA (picos)
- Autonomia estimada: 6-8 h com bateria 1000 mAh

## Galeria de Componentes

### Raspberry Pi Pico
<img src="./images/pipico.png" width="220" alt="Raspberry Pi Pico"/>

Microcontrolador dual-core ARM Cortex-M0+ (RP2040), 264KB SRAM, 2MB Flash.

### Módulo LoRa SX1268
<img src="./images/lora.png" width="220" alt="Módulo LoRa SX1268"/>

Transceptor LoRa de longo alcance operando em **433 MHz**. Chip Semtech SX1268.

### Botão Liga/Desliga
<img src="./images/power-button.avif" width="220" alt="Botão Liga/Desliga"/>

Botão de potência **12V 20A** para controle de energia das estações.

### Botão de Ignição
<img src="./images/ignition-button.png" width="220" alt="Botão de Ignição"/>

Botão momentâneo (Momentary Reset), **22mm**, vermelho, **3-9V (5V)**. Usado exclusivamente na Estação de Comando para iniciar a sequência de ignição.

## Arquivos de Fabricação

- [Esquemático Estação de Comando (PNG)](./images/schematics/ESTA%C3%87AO%20DE%20COMANDO_bb.png)
- [Esquemático Estação de Ignição (PNG)](./images/schematics/ESTA%C3%87%C3%83O%20DE%20IGNI%C3%87%C3%83O_bb.png)
- Esquemático PDF - *A criar*
- [PCB Gerbers](./gerbers/) - *A criar*
- [Modelos 3D dos cases](./3d_models/) - *A criar*

## Notas Importantes

- **Sempre conectar antena LoRa antes de energizar** (dano ao módulo)
- Isolar circuito do ignitor via MOSFET/relé
- Testar sequência completa com carga dummy (lâmpada) antes de uso real
- Manter distância mínima de 10 m entre operador e foguete
