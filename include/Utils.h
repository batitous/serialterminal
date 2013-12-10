/*
The MIT License (MIT)

Copyright (c) 2013 Baptiste Burles, Sylvain Fay-Chatelard

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QComboBox>

class Utils
{

public:

	static QString getFullPathFile(QString &file);

	/** @brief get relative file.
	 */
	static QString getRelativeFile(QString &projectpath,QString &file);

	/** @brief get relative file (without extension in filename).
	 */
	static QString getRelativeBaseName(QString &projectPath,QString &file);

	/** @brief get relative path from a reference path.
	 *
	 * @param PathReference		Reference path.
	 * @param PathLink			Path.
	 * @param sep				Path separator.
	 * @return New path.
	 */
	static QString getRelativePath(QString &PathReference , QString &PathLink, QString &sep );

	/** @brief Run a command in a new process.
	 *
	 * @param workingDir	Working directory.
	 * @param exe			Command and arguments.
	 * @param [out] output	Pointer to a QString output.
	 */
	static bool runProcess(QString &workingDir, QString &exe, QString *output, int *exitCode);

	/** @brief Set Port COM to the specified QComboBox.
	 *
	 */
	static int setPortComList(QComboBox *portCombo);
private:

	static QString getPortCom(QString subkey, QString key);
};

#endif
