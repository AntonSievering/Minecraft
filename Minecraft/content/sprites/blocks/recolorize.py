import PIL.Image as Image
import sys

if len(sys.argv) < 2:
    sys.exit()

print(sys.argv[1])

io = sys.argv[1]

input = Image.open(io)
input_data = input.load()

output = Image.new("RGBA", (16, 16))
output_data = output.load()

def colorize(v, a):
    r = v * 0.7
    g = v * 1.2
    b = v * 0.45
    return (int(r), int(g), int(b), int(a))

for y in range(0, 16):
    for x in range(0, 16):
        v = input_data[x, y]
        output_data[x, y] = colorize(v[0], v[3])
        
output.save(io)
