# Guia de Instalação Detalhado

## Visão Geral

Este guia orienta a montagem das duas estações do sistema de ignição:
- **Estação de Comando** (operada remotamente)
- **Estação de Ignição** (conectada ao foguete)

## 1. Pré-requisitos de Ambiente

### Software
- Python 3.8+ (`pip install -r requirements.txt`)
- Thonny IDE (recomendado para Raspberry Pi Pico) ou VS Code com extensão MicroPython
- Ferramenta de flash: `picotool` ou via Thonny

### Hardware
- Multímetro para verificação de continuidade
- Fonte de bancada 3.3-5 V ou baterias
- Ferro de solda (montagem em protoboard ou PCB)

## 2. Clonar e Preparar

```bash
git clone https://github.com/Serra-Rocketry/Serra-Rocketry-Ignitor.git
cd Serra-Rocketry-Ignitor
pip install -r requirements.txt
```

## 3. Montagem de Hardware

### 3.1. Estação de Comando

**Componentes necessários:**
- 1x Raspberry Pi Pico
- 1x Módulo LoRa 915 MHz
- 3x LEDs (verde, amarelo, vermelho) + 3x resistores 220Ω
- 1x Buzzer ativo 5V
- 2x Botões tácteis (power, ignição)
- Bateria e regulador (se necessário)

**Conexões:** Ver pinagem detalhada em `hardware/README.md`.

**Checklist:**
1. Soldar headers no Raspberry Pi Pico (se necessário)
2. Conectar módulo LoRa via SPI (GP0-GP4)
3. Conectar LEDs aos GPIOs 10-12 com resistores
4. Conectar buzzer ao GP13
5. Conectar botões aos GP14 (power) e GP15 (ignição) com pull-down
6. Fixar antena no módulo LoRa **antes** de energizar
7. Conectar bateria/fonte

### 3.2. Estação de Ignição

**Componentes necessários:**
- 1x Raspberry Pi Pico
- 1x Módulo LoRa 915 MHz
- 3x LEDs (verde, amarelo, vermelho) + 3x resistores 220Ω
- 1x Buzzer ativo 5V
- 1x MOSFET ou relé para ignitor
- Bateria e regulador (se necessário)

**Conexões:** Ver pinagem detalhada em `hardware/README.md`.

**Checklist:**
1. Soldar headers no Raspberry Pi Pico
2. Conectar módulo LoRa via SPI (GP0-GP4)
3. Conectar LEDs aos GPIOs 10-12 com resistores
4. Conectar buzzer ao GP13
5. Conectar MOSFET/relé ao GP16 (gate do ignitor)
6. Isolar circuito do ignitor com diodo flyback (se usar relé)
7. Fixar antena no módulo LoRa **antes** de energizar
8. Conectar bateria/fonte

### 3.3. Teste de Continuidade

Antes de energizar:
- Verificar curto-circuitos com multímetro
- Confirmar polaridade da bateria
- Verificar que antenas estão conectadas

## 4. Configuração do Firmware

### 4.1. Copiar Configuração

```bash
cp firmware/config.example.h firmware/config.h
```

Editar `config.h` e ajustar:
- `LORA_FREQ` → 915.0 (MHz)
- `STATION_TYPE` → `COMMAND` ou `IGNITION`
- IDs únicos para cada estação

### 4.2. Compilar e Gravar

#### Usando Thonny:
1. Abrir Thonny IDE
2. Conectar Raspberry Pi Pico via USB (segurar BOOTSEL ao conectar)
3. Instalar MicroPython: Tools → Options → Interpreter → Install or update MicroPython
4. Abrir `firmware/main.py` (Estação de Comando ou Ignição)
5. Clicar em Run → Save to Raspberry Pi Pico
6. Repetir para a outra estação

#### Usando linha de comando:
```bash
cd firmware
# Para Estação de Comando
ampy --port /dev/ttyACM0 put command_station.py main.py

# Para Estação de Ignição
ampy --port /dev/ttyACM1 put ignition_station.py main.py
```

## 5. Validar Operação

### 5.1. Teste Inicial (sem ignitor)

1. Energizar **apenas** a Estação de Comando
   - LED verde deve acender
   - LED amarelo permanece apagado (sem conexão)

2. Energizar a Estação de Ignição
   - LED verde acende em ambas
   - Após 1-2 s, LEDs amarelos acendem (heartbeat estabelecido)
   - Monitor serial mostra mensagens `HEARTBEAT` e `HEARTBEAT_ACK`

3. Testar sequência de ignição (com carga dummy)
   - Conectar LED/lâmpada ao GP16 da Estação de Ignição
   - Pressionar e segurar botão de ignição na Estação de Comando
   - Observar: LEDs vermelhos piscam, buzzer emite 5 apitos
   - Ao final, LED/lâmpada conectada ao GP16 acende

### 5.2. Teste de Abort

1. Iniciar sequência de ignição
2. Soltar botão após 2-3 segundos
3. Confirmar que carga dummy **não** aciona

### 5.3. Teste de Alcance

1. Posicionar estações a 50 m de distância (LOS)
2. Confirmar LEDs amarelos permanecem acesos
3. Executar sequência completa de ignição
4. Se sucesso, aumentar distância gradualmente

## 6. Montagem nos Cases 3D

1. Imprimir cases: `hardware/3d_models/case_command.stl` e `case_ignition.stl`
2. Fixar componentes com parafusos M3 ou cola quente
3. Garantir que antenas ficam externas ao case
4. Posicionar LEDs e botões nos furos frontais

## 7. Integração com Ignitor Real

⚠️ **ATENÇÃO: Realizar apenas em local seguro e com supervisão!**

1. Conectar ignitor aos terminais da Estação de Ignição (saída do MOSFET/relé)
2. Confirmar continuidade com multímetro
3. Posicionar Estação de Ignição junto ao foguete (≥ 10 m de distância)
4. Operador permanece com Estação de Comando em posição segura
5. Executar sequência de ignição conforme procedimento operacional

## 8. Troubleshooting Inicial

| Problema | Possível Causa | Solução |
|----------|----------------|---------|
| LED verde não acende | Sem alimentação | Verificar bateria/fonte |
| LED amarelo não acende | LoRa não comunica | Confirmar antenas, frequência, conexões SPI |
| Buzzer não soa | GPIO mal conectado | Verificar GP13, trocar buzzer |
| Ignitor não aciona | MOSFET/relé com problema | Testar GP16 com LED, verificar gate driver |

Ver mais em `docs/TROUBLESHOOTING.md`.

## 9. Próximos Passos

- Executar plano de testes completo: `test/README.md`
- Registrar resultados em `docs/TESTES.md`
- Calibrar alcance LoRa conforme necessidade
- Imprimir cases finais após validação
