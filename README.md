- [pyImageStreamIO](#pyimagestreamio)
    - [dependency pyBind11](#dependency-pybind11)
    - [usage](#usage)

# pyImageStreamIO

## dependency pyBind11

## usage 

```python
import numpy as np
from pyImageStreamIO import Image
img = Image()
img.create("test", [512, 512], pyImageStreamIO.Datatype.FLOAT, 1, 1)  
img.write(np.ones((512,512), dtype=np.float32))                       
img.semwait(0)
img.read("test")
```