#include "Renderer.h"
#include <SpriteBatch.h>
#include <DebugRenderer.h>

Renderer::Renderer(NS2::SpriteBatch& spriteBatch, NS2::DebugRenderer& debugRenderer)
	: m_spriteBatch(spriteBatch), m_debugRenderer(debugRenderer)
{
	m_debugColor = NS2::ColorRGBA8(255, 255, 255, 255);
}