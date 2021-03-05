#pragma once

#include <hzrpch.h>
#include "glad/glad.h"
#include "OpenGLOcclusionQuery.h"

namespace Hazard::Rendering {

	

	OpenGLOcclusionQuery::OpenGLOcclusionQuery()
	{
		HZR_CORE_INFO("Creating occlusion query");
		glGenQueries(1, &queryID);
	}
	OpenGLOcclusionQuery::~OpenGLOcclusionQuery()
	{
		Flush();
	}
	void OpenGLOcclusionQuery::BeginQuery()
	{
		glBeginQuery(GL_SAMPLES_PASSED, queryID);
		inUse = true;
	}
	void OpenGLOcclusionQuery::EndQuery()
	{
		glEndQuery(queryID);
		inUse = false;
	}
	bool OpenGLOcclusionQuery::IsAvailable() const
	{
		GLint result;
		glGetQueryObjectiv(queryID, GL_QUERY_RESULT_AVAILABLE, &result);
		return result == GL_TRUE;
	}
	int OpenGLOcclusionQuery::GetResult()
	{
		GLint result;
		glGetQueryObjectiv(queryID, GL_QUERY_RESULT, &result);
		inUse = false;
		return result;
	}
	void OpenGLOcclusionQuery::Flush()
	{
		glDeleteQueries(1, &queryID);
	}
	uint32_t OpenGLOcclusionQuery::GetID() const
	{
		return queryID;
	}
}