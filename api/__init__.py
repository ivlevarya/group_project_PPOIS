import os
import shutil

_dll_directory = None

if os.name == "nt":
    compiler = shutil.which("g++")

    if compiler:
        compiler_directory = os.path.dirname(compiler)
        _dll_directory = os.add_dll_directory(
            compiler_directory
        )
