/*!
* \file IOManager.h
* \brief Simple IO manager.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <vector>
#include <string>

namespace NS2 
{
	/*!
	* \brief File structure.
	*/
	struct DirEntry 
	{
		std::string path;
		bool isDirectory = false;
	};

	class IOManager
	{
	public:
		/*!
		* \brief Load file into buffer.
		* \param filePath: file path
		* \param buffer: output file buffer
		* \return true for no error, otherwise return false
		*/
		static bool readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer);
		/*!
		* \brief Load file into string buffer.
		* \param filePath: file path
		* \param buffer: output file buffer
		* \return true for no error, otherwise return false
		*/
		static bool readFileToBuffer(const std::string filePath, std::string& buffer);
		/*!
		* \brief Gets all directory entries in the directory specified by path and stores in rvEntries.
		* \return false if path is not a directory
		*/
		static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries, bool addDir = true);
		/*!
		* \brief Gets recursively all directory and sub-directory entries in the directory 
		*	specified by path and stores in rvEntries.
		* \return false if path is not a directory
		*/
		static bool getRecDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries, bool addDir = true);
		/*!
		* \brief Create directory.
		* \param path: path of directory to make
		* \return true for no error, otherwise return false 
		*/
		static bool makeDirectory(const char* path);
		/*!
		* \brief Copy file.
		* \param from: file we intend to copy
		* \param to: created file name
		* \return true for no error, otherwise return false
		*/
		static bool copyFile(const char* from, const char* to);
		/*!
		* \brief Copy all files (not dir) in the "from" directory into the "to" directory.
		* \param from: directory to copy content
		* \param to: directory to copy content into
		* \return true for no error, otherwise return false
		*/
		static bool copyDir(const char* from, const char* to);
		/*!
		* \brief Check for existance of a given file.
		* \param path: path of file
		* \return true if the file exists, otherwise return false
		*/
		static bool exists(const char* path);
		/*!
		* \brief Delete the given directory.
		* \param path: path of directory
		* \return true if the file exists, otherwise return false
		*/
		static bool deleteDir(const char* path);
		/*!
		 * \brief Count the number of files with a specific extension in a given directory.
		 * \param pathStr: path of the directory
		 * \param extension: file extension to count
		 */
		static int countFiles(const std::string& pathStr, const std::string& extension);
	};
}