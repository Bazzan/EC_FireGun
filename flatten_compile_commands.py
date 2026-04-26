import json
import shlex
import os

def inline_rsp():
    input_file = '.vscode/compileCommands_EC_FireGun.json'
    output_file = 'compile_commands.json'
    
    if not os.path.exists(input_file):
        print(f"Error: {input_file} not found.")
        return

    with open(input_file, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    rsp_cache = {}
    
    for entry in data:
        new_args = []
        for arg in entry.get('arguments', []):
            if arg.startswith('@'):
                rsp_path = arg[1:]
                if rsp_path not in rsp_cache:
                    try:
                        with open(rsp_path, 'r', encoding='utf-8', errors='ignore') as rsp_f:
                            content = rsp_f.read()
                            args = shlex.split(content, posix=False)
                            args = [a.strip('"') for a in args]
                            
                            # Convert MSVC flags to Clang flags!
                            clang_args = []
                            i = 0
                            while i < len(args):
                                current = args[i]
                                if current == '/FI':
                                    clang_args.append('-include')
                                    clang_args.append(args[i+1].replace('\\', '/'))
                                    i += 1
                                elif current == '/I':
                                    clang_args.append('-I')
                                    clang_args.append(args[i+1].replace('\\', '/'))
                                    i += 1
                                elif current.startswith('/I'):
                                    clang_args.append('-I' + current[2:].replace('\\', '/'))
                                elif current.startswith('/D'):
                                    clang_args.append('-D' + current[2:])
                                else:
                                    if current.startswith('/'):
                                        # Ignore other MSVC specific flags like /Zc:inline to prevent clang warnings
                                        pass
                                    else:
                                        clang_args.append(current)
                                i += 1
                                
                            rsp_cache[rsp_path] = clang_args
                    except Exception as e:
                        print(f"Failed to read rsp file {rsp_path}: {e}")
                        rsp_cache[rsp_path] = []
                
                new_args.extend(rsp_cache[rsp_path])
            else:
                # Force standard clang mode instead of clang-cl
                if 'cl.exe' in arg.lower():
                    new_args.append('clang++.exe')
                else:
                    new_args.append(arg.replace('\\', '/'))
        entry['arguments'] = new_args

    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=4)
    print(f"Successfully generated Clang-native {output_file}!")

if __name__ == '__main__':
    inline_rsp()
