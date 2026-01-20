# Архитектура Rdk-TensorflowLib

## RU

### Обзор

Rdk-TensorflowLib предоставляет компонентный интерфейс для работы с TensorFlow.

### Структура библиотеки

```mermaid
flowchart TB
    UTfComponent["UTfComponent (Базовый_компонент)"]
    UTfClassifier["UTfClassifier (Классификатор)"]
    UTfDetector["UTfDetector (Детектор)"]
    UTfSqDetector["UTfSqDetector (SqueezeDet)"]
    TfSession[TensorFlow Session]
    
    UTfComponent --> UTfClassifier
    UTfComponent --> UTfDetector
    UTfComponent --> UTfSqDetector
    UTfClassifier --> TfSession
    UTfDetector --> TfSession
```

### Основные модули

#### Базовый компонент

- **UTfComponent** - базовый компонент для работы с TensorFlow

#### Классификация

- **UTfClassifier** - классификатор изображений на базе TensorFlow

#### Детекция

- **UTfDetector** - детектор объектов на базе TensorFlow
- **UTfSqDetector** - детектор SqueezeDet

#### Сессии TensorFlow

- **ttfsession** - управление сессиями TensorFlow

### Зависимости

- `rdk.static.qt` - ядро Rdk
- TensorFlow - библиотека TensorFlow

### См. также

- [Usage-Examples.md](Usage-Examples.md) - примеры использования
- [API-Overview.md](API-Overview.md) - обзор API

---

## EN

### Overview

Rdk-TensorflowLib provides a component interface for working with TensorFlow.

### Library Structure

```mermaid
flowchart TB
    UTfComponent_EN["UTfComponent (Base)"]
    UTfClassifier_EN["UTfClassifier (Classifier)"]
    UTfDetector_EN["UTfDetector (Detector)"]
    UTfSqDetector_EN["UTfSqDetector (SqueezeDet)"]
    TfSession_EN["TensorFlow Session"]
    
    UTfComponent_EN --> UTfClassifier_EN
    UTfComponent_EN --> UTfDetector_EN
    UTfComponent_EN --> UTfSqDetector_EN
    UTfClassifier_EN --> TfSession_EN
    UTfDetector_EN --> TfSession_EN
```

The library wraps TensorFlow runtime into Rdk components. A typical flow is: load model → prepare input tensors → run session → map output tensors into component output properties.

### Main Modules

#### Base Component

- **UTfComponent** - base component for TensorFlow operations

#### Classification

- **UTfClassifier** - TensorFlow-based image classifier

#### Detection

- **UTfDetector** - TensorFlow-based object detector
- **UTfSqDetector** - SqueezeDet detector

#### TensorFlow Sessions

- **ttfsession** - TensorFlow session management

### Dependencies

- `rdk.static.qt` - Rdk core
- TensorFlow - TensorFlow library

### See Also

- [Usage-Examples.md](Usage-Examples.md) - usage examples
- [API-Overview.md](API-Overview.md) - API overview
