# Обзор API Rdk-TensorflowLib

## RU

### Основные классы

#### TensorflowLib

Главный класс библиотеки, наследник `ULibrary`.

#### UTfComponent

Базовый компонент TensorFlow.

**Основные свойства:**
- `ModelPath` - путь к модели
- `Session` - сессия TensorFlow

#### UTfClassifier

Классификатор TensorFlow.

**Основные свойства:**
- `ModelPath` - путь к модели
- `InputLayerName` - имя входного слоя
- `OutputLayerName` - имя выходного слоя
- `InputImage` - входное изображение
- `ClassLabel` - метка класса
- `Confidence` - уверенность

#### UTfDetector

Детектор объектов TensorFlow.

**Основные свойства:**
- `ModelPath` - путь к модели
- `InputImage` - входное изображение
- `DetectedObjects` - обнаруженные объекты

### См. также

- [Reports/08-ML-Libraries.md](../../../Reports/08-ML-Libraries.md) - детальное описание ML библиотек
- Исходный код: `Libraries/Rdk-TensorflowLib/Core/`

---

## EN

### Main Classes

#### TensorflowLib

Main library class, inherits from `ULibrary`.

#### UTfComponent

Base TensorFlow component.

**Main Properties:**
- `ModelPath` - model path
- `Session` - TensorFlow session

#### UTfClassifier

TensorFlow classifier.

**Main Properties:**
- `ModelPath` - model path
- `InputLayerName` - input layer name
- `OutputLayerName` - output layer name
- `InputImage` - input image
- `ClassLabel` - class label
- `Confidence` - confidence

#### UTfDetector

TensorFlow object detector.

**Main Properties:**
- `ModelPath` - model path
- `InputImage` - input image
- `DetectedObjects` - detected objects

### See Also

- [Reports/08-ML-Libraries.md](../../../Reports/08-ML-Libraries.md) - detailed ML libraries description
- Source code: `Libraries/Rdk-TensorflowLib/Core/`
