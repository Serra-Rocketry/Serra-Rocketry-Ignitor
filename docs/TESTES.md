# Resultados de Testes

## Matriz de Testes

| ID | Tipo | Descrição | Critério de Sucesso | Status |
|----|------|-----------|---------------------|--------|
| LORA-01 | Comunicação | Heartbeat bidirecional | ACK < 200 ms | ⏳ Pendente |
| LORA-02 | Comunicação | RSSI a 50 m | RSSI > -80 dBm | ⏳ Pendente |
| LORA-03 | Comunicação | RSSI a 500 m | RSSI > -120 dBm | ⏳ Pendente |
| IGN-01 | Ignição | Sequência completa (5 s) | Ignitor aciona após 5 s | ⏳ Pendente |
| IGN-02 | Ignição | Contagem de apitos | 5 apitos audíveis | ⏳ Pendente |
| IGN-03 | Ignição | LEDs durante ignição | Vermelho pisca → sólido | ⏳ Pendente |
| ABORT-01 | Segurança | Abort manual (botão solto) | Ignitor não aciona | ⏳ Pendente |
| ABORT-02 | Segurança | Abort por timeout | Ignitor não aciona após 2 s sem sinal | ⏳ Pendente |
| LED-01 | Interface | LED verde (ligado) | Acende ao energizar | ⏳ Pendente |
| LED-02 | Interface | LED amarelo (conectado) | Acende com heartbeat OK | ⏳ Pendente |
| LED-03 | Interface | LED vermelho (ignição) | Comportamento correto | ⏳ Pendente |
| BUZZ-01 | Interface | Buzzer contagem (Ignição) | 5 apitos separados | ⏳ Pendente |
| BUZZ-02 | Interface | Buzzer comando (Comando) | Tom contínuo durante press | ⏳ Pendente |

## Registro de Sessões de Teste

### Formato
| Data | ID Teste | Resultado | Distância | RSSI | Observações | Evidências |
|------|----------|-----------|-----------|------|-------------|------------|
| - | - | - | - | - | - | - |

### Histórico
| Data | ID Teste | Resultado | Distância | RSSI | Observações | Evidências |
|------|----------|-----------|-----------|------|-------------|------------|
| *Aguardando testes* | - | - | - | - | - | - |

## Notas
- **Status:** ⏳ Pendente | ✅ Passou | ❌ Falhou
- Logs CSV devem ser salvos em `test/logs/` (não versionado)
- Vídeos/fotos grandes devem ser linkados externamente (Google Drive, etc.)
- Atualizar esta tabela após cada sessão de testes
