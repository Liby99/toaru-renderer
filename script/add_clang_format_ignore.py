import os, sys

filename = sys.argv[1]

print("Adding clang format ignore in file {}".format(filename))

with open(filename, 'r') as original:
    data = original.read()
with open(filename, 'w') as modified:
    modified.write("// clang-format off\n" + data)
