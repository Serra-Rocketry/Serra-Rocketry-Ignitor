# Troubleshooting

## Problemas Comuns

| Sintoma | Causa Provável | Solução |
|---------|----------------|---------|
| LED verde não acende | Sem alimentação | Verificar conexão da bateria, medir tensão com multímetro |
| LED amarelo não acende (não conecta) | LoRa não comunica | Confirmar antenas conectadas, frequência em `config.h`, conexões SPI |
| LED amarelo pisca (conecta e desconecta) | Sinal fraco ou interferência | Aproximar estações, verificar RSSI no serial, trocar canal |
| Buzzer não emite som | GPIO mal conectado ou buzzer danificado | Verificar GP13, testar buzzer em 5V direto |
| Botão de ignição não responde | Botão com problema ou GPIO | Testar continuidade do botão, verificar GP15 com multímetro |
| Ignitor não aciona | MOSFET/relé com problema | Testar GP16 com LED, medir tensão no gate, verificar flyback diode |
| Contagem começa mas aborta sozinha | Timeout de comunicação | Melhorar sinal LoRa, verificar RSSI, reduzir distância |
| Sistema reinicia aleatoriamente | Watchdog ou fonte insuficiente | Desabilitar watchdog temporariamente, usar fonte >= 2A |

## Checklist Rápido de Diagnóstico

### 1. Verificações Básicas
- [ ] Bateria carregada (medir tensão >= 3.3V)
- [ ] Antenas LoRa conectadas **antes** de energizar
- [ ] Cabo USB é de dados (não apenas carga) para programação
- [ ] Firmware correto gravado (COMMAND vs IGNITION)
- [ ] Configuração `config.h` correta (frequência LoRa, IDs)

### 2. Teste de LEDs
Conectar cada LED diretamente a 3.3V com resistor 220Ω:
- Verde, amarelo, vermelho devem acender
- Se não acender: LED queimado ou polaridade invertida

### 3. Teste de Buzzer
Conectar buzzer a 5V diretamente:
- Deve emitir som contínuo
- Se não: buzzer danificado ou não é buzzer ativo

### 4. Teste de LoRa
Monitorar serial (115200 baud) ao ligar:
```
[COMMAND] Boot OK
[COMMAND] LoRa init: 915.0 MHz
```
Se não aparecer "LoRa init": problema no módulo ou conexões SPI.

### 5. Teste de Conexão
Com ambas estações ligadas, verificar no serial:
```
[COMMAND] -> HEARTBEAT
[COMMAND] <- HEARTBEAT_ACK (RSSI: -45)
```
RSSI bom: -30 a -80 dBm  
RSSI fraco: -80 a -110 dBm  
RSSI crítico: < -110 dBm

## Problemas Específicos

### Estação de Comando não envia ARM

**Sintomas:**
- Pressionar botão de ignição não inicia contagem
- Nenhuma mensagem no serial ao pressionar botão

**Causas:**
1. Botão mal conectado
2. Estado não é CONNECTED (LED amarelo apagado)
3. GPIO15 não configurado corretamente

**Solução:**
1. Verificar continuidade do botão com multímetro
2. Confirmar que LED amarelo está aceso (conexão estabelecida)
3. Testar GPIO15 com código de teste:
```python
from machine import Pin
btn = Pin(15, Pin.IN, Pin.PULL_DOWN)
print(btn.value())  # Deve mostrar 0 (solto) ou 1 (pressionado)
```

### Estação de Ignição não recebe comandos

**Sintomas:**
- Estação de Comando envia ARM, mas Ignição não responde
- LED amarelo aceso (conectado), mas contagem não inicia

**Causas:**
1. CRC dos pacotes com erro
2. Frequência LoRa descasada entre estações
3. Interferência no canal

**Solução:**
1. Confirmar que ambas estações usam mesma frequência em `config.h`
2. Verificar logs de CRC no serial: `[ERROR] CRC mismatch`
3. Testar em ambiente sem interferência (distante de Wi-Fi 2.4 GHz)
4. Aumentar spreading factor (SF 7 → 10) para mais robustez

