#======================================================================
# A function to convert files to be included as raw string literals
#======================================================================
function(create_raw_string_file input_file)
    # Get the directory and name of the iput file so we can use the "literals" subdirectory
    get_filename_component(input_file_directory ${input_file} DIRECTORY)
    get_filename_component(input_file_name ${input_file} NAME)
    set(output_file ${input_file_directory}/literals/${input_file_name})
    message(STATUS "Making ${input_file} an includable raw string file at ${output_file}")
    
    # Read in the contents of the file
    file(READ ${input_file} input_contents)

    # Wrap the contents of a file in R"( ... )" with proper newlines
    set(literal_wrapped_contents "R\"(\n${input_contents})\"")
    
    # Write the raw string literal wrapped contents to the new output file
    file(WRITE ${output_file} "${literal_wrapped_contents}")
endfunction()
