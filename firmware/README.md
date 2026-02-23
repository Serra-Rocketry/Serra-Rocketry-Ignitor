# Firmware - Serra Rocketry Ignitor

## Visão Geral

O firmware é desenvolvido em **MicroPython** para Raspberry Pi Pico. Existem dois programas principais:
- `command_station.py` - Firmware da Estação de Comando
- `ignition_station.py` - Firmware da Estação de Ignição

## Arquitetura

### Estação de Comando

**Máquina de Estados:**
1. **IDLE** - Sistema ligado, aguardando conexão
2. **CONNECTED** - Heartbeat estabelecido com Estação de Ignição
3. **ARMING** - Botão de ignição pressionado, enviando comandos ARM
4. **IGNITION** - Ignição confirmada pela Estação de Ignição
5. **ERROR** - Erro de comunicação ou timeout

**Responsabilidades:**
- Enviar `HEARTBEAT` a cada 1 s
- Monitorar botão de ignição (GP15)
- Enviar sequência `ARM_ACTIVE` com contadores 5→0
- Enviar `ABORT` se botão for solto prematuramente
- Atualizar LEDs e buzzer conforme estado

### Estação de Ignição

**Máquina de Estados:**
1. **IDLE** - Sistema ligado, aguardando conexão
2. **CONNECTED** - Recebendo heartbeats da Estação de Comando
3. **ARMED** - Recebendo comandos ARM, executando contagem regressiva
4. **IGNITION** - Acionando ignitor (GP16 HIGH)
5. **ERROR** - Timeout de comunicação ou falha de hardware

**Responsabilidades:**
- Responder `HEARTBEAT_ACK` aos heartbeats
- Validar sequência de comandos `ARM_ACTIVE`
- Executar contagem regressiva com apitos
- Acionar ignitor (GP16) ao receber `ARM_ACTIVE(0)`
- Abortar se não receber heartbeat por 2 s

## Configuração

### 1. Copiar Template

```bash
cp firmware/config.example.h firmware/config.h
```

### 2. Ajustar Parâmetros

Editar `config.h`:

```python
# Identificação da estação
STATION_TYPE = "COMMAND"  # ou "IGNITION"
STATION_ID = 0x01

# LoRa
LORA_FREQ = 915.0  # MHz
LORA_BANDWIDTH = 125000  # Hz
LORA_SPREADING_FACTOR = 7  # 7-12 (maior = mais alcance, menor velocidade)
LORA_TX_POWER = 20  # dBm

# Timeouts
HEARTBEAT_INTERVAL = 1.0  # segundos
HEARTBEAT_TIMEOUT = 2.0   # segundos
ARM_INTERVAL = 1.0        # segundos (1 por segundo na contagem)

# GPIOs
PIN_LED_GREEN = 10
PIN_LED_YELLOW = 11
PIN_LED_RED = 12
PIN_BUZZER = 13
PIN_BUTTON_POWER = 14
PIN_BUTTON_IGNITION = 15  # apenas Estação de Comando
PIN_IGNITOR_GATE = 16     # apenas Estação de Ignição
```

## Estrutura do Código

```
firmware/
├── config.example.h       # Template de configuração
├── config.h               # Configuração local (gitignored)
├── lib/
│   ├── lora.py           # Driver LoRa (SX1276/SX1278)
│   ├── protocol.py       # Codificação/decodificação de pacotes
│   └── state_machine.py  # Lógica de estados
├── command_station.py     # Firmware Estação de Comando
└── ignition_station.py    # Firmware Estação de Ignição
```

## Fluxo de Dados

### Estação de Comando

```
main loop (100 Hz):
  1. Ler botão de ignição (GP15)
  2. Se pressionado e CONNECTED:
       - Entrar em ARMING
       - Enviar ARM_ACTIVE(counter)
       - Decrementar counter a cada 1 s
  3. Se solto durante ARMING:
       - Enviar ABORT
       - Voltar para CONNECTED
  4. A cada 1 s:
       - Enviar HEARTBEAT
       - Atualizar LEDs conforme estado
  5. Processar respostas do LoRa
```

### Estação de Ignição

