# Rdk-TensorflowLib — Component Catalog

Компоненты из `Core/Lib.cpp` (условная компиляция `RDK_USE_TENSORFLOW`) с кратким назначением.

## Classifiers
- **UTfClassifier** / `ClassifierTF` — классификация на TensorFlow модели; вход: тензор/изображение; выход: метки/скор.

## Detectors
- **UTfDetector** / `DetectorTF` — детекция объектов; вход: изображение/тензор; выход: bounding boxes/классы.
- **UTfSqDetector** / `DetectorTFSq` — детекция (SqueezeDet); вход: изображение/тензор; выход: детекции.

**Приоритет описания:** загрузка TF-моделей, подготовка тензоров, маппинг выходов в свойства, минимальные примеры пайплайна. См. `Docs/Components/TfComponents.md` и детальные файлы: [`UTfClassifier`](Components/UTfClassifier.md), [`UTfDetector`](Components/UTfDetector.md), [`UTfSqDetector`](Components/UTfSqDetector.md).
