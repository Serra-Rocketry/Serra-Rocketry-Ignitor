# config.example.h - Template de configuração
# Copie para config.h e ajuste conforme necessário

# ============================================
# IDENTIFICAÇÃO DA ESTAÇÃO
# ============================================
STATION_TYPE = "COMMAND"  # "COMMAND" ou "IGNITION"
STATION_ID = 0x01         # ID único da estação

# ============================================
# CONFIGURAÇÃO LoRa
# ============================================
LORA_FREQ = 915.0              # Frequência em MHz (915 para América do Sul)
LORA_BANDWIDTH = 125000        # Largura de banda em Hz (125 kHz)
LORA_SPREADING_FACTOR = 7      # 7-12 (maior = + alcance, - velocidade)
LORA_CODING_RATE = 5           # 5-8 (taxa de correção de erros)
LORA_TX_POWER = 20             # Potência de transmissão em dBm (max 20)
LORA_PREAMBLE_LENGTH = 8       # Comprimento do preâmbulo

# ============================================
# TIMEOUTS E INTERVALOS
# ============================================
HEARTBEAT_INTERVAL = 1.0       # Intervalo de envio do heartbeat (segundos)
HEARTBEAT_TIMEOUT = 2.0        # Timeout para considerar conexão perdida (segundos)
ARM_INTERVAL = 1.0             # Intervalo entre comandos ARM_ACTIVE (segundos)
ARM_COUNTDOWN = 5              # Contagem regressiva em segundos (5→0)
IGNITION_DURATION = 3.0        # Tempo de acionamento do ignitor (segundos)

# ============================================
# PINAGEM GPIO (Raspberry Pi Pico)
# ============================================
# LoRa SPI
PIN_LORA_MISO = 0
PIN_LORA_CS = 1
PIN_LORA_SCK = 2
PIN_LORA_MOSI = 3
PIN_LORA_RESET = 4

# Indicadores
PIN_LED_GREEN = 10      # LED verde (sistema ligado)
PIN_LED_YELLOW = 11     # LED amarelo (conectado)
PIN_LED_RED = 12        # LED vermelho (ignição iminente)
PIN_BUZZER = 13         # Buzzer ativo

# Controles (Estação de Comando)
PIN_BUTTON_POWER = 14      # Botão de ligar/desligar
PIN_BUTTON_IGNITION = 15   # Botão de ignição (segurar 5 s)

# Ignitor (Estação de Ignição)
PIN_IGNITOR_GATE = 16      # Saída para MOSFET/relé do ignitor
PIN_CONTINUITY_SENSE = 17  # Sensor de continuidade (opcional)

# ============================================
# PADRÕES DE FEEDBACK
# ============================================
# Buzzer
BUZZER_BEEP_DURATION = 0.1     # Duração de um apito (segundos)
BUZZER_COUNTDOWN_FREQ = 2000   # Frequência dos apitos de contagem (Hz)
BUZZER_IGNITION_FREQ = 1000    # Frequência do tom de ignição (Hz)
BUZZER_ERROR_FREQ = 3000       # Frequência do alerta de erro (Hz)

# LED
LED_BLINK_FAST = 0.1           # Intervalo de piscada rápida (segundos)
LED_BLINK_SLOW = 0.5           # Intervalo de piscada lenta (segundos)

# ============================================
# SEGURANÇA
# ============================================
ENABLE_CONTINUITY_CHECK = False  # Verificar continuidade do ignitor antes de armar
MIN_BATTERY_VOLTAGE = 3.0        # Tensão mínima da bateria (V)
ENABLE_WATCHDOG = True           # Habilitar watchdog timer
WATCHDOG_TIMEOUT = 5000          # Timeout do watchdog (ms)

# ============================================
# DEBUG
# ============================================
DEBUG_MODE = True          # Ativar logs detalhados no serial
DEBUG_BAUD_RATE = 115200   # Baud rate do monitor serial
