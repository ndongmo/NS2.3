#include "GameRenderer.h"
#include "Level.h"
#include "PhysicsEngine.h"
#include "EditorGui.h"
#include <Camera.h>

GameRenderer::GameRenderer(PhysicsEngine& physicsEngine) :
    m_physicEngine(physicsEngine),
    m_entityRenderer(m_spriteBatch, m_debugRenderer, physicsEngine),
    m_tileRenderer(m_spriteBatch, m_debugRenderer),
    m_lightRenderer(m_spriteBatch, m_debugRenderer, m_textureShader)
{}

void GameRenderer::init(float screenWidth, float screenHeight, bool debug)
{
	m_lineWidth = 2.0f;
	m_debug = debug;

	m_textureShader.compileShaders("Texture.vert", "Texture.frag");
    m_textureShader.addAttribute("vertexData");
    m_textureShader.addAttribute("destRect");
    m_textureShader.addAttribute("uvRect");
    m_textureShader.addAttribute("vertexColor");
	m_textureShader.linkShaders();

    m_spriteBatch.init();
    m_debugRenderer.init();
}

void GameRenderer::dispose()
{
    m_debugRenderer.dispose();
	m_textureShader.dispose();
}

void GameRenderer::clearAmbientColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void GameRenderer::render(NS2::Camera& camera, Level& level)
{
    camera.setScreenBox(m_screenBox);

    glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Normal transparency

    m_textureShader.use();
    {
        m_textureShader.setUniform("ns2TextureSampler", 0);
        m_textureShader.setUniform("cameraMatrix", camera.getCameraMatrix());
        m_textureShader.setUniform("u_AmbientColor", level.getAmbientColor());

        m_lightRenderer.render(level, m_screenBox);

        m_spriteBatch.begin();
        {
			m_tileRenderer.render(level, m_screenBox);
			m_entityRenderer.render(level, m_screenBox);
        }
        m_spriteBatch.end();
        m_spriteBatch.render();
    }
    m_textureShader.unuse();

    if (m_debug)
    {
		m_entityRenderer.renderDebug(level, m_screenBox);
		m_lightRenderer.renderDebug(level, m_screenBox);
		m_tileRenderer.renderDebug(level, m_screenBox);

        m_debugRenderer.end();
        m_debugRenderer.render(camera.getCameraMatrix(), m_lineWidth);
    }
}

void GameRenderer::render(NS2::Camera& camera, Level& level, EditorGui& editor)
{
    camera.setScreenBox(m_screenBox);

    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Normal transparency

    m_textureShader.use();
    {
        m_textureShader.setUniform("ns2TextureSampler", 0);
        m_textureShader.setUniform("cameraMatrix", camera.getCameraMatrix());
        m_textureShader.setUniform("u_AmbientColor", level.getAmbientColor());

        editor.draw(m_lightRenderer, m_screenBox);

        m_spriteBatch.begin();
        {
            editor.draw(m_tileRenderer, m_screenBox);
            editor.draw(m_entityRenderer, m_screenBox);
        }
        m_spriteBatch.end();
        m_spriteBatch.render();
    }
    m_textureShader.unuse();

    if (editor.debug(DebugType::DEBUG))
    {
        editor.drawDebug(m_entityRenderer, m_screenBox);
        editor.drawDebug(m_lightRenderer, m_screenBox);
        editor.drawDebug(m_tileRenderer, m_screenBox);

        m_debugRenderer.end();
        m_debugRenderer.render(camera.getCameraMatrix(), m_lineWidth);
    }
}
