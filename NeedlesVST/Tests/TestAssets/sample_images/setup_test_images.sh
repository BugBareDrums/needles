#!/bin/bash

# Create sample test images for Needles VST Plugin testing
# Run from Tests/TestAssets/sample_images/ directory

echo "Creating sample test images for Needles VST testing..."

# Simple Python script to generate test images if ImageMagick is not available
cat << 'EOF' > create_test_images.py
#!/usr/bin/env python3
"""
Create sample test images for Needles VST Plugin
Generates various image patterns for testing different scenarios
"""

try:
    from PIL import Image, ImageDraw
    import os
    
    def create_solid_color_image(filename, color, size=(100, 100)):
        """Create solid color image"""
        img = Image.new('RGB', size, color)
        img.save(filename)
        print(f"Created {filename}")
    
    def create_gradient_image(filename, size=(100, 100)):
        """Create horizontal gradient image"""
        img = Image.new('RGB', size)
        draw = ImageDraw.Draw(img)
        
        for x in range(size[0]):
            color_value = int(255 * x / size[0])
            for y in range(size[1]):
                draw.point((x, y), (color_value, color_value, color_value))
        
        img.save(filename)
        print(f"Created {filename}")
    
    def create_rgb_test_image(filename, size=(90, 30)):
        """Create RGB test pattern"""
        img = Image.new('RGB', size)
        draw = ImageDraw.Draw(img)
        
        # Red section
        draw.rectangle([0, 0, 29, 29], fill=(255, 0, 0))
        # Green section  
        draw.rectangle([30, 0, 59, 29], fill=(0, 255, 0))
        # Blue section
        draw.rectangle([60, 0, 89, 29], fill=(0, 0, 255))
        
        img.save(filename)
        print(f"Created {filename}")
    
    def create_checkerboard_image(filename, size=(64, 64), square_size=8):
        """Create checkerboard pattern"""
        img = Image.new('RGB', size)
        draw = ImageDraw.Draw(img)
        
        for x in range(0, size[0], square_size):
            for y in range(0, size[1], square_size):
                if ((x // square_size) + (y // square_size)) % 2 == 0:
                    color = (255, 255, 255)  # White
                else:
                    color = (0, 0, 0)        # Black
                    
                draw.rectangle([x, y, x + square_size - 1, y + square_size - 1], fill=color)
        
        img.save(filename)
        print(f"Created {filename}")
    
    # Create test images
    print("Generating test images with PIL...")
    
    create_solid_color_image("solid_red.png", (255, 0, 0))
    create_solid_color_image("solid_green.png", (0, 255, 0)) 
    create_solid_color_image("solid_blue.png", (0, 0, 255))
    create_solid_color_image("solid_white.png", (255, 255, 255))
    create_solid_color_image("solid_black.png", (0, 0, 0))
    
    create_gradient_image("gradient_horizontal.png")
    create_rgb_test_image("rgb_test_pattern.png")
    create_checkerboard_image("checkerboard.png")
    
    print("✓ Test images created successfully!")
    
except ImportError:
    print("PIL (Pillow) not available. Trying ImageMagick...")
    print("Install Pillow with: pip install Pillow")

EOF

chmod +x create_test_images.py

# Try to run the Python script
if command -v python3 &> /dev/null; then
    python3 create_test_images.py
elif command -v python &> /dev/null; then
    python create_test_images.py
else
    echo "Python not available. Creating placeholder files..."
    
    # Create placeholder files that indicate what should be there
    cat << 'EOF' > README.md
# Test Images Directory

This directory should contain sample images for testing:

## Required Test Images:
- solid_red.png (100x100 red image)
- solid_green.png (100x100 green image)  
- solid_blue.png (100x100 blue image)
- solid_white.png (100x100 white image)
- solid_black.png (100x100 black image)
- gradient_horizontal.png (100x100 horizontal gradient)
- rgb_test_pattern.png (90x30 RGB stripes)
- checkerboard.png (64x64 checkerboard)

## Generate Images:
Run `create_test_images.py` with Python and PIL/Pillow installed
Or manually create these images with any image editor

## Usage:
These images are used by unit tests to validate:
- Image loading functionality
- RGB extraction algorithms
- Area averaging calculations
- Scan pattern behavior
EOF

    echo "Created README.md with instructions"
fi

echo "Test image setup complete!"