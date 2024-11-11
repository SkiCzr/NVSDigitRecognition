from PIL import Image
import cv2
import numpy as np


def parse_images_from_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    # Split the content by '},' which indicates the end of each image block
    raw_images = content.strip().split("},")

    images = []
    for raw_image in raw_images:
        # Remove '{' and '}' and split into lines
        lines = raw_image.replace("{", "").replace("}", "").strip().splitlines()
        image_data = []

        for line in lines:
            # Convert each character to an integer (0 or 1)
            row = [int(char) for char in line]
            image_data.append(row)

        images.append(image_data)
    return images


def save_images_as_png(images):
    for idx, image_data in enumerate(images):
        # Create a new 28x28 grayscale image (mode "L")
        img = Image.new('L', (28, 28))

        # Fill the image with pixel values
        for y in range(28):
            for x in range(28):
                # Map 0 -> 0 (black) and 1 -> 255 (white)
                img.putpixel((x, y), 255 * image_data[y][x])

        # Save each image with a unique name
        img.save(f'image_{idx + 1}.png')


# Load the images from imgs.txt
images = parse_images_from_file("imgs.txt")
image_files = [f'image_{i + 1}.png' for i in range(len(images))]  # List of image filenames

# Save each image as a 28x28 PNG
save_images_as_png(images)