# Indexer
This is a reverse file indexer that outputs in JSON format.
It takes two filenames. The first is the name of the output file you would like to use. 
If it does not exist, it is created. If it does, it prompts to ask if you want to overwrite the data that was there.
The second filename can be a file or a directory and is the target of the indexer.

If it is a file it outputs all of the words in that file along with their frequencies.
If it is a directory, it walks through all of the file tree structure below that point and indexes all of the files,
sorting the results first by word, then by frequency, and finally by the filepath.

Ex:
Given a directory containing two files foo and bar defined as follows:

foo:"baa baa boo"

bar:"boo baa"

the indexer would output

{"list" : [
	"baa" : [
		{"test2/foo" : 2}
		{"test2/bar" : 1}
		]},

	"boo" : [
		{"test2/bar" : 1},
		{"test2/foo" : 1}
	]}
]}

