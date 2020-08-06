import pathlib
from setuptools import setup
from setuptools.extension import Extension
from Cython.Build import cythonize
# from cysetuptools import setup

# The directory containing this file
HERE = pathlib.Path(__file__).parent

# The text of the README file
README = (HERE / "Readme.md").read_text()

extensions = [
    Extension(
        "cqi_cpp.src.wrapper.qtree_wrapper",
        ["cqi_cpp/src/wrapper/qtree_wrapper.pyx"],
    )
]

setup(
    name="cqi_rl",
    version="0.0.3",
    description="A C++ implementation of Conservative Q-Improvement",
    # long_description=README,
    long_description="TODO",
    long_description_content_type="text/markdown",
    url="https://github.com/AMR-/Conservative-Q-Improvement",
    author="Aaron M. Roth, Nicholay Topin, Saadiq K. Shaik",
    author_email="amroth@umd.edu",
    license="MIT",
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        # "Programming Language :: Python :: 3.5",
        # "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "Topic :: Scientific/Engineering",
        "Topic :: Scientific/Engineering :: Artificial Intelligence",
        "Topic :: Software Development :: Libraries :: Python Modules"
    ],
    packages=["cqi_python"],
    include_package_data=True,
    install_requires=["cython", "ray"],
    # entry_points={
    # },
    # ext_modules=cythonize('cqi_cpp/src/wrapper/qtree_wrapper.pyx'),
    ext_modules=cythonize(extensions),
    zip_safe=False,
)
