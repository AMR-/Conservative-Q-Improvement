from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

file_list = ["qtree_wrapper.pyx", "../discrete.cpp", "../box.cpp", "../leafsplit.cpp", "../qtreeleaf.cpp", "../qtreeinternal.cpp", "../qtree.cpp", "../state.cpp", "../action.cpp"]

setup(
	ext_modules=[Extension("qtree_wrapper", file_list, language="c++")],
	cmdclass = {'build_ext': build_ext}
)
