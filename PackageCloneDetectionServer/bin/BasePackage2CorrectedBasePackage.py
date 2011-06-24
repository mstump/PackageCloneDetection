#!/usr/bin/python

import sys

dict = {}

# edit_distance bsd license
# www.korokithakis.net/node/87
def edit_distance(first, second):
	"""Find the Levenshtein distance between two strings."""
	if len(first) > len(second):
		first, second = second, first
	if len(second) == 0:
		return len(first)
	first_length = len(first) + 1
	second_length = len(second) + 1
	distance_matrix = [[0] * second_length for x in range(first_length)]
	for i in range(first_length):
		distance_matrix[i][0] = i
	for j in range(second_length):
		distance_matrix[0][j]=j
	for i in xrange(1, first_length):
		for j in range(1, second_length):
			deletion = distance_matrix[i-1][j] + 1
			insertion = distance_matrix[i][j-1] + 1
			substitution = distance_matrix[i-1][j-1]
			if first[i-1] != second[j-1]:
				substitution += 1
			distance_matrix[i][j] = min(insertion, deletion, substitution)
	return distance_matrix[first_length-1][second_length-1]

# replace with a metric tree. oh dear
def find_closest(name):
	if dict.has_key(name):
		return name
	print '# nonexact match %s' % name
	min = -1
	minD = ""
	for d, v in dict.iteritems():
		dist = edit_distance(d, name)
		if min == -1 or dist < min:
			minD = d
			min = dist
	return minD

# arg1 pkglist
# arg2 basepackagedb2.txt

f = open(sys.argv[1], "r")
for l in f:
	dict[l.strip()] = 1
f.close()

print '#'

f = open(sys.argv[2], "r")
for l in f:
	names=l.strip().split(":")
	print '# checking %s %s' % (names[0], names[1])
	n1 = find_closest(names[0])
	n2 = find_closest(names[1])
	print '%s:%s' % (n1, n2)
f.close()
