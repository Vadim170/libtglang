## Info

Project was developed on mac os, tested on linux x86
Used python 3.9.1

Contest: https://contest.com/docs/ML-Competition-2023

Contest first results:
https://t.me/contest/341

This project line:
```
    | Entry | Result | Time  
----+-------+--------+-------
6.  | 4672  | 74.5%  | 265ms
```


## Build

```
cd src
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Train

1. Run `dataset-prepare.ipynb` to prepare dataset from `raw_data_v2.zip`. In result: 50000+ batches in `dataset_v4o_train` and 5000 batches in `dataset_v4o_validate` 
2. Run `dataset_tg-prepare.ipynb` to prepare dataset from `ml2023-r1-dataset.tar` and `ml2023-d1-dataset.tar`
3. Run `train.ipynb` to train on data `dataset_v4o` batches. And save onnx model
4. Watch at `convert-model.ipynb` to convert onnx model to ort and compile onnxruntime
