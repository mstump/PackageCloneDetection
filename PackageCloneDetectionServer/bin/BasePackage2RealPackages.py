#!/usr/bin/python

import sys

dict = {}

# arg1 sourcepktlist
# arg2 basepackagedb.txt

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

f = open(sys.argv[1], "r")
for l in f:
	names=l.strip().split(":")
	if dict.has_key(names[1]):
		dict['%s__%s' % (names[1], names[1])].append(names[0])
	else:
		dict['%s__%s' % (names[1], names[1])] = [ names[0] ]
f.close()

def find_closest(name):
	minD = -1
	minK = None
	for k, v in dict.iteritems():
		d = edit_distance(k, name)
		if min == -1 or d < min:
			minD = d
			minK = k
	print '# edit_distance %i' % minD
	dict[name] = dict[minK]
	return minK
	
def find_matches(name):
	if dict.has_key(name):
		return dict[name]
	print '# didnt match %s' % name
	return dict[find_closest(name)]
	
def print_matches(n1, n2):
	for x in n1:
		for y in n2:
			print '%s:%s' % (x, y)
print '#'

f = open(sys.argv[2], "r")
for l in f:
	names=l.strip().split(":")
	print '# checking %s %s' % (names[1], names[2])
	n1 = find_matches('%s' % (names[1]))
	n2 = find_matches('%s' % (names[2]))
	print_matches(n1, n2)
f.close()
