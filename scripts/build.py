#!/usr/bin/env python3

from os import name, walk, path
import subprocess

def files():
    c_files = []
    
    for root, dirs, files in walk("src/"):
        for file in files:
            if file.endswith(".cpp"):
                c_files.append(path.join(root, file).replace("\\\\", "/").replace("\\", "/"))
    return ' '.join(c_files)

def windows():
    r=subprocess.Popen("g++ -v", shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).wait()
    if r!=0: raise Exception("Mingw64 is Missing!")
    command = "llvm-config --cxxflags"
    llconf=subprocess.run(command.split(" "), capture_output=True, text=True).stdout.strip().replace("\n", "")
    subprocess.Popen(f"g++ {files()} -I./src/ -o build64/mgc.exe {llconf}", shell=True).wait()

def unix():
    command = "llvm-config --cxxflags"
    llconf=subprocess.run(command.split(" "), capture_output=True, text=True).stdout.strip().replace("\n", "")
    subprocess.Popen(f"g++ {files()} -I./src/ -o build64/mgc {llconf}", shell=True).wait()


match name:
    case "nt":
        windows()
    case _:
        unix()