/***************************************************************************
 *   Copyright (C) 2006-2008 by Thomas Schweitzer                          *
 *   thomas-schweitzer(at)arcor.de                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2.0 as   *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program in the file LICENSE.GPL; if not, write to the *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef UIGUIINIFILEPARSER_H
#define UIGUIINIFILEPARSER_H

#include <QFile>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <vector>

/*!
    \class UiguiIniFileParser
    \brief This class can be used to parse and access the contents of well formed ini files, but only readable.
 */

class UiguiIniFileParser
{
public:
    UiguiIniFileParser(void);
    UiguiIniFileParser(const QString &iniFileName);
    ~UiguiIniFileParser(void);
    QVariant value(const QString &keyName, const QString &defaultValue="");
    QStringList childGroups();

private:
    void parseIniFile();

    QString iniFileName;
    std::vector<QString> sections;
    QMap<QString, QVariant> keyValueMap;
};

#endif // UIGUIINIFILEPARSER_H
