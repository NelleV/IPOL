import numpy as np

filename = 'ToyProblemSimilarities.txt'

infile = open(filename, 'r')

indices = []
for line in infile.readlines():
    a, b, c = line.split()
    indices.append(int(a))
    indices.append(int(b))

similarities = np.zeros((max(indices), max(indices)))
infile = open(filename, 'r')
for line in infile.readlines():
    a, b, c = line.split()
    similarities[int(a) - 1, int(b) - 1] = float(c)

outfile = open("output.csv", "w")
for line in similarities:
    outfile.write(' '.join([str(i) for i in line]))
    outfile.write('\n')
