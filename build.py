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
compiler = "g++"
compile_pattern = ".*\.cpp$"

cflags = f"-I{source_dir} -I{os.path.join(source_dir, 'util', 'dear_imgui')}"

# err_flags = "-Wall -Wunused-variable -Wextra -Wno-enum-compare -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast -g -ggdb -fdiagnostics-color=always"
err_flags = "-Wall -Wunused-variable -Wextra -Wno-enum-compare -g -ggdb -fdiagnostics-color=always"

libs = ""
if platform.system() == "Windows":
    libs += "-LC:/msys64/mingw64/bin/../lib -lraylib -lopengl32 -lgdi32 -lwinmm"
else:
    raise NotImplementedError()

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

        if os.path.exists(out_path) and os.path.getmtime(compile_path) < os.path.getmtime(out_path):
            compiled_objects.append(out_path)
            return
        
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