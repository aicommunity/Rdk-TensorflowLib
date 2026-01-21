## UTfDetector — детектор TensorFlow

**Класс**: `UTfDetector` (`DetectorTF`) — детекция объектов с помощью TF модели.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("DetectorTF", ...)`.  
**Storage-инстансы**: `ClassName = "DetectorTF"`; параметры: модель, anchors, labels, NMS.

```mermaid
classDiagram
    UTfComponent <|-- UTfDetector
```

### Входы/выходы
- Вход: изображение/тензор.
- Выход: bounding boxes, классы, scores.

```mermaid
flowchart LR
    img[Image/Tensor] --> det[UTfDetector]
    det --> boxes[Boxes/classes]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cfg as Config
    participant Det as UTfDetector
    Cfg->>Det: load model
    loop per frame
        Cfg-->>Det: image/tensor
        Det-->>Cfg: detections
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## UTfDetector — TensorFlow detector

Runs TF detection graph and returns boxes/classes/scores.
