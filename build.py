import os
import subprocess
import re
import platform
import threading

def call_shell(*args):
    try:
        return subprocess.check_output(" ".join(args), shell=True).strip().decode('utf-8')
    except:
        print(f'ERROR: invalid command "{" ".join(args)}"')
        exit(1)

source_dir = "src"
object_dir = "obj"
compiler = "c++"
compile_pattern = ".*\.cpp$"

cflags = f"-I{source_dir} -I{os.path.join(source_dir, 'util', 'dear_imgui')} {call_shell('pkg-config --cflags SDL2')}"
err_flags = "-Wall -Wunused-variable -Wextra -Wno-enum-compare -fpermissive -g -ggdb -fdiagnostics-color=always"

libs = ""
if platform.system() == "Windows":
    libs = "-LC:/msys64/mingw64/bin/../lib -lSDL2_image -lmingw32 -lSDL2main -lSDL2"
else:
    libs = call_shell("pkg-config --libs sdl2") + " " + call_shell("pkg-config --libs SDL2_image")

out_ext = ""
if platform.system() == "Windows":
    out_ext = "exe"
else:
    out_ext = "out"

compiled_objects = []
thread_pool = []

def compile_file(file):
    if not re.search(compile_pattern, file) == None:
        compile_path = os.path.join(dir, file)
        out_path = os.path.join(object_dir, file.split('.')[0]) + '.o'
        call_shell(f"{compiler} {cflags} {err_flags} -c {compile_path} -o {out_path} {libs}")
        print(f"INFO: Compiled {compile_path}!")
        compiled_objects.append(out_path)

for [dir, subDirs, files] in os.walk(source_dir):
    for file in files:
        t = threading.Thread(target=compile_file, args=(file,))
        t.start()
        thread_pool.append(t)

        if len(thread_pool) == 4:
            for thread in thread_pool:
                thread.join()

for thread in thread_pool:
    thread.join()

print("INFO: Linking...")
call_shell(f'{compiler} {cflags} {err_flags} -o bin/main.{out_ext} {" ".join(compiled_objects)} {libs}')