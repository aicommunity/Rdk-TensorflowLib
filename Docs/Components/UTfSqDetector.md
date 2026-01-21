## UTfSqDetector — SqueezeDet на TF

**Класс**: `UTfSqDetector` (`DetectorTFSq`) — детектор на основе SqueezeDet в TF.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("DetectorTFSq", ...)`.  
**Storage-инстансы**: `ClassName = "DetectorTFSq"`; параметры: модель, anchors, препро/постпроцессинг.

```mermaid
classDiagram
    UTfComponent <|-- UTfSqDetector
```

### Входы/выходы
- Вход: изображение/тензор.
- Выход: детекции (boxes/classes/scores).

```mermaid
flowchart LR
    img[Image] --> det[UTfSqDetector]
    det --> boxes[Boxes/classes]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cfg as Config
    participant Det as UTfSqDetector
    Cfg->>Det: load SqueezeDet model
    loop per frame
        Cfg-->>Det: image
        Det-->>Cfg: detections
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## UTfSqDetector — TensorFlow SqueezeDet

Runs SqueezeDet TensorFlow model to produce detections.
