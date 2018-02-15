# Using pyImageStreamIO to read/write python data from/into CACAO SHM

- [CACAO installation](#cacao-installation)
- [Anaconda installation](#anaconda-installation)
- [PyImageStreamIO + pyBind11 library](#pyimagestreamio-pybind11-library)
- [Summary environment variables](#summary-environment-variables)
- [usage](#usage)

## CACAO installation

source: https://github.com/CACAO-org/CACAO

```bash
cd $HOME
git clone --recursive https://github.com/cacao-org/cacao cacao
cd cacao
autoreconf -vif
./configure
make
make install
export CACAO_ROOT=$HOME/cacao
```

## Anaconda installation

For an easier maintenance, I recommend the use of anaconda.
You can also use virtualenv to have a python sandbox but I can't help you.

```bash
cd $HOME
export CONDA_ROOT=$HOME/miniconda3
wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh -b -p $CONDA_ROOT
export PATH=$CONDA_ROOT/bin:$PATH
```

## PyImageStreamIO + pyBind11 library

```bash
cd $HOME
git clone --recursive https://github.com/pybind/pybind11.git
export PYBIND11_ROOT=$HOME/pybind11
git clone https://github.com/milk-org/pyImageStreamIO.git
make
make install_conda
```

## Summary environment variables

In your .bashrc, you should have defined:

```bash
export CACAO_ROOT=$HOME/cacao
export PYBIND11_ROOT=$HOME/pybind11
export CONDA_ROOT=$HOME/miniconda3
export PATH=$CONDA_ROOT/bin:$PATH
```

## usage

```python
import numpy as np
from pyImageStreamIO import Image, Datatype
img = Image()
img.create("test", [512, 512], Datatype.FLOAT, 1, 1)
img.write(np.ones((512,512), dtype=np.float32))

img2 = Image()
img2.link("test")
img2.semwait(0)
data = np.array(img2, copy=False)
```

note: the keyword ``copy=False`` avoid the copy operation