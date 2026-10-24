/*!
* \file IFileManager.h
* \brief File management interface.
* \author Ndongmo Silatsa Fabrice
* \date 16-05-2017
* \version 2.0
*/

#pragma once

#include <string>

class IFileManager
{
public:
	virtual ~IFileManager() = default;

	/* SETTERS */
	void setPath(std::string path) { m_path = path; }
	/* GETTERS */
	std::string getPath() const { return m_path; }
protected:
	std::string m_path;			/* File path */
};

