#pragma once

#include <hzrpch.h>
#include "glad/glad.h"
#include "OpenGLOcclusionQuery.h"

namespace Hazard::Rendering::OpenGL {

	

	OpenGLOcclusionQuery::OpenGLOcclusionQuery()
	{
		glGenQueries(1, &m_QueryID);
	}
	OpenGLOcclusionQuery::~OpenGLOcclusionQuery()
	{
		Flush();
	}
	void OpenGLOcclusionQuery::BeginQuery()
	{
		glBeginQuery(GL_SAMPLES_PASSED, m_QueryID);
		m_InUse = true;
	}
	void OpenGLOcclusionQuery::EndQuery()
	{
		glEndQuery(m_QueryID);
		m_InUse = false;
	}
	bool OpenGLOcclusionQuery::IsAvailable() const
	{
		GLint result;
		glGetQueryObjectiv(m_QueryID, GL_QUERY_RESULT_AVAILABLE, &result);
		return result == GL_TRUE;
	}
	int OpenGLOcclusionQuery::GetResult()
	{
		GLint result;
		glGetQueryObjectiv(m_QueryID, GL_QUERY_RESULT, &result);
		m_InUse = false;
		return result;
	}
	void OpenGLOcclusionQuery::Flush()
	{
		glDeleteQueries(1, &m_QueryID);
	}
	uint32_t OpenGLOcclusionQuery::GetID() const
	{
		return m_QueryID;
	}
}