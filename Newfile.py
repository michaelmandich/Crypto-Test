import os
import sys
import time
import csv
from collections import defaultdict

start_time = time.time()

class Node:
    def __init__(self):
        self.children = defaultdict(Node)
        self.hex_value = None

    def insert(self, hex_str):
        current = self
        for c in hex_str:
            current = current.children[c]
        current.hex_value = hex_str

    def write_tree_to_csv(self, csv_writer, prefix=""):
        if self.hex_value is not None:
            csv_writer.writerow([self.hex_value])
        for k, v in sorted(self.children.items()):
            v.write_tree_to_csv(csv_writer, prefix + k)

def generate_random_hex(length):
    return ''.join(['{:02x}'.format(x) for x in os.urandom(length // 2)])

if len(sys.argv) < 2:
    print("Usage: python3.6 newfile.py <number_of_random_hex_strings>")
    sys.exit(1)

num_random_hex_strings = int(sys.argv[1])

tree = Node()
for _ in range(num_random_hex_strings):
    random_hex = generate_random_hex(256 // 4)
    tree.insert(random_hex)

end_time = time.time()
duration = end_time - start_time

print("The Python script took {:.2f} seconds to run at ".format(duration) + str(num_random_hex_strings) + " keys")

# Write the tree to a CSV file
output_filename = f"Python_{num_random_hex_strings}_values.csv"
with open(output_filename, "w", newline="") as file:
    csv_writer = csv.writer(file)
    tree.write_tree_to_csv(csv_writer)
