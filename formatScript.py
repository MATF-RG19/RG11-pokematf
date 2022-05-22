import subprocess
import os
import sys

def codeFile(filename):
    return filename.endswith('.h') \
        or filename.endswith('.c') \
        or filename.endswith('.cpp')  \
        or filename.endswith('.hpp');
    
if __name__ == "__main__":
    args = sys.argv[1:]
    
    path = "."
    
    for dir, _, files in os.walk(path):
        for file in files:
            if codeFile(file):
                filepath = dir + "/" + file
                print(filepath)
                subprocess.run(f'clang-format {filepath} -i', shell = True)
                
print("Done.\n")       

            
