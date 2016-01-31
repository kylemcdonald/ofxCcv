Linux 64 bit binary build instructions (Tested on Ubuntu 14.04 x64).

# 1 - install BLAS (Basic Linear Algebra Subprograms)

```
sudo apt-get install libblas-dev
sudo apt-get install liblapacke-dev
cp /usr/lib/libblas.a ./
```

# 2 - Build CBLAS (the C library)

```
sudo apt-get install gfortran
wget -O cblas.tgz http://www.netlib.org/blas/blast-forum/cblas.tgz
tar -zxvf cblas.tgz
cd CBLAS
rm Makefile.in
cp Makefile.LINUX Makefile.in
cp /usr/lib/libblas.a testing/
make all
cd ..
cp CBLAS/lib/cblas_LINUX.a ./libcblas.a
```

# 3 - Build CCV

```
git clone --depth=1 https://github.com/liuliu/ccv.git
cd ccv/lib
./configure
make
cd ../..
cp ccv/lib/libccv.a ./
```

# 4 - Merge libraries

To avoid linking order issues, we will merge the 3 libraries to one :

```
mkdir tmp
mv *.a tmp/
cd tmp
ar x libblas.a
ar x libcblas.a
ar x libccv.a
rm *.a
ar -qc ../libccv.a *.o
cd ..
```

Final file *libccv.a* can be copied to addon *libs/ccv/lib/linux64* directory.
