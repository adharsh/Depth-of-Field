#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <vector>

#include "Texture.h"

Texture::Texture(const std::string& filepath, bool interpolate)
	: filepath(filepath)
{
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	GLenum option = interpolate ? GL_LINEAR : GL_NEAREST;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, option);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, option);
	
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(filepath.c_str(), &width, &height, &num_channels, 0);
	if (data)
	{
		if (num_channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (num_channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load_data(const std::string& filepath, unsigned char** out_data, int* out_width, int* out_height, int* out_num_channels)
{
	stbi_set_flip_vertically_on_load(true);
	*out_data = stbi_load(filepath.c_str(), out_width, out_height, out_num_channels, 0);
}

Texture::Texture(const unsigned char* data, int width, int height, int num_channels, bool interpolate)
{
	this->data = NULL;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	GLenum option = interpolate ? GL_LINEAR : GL_NEAREST;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, option);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, option);

	if (data)
	{
		if (num_channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (num_channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Passed in image data is null" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

const std::vector<int> Texture::getPixelValue(unsigned int row, unsigned int col) const
{
	unsigned bytePerPixel = num_channels;
	unsigned char* pixelOffset = data + (row * getWidth() +  col) * bytePerPixel;
	unsigned char r = pixelOffset[0];
	unsigned char g = pixelOffset[1];
	unsigned char b = pixelOffset[2];
	unsigned char a = num_channels >= 4 ? pixelOffset[3] : 0xff;

	return { r, g, b, a };
};

Texture::~Texture()
{
	if(data)
		stbi_image_free(data);
	glDeleteTextures(1, &texture_id);
}

void Texture::deleteTexture() const
{
	glDeleteTextures(1, &texture_id);
}