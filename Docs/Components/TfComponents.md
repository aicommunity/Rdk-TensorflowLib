# TensorFlow Components — обзор (Rdk-TensorflowLib)

## RU

### Обзор компонентов

Библиотека Rdk-TensorflowLib предоставляет три основных компонента для работы с TensorFlow моделями:

1. **UTfClassifier** — классификатор изображений
2. **UTfDetector** — детектор объектов
3. **UTfSqDetector** — детектор SqueezeDet

Все компоненты наследуются от базового класса **UTfComponent**, который предоставляет общую функциональность для работы с TensorFlow.

### Сравнительная таблица компонентов

| Компонент | Назначение | Вход | Выход | Особенности |
|-----------|------------|------|-------|-------------|
| **UTfClassifier** | Классификация изображений | `InputImage` (UBitmap) или `InputImages` (vector<UBitmap>) | `OutputClasses` (MDMatrix<int>), `OutputConfidences` (MDMatrix<double>) | Поддержка пакетной классификации, сохранение результатов для отладки |
| **UTfDetector** | Детекция объектов | `InputImage` (UBitmap) | `OutputRects` (MDMatrix<double>), `OutputClasses` (MDMatrix<int>), `OutputReliability` (MDMatrix<double>) | Стандартные модели детекции (SSD, Faster R-CNN), поддержка отладочного изображения |
| **UTfSqDetector** | Детекция объектов (SqueezeDet) | `InputImage` (UBitmap) | `OutputRects` (MDMatrix<double>), `OutputClasses` (MDMatrix<int>), `OutputReliability` (MDMatrix<double>) | Специализированная архитектура, автоматическая постобработка, требует конфигурационный файл JSON, автоматическое масштабирование координат |

### Общие свойства (от UTfComponent)

Все компоненты наследуют следующие свойства:

**Параметры модели:**
- `PbModelPath` (string) — путь к замороженной модели TensorFlow (`.pb` файл)
- `InputNodeName` (string) — имя входного узла графа
- `OutputNodeName` (vector<string>) — вектор имен выходных узлов графа

**Параметры нормализации:**
- `ImgDiv` (float) — делитель для нормализации (по умолчанию: 255.0)
- `ImgSub` (vector<float>) — вектор значений для вычитания (по умолчанию: {0.0, 0.0, 0.0})
- `UseBGR` (bool) — флаг использования формата BGR (по умолчанию: false)

**Параметры GPU:**
- `GpuFraction` (double) — доля памяти GPU для использования (по умолчанию: 0.8)

**Состояния:**
- `DebugString` (string) — информация об ошибках
- `ExpectedHeight`, `ExpectedWidth`, `ExpectedChannels` (int) — ожидаемые размеры изображения (устанавливаются автоматически)

### Типичные сценарии использования

#### Классификация изображений (UTfClassifier)

```cpp
// Создание и настройка классификатора
auto classifier = storage->CreateComponent<UTfClassifier>();
classifier->Default();
classifier->PbModelPath = "models/inception_v3.pb";
classifier->InputNodeName = "input";
classifier->OutputNodeName = {"output"};
classifier->NumClasses = 1000;
classifier->ConfidenceThreshold = 0.5;
classifier->Build();

// Классификация
classifier->InputImage = inputImage;
classifier->Calculate();
int classId = classifier->OutputClasses(0, 0);
double confidence = classifier->OutputConfidences(0, classId);
```

**Применение:**
- Распознавание объектов на изображениях
- Классификация сцен
- Медицинская диагностика по изображениям

#### Детекция объектов (UTfDetector)

```cpp
// Создание и настройка детектора
auto detector = storage->CreateComponent<UTfDetector>();
detector->Default();
detector->PbModelPath = "models/ssd_mobilenet_v2.pb";
detector->InputNodeName = "image_tensor";
detector->OutputNodeName = {"detection_boxes", "detection_classes", 
                            "detection_scores", "num_detections"};
detector->ConfidenceThreshold = 0.5;
detector->NMSthreshold = 0.45;
detector->Build();

// Детекция
detector->InputImage = inputImage;
detector->Calculate();
int numDetections = detector->OutputRects->GetRows();
```

**Применение:**
- Детекция людей и транспортных средств
- Системы видеонаблюдения
- Детекция товаров на полках

#### Детекция SqueezeDet (UTfSqDetector)

```cpp
// Создание и настройка детектора SqueezeDet
auto detector = storage->CreateComponent<UTfSqDetector>();
detector->Default();
detector->PbModelPath = "models/squeezedet.pb";
detector->ConfigPath = "configs/squeezedet_config.json";
detector->InputNodeName = "input";
detector->OutputNodeName = {"output"};
detector->Build();

// Детекция
detector->InputImage = inputImage;
detector->Calculate();
```

**Применение:**
- Компактные модели для мобильных устройств
- Real-time детекция объектов
- Встраиваемые системы

### Выбор компонента

**Используйте UTfClassifier, если:**
- Нужна классификация всего изображения (один класс на изображение)
- Работаете с моделями типа Inception, ResNet, MobileNet
- Нужна пакетная обработка изображений

