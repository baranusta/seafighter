#pragma once

#include "game_object.h"
#include "quad.h"

//#define DEBUG_SHADOW true

class Scene
{
private:
	std::vector<GameObject*> ToRender;
	std::vector<GameObject*> ToShadowRender;
	GameObject* player;


	glm::vec3 light;
	glm::vec3 lightOffset;
	glm::mat4 lightMVP;

	GLuint FramebufferName = 0;
	GLuint depthTexture;

	int shadowWidth = 1024, shadowHeight = 1024;
	int screenWidth, screenHeight;


	glm::mat4 depthProjectionMatrix;
	glm::mat4 depthViewMatrix;
	glm::mat4 depthViewInvMatrix;
	glm::mat4 depthModelMatrix;

	bool renderVpCam = true;

	void renderShadow()
	{
		glCullFace(GL_FRONT);
		glViewport(0,0, shadowWidth, shadowHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);

		if(player != nullptr)
			player->renderShadowMap(lightMVP);

		for (auto& child : ToShadowRender)
			child->renderShadowMap(lightMVP);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
		glCullFace(GL_BACK);
	}

public:
	Scene(int screenWidth, int screenHeight): screenWidth(screenWidth), screenHeight(screenHeight), player(nullptr)
	{
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "framebuffer problem";

		glDrawBuffer(GL_NONE); // No color buffer is drawn to.
		// switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		depthProjectionMatrix = glm::ortho<float>(-10.f,10.f, -10.0f, 10.f, 1., 25.);
		depthModelMatrix = glm::mat4(1.0);
	}

	~Scene()
	{
		glDeleteFramebuffers(1,&FramebufferName);
	}

	void addChildForShadow(GameObject* child)
	{
		ToShadowRender.push_back(child);
	}

	void addChildForRender(GameObject* child)
	{
		ToRender.push_back(child);
	}

	void addPlayer(GameObject* child)
	{
		player = child;
	}

	void setLight(glm::vec3 light)
	{
		if (this->light != light)
		{
			this->light = light;
			depthViewMatrix = glm::lookAt(light + lightOffset, lightOffset, glm::vec3(0, 0, 1));
			lightMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
		} 
	}
	glm::vec3 getPos(glm::vec3 pos)
	{
		glm::vec4 screenPos = depthViewMatrix * glm::vec4(pos, 1);
		return screenPos / screenPos[3];
	}

	void updateViewArea(glm::vec3 offset)
	{
		lightOffset = offset;
		depthViewMatrix = glm::lookAt(offset + light, offset, glm::vec3(0, 0, 1));
		glm::vec3 shallowLight = light;
		shallowLight[2] = 0;
		shallowLight = glm::normalize(shallowLight);
		glm::vec3 cosVec = glm::rotateZ(shallowLight, glm::radians(90.f));
		if (player != nullptr && player->getPosition().length!= 0)
		{
			float lengthX = glm::dot(cosVec, player->getPosition()) / 30;
			float lengthY = glm::dot(shallowLight, player->getPosition()) / 30;
			depthProjectionMatrix = glm::ortho<float>(lengthX - 3, lengthX + 3, -4 - lengthY, +4 - lengthY, 1., 25.);
		}
		lightMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	}

	void changeRenderedCam()
	{
		renderVpCam = !renderVpCam;
	}

	void renderScene(glm::vec3 viewPos, glm::mat4 cameraVp)
	{
		renderShadow();
		printError("3423423aaaaaa");

		// clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, depthTexture);
		printError("dfgdfgdfdaaaaaa");

#ifdef DEBUG_SHADOW
		Quad shadow;
		printError("kaaaaaaaa");
		shadow.draw();
#else
		if(player != nullptr)
			player->draw(viewPos, renderVpCam == true ? cameraVp : lightMVP, lightMVP, light, depthTexture);

		for (auto& child : ToRender)
			child->draw(viewPos, renderVpCam == true ? cameraVp: lightMVP, lightMVP, light + lightOffset, depthTexture);
#endif
	}
};
