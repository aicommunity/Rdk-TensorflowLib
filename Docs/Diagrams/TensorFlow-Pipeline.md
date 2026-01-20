# Пайплайн TensorFlow

## RU

### Интеграция TensorFlow с движком

```mermaid
sequenceDiagram
    participant Component as UTfComponent
    participant Session as TensorFlow Session
    participant Model as TensorFlow Model
    participant Engine as Rdk Engine
    
    Component->>Session: LoadModel(model.pb)
    Component->>Component: PrepareInput()
    Component->>Session: Run(input_tensor)
    Session->>Model: Inference
    Model-->>Session: Output tensor
    Session-->>Component: Result
    Component->>Engine: UpdateOutput()
```

### Архитектура компонентов TensorFlow

```mermaid
classDiagram
    class UTfComponent {
        +ModelPath
        +Session
        +LoadModel()
        +Run()
    }
    
    class UTfClassifier {
        +InputLayerName
        +OutputLayerName
        +ClassLabel
        +Confidence
    }
    
    class UTfDetector {
        +DetectedObjects
        +ConfidenceThreshold
    }
    
    UTfComponent <|-- UTfClassifier
    UTfComponent <|-- UTfDetector
```

---

## EN

### TensorFlow Integration with Engine

### TensorFlow Components Architecture
