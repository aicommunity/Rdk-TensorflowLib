# Примеры использования Rdk-TensorflowLib

## RU

### Пример 1: Классификация изображений

```cpp
// Создание классификатора TensorFlow
auto classifier = storage->CreateComponent<UTfClassifier>();
classifier->ModelPath = "model.pb";
classifier->InputLayerName = "input";
classifier->OutputLayerName = "output";
classifier->Build();

// Классификация
classifier->InputImage = inputImage;
classifier->Calculate();
auto classLabel = classifier->ClassLabel;
auto confidence = classifier->Confidence;
```

### Пример 2: Детекция объектов

```cpp
// Создание детектора TensorFlow
auto detector = storage->CreateComponent<UTfDetector>();
detector->ModelPath = "detector.pb";
detector->Build();

// Детекция
detector->InputImage = inputImage;
detector->Calculate();
auto objects = detector->DetectedObjects;
```

---

## EN

### Example 1: Image Classification

```cpp
// Creating TensorFlow classifier
auto classifier = storage->CreateComponent<UTfClassifier>();
classifier->ModelPath = "model.pb";
classifier->InputLayerName = "input";
classifier->OutputLayerName = "output";
classifier->Build();

// Classification
classifier->InputImage = inputImage;
classifier->Calculate();
auto classLabel = classifier->ClassLabel;
auto confidence = classifier->Confidence;
```

### Example 2: Object Detection

```cpp
// Creating TensorFlow detector
auto detector = storage->CreateComponent<UTfDetector>();
detector->ModelPath = "detector.pb";
detector->Build();

// Detection
detector->InputImage = inputImage;
detector->Calculate();
auto objects = detector->DetectedObjects;
```
