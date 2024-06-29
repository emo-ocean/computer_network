def generate_large_file(file_name, size_in_mb):
    # 1 MB = 1024 * 1024 bytes
    size_in_bytes = size_in_mb * 1024 * 1024

    with open(file_name, 'wb') as file:
        file.write(b'0' * size_in_bytes)

if __name__ == "__main__":
    file_name = 'large_file.txt'
    size_in_mb = 10
    generate_large_file(file_name, size_in_mb)
    print(f"Generated {size_in_mb} MB file: {file_name}")
