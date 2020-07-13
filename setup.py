import pathlib
from setuptools import setup
# from cysetuptools import setup

# The directory containing this file
HERE = pathlib.Path(__file__).parent

# The text of the README file
README = (HERE / "Readme.md").read_text()

# This call to setup() does all the work
setup(
    name="cqi-rl",
    version="0.0.1",
    description="A C++ implementation of Conservative Q-Improvement",
    # long_description=README,
    long_description="TODO",
    long_description_content_type="text/markdown",
    url="https://github.com/AMR-/Conservative-Q-Improvement",
    author="Aaron M. Roth, Nicholay Topin, Saadiq Shah",
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
    #     # "console_scripts": [
    #     #     "realpython=reader.__main__:main",
    #     # ]
    # },
)