```
main loop (100 Hz):
  1. Receber pacotes LoRa
  2. Se HEARTBEAT recebido:
       - Responder HEARTBEAT_ACK
       - Resetar timer de timeout
  3. Se ARM_ACTIVE recebido:
       - Validar contador sequencial
       - Emitir apito do buzzer
       - Atualizar LED vermelho (piscar)
  4. Se ARM_ACTIVE(0) recebido:
       - Acionar GP16 HIGH (ignitor)
       - LED vermelho sólido
       - Buzzer tom longo
  5. Se timeout (2 s sem HEARTBEAT):
       - Abortar, entrar em ERROR
       - LED vermelho pisca rápido
  6. Atualizar LEDs/buzzer conforme estado
```

## Comandos e Protocolos

Ver documentação completa em `docs/API.md`.

**Mensagens principais:**
- `HEARTBEAT` (0x10) - Comando → Ignição
- `HEARTBEAT_ACK` (0x11) - Ignição → Comando
- `ARM_ACTIVE(N)` (0x21) - Comando → Ignição, N = 5..0
- `ABORT` (0x22) - Comando → Ignição
- `STATUS_ARMED(N)` (0x42) - Ignição → Comando

## Build e Deploy

### Usando Thonny IDE

1. Conectar Raspberry Pi Pico via USB (segurar BOOTSEL)
2. Instalar MicroPython via Thonny (Tools → Options → Interpreter)
3. Carregar bibliotecas:
   - File → Save as → Raspberry Pi Pico → `lib/lora.py`
   - Repetir para `protocol.py` e `state_machine.py`
4. Carregar firmware principal:
   - Abrir `command_station.py` (ou `ignition_station.py`)
   - File → Save as → Raspberry Pi Pico → `main.py`
5. Desconectar e reconectar Pico (sem BOOTSEL) - deve executar automaticamente

### Usando ampy (linha de comando)

```bash
# Instalar ampy
pip install adafruit-ampy

# Upload de bibliotecas
ampy --port /dev/ttyACM0 mkdir lib
ampy --port /dev/ttyACM0 put lib/lora.py lib/lora.py
ampy --port /dev/ttyACM0 put lib/protocol.py lib/protocol.py
ampy --port /dev/ttyACM0 put lib/state_machine.py lib/state_machine.py

# Upload firmware (Estação de Comando)
ampy --port /dev/ttyACM0 put command_station.py main.py

# Upload firmware (Estação de Ignição)
ampy --port /dev/ttyACM1 put ignition_station.py main.py
```

## Debug via Serial

Conectar monitor serial (115200 baud) para ver logs:

```bash
screen /dev/ttyACM0 115200
# ou
minicom -D /dev/ttyACM0 -b 115200
```

**Mensagens esperadas:**
```
[COMMAND] Boot OK
[COMMAND] LoRa init: 915.0 MHz
[COMMAND] State: IDLE
[COMMAND] -> HEARTBEAT
[COMMAND] <- HEARTBEAT_ACK (RSSI: -45)
[COMMAND] State: CONNECTED
[COMMAND] Button pressed, starting ARM
[COMMAND] -> ARM_ACTIVE(5)
[COMMAND] <- STATUS_ARMED(5)
...
[COMMAND] -> ARM_ACTIVE(0)
[COMMAND] <- STATUS_IGNITION
[COMMAND] State: IGNITION
```

## Testes

### Teste Unitário (sem hardware)

```bash
cd firmware
python -m pytest tests/
```

### Teste de Integração (com hardware)

Ver `test/README.md` para procedimentos completos.

## Segurança no Código

- **Watchdog timer:** Reinicia Pico se main loop travar
- **Validação de CRC:** Todo pacote LoRa é verificado antes de processar
- **Timeout rígido:** Estação de Ignição aborta se não receber heartbeat por 2 s
- **Sequência monotônica:** Contador ARM_ACTIVE deve ser decrescente e sequencial (5→4→3→2→1→0)

## Próximas Implementações

- [ ] Criptografia AES-128 nos pacotes LoRa
- [ ] Sensor de continuidade do ignitor (GP17)
- [ ] Log em flash interno (últimas 10 ignições)
- [ ] Modo de diagnóstico via serial
- [ ] Over-the-air (OTA) update via LoRa
