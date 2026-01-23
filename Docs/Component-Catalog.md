# Rdk-TensorflowLib — Component Catalog

Компоненты из `Core/Lib.cpp` (условная компиляция `RDK_USE_TENSORFLOW`) с кратким назначением.

## Базовые компоненты

- **UTfComponent** / (базовый класс) — базовый абстрактный компонент для работы с TensorFlow моделями. Предоставляет общую функциональность: загрузку моделей, настройку параметров нормализации изображений, управление TensorFlow сессией. Не регистрируется напрямую, используется как базовый класс для всех TensorFlow компонентов. См. детальную документацию: [`UTfComponent`](Components/UTfComponent.md).

## Classifiers

- **UTfClassifier** / `ClassifierTF` — классификатор изображений с использованием TensorFlow моделей. Загружает замороженную модель (`.pb` файл), выполняет инференс на входных изображениях и возвращает метки классов с оценками уверенности. Поддерживает классификацию одного изображения или пакетную классификацию. Вход: `InputImage` (UBitmap) или `InputImages` (vector<UBitmap>); выход: `OutputClasses` (MDMatrix<int>), `OutputConfidences` (MDMatrix<double>). См. детальную документацию: [`UTfClassifier`](Components/UTfClassifier.md).

## Detectors

- **UTfDetector** / `DetectorTF` — детектор объектов с использованием TensorFlow моделей детекции (SSD, Faster R-CNN и т.д.). Загружает замороженную модель, выполняет инференс и возвращает координаты ограничивающих прямоугольников (bounding boxes), классы объектов и оценки уверенности. Вход: `InputImage` (UBitmap); выход: `OutputRects` (MDMatrix<double>), `OutputClasses` (MDMatrix<int>), `OutputReliability` (MDMatrix<double>), `OutputObjects` (MDMatrix<double>). См. детальную документацию: [`UTfDetector`](Components/UTfDetector.md).

- **UTfSqDetector** / `DetectorTFSq` — детектор объектов на основе архитектуры SqueezeDet в TensorFlow. Использует специализированную сессию `TTfSessionSqDet` с автоматической постобработкой результатов (расчет anchor boxes, NMS) и требует конфигурационный файл JSON с параметрами модели. Координаты детекций автоматически масштабируются к размеру входного изображения. Вход: `InputImage` (UBitmap); выход: `OutputRects` (MDMatrix<double>), `OutputClasses` (MDMatrix<int>), `OutputReliability` (MDMatrix<double>), `OutputObjects` (MDMatrix<double>). См. детальную документацию: [`UTfSqDetector`](Components/UTfSqDetector.md).

## Детальная документация

Все компоненты имеют расширенную документацию с UML-диаграммами:

- **[UTfComponent](Components/UTfComponent.md)** — базовый компонент TensorFlow
  - UML-диаграмма классов (иерархия наследования, свойства, методы)
  - UML-диаграмма последовательности (жизненный цикл)
  - UML-диаграмма состояний (состояния компонента)
  - UML-диаграмма активности (алгоритмы основных методов)
  - UML-диаграмма компонентов (зависимости)
  - Детальное описание свойств и методов
  - Примеры использования

- **[UTfClassifier](Components/UTfClassifier.md)** — классификатор TensorFlow
  - Полная документация с UML-диаграммами
  - Описание свойств от `UClassifierBase` и `UTfComponent`
  - Алгоритм классификации
  - Примеры использования и конфигурации XML

- **[UTfDetector](Components/UTfDetector.md)** — детектор объектов TensorFlow
  - Полная документация с UML-диаграммами
  - Описание свойств от `UDetectorBase` и `UTfComponent`
  - Алгоритм детекции
  - Примеры использования и конфигурации XML

- **[UTfSqDetector](Components/UTfSqDetector.md)** — детектор SqueezeDet TensorFlow
  - Полная документация с UML-диаграммами
  - Описание конфигурационного файла JSON
  - Отличия от `UTfDetector`
  - Алгоритм детекции с постобработкой
  - Примеры использования и конфигурации XML

## Общие возможности

Все TensorFlow компоненты поддерживают:

- **Загрузку моделей**: Замороженные модели TensorFlow (`.pb` файлы)
- **Нормализацию изображений**: Настройка параметров `ImgDiv`, `ImgSub`, `UseBGR`
- **GPU ускорение**: Управление долей памяти GPU через `GpuFraction`
- **Обработку ошибок**: Информация об ошибках в `DebugString`
- **Автоматическое определение размеров**: `ExpectedHeight`, `ExpectedWidth`, `ExpectedChannels` устанавливаются автоматически после загрузки модели

## См. также

- [Architecture.md](Architecture.md) — архитектура библиотеки
- [API-Overview.md](API-Overview.md) — обзор API
- [Usage-Examples.md](Usage-Examples.md) — примеры использования
- [Components/TfComponents.md](Components/TfComponents.md) — обзор компонентов
