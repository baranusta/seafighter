#pragma once
#include "shader_controller.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class Text
{
private: 
	unsigned int Text2DTextureID;
	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;
	unsigned int Text2DShaderID;
	unsigned int Text2DUniformID;
	ShaderController shader;
	const char * texturePath;
	glm::vec3 pos;

	GLuint loadDDS(const char * imagepath) {

		unsigned char header[124];

		FILE *fp;

		/* try to open the file */
		fp = fopen(imagepath, "rb");
		if (fp == NULL) {
			printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
			return 0;
		}

		/* verify the type of file */
		char filecode[4];
		fread(filecode, 1, 4, fp);
		if (strncmp(filecode, "DDS ", 4) != 0) {
			fclose(fp);
			return 0;
		}

		/* get the surface desc */
		fread(&header, 124, 1, fp);

		unsigned int height = *(unsigned int*)&(header[8]);
		unsigned int width = *(unsigned int*)&(header[12]);
		unsigned int linearSize = *(unsigned int*)&(header[16]);
		unsigned int mipMapCount = *(unsigned int*)&(header[24]);
		unsigned int fourCC = *(unsigned int*)&(header[80]);


		unsigned char * buffer;
		unsigned int bufsize;
		/* how big is it going to be including all mipmaps? */
		bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
		fread(buffer, 1, bufsize, fp);
		/* close the file pointer */
		fclose(fp);

		unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
		unsigned int format;
		switch (fourCC)
		{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			return 0;
		}

		// Create one OpenGL texture
		GLuint textureID;
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		/* load the mipmaps */
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		{
			unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
				0, size, buffer + offset);

			offset += size;
			width /= 2;
			height /= 2;

			// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
			if (width < 1) width = 1;
			if (height < 1) height = 1;

		}

		free(buffer);

		return textureID;


	}

public:

	Text(glm::vec3 position, const char * texture, std::string vShader = "text_vs.glslx", std::string fShader = "text_fs.glslx")
		: pos(position), texturePath(texture),shader(vShader, fShader)
	{
		// Initialize texture
		Text2DTextureID = loadDDS(texturePath);

		// Initialize VBO
		glGenBuffers(1, &Text2DVertexBufferID);
		glGenBuffers(1, &Text2DUVBufferID);

		// Initialize Shader
		//Text2DShaderID = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
		Text2DShaderID = shader.operator GLuint();
		// Initialize uniforms' IDs
		Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");
	}


	void printText2D(const char * text, int x, int y, int size) {

		unsigned int length = strlen(text);

		// Fill buffers
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> UVs;
		for (unsigned int i = 0; i<length; i++) {

			glm::vec2 vertex_up_left = glm::vec2(x + i*size, y + size);
			glm::vec2 vertex_up_right = glm::vec2(x + i*size + size, y + size);
			glm::vec2 vertex_down_right = glm::vec2(x + i*size + size, y);
			glm::vec2 vertex_down_left = glm::vec2(x + i*size, y);

			vertices.push_back(vertex_up_left);
			vertices.push_back(vertex_down_left);
			vertices.push_back(vertex_up_right);

			vertices.push_back(vertex_down_right);
			vertices.push_back(vertex_up_right);
			vertices.push_back(vertex_down_left);

			char character = text[i];
			float uv_x = (character % 16) / 16.0f;
			float uv_y = (character / 16) / 16.0f;

			glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
			glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
			glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
			glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
			UVs.push_back(uv_up_left);
			UVs.push_back(uv_down_left);
			UVs.push_back(uv_up_right);

			UVs.push_back(uv_down_right);
			UVs.push_back(uv_up_right);
			UVs.push_back(uv_down_left);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

		// Bind shader
		glUseProgram(Text2DShaderID);
		shader();
		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(Text2DUniformID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

	}

	void cleanupText2D() {

		// Delete buffers
		glDeleteBuffers(1, &Text2DVertexBufferID);
		glDeleteBuffers(1, &Text2DUVBufferID);

		// Delete texture
		glDeleteTextures(1, &Text2DTextureID);

		// Delete shader
		glDeleteProgram(Text2DShaderID);
	}

};