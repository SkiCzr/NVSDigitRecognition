def format_numbers_for_c_arrays(input_file, output_file, array_sizes):
    try:
        # Open the input file and read all lines
        with open(input_file, 'r') as infile:
            # Read each line, convert to integers, and store in a list
            numbers = [line.strip() for line in infile.readlines() if line.strip()]
        
        
        
        arrays = []
        start_index = 0
        for size in array_sizes:
            arrays.append(numbers[start_index:start_index + size])
            start_index += size

        # Write formatted arrays to the output file
        with open(output_file, 'w') as outfile:
            for idx, array in enumerate(arrays, 1):
                formatted_array = ', '.join(array)
                outfile.write(f'int array_{idx}[] = {{{formatted_array}}};\n\n')

        print(f"Formatted arrays have been written to {output_file}.")
    
    except Exception as e:
        print(f"An error occurred: {e}")

# Usage
# Split numbers into 4 arrays with specified CNN layer sizes
array_sizes = [75, 450, 4992, 520]
input_file = 'python_scripts/quantized_weights.txt'
output_file = 'formatted_arrays_not_quant.txt'
format_numbers_for_c_arrays(input_file, output_file, array_sizes)