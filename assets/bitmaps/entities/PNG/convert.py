import numpy as np
from PIL import Image
from sys import argv

def convert_to_opengl_format(pixel):
    r, g, b, a = pixel
    return (a << 24) | (b << 16) | (g << 8) | r

def image_to_c_array(image_path, grid_size):
    # Load the image
    image = Image.open(image_path).convert("RGBA")
    width, height = image.size
    
    # Ensure the image dimensions are multiples of the grid size
    assert width % grid_size == 0 and height % grid_size == 0, "Image dimensions must be multiples of grid size."
    
    # Get the pixels of the image
    pixels = np.array(image)
    
    c_array = []

    for tile_y in range(0, height, grid_size):
        for tile_x in range(0, width, grid_size):
            for y in range(grid_size):
                for x in range(grid_size):
                    pixel = pixels[tile_y + y, tile_x + x]
                    opengl_pixel = convert_to_opengl_format(pixel)
                    c_array.append(opengl_pixel)

    return c_array

def format_as_c_array(c_array):
    formatted_array = "{\n"
    formatted_array += ", ".join(f"0x{pixel:08X}" for pixel in c_array)
    formatted_array += "\n};"
    return formatted_array

# Example usage
image_path = argv[1]
grid_size = 8  # Define your grid size (e.g., 8x8)
c_array = image_to_c_array(image_path, grid_size)
formatted_c_array = format_as_c_array(c_array)

print(formatted_c_array)
