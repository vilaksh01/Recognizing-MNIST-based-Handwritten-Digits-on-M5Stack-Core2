# How to integrate Neuton into your firmware project 

## Include header file

Copy all files from this archive to your project and include header file:
``` C
#include "neuton.h"
```

The library contains functions to get model information such as:
* task type (regression, classification, etc.);
* neurons and weights count;
* window buffer size;
* input and output features count;
* model size and RAM usage;
* float support flag;
* quantization level.

Main functions are:
* `neuton_model_set_inputs` - to set input values;
* `neuton_model_run_inference` - to make predictions.


## Set input values

Make a float array with model inputs. Inputs count and order should be the same as in the training dataset.

``` C
float inputs[] = {
    feature_0,
    feature_1,
    // ...
    feature_N
};
```

Pass this array to `neuton_model_set_inputs` function. 

If digital signal processing option was selected on platform you should call `neuton_model_set_inputs` multiple times for each sample to fill internal window buffer. Function will return `0` when buffer is full, this indicates that model is ready for prediction.


##	Make prediction

When buffer is ready you should call `neuton_model_run_inference` with two arguments:
* pointer to `index` of predicted class;
* pointer to neural net `outputs` (dimension of array can be read using `neuton_model_outputs_count` function).

For regression task output value will be stored at `outputs[0]`.
For classifications task `index` will contain class index with maximal probability, `outputs` will contains probabilities of each class. Thus, you can get predicted class probability at `outputs[index]`.

Function will return `0` on successful prediction.
``` C
if (neuton_model_set_inputs(inputs) == 0)
{
    uint16_t index;
    float* outputs;
    
    if (neuton_model_run_inference(&index, &outputs) == 0)
    {
        // code for handling prediction result
    }
}
```


