## UTfClassifier — классификатор TensorFlow

**Класс**: `UTfClassifier` (`ClassifierTF`) — инференс классификации с использованием TF-модели.  
**Регистрация**: `Core/Lib.cpp` → `UploadClass("ClassifierTF", ...)`.  
**Storage-инстансы**: `ClassName = "ClassifierTF"`; параметры: путь к модели, labels, препроцессинг.

```mermaid
classDiagram
    UComponent <|-- UTfComponent
    UTfComponent <|-- UTfClassifier
    class UTfClassifier {
        +modelPath : string
        +labels : list
    }
```

### Входы/выходы
- Вход: изображение/тензор.
- Выход: класс/скор.

```mermaid
flowchart LR
    img[Image/Tensor] --> clf[UTfClassifier]
    clf --> label[Class/score]
```

Пояснение: блок-схема показывает поток данных/сигналов (входы → компонент → выходы).

```mermaid
sequenceDiagram
    participant Cfg as Config
    participant Clf as UTfClassifier
    Cfg->>Clf: load model
    loop per request
        Cfg-->>Clf: tensor
        Clf-->>Cfg: class/score
    end
```

Пояснение: диаграмма последовательности показывает типовой сценарий взаимодействия и порядок вызовов.

---

## UTfClassifier — TensorFlow classifier

Loads TF model, runs classification, returns label/score.
