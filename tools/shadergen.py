#!/usr/bin/env python3

import os, argparse

TEMPLATE_VERTEX = """#version 120
attribute vec3 aPos;
uniform mat4 uMVP;
void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
}
"""

TEMPLATE_FRAGMENT = """#version 120
void main() {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
"""

def make(name):
    outdir = os.getcwd()
    vpath = os.path.join(outdir, f"{name}.vert")
    fpath = os.path.join(outdir, f"{name}.frag")

    if os.path.exists(vpath) or os.path.exists(fpath):
        print(f"Error: {name} already exists.")
        return

    open(vpath, "w").write(TEMPLATE_VERTEX)
    open(fpath, "w").write(TEMPLATE_FRAGMENT)
    print(f"Created {vpath} and {fpath}")

def main():
    parser = argparse.ArgumentParser(description="Shadergen (version 120)")
    parser.add_argument("name", help="Base name for shader (no extension)")
    args = parser.parse_args()
    make(args.name)

if __name__ == "__main__":
    main()