### Contagem aborta antes de 5 segundos

**Sintomas:**
- Contagem inicia (apitos começam), mas para no meio
- LED vermelho para de piscar
- Serial mostra "ABORT" ou "TIMEOUT"

**Causas:**
1. Botão de ignição solto antes de 5 s
2. Perda de pacotes LoRa durante sequência
3. Timeout de heartbeat (2 s sem comunicação)

**Solução:**
1. Segurar botão firmemente por todos os 5 s
2. Melhorar sinal LoRa (aproximar estações, elevar antenas)
3. Verificar RSSI durante contagem: deve permanecer > -100 dBm
4. Aumentar `HEARTBEAT_TIMEOUT` em `config.h` para 3.0 s (temporário)

### Ignitor não aciona (GPIO16 não sobe)

**Sintomas:**
- Contagem completa (5 apitos), mas ignitor não aciona
- LED no GP16 (teste) não acende
- Multímetro mostra 0V no GP16

**Causas:**
1. GPIO16 configurado incorretamente
2. Firmware da Estação de Ignição não atualizado
3. Estado não chega a IGNITION

**Solução:**
1. Testar GPIO16 com código simples:
```python
from machine import Pin
ignitor = Pin(16, Pin.OUT)
ignitor.value(1)  # Deve medir 3.3V
```
2. Recarregar firmware da Estação de Ignição
3. Verificar logs: deve mostrar `[IGNITION] -> GPIO16 HIGH`

## Logs de Diagnóstico

### Ativar logs verbosos

Editar `config.h`:
```python
DEBUG_MODE = True
DEBUG_BAUD_RATE = 115200
```

Reconectar serial e observar mensagens detalhadas.

### Salvar logs em arquivo

```bash
screen -L /dev/ttyACM0 115200
# Logs salvos em screenlog.0
```

Ou:
```bash
cat /dev/ttyACM0 | tee logs/session_$(date +%Y%m%d_%H%M%S).log
```

### Interpretar logs

**Bom:**
```
[COMMAND] Boot OK
[COMMAND] LoRa init: 915.0 MHz
[COMMAND] State: IDLE
[COMMAND] -> HEARTBEAT
[COMMAND] <- HEARTBEAT_ACK (RSSI: -50)
[COMMAND] State: CONNECTED
```

**Problema de LoRa:**
```
[COMMAND] Boot OK
[ERROR] LoRa init failed
```

**Problema de timeout:**
```
[IGNITION] <- HEARTBEAT (RSSI: -60)
[IGNITION] Heartbeat timeout! Last: 2.5s ago
[IGNITION] State: ERROR
```

## Testes Avançados

### Teste de alcance progressivo

1. Posicionar estações a 10 m
2. Executar 10 sequências de ignição
3. Se 100% sucesso, aumentar para 50 m
4. Repetir até encontrar distância máxima confiável (> 95% sucesso)

### Teste de interferência

1. Ligar roteador Wi-Fi 2.4 GHz próximo
2. Executar 10 sequências
3. Se taxa de sucesso < 90%, aumentar spreading factor

### Teste de bateria

1. Carregar bateria totalmente
2. Executar sequências de ignição a cada 10 min
3. Registrar quantas sequências até bateria crítica (< 3.0V)
4. Calcular autonomia real

## Contato e Suporte

Se problema persistir:

1. Abrir issue no GitHub
2. Incluir:
   - Versão do firmware (`git describe --tags`)
   - Configuração usada (`config.h`)
   - Logs do serial (últimas 50 linhas)
   - Fotos do setup (placas, conexões)
   - Distância entre estações
   - RSSI observado

**Template de issue:**
```
### Descrição do problema
[Descrever sintoma]

### Passos para reproduzir
1. [Passo 1]
2. [Passo 2]

### Configuração
- Firmware: v1.0.0
- Distância: 100 m
- RSSI: -85 dBm

### Logs
[Colar logs aqui]

### Fotos
[Link para fotos]
```
