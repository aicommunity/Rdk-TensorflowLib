# Rdk-TensorflowLib - Документация

## RU

### Назначение

**Rdk-TensorflowLib** предоставляет интеграцию с TensorFlow для машинного обучения.

### Краткий обзор

Библиотека включает компоненты для:
- Классификации изображений с использованием TensorFlow
- Детекции объектов
- Работы с TensorFlow сессиями

### Быстрый старт

#### Классификация изображения с TensorFlow

```cpp
// Создание классификатора
auto classifier = storage->CreateComponent<UTfClassifier>("Classifier");
classifier->ModelPath = "model.pb";
classifier->InputImage.AttachTo(&source->OutputImage);
classifier->Build();
classifier->Calculate();
auto classLabel = classifier->ClassLabel();
```

### Связь с корневой документацией

Для обзорной информации см. корневую документацию проекта:
- `Docs/Libraries/Rdk-TensorflowLib.md` - обзор библиотеки (в корневом репозитории)

### Детальная документация

- [Architecture.md](Architecture.md) - архитектура библиотеки
- [Usage-Examples.md](Usage-Examples.md) - примеры использования
- [API-Overview.md](API-Overview.md) - обзор API
- [Component-Catalog.md](Component-Catalog.md) - каталог компонентов

---

## EN

### Purpose

**Rdk-TensorflowLib** provides integration with TensorFlow for machine learning.

### Brief Overview

The library includes components for:
- Image classification using TensorFlow
- Object detection
- TensorFlow session operations

### Quick Start

#### Image Classification with TensorFlow

```cpp
// Create classifier
auto classifier = storage->CreateComponent<UTfClassifier>("Classifier");
classifier->ModelPath = "model.pb";
classifier->InputImage.AttachTo(&source->OutputImage);
classifier->Build();
classifier->Calculate();
auto classLabel = classifier->ClassLabel();
```

### Link to Root Documentation

For overview information see root project documentation:
- `Docs/Libraries/Rdk-TensorflowLib.md` - library overview (in root repository)

### Detailed Documentation

- [Architecture.md](Architecture.md) - library architecture
- [Usage-Examples.md](Usage-Examples.md) - usage examples
- [API-Overview.md](API-Overview.md) - API overview
- [Component-Catalog.md](Component-Catalog.md) - component catalog
