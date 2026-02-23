# API e Protocolos

## Visão Geral

Comunicação bidirecional entre **Estação de Comando** e **Estação de Ignição** via LoRa.

## Camada Física
- **Frequência:** 915 MHz (ISM Band)
- **Largura de banda:** 125 kHz
- **Spreading Factor:** 7-12 (a definir conforme testes de alcance)
- **Potência:** 20 dBm (100 mW)
- **Alcance esperado:** > 500 m (LOS)

## Estrutura de Pacote

Todos os pacotes seguem formato padronizado:

| Campo | Bytes | Descrição |
|-------|-------|-----------|
| Preamble | 2 | `0xAA55` - Sincronização |
| Msg ID | 1 | Tipo de mensagem (ver tabela) |
| Payload | 0-32 | Dados da mensagem |
| CRC16 | 2 | CRC-CCITT-FALSE para validação |

**Total máximo:** 37 bytes por pacote

## Tipos de Mensagem

### Comando → Ignição

| Msg ID | Nome | Payload | Descrição |
|--------|------|---------|-----------|
| 0x10 | `HEARTBEAT` | Timestamp (4 bytes) | Mantém conexão ativa |
| 0x20 | `ARM_REQUEST` | - | Solicita arme do sistema |
| 0x21 | `ARM_ACTIVE` | Counter (1 byte) | Comando ativo, contador 5-0 |
| 0x22 | `ABORT` | - | Cancela sequência de ignição |
| 0x30 | `STATUS_REQUEST` | - | Solicita status da Estação de Ignição |

### Ignição → Comando

| Msg ID | Nome | Payload | Descrição |
|--------|------|---------|-----------|
| 0x11 | `HEARTBEAT_ACK` | RSSI (1 byte) | Confirma heartbeat |
| 0x40 | `STATUS_IDLE` | Battery % (1 byte) | Sistema em idle |
| 0x41 | `STATUS_CONNECTED` | Battery % (1 byte) | Conectado e pronto |
| 0x42 | `STATUS_ARMED` | Countdown (1 byte) | Armado, contagem N s |
| 0x43 | `STATUS_IGNITION` | - | Ignição acionada |
| 0x44 | `STATUS_ERROR` | Error code (1 byte) | Erro detectado |
| 0x50 | `ACK` | Msg ID ackd (1 byte) | Confirma recebimento |
| 0x51 | `NACK` | Reason (1 byte) | Rejeita comando |

## Códigos de Erro

| Código | Descrição |
|--------|-----------|
| 0x01 | Perda de heartbeat (timeout) |
| 0x02 | Circuito do ignitor aberto |
| 0x03 | Bateria crítica (< 10%) |
| 0x04 | Falha no LoRa |

## Fluxo de Comunicação

### 1. Estabelecimento de Conexão

```
Comando              Ignição
  |                     |
  |--- HEARTBEAT ------>|
  |<-- HEARTBEAT_ACK ---|
  |                     |
  (repete a cada 1 s)
```

**LED Amarelo ON** em ambas estações quando heartbeats bem-sucedidos.

### 2. Sequência de Ignição Completa

```
Comando                          Ignição
  |                                 |
  |--- ARM_REQUEST --------------->|
  |<-- ACK (0x20) -----------------|
  |                                 | [Buzzer: apito 1]
  |                                 |
  |--- ARM_ACTIVE (counter=5) ---->|
  |<-- STATUS_ARMED (5) -----------|
  |                                 | [Buzzer: apito 2]
  |                                 |
  |--- ARM_ACTIVE (counter=4) ---->|
  |<-- STATUS_ARMED (4) -----------|
  |                                 | [Buzzer: apito 3]
  |                                 |
  |--- ARM_ACTIVE (counter=3) ---->|
  |<-- STATUS_ARMED (3) -----------|
  |                                 | [Buzzer: apito 4]
  |                                 |
  |--- ARM_ACTIVE (counter=2) ---->|
  |<-- STATUS_ARMED (2) -----------|
  |                                 | [Buzzer: apito 5]
  |                                 |
  |--- ARM_ACTIVE (counter=1) ---->|
  |<-- STATUS_ARMED (1) -----------|
  |                                 |
  |--- ARM_ACTIVE (counter=0) ---->|
  |                                 | [ACIONA IGNITOR]
  |<-- STATUS_IGNITION ------------|
  |                                 | [Buzzer: tom longo]
```

**Duração total:** 5 segundos (1 s por passo)

### 3. Abort durante Sequência

```
Comando                          Ignição
  |                                 |
  |--- ARM_ACTIVE (counter=3) ---->|
  |<-- STATUS_ARMED (3) -----------|
  |                                 |
  [Botão solto]                     |
  |--- ABORT --------------------->|
  |<-- ACK (0x22) -----------------|
  |                                 | [Volta para CONNECTED]
  |<-- STATUS_CONNECTED -----------|
```

## Timeouts e Segurança

| Parâmetro | Valor | Ação |
|-----------|-------|------|
| Heartbeat interval | 1 s | Comando envia a cada 1 s |
| Heartbeat timeout | 2 s | Se Ignição não receber, aborta e sinaliza erro |
| ARM_ACTIVE interval | 1 s | Um pacote por segundo da contagem |
| Comando perdido | 1 miss | Ignição aborta se não receber contador sequencial |

## Validação de Pacotes

1. **Verificação de CRC:** Todo pacote recebido deve ter CRC válido
2. **Timestamp/Sequência:** Pacotes `ARM_ACTIVE` devem ter contadores decrescentes sequenciais (5→0)
3. **ACK obrigatório:** Comandos críticos (`ARM_REQUEST`, `ABORT`) exigem ACK

## Implementação Futura

- Autenticação simétrica (AES-128) para prevenir comandos não autorizados
- Compressão de payload para mensagens longas
- Modo de diagnóstico para testes em bancada
