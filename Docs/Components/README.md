# Компоненты Rdk-TensorflowLib

## RU

### Обзор

Библиотека Rdk-TensorflowLib предоставляет компоненты для работы с TensorFlow моделями в рамках компонентной системы RDK. Все компоненты наследуются от базового класса `UTfComponent` и реализуют различные задачи машинного обучения: классификацию изображений и детекцию объектов.

### Структура компонентов

```
UTfComponent (базовый класс)
├── UTfClassifier (классификатор)
├── UTfDetector (детектор объектов)
└── UTfSqDetector (детектор SqueezeDet)
```

### Компоненты

#### Базовый компонент

- **[UTfComponent](UTfComponent.md)** — базовый абстрактный компонент для работы с TensorFlow моделями
  - Загрузка замороженных моделей (`.pb` файлы)
  - Настройка параметров нормализации изображений
  - Управление TensorFlow сессией
  - Обработка входных данных и выполнение инференса

#### Классификаторы

- **[UTfClassifier](UTfClassifier.md)** (`ClassifierTF`) — классификатор изображений
  - Классификация одного изображения или пакетная классификация
  - Возвращает метки классов и оценки уверенности
  - Поддержка сохранения результатов для отладки

#### Детекторы

- **[UTfDetector](UTfDetector.md)** (`DetectorTF`) — детектор объектов
  - Детекция объектов на изображениях (SSD, Faster R-CNN и т.д.)
  - Возвращает координаты bounding boxes, классы и оценки уверенности
  - Поддержка отладочного изображения с визуализацией результатов

- **[UTfSqDetector](UTfSqDetector.md)** (`DetectorTFSq`) — детектор SqueezeDet
  - Специализированный детектор на основе архитектуры SqueezeDet
  - Автоматическая постобработка результатов (anchor boxes, NMS)
  - Требует конфигурационный файл JSON
  - Автоматическое масштабирование координат к размеру входного изображения

### Документация компонентов

Каждый компонент имеет расширенную документацию с:

- **UML-диаграмма классов** — полная иерархия наследования, свойства, методы
- **UML-диаграмма последовательности** — жизненный цикл объекта
- **UML-диаграмма состояний** — состояния компонента и переходы
- **UML-диаграмма активности** — алгоритмы работы основных методов
- **UML-диаграмма компонентов** — зависимости от других компонентов и библиотек
- **Детальное описание свойств** — все параметры, входы, выходы, состояния
- **Описание методов** — параметры и возвращаемые значения
- **Примеры использования** — примеры кода C++ и конфигурации XML

### Быстрый старт

#### Классификация изображений

```cpp
auto classifier = storage->CreateComponent<UTfClassifier>();
classifier->Default();
classifier->PbModelPath = "models/inception_v3.pb";
classifier->InputNodeName = "input";
classifier->OutputNodeName = {"output"};
classifier->NumClasses = 1000;
classifier->Build();

classifier->InputImage = inputImage;
classifier->Calculate();
int classId = classifier->OutputClasses(0, 0);
```

#### Детекция объектов

```cpp
auto detector = storage->CreateComponent<UTfDetector>();
detector->Default();
detector->PbModelPath = "models/ssd_mobilenet_v2.pb";
detector->InputNodeName = "image_tensor";
detector->OutputNodeName = {"detection_boxes", "detection_classes", 
                            "detection_scores", "num_detections"};
detector->Build();

detector->InputImage = inputImage;
detector->Calculate();
int numDetections = detector->OutputRects->GetRows();
```

### См. также

- [Component-Catalog.md](../Component-Catalog.md) — каталог компонентов
- [Architecture.md](../Architecture.md) — архитектура библиотеки
- [API-Overview.md](../API-Overview.md) — обзор API
- [Usage-Examples.md](../Usage-Examples.md) — примеры использования

---

## EN

### Overview

Rdk-TensorflowLib provides components for working with TensorFlow models within the RDK component system. All components inherit from the base class `UTfComponent` and implement various machine learning tasks: image classification and object detection.

### Component Structure

```
UTfComponent (base class)
├── UTfClassifier (classifier)
├── UTfDetector (object detector)
└── UTfSqDetector (SqueezeDet detector)
```

### Components

#### Base Component

- **[UTfComponent](UTfComponent.md)** — base abstract component for working with TensorFlow models
  - Loading frozen models (`.pb` files)
  - Image normalization parameters setup
  - TensorFlow session management
  - Input data processing and inference execution

#### Classifiers

- **[UTfClassifier](UTfClassifier.md)** (`ClassifierTF`) — image classifier
  - Single image or batch classification
  - Returns class labels and confidence scores
  - Support for saving results for debugging

#### Detectors

- **[UTfDetector](UTfDetector.md)** (`DetectorTF`) — object detector
  - Object detection on images (SSD, Faster R-CNN, etc.)
  - Returns bounding box coordinates, classes, and confidence scores
  - Support for debug image with visualization

- **[UTfSqDetector](UTfSqDetector.md)** (`DetectorTFSq`) — SqueezeDet detector
  - Specialized detector based on SqueezeDet architecture
  - Automatic post-processing (anchor boxes, NMS)
  - Requires JSON configuration file
  - Automatic coordinate scaling to input image size

### Component Documentation

Each component has extended documentation with:

- **UML Class Diagram** — full inheritance hierarchy, properties, methods
- **UML Sequence Diagram** — object lifecycle
- **UML State Diagram** — component states and transitions
- **UML Activity Diagram** — algorithms of main methods
- **UML Component Diagram** — dependencies on other components and libraries
- **Detailed Property Description** — all parameters, inputs, outputs, states
- **Method Description** — parameters and return values
- **Usage Examples** — C++ code examples and XML configuration

### Quick Start

#### Image Classification

```cpp
auto classifier = storage->CreateComponent<UTfClassifier>();
classifier->Default();
classifier->PbModelPath = "models/inception_v3.pb";
classifier->InputNodeName = "input";
classifier->OutputNodeName = {"output"};
classifier->NumClasses = 1000;
classifier->Build();

classifier->InputImage = inputImage;
classifier->Calculate();
int classId = classifier->OutputClasses(0, 0);
```

#### Object Detection

```cpp
auto detector = storage->CreateComponent<UTfDetector>();
detector->Default();
detector->PbModelPath = "models/ssd_mobilenet_v2.pb";
detector->InputNodeName = "image_tensor";
detector->OutputNodeName = {"detection_boxes", "detection_classes", 
                            "detection_scores", "num_detections"};
detector->Build();

detector->InputImage = inputImage;
detector->Calculate();
int numDetections = detector->OutputRects->GetRows();
```

### See Also

- [Component-Catalog.md](../Component-Catalog.md) — component catalog
- [Architecture.md](../Architecture.md) — library architecture
- [API-Overview.md](../API-Overview.md) — API overview
- [Usage-Examples.md](../Usage-Examples.md) — usage examples
