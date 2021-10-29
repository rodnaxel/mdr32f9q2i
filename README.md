
# Конфигурация проекта

- Установить Pack от Milandr для MDR32F9Q2I (M1986BE92)
- Создать новый проект в качестве Device выбрать M1986BE92
- Добавить в Manage Runtime 
    - СMSIS - Core
    - Device - Startup
    - Drivers - необходимую переферию
- Внести изменения в файле MDR32F9Qx_config.h в строку с

```
// Вместо 8000000 установить частоту внешнего генератора
define HSE_VALUE ((uint32_t)8000000)
```
- В <Options for Target> - <Debug> выбрать St-Link Debugger
- Зайти в <Settings> вкладка <Flash Download> установить следующее:
    - <Download Function> Erase Full Chip, Programm, Verify
    - <RAM for algoritm> Start=0x20000000, Size=0x00008000



# F.A.Q

При программирование микроконтроллера появляется ошибка Flash памяти
---

1. Или понизить системную частоту до 64 Мгц
2. Или <Options for Target> - <Utilities> - <Settings> на вкладке Flash Download
  - Установить Erase Full Chip
  - Убрать галку с опции <Verify>  