import argparse
import subprocess
import os
import shutil
import sys

def init():
    parser = argparse.ArgumentParser(description='Build tool')
    parser.add_argument('-t', '--target', help='target platform', required=False, default="linux")
    parser.add_argument('-e', '--executable', help='executable', default='all', required=False)
    parser.add_argument('-s', '--suppress', dest='suppress', action='store_true', help="Boolean flag for suppress",default=False)
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true', help="Boolean flag for verbose", default=False)                         
    parser.add_argument('-c', '--clean', dest='clean', action='store_true', help="Boolean flag for clean build", default=False)
    parser.add_argument('-d', '--debug', dest='debug', action='store_true', help="Compile with debug symbols", default=False)    
    args = parser.parse_args()

    return args

def run_command(cmd, print_cmd=True):
    if print_cmd:
        print('>> {}'.format(cmd))
    process = subprocess.Popen(cmd, cwd=os.curdir, stdout=subprocess.PIPE, shell=True)
    while True:
        output = process.stdout.readline()
        if process.poll() is not None and output == b'':
            break
        if output:
            print(output.strip().decode('utf-8'))
    return process.returncode == 0

def __main__():
    args = init()
    build_dir = os.path.join(os.path.abspath(os.curdir), 'build')
    if args.clean:
        if os.path.exists(build_dir):
            shutil.rmtree(build_dir)

    if "linux" in args.target.lower():
        cmd = r'cmake -DPLATFORM="{}" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -Bbuild -H.'.format(args.target.lower())
        
        if args.verbose:   
            cmd += ' -DCMAKE_VERBOSE_MAKEFILE=ON '
        else:
            cmd += ' -DCMAKE_VERBOSE_MAKEFILE=OFF '


        if args.debug:
            cmd += ' -DCMAKE_BUILD_TYPE=Debug'
        else:
            cmd += ' -DCMAKE_BUILD_TYPE=Release'

            
        if args.suppress:
            cmd = cmd + ' -DCMAKE_C_FLAGS="${CMAKE_C_FLAGS} -w -s" -DCMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -w -s" '
            
        if run_command(cmd):
            if run_command("cmake --build build/ --target {} -- -j 16".format(args.executable)):
                if run_command("mv build/compile_commands.json ."):
                        print("SUCCESS")
                else:
                    sys.exit(1)
            else:
                sys.exit(1)
        else:
            sys.exit(1)
                        
    else:
        print("Target Mismatch")
        sys.exit(1)

__main__()
