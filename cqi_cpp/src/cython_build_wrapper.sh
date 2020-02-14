rm *so
rm -r build
python setup.py build_ext --inplace
python test.py