**Используйте UTfDetector, если:**
- Нужна детекция нескольких объектов на изображении
- Работаете со стандартными моделями TensorFlow Object Detection API (SSD, Faster R-CNN)
- Нужны координаты bounding boxes объектов

**Используйте UTfSqDetector, если:**
- Нужна компактная модель для мобильных устройств
- Работаете с архитектурой SqueezeDet
- Нужна автоматическая постобработка и масштабирование координат

### Детальная документация

Для получения подробной информации о каждом компоненте см.:

- **[UTfComponent](UTfComponent.md)** — базовый компонент TensorFlow
- **[UTfClassifier](UTfClassifier.md)** — классификатор изображений
- **[UTfDetector](UTfDetector.md)** — детектор объектов
- **[UTfSqDetector](UTfSqDetector.md)** — детектор SqueezeDet

Каждый компонент имеет полную документацию с UML-диаграммами, описанием свойств и методов, примерами использования.

### См. также

- [Component-Catalog.md](../Component-Catalog.md) — каталог компонентов
- [Architecture.md](../Architecture.md) — архитектура библиотеки
- [API-Overview.md](../API-Overview.md) — обзор API
- [Usage-Examples.md](../Usage-Examples.md) — примеры использования

---

## EN

### Component Overview

Rdk-TensorflowLib provides three main components for working with TensorFlow models:

1. **UTfClassifier** — image classifier
2. **UTfDetector** — object detector
3. **UTfSqDetector** — SqueezeDet detector

All components inherit from the base class **UTfComponent**, which provides common functionality for working with TensorFlow.

### Component Comparison

| Component | Purpose | Input | Output | Features |
|-----------|---------|-------|--------|----------|
| **UTfClassifier** | Image classification | `InputImage` (UBitmap) or `InputImages` (vector<UBitmap>) | `OutputClasses` (MDMatrix<int>), `OutputConfidences` (MDMatrix<double>) | Batch classification support, debug result saving |
| **UTfDetector** | Object detection | `InputImage` (UBitmap) | `OutputRects` (MDMatrix<double>), `OutputClasses` (MDMatrix<int>), `OutputReliability` (MDMatrix<double>) | Standard detection models (SSD, Faster R-CNN), debug image support |
| **UTfSqDetector** | Object detection (SqueezeDet) | `InputImage` (UBitmap) | `OutputRects` (MDMatrix<double>), `OutputClasses` (MDMatrix<int>), `OutputReliability` (MDMatrix<double>) | Specialized architecture, automatic post-processing, requires JSON config file, automatic coordinate scaling |

### Common Properties (from UTfComponent)

All components inherit the following properties:

**Model Parameters:**
- `PbModelPath` — path to frozen TensorFlow model (`.pb` file)
- `InputNodeName` — input node name in graph
- `OutputNodeName` — vector of output node names

**Normalization Parameters:**
- `ImgDiv` — divisor for normalization (default: 255.0)
- `ImgSub` — vector of subtract values (default: {0.0, 0.0, 0.0})
- `UseBGR` — flag to use BGR format (default: false)

**GPU Parameters:**
- `GpuFraction` — fraction of GPU memory to use (default: 0.8)

**States:**
- `DebugString` — error information
- `ExpectedHeight`, `ExpectedWidth`, `ExpectedChannels` — expected image dimensions (set automatically)

### Typical Usage Scenarios

#### Image Classification (UTfClassifier)

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
```

#### Object Detection (UTfDetector)

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
```

#### SqueezeDet Detection (UTfSqDetector)

```cpp
auto detector = storage->CreateComponent<UTfSqDetector>();
detector->Default();
detector->PbModelPath = "models/squeezedet.pb";
detector->ConfigPath = "configs/squeezedet_config.json";
detector->Build();

detector->InputImage = inputImage;
detector->Calculate();
```

### Component Selection

**Use UTfClassifier if:**
- You need to classify entire images (one class per image)
- Working with models like Inception, ResNet, MobileNet
- Need batch image processing

**Use UTfDetector if:**
- You need to detect multiple objects on an image
- Working with standard TensorFlow Object Detection API models (SSD, Faster R-CNN)
- Need bounding box coordinates

**Use UTfSqDetector if:**
- You need a compact model for mobile devices
- Working with SqueezeDet architecture
- Need automatic post-processing and coordinate scaling

### Detailed Documentation

For detailed information about each component, see:

- **[UTfComponent](UTfComponent.md)** — base TensorFlow component
- **[UTfClassifier](UTfClassifier.md)** — image classifier
- **[UTfDetector](UTfDetector.md)** — object detector
- **[UTfSqDetector](UTfSqDetector.md)** — SqueezeDet detector

Each component has complete documentation with UML diagrams, property and method descriptions, and usage examples.

### See Also

- [Component-Catalog.md](../Component-Catalog.md) — component catalog
- [Architecture.md](../Architecture.md) — library architecture
- [API-Overview.md](../API-Overview.md) — API overview
- [Usage-Examples.md](../Usage-Examples.md) — usage examples
