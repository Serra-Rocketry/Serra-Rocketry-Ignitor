# Plano de Testes

## Objetivos
- Validar comunicação LoRa bidirecional entre as duas estações
- Confirmar sequência de segurança (botão mantido por 5 s)
- Testar feedback visual (LEDs) e sonoro (buzzers) em ambas estações
- Verificar abort automático e manual

## Testes Prioritários

### 1. Comunicação LoRa Básica
**Objetivo:** Confirmar que as duas estações se comunicam de forma confiável.

Checklist:
- [ ] Estação de Comando envia `HEARTBEAT`, Estação de Ignição responde com `HEARTBEAT_ACK`
- [ ] LED amarelo acende em ambas estações quando conectadas
- [ ] RSSI da Estação de Ignição visível no serial monitor
- [ ] Testar perda de comunicação: desligar uma estação e verificar timeout

**Procedimento:**
1. Ligar ambas estações a 1 m de distância
2. Observar LEDs e monitor serial
3. Afastar estações gradualmente (10 m, 50 m, 100 m, 500 m)
4. Registrar RSSI e taxa de perda de pacotes

### 2. Sequência de Ignição Completa
**Objetivo:** Validar contagem regressiva de 5 segundos e acionamento do ignitor.

Checklist:
- [ ] Pressionar botão de ignição na Estação de Comando por 5 s completos
- [ ] LED vermelho pisca em ambas estações durante contagem
- [ ] Buzzer da Estação de Ignição emite 5 apitos (1 por segundo)
- [ ] Ao final, GPIO16 da Estação de Ignição vai HIGH
- [ ] LED vermelho fica sólido ao acionar ignitor
- [ ] Usar **carga dummy** (lâmpada 12 V ou LED) no lugar do ignitor real

**Procedimento:**
1. Conectar lâmpada/LED ao GPIO16 da Estação de Ignição
2. Pressionar e **segurar** botão de ignição
3. Observar: LEDs, buzzers, e acionamento da carga
4. Confirmar que lâmpada acende apenas após 5 s

### 3. Abort Manual
**Objetivo:** Garantir que soltar o botão antes de 5 s cancela a ignição.

Checklist:
- [ ] Pressionar botão de ignição
- [ ] Soltar botão após 2-3 segundos
- [ ] Estação de Comando envia `ABORT`
- [ ] Estação de Ignição interrompe contagem
- [ ] Ambas voltam ao estado `CONNECTED` (LED amarelo)
- [ ] Ignitor **não** é acionado

**Procedimento:**
1. Iniciar sequência de ignição
2. Soltar botão no 2º ou 3º apito
3. Verificar que GPIO16 permanece LOW
4. Repetir 5 vezes para consistência

### 4. Abort Automático por Timeout
**Objetivo:** Estação de Ignição aborta se perder comunicação durante sequência.

Checklist:
- [ ] Iniciar sequência de ignição
- [ ] Desligar Estação de Comando no 3º segundo
- [ ] Estação de Ignição detecta timeout (2 s sem heartbeat)
- [ ] LED vermelho pisca rapidamente (erro)
- [ ] Buzzer emite padrão de erro
- [ ] Ignitor **não** é acionado

**Procedimento:**
1. Iniciar sequência de ignição
2. Desconectar antena ou desligar Estação de Comando após 2-3 s
3. Observar comportamento da Estação de Ignição

### 5. Indicadores Visuais e Sonoros
**Objetivo:** Confirmar padrões de LEDs e buzzer conforme especificação.

Checklist:
- [ ] **Verde:** acende ao ligar, permanece durante operação
- [ ] **Amarelo:** acende quando conectado (heartbeats OK)
- [ ] **Vermelho:** pisca durante ignição iminente, sólido ao acionar
- [ ] **Buzzer Ignição:** 5 apitos durante contagem + tom longo ao acionar
- [ ] **Buzzer Comando:** tom contínuo enquanto botão pressionado

**Procedimento:**
1. Testar cada estado individualmente via comandos seriais (debug)
2. Testar sequência completa e documentar com vídeo

### 6. Teste de Alcance em Campo
**Objetivo:** Determinar alcance máximo confiável.

Checklist:
- [ ] Posicionar estações em LOS (linha de visão)
- [ ] Aumentar distância: 50 m, 100 m, 200 m, 500 m, 1 km
- [ ] Registrar RSSI e taxa de sucesso de pacotes
- [ ] Executar sequência completa de ignição na distância máxima validada

**Procedimento:**
1. Usar tripé/suporte para estações
2. Medir distância com GPS ou trena a laser
3. Executar 10 ciclos de ignição (com carga dummy) em cada distância
4. Registrar falhas em `docs/TESTES.md`

## Como Executar

### Testes Automatizados
```bash
cd test
python run_tests.py --all         # todos os testes
python run_tests.py --lora        # apenas comunicação LoRa
python run_tests.py --ignition    # sequência de ignição
```

### Testes Manuais
Seguir procedimentos descritos acima. Documentar resultados em `docs/TESTES.md`.

## Segurança nos Testes

⚠️ **NUNCA usar ignitor real durante testes!**

- Sempre usar carga dummy (lâmpada, LED, resistor)
- Confirmar que não há propelente próximo
- Testar a >= 10 m de pessoas
- Usar óculos de proteção ao manipular eletrônica energizada

## Registro de Resultados

Após cada sessão de testes, preencher tabela em `docs/TESTES.md` com:
- Data
- ID do teste
- Resultado (pass/fail)
- Observações
- Link para logs/vídeos